/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Sample Encapsulating Shell -- Generic Multiplexed Sockets Interface    **/
/**                                                                        **/
/****************************************************************************/

/* ./test-cv -s 'tcps(8080)*|cspl unix(/tmp/foo)/tmp/barr|direct(1);*|cspl=upper tcpc(139.85.243.102:9999)*|cspl' */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<fcntl.h>
#include 	<unistd.h>

#include	<errno.h>

#include	<shell.h>
#include	<stacklayer.h>

typedef	struct	LAPI {
	const LSTYLE	
		*operations;
	void	*data;
} LAPI;

typedef	struct	LSOCK {
	void	*(*newsocket)( char * );
	int	(*waitfor)( void * );
	LAPI	*(*find)( void * );
	const char
		*(*peername)( void * );
	int	(*receive)( void *, unsigned char *, unsigned int, unsigned int );
	void	(*cleanup)( void * );
	void	(*addsearch)(void *, char *, LAPI *);
	void	*(*hash)( void *, char * );
	int	(*send)( void *, unsigned char *, int, const void * );
} LSOCK;

typedef	struct	{
	YLNODE	__header__;
	const LSOCK	
		*handler;
	void	*data;
} LENTRY;

typedef	struct	LNAMED {
	YLNODE	__header__;

	const char	*name;
	LAPI		*api;
	LENTRY		*socket;
} LNAMED;

typedef	struct	LSEND {
	LNAMED	*how;
	void	*data;
} LSEND;

/** The `Listener' Structure ************************************************/
typedef	struct	{
	YLIST	list;
	YLIST	namelist;
} LISTENER;

static	const int	yes = 1, no = 0;

/** Forward declaration for static functions ********************************/
static	void	errorabort( const char *s );

#if defined(__sun__) || defined(__linux) || defined(__CYGWIN__)
#define	NAMERESOLVE
#endif

#ifdef NAMERESOLVE
#include	<netdb.h>

in_addr_t	resolve( char *name )
{
	struct hostent	
		*H;
	char	**p;
	int	plen;
	in_addr_t
		resolved;

	resolved = inet_addr(name);
	if( resolved == (in_addr_t)(-1) ) {
		H = gethostbyname( name );

		if( H && (p = H->h_addr_list) ) {
			memcpy( &(resolved), *p, sizeof(resolved) );
		} else {
			errorabort("gethostbyname");
			return (in_addr_t)(-1);
		}
	} 

	return resolved;
}

#else
#define	resolve	inet_addr
#endif

#ifdef UNIXDOMAIN
/** The `Unix' Socket Handling Method ***************************************/
typedef	struct	{
	int	sd;
	YLIST	fromlist;
	
	struct	sockaddr_un	from;
	const char *peer;
} _TY_unix;

typedef	struct	{
	YLIST	__header__;
	const char	*name;
	LAPI		*how;
} _SR_unix;

#define	NAMEOFF	YOFFSETOF(struct sockaddr_un, sun_path)

static	void	*unix_newsocket( char *sockname )
{
	_TY_unix	*t = (_TY_unix *)qvSysMalloc( sizeof(_TY_unix) );
	struct	sockaddr_un	self;
    /* Start SPR 5903 Changes. */
    long unsigned int len;
    /* End SPR 5903 Changes. */
    
	ylInit( &(t->fromlist) );
	t->peer = 0;

	memset( &self, 0, sizeof(self) );
	self.sun_family = AF_UNIX;
    
    /* Klocwork fix */    
    /* Start SPR 5903 Changes. */
	len=strlen(sockname);
	if(len < sizeof(self.sun_path))
	{
		strncpy( self.sun_path, sockname, (sizeof(self.sun_path) - 1));
		self.sun_path[(sizeof(self.sun_path) - 1)]='\0';
	}
	else
	{
		qvPanic("%s: size is more than destination string", sockname );
	}
    /* End SPR 5903 Changes. */
    
	if( (t->sd = socket( PF_UNIX, SOCK_DGRAM, 0 )) < 0 ) {
		errorabort("socket");
	}

	unlink( sockname );
	if( bind(t->sd, (struct sockaddr *)&self, NAMEOFF + strlen(sockname) ) < 0 ) {
		errorabort("bind");
	}

	return (void *)t;
}

static	int	unix_waitfor( void *P )
{
	_TY_unix	*t = (_TY_unix *)P;

	return t->sd;
}

static	LAPI	*unix_find( void *P )
{
	_TY_unix	*t = (_TY_unix *)P;
	char		dummy;
	char		*name;
	int		n;
	_SR_unix	*s;
	int		fromlen;

	fromlen = sizeof( t->from );
	if( (n = recvfrom(t->sd, &dummy, sizeof(dummy), MSG_PEEK, (struct sockaddr *)(&(t->from)), &(fromlen) )) <= 0 ) {
		errorabort("recvfrom[from]");
	}

	name = (char *)&(t->from);
	name += NAMEOFF; 
	fromlen -= NAMEOFF;
	name[fromlen] = 0;
	t->peer = name;

	qvLog(0, "Received from <%s>\n", name );
	for( s = (_SR_unix *)ylFirst(&(t->fromlist)); s; s = (_SR_unix *)ylNext((YLNODE *)s) ) {
		if( !strcmp(s->name, "*") || !strcmp(s->name, t->peer) ) {
			return s->how;
		}
	}
	return 0;
}

static	const char	*unix_peername( void *P )
{
	_TY_unix	*t = (_TY_unix *)P;

	return t->peer;
}

static	int	unix_receive( void *P, unsigned char *buffer, unsigned int count, unsigned int sofar )
{
	_TY_unix	*t = (_TY_unix *)P;
	int	n;

	if( (n = recvfrom(t->sd, (char *)buffer, count, MSG_PEEK, 0, 0 )) <= 0 ) {
		errorabort("recvfrom[receive]");
	}

	return n;
}

static	void	unix_cleanup( void *P )
{
	_TY_unix	*t = (_TY_unix *)P;
	char		dummy;
	int	n;

	if( (n = recvfrom(t->sd, &dummy, sizeof(dummy), 0, 0, 0 )) <= 0 ) {
		errorabort("recvfrom[cleanup]");
	}
	t->peer = 0;
}

static	void	unix_addsearch(void *P, char *toname, LAPI *how)
{
	_TY_unix	*t = (_TY_unix *)P;
	_SR_unix	*s = (_SR_unix *)qvSysMalloc( sizeof(_SR_unix) );

	s->name = toname;
	s->how = how;
	ylPushTail( &(t->fromlist), (YLNODE *)s );
}

static	void	*unix_hash( void *P, char *name )
{
	_TY_unix	*t = (_TY_unix *)P;

	return (void *) name;
}

static	int	unix_send( void *P, unsigned char *buffer, int count, const void *todata )
{
	_TY_unix	*t = (_TY_unix *)P;
	struct sockaddr_un	
			to;
    /* Start SPR 5903 Changes. */
    long unsigned int len;
    /* End SPR 5903 Changes. */
    
	memset( &to, 0, sizeof(to) );
	to.sun_family = AF_UNIX;
     
    /* Klocwork fix */ 
    /* Start SPR 5903 Changes. */    
	len=strlen(todata);
	if(len < sizeof(to.sun_path))
	{
		strncpy( to.sun_path, (char *)(todata), (sizeof(to.sun_path) - 1));
		to.sun_path[(sizeof(to.sun_path) - 1)]='\0';
	}
	else
	{
		qvPanic("%s: size is more than destination string", todata );
	}
    /* End SPR 5903 Changes. */

	if ( sendto( t->sd, (char *)buffer, count, 0, (struct sockaddr *)&to, NAMEOFF + strlen((char *)todata) ) < 0 ) {
		errorabort("sendto");
	}

	return 0;
}

static	const LSOCK unix_methods = {
	unix_newsocket,
	unix_waitfor,
	unix_find,
	unix_peername,
	unix_receive,
	unix_cleanup,
	unix_addsearch,
	unix_hash,
	unix_send
};
#endif

/** The `UDP' Socket Handling Method ****************************************/
typedef	struct	{
	int	sd;
	YLIST	fromlist;
	char	peer[64];
} _TY_udp;

typedef	struct	{
	YLIST	__header__;
	unsigned long	ipa;
	unsigned long	mask;
	unsigned short	port;
	LAPI		*how;
} _SR_udp;

static	void	*udp_newsocket( char *sockname )
{
	_TY_udp	*t = (_TY_udp *)qvSysMalloc( sizeof(_TY_udp) );
	struct	sockaddr_in	self;
	char	to_ipa[64], to_port[64];
#ifdef	__linux__
	int	value;
#endif

	memset( &self, 0, sizeof(self) );
	self.sin_family = AF_INET;

	if( strchr( sockname, ':') ) {
		if( sscanf( sockname, "%[^:]:%s", to_ipa, to_port ) != 2 ) {
			qvPanic("Cannot decipher TCP destination address <%s>", sockname );
			return 0;
		}
		self.sin_port = htons( atoi(to_port) );
		self.sin_addr.s_addr = resolve(to_ipa);
	} else {
		self.sin_port = htons( atoi(sockname) );
		self.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	ylInit( &(t->fromlist) );
	t->peer[0] = 0;

	if( (t->sd = socket( PF_INET, SOCK_DGRAM, 0 )) < 0 ) {
		errorabort("socket");
	}

#ifdef	__linux__
	value = 1;
	if( setsockopt( t->sd, SOL_SOCKET, SO_BSDCOMPAT, &value, sizeof(value) ) < 0 ) {
		errorabort("setsockopt[BSDCOMPAT]");
	}
#endif

	if( bind(t->sd, (struct sockaddr *)&self, sizeof(self) ) < 0 ) {
		errorabort("bind");
	}

	return (void *)t;
}

static	int	udp_waitfor( void *P )
{
	_TY_udp	*t = (_TY_udp *)P;

	return t->sd;
}

static	LAPI	*udp_find( void *P )
{
	_TY_udp	*t = (_TY_udp *)P;
	char		dummy;
	int		n;
	unsigned long	ipa;
	unsigned short	port;
	struct	sockaddr_in	from;
	int	fromlen = sizeof( from );
	_SR_udp	*s;

	if( (n = recvfrom(t->sd, &dummy, sizeof(dummy), MSG_PEEK, (struct sockaddr *)(&from), &fromlen )) <= 0 ) {
		errorabort("recvfrom[from]");
	}

	ipa = from.sin_addr.s_addr;
	port = from.sin_port;

	for( s = (_SR_udp *)ylFirst(&(t->fromlist)); s; s = (_SR_udp *)ylNext((YLNODE *)s) ) {
		if( ((ipa & s->mask) == (s->ipa & s->mask)) && ((s->port == 0) || (s->port == port)) ) {
			ipa = ntohl(ipa);
			port = ntohs(port);
			sprintf( t->peer, "%u.%u.%u.%u:%hu", 
				YGETBYTE(ipa,3), 
				YGETBYTE(ipa,2), 
				YGETBYTE(ipa,1), 
				YGETBYTE(ipa,0), 
				port );
			return s->how;
		}
	}
	ipa = ntohl(ipa);
	port = ntohs(port);
	qvWarning("Could not locate operations for %u.%u.%u.%u:%hu", 
			YGETBYTE(ipa,3), 
			YGETBYTE(ipa,2), 
			YGETBYTE(ipa,1), 
			YGETBYTE(ipa,0), 
			port );
	return 0;
}

static	const char	*udp_peername( void *P )
{
	_TY_udp	*t = (_TY_udp *)P;

	return t->peer;
}

static	int	udp_receive( void *P, unsigned char *buffer, unsigned int count, unsigned int sofar )
{
	_TY_udp	*t = (_TY_udp *)P;
	int	n;

	if( (n = recvfrom(t->sd, (char *)buffer, count, MSG_PEEK, 0, 0 )) <= 0 ) {
		errorabort("recvfrom[receive]");
	}

	return n;
}

static	void	udp_cleanup( void *P )
{
	_TY_udp	*t = (_TY_udp *)P;
	char		dummy;
	int	n;

	t->peer[0] = 0;
	if( (n = recvfrom(t->sd, &dummy, sizeof(dummy), 0, 0, 0 )) <= 0 ) {
		errorabort("recvfrom[cleanup]");
	}
}

static	void	udp_addsearch(void *P, char *toname, LAPI *how)
{
	_TY_udp	*t = (_TY_udp *)P;
	_SR_udp	*s = (_SR_udp *)qvSysMalloc( sizeof(_SR_udp) );
	char	ipa[64], mask[64], port[64];

	if( !strcmp(toname, "*") ) {
		strcpy(ipa, "*");
		strcpy(mask, "*");
		strcpy(port, "*");
	} else if( strchr( toname, '/') == 0 ) {
		if( sscanf( toname, "%[^:]:%s", ipa, port ) != 2 ) {
			qvPanic("Cannot process UDP address <%s>", toname );
			return;
		}
		strcpy(mask, "*");
	} else {
		if( sscanf( toname, "%[^/]/%[^:]:%s", ipa, mask, port ) != 3 ) {
			qvPanic("Cannot process UDP address <%s>", toname );
			return;
		}
	}

	if( !strcmp(port, "*") ) {
		s->port = 0;
	} else {
		s->port = htons(atoi(port));
	}

	if( !strcmp(mask, "*") ) {
		s->mask = ~0;
	} else {
		s->mask = resolve(mask);
	}

	if( !strcmp(ipa, "*") ) {
		s->ipa = ntohl(INADDR_ANY);
		s->mask = 0;
	} else {
		s->ipa = resolve(ipa);
	}

	s->how = how;
	ylPushTail( &(t->fromlist), (YLNODE *)s );
}

static	void	*udp_hash( void *P, char *name )
{
	return (void *) name;
}

static	int	udp_send( void *P, unsigned char *buffer, int count, const void *todata )
{
	_TY_udp	*t = (_TY_udp *)P;
	struct sockaddr_in	
			to;
	char	ipa[64];
	unsigned short
		port;

	if( !todata || (sscanf( (char *)todata, "%[^:]:%hu", ipa, &port ) != 2) ) {
		qvPanic("Cannot process UDP address <%s>", todata?(char *)todata:"empty" );
		return 0;
	}

	memset( &to, 0, sizeof(to) );
	to.sin_family = AF_INET;
	to.sin_port = htons( port );
	to.sin_addr.s_addr = resolve( ipa );

	if ( sendto( t->sd, (char *)buffer, count, 0, (struct sockaddr *)&to, sizeof(to) ) < 0 ) {
		errorabort("sendto");
	}

	return 0;
}

static	const LSOCK udp_methods = {
	udp_newsocket,
	udp_waitfor,
	udp_find,
	udp_peername,
	udp_receive,
	udp_cleanup,
	udp_addsearch,
	udp_hash,
	udp_send
};

/** The `TCPS' Socket Handling Method ***************************************/
typedef	struct	{
	int	sd, asd;
	LAPI	*how;
	char	peer[64];
} _TY_tcps;

static	void	*tcps_newsocket( char *sockname )
{
	_TY_tcps	*t = (_TY_tcps *)qvSysMalloc( sizeof(_TY_tcps) );
	struct	sockaddr_in	self;
	struct	sockaddr_in	from;
	int	fromlen = sizeof( from );
	char	to_ipa[64], to_port[64];
	unsigned long ipa;
	unsigned short port;

	t->how = 0;

	memset( &self, 0, sizeof(self) );
	self.sin_family = AF_INET;

	if( strchr( sockname, ':') ) {
		if( sscanf( sockname, "%[^:]:%s", to_ipa, to_port ) != 2 ) {
			qvPanic("Cannot decipher TCP destination address <%s>", sockname );
			return 0;
		}
		self.sin_port = htons( atoi(to_port) );
		self.sin_addr.s_addr = resolve(to_ipa);
	} else {
		self.sin_port = htons( atoi(sockname) );
		self.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	if( (t->sd = socket( PF_INET, SOCK_STREAM, 0 )) < 0 ) {
		errorabort("socket");
	}

	if( setsockopt(t->sd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes) ) < 0 ) {
		errorabort("setsockopt:REUSEADDR");
	}

	if( bind(t->sd, (struct sockaddr *)&self, sizeof(self) ) < 0 ) {
		errorabort("bind");
	}

	if( listen(t->sd, 1) < 0 ) {
		errorabort("listen");
	}

	if( (t->asd = accept(t->sd, (struct sockaddr *)&from, &fromlen)) < 0 ) {
		errorabort("accept");
	}

	ipa = from.sin_addr.s_addr;
	port = from.sin_port;

	ipa = ntohl(ipa);
	port = ntohs(port);
	sprintf( t->peer, "%u.%u.%u.%u:%hu", 
		YGETBYTE(ipa,3), 
		YGETBYTE(ipa,2), 
		YGETBYTE(ipa,1), 
		YGETBYTE(ipa,0), 
		port );

	qvLog(0, "Accepted connection from %s", t->peer );

	return (void *)t;
}

static	int	tcps_waitfor( void *P )
{
	_TY_tcps	*t = (_TY_tcps *)P;

	return t->asd;
}

static	LAPI	*tcps_find( void *P )
{
	_TY_tcps	*t = (_TY_tcps *)P;

	return t->how;
}

static	const char	*tcps_peername( void *P )
{
	_TY_tcps	*t = (_TY_tcps *)P;

	return t->peer;
}

static	int	tcps_receive( void *P, unsigned char *buffer, unsigned int count, unsigned int sofar )
{
	_TY_tcps	*t = (_TY_tcps *)P;
	int	n;

	if( count == sofar ) {
		return sofar;
	}

	if( (n = recv(t->asd, (char *)(buffer + sofar), (count - sofar), 0 )) < 0 ) {
		errorabort("recv");
	}

	if( n == 0 ) {
		unsigned long ipa;
		unsigned short port;
		struct	sockaddr_in	from;
		int	fromlen = sizeof( from );

		qvWarning("Client seems to have disconnected, waiting for reconnection");
		close( t->asd );
		if( (t->asd = accept(t->sd, (struct sockaddr *)&from, &fromlen)) < 0 ) {
			errorabort("accept");
		}

		ipa = from.sin_addr.s_addr;
		port = from.sin_port;

		ipa = ntohl(ipa);
		port = ntohs(port);
		sprintf( t->peer, "%u.%u.%u.%u:%hu", 
			YGETBYTE(ipa,3), 
			YGETBYTE(ipa,2), 
			YGETBYTE(ipa,1), 
			YGETBYTE(ipa,0), 
			port );
		qvLog(0, "Accepted connection from %s", t->peer );
		return -1;
	}

	return sofar + n;
}

static	void	tcps_cleanup( void *P )
{
}

static	void	tcps_addsearch(void *P, char *toname, LAPI *how)
{
	_TY_tcps	*t = (_TY_tcps *)P;

	t->how = how;
}

static	void	*tcps_hash( void *P, char *name )
{
	return (void *) name;
}

static	int	tcps_send( void *P, unsigned char *buffer, int count, const void *todata )
{
	_TY_tcps	*t = (_TY_tcps *)P;

	if ( send( t->asd, (char *)buffer, count, 0 ) < 0 ) {
		errorabort("send");
	}

	return 0;
}

static	const LSOCK tcps_methods = {
	tcps_newsocket,
	tcps_waitfor,
	tcps_find,
	tcps_peername,
	tcps_receive,
	tcps_cleanup,
	tcps_addsearch,
	tcps_hash,
	tcps_send
};

/** The `TCPC' Socket Handling Method ***************************************/
typedef	struct	{
	int	asd;
	LAPI	*how;
	char	peer[64];
} _TY_tcpc;

static	void	*tcpc_newsocket( char *sockname )
{
	_TY_tcpc	*t = (_TY_tcpc *)qvSysMalloc( sizeof(_TY_tcpc) );
	struct	sockaddr_in	to;
	char	to_ipa[64], to_port[64];
	unsigned long ipa;
	unsigned short port;

	if( sscanf( sockname, "%[^:]:%s", to_ipa, to_port ) != 2 ) {
		qvPanic("Cannot decipher TCP destination address <%s>", sockname );
		return 0;
	}

	t->how = 0;

	memset( &to, 0, sizeof(to) );
	to.sin_family = AF_INET;
	to.sin_port = htons( atoi(to_port) );
	to.sin_addr.s_addr = resolve( to_ipa );

	if( (t->asd = socket( PF_INET, SOCK_STREAM, 0 )) < 0 ) {
		errorabort("socket");
	}

	if( connect(t->asd, (struct sockaddr *)&to, sizeof(to) ) < 0 ) {
		errorabort("connect");
	}

	ipa = to.sin_addr.s_addr;
	port = to.sin_port;

	ipa = ntohl(ipa);
	port = ntohs(port);
	sprintf( t->peer, "%u.%u.%u.%u:%hu", 
		YGETBYTE(ipa,3), 
		YGETBYTE(ipa,2), 
		YGETBYTE(ipa,1), 
		YGETBYTE(ipa,0), 
		port );
	qvLog(0, "Connected to %s", t->peer );
	return (void *)t;
}

static	int	tcpc_waitfor( void *P )
{
	_TY_tcpc	*t = (_TY_tcpc *)P;

	return t->asd;
}

static	LAPI	*tcpc_find( void *P )
{
	_TY_tcpc	*t = (_TY_tcpc *)P;

	return t->how;
}

static	const char	*tcpc_peername( void *P )
{
	_TY_tcpc	*t = (_TY_tcpc *)P;

	return t->peer;
}

static	int	tcpc_receive( void *P, unsigned char *buffer, unsigned int count, unsigned int sofar )
{
	_TY_tcpc	*t = (_TY_tcpc *)P;
	int	n;

	if( count == sofar ) {
		return sofar;
	}

	if( (n = recv(t->asd, (char *)(buffer + sofar), (count - sofar), 0 )) < 0 ) {
		errorabort("recv");
	}

	if( n == 0 ) {
		qvPanic("Server has closed connection");
	}

	return sofar + n;
}

static	void	tcpc_cleanup( void *P )
{
}

static	void	tcpc_addsearch(void *P, char *toname, LAPI *how)
{
	_TY_tcpc	*t = (_TY_tcpc *)P;

	t->how = how;
}

static	void	*tcpc_hash( void *P, char *name )
{
	return (void *) name;
}

static	int	tcpc_send( void *P, unsigned char *buffer, int count, const void *todata )
{
	_TY_tcpc	*t = (_TY_tcpc *)P;

	if ( send( t->asd, (char *)buffer, count, 0 ) < 0 ) {
		errorabort("send");
	}

	return 0;
}

static	const LSOCK tcpc_methods = {
	tcpc_newsocket,
	tcpc_waitfor,
	tcpc_find,
	tcpc_peername,
	tcpc_receive,
	tcpc_cleanup,
	tcpc_addsearch,
	tcpc_hash,
	tcpc_send
};

/** The `TCPU' Socket Handling Method ***************************************/
typedef	struct	{
	int	sd, asd;
	YLIST	fromlist;
	char	peer[64];
} _TY_tcpu;

typedef	struct	{
	YLIST	__header__;
	unsigned long	ipa;
	unsigned long	mask;
	LAPI		*how;
} _SR_tcpu;

static	void	*tcpu_newsocket( char *sockname )
{
	_TY_tcpu	*t = (_TY_tcpu *)qvSysMalloc( sizeof(_TY_tcpu) );
	char	to_ipa[64], to_port[64];
	struct	sockaddr_in	self;

	ylInit( &(t->fromlist) );
	t->peer[0] = 0;

	memset( &self, 0, sizeof(self) );
	self.sin_family = AF_INET;
	if( strchr( sockname, ':') ) {
		if( sscanf( sockname, "%[^:]:%s", to_ipa, to_port ) != 2 ) {
			qvPanic("Cannot decipher TCP destination address <%s>", sockname );
			return 0;
		}
		self.sin_port = htons( atoi(to_port) );
		self.sin_addr.s_addr = resolve(to_ipa);
	} else {
		self.sin_port = htons( atoi(sockname) );
		self.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	if( (t->sd = socket( PF_INET, SOCK_STREAM, 0 )) < 0 ) {
		errorabort("socket");
	}

	if( setsockopt(t->sd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes) ) < 0 ) {
		errorabort("setsockopt:REUSEADDR");
	}

	if( bind(t->sd, (struct sockaddr *)&self, sizeof(self) ) < 0 ) {
		errorabort("bind");
	}

	if( listen(t->sd, 10) < 0 ) {
		errorabort("listen");
	}

	t->asd = (-1);

	return (void *)t;
}

static	int	tcpu_waitfor( void *P )
{
	_TY_tcpu	*t = (_TY_tcpu *)P;

	return t->sd;
}

static	LAPI	*tcpu_find( void *P )
{
	_TY_tcpu	*t = (_TY_tcpu *)P;
	struct	sockaddr_in	from;
	int	fromlen = sizeof( from );
	unsigned long	ipa;
	unsigned short	port;
	_SR_tcpu	*s;

	if( (t->asd = accept(t->sd, (struct sockaddr *)&from, &fromlen)) < 0 ) {
		errorabort("accept");
	}

	ipa = from.sin_addr.s_addr;
	port = from.sin_port;

	for( s = (_SR_tcpu *)ylFirst(&(t->fromlist)); s; s = (_SR_tcpu *)ylNext((YLNODE *)s) ) {
		if( (ipa & s->mask) == (s->ipa & s->mask) ) {
			ipa = ntohl(ipa);
			port = ntohs(port);
			sprintf( t->peer, "%u.%u.%u.%u:%hu", 
				YGETBYTE(ipa,3), 
				YGETBYTE(ipa,2), 
				YGETBYTE(ipa,1), 
				YGETBYTE(ipa,0), 
				port );
			return s->how;
		}
	}

	ipa = ntohl(ipa);
	port = ntohs(port);
	qvWarning("Could not locate operations for %u.%u.%u.%u", 
			YGETBYTE(ipa,3), 
			YGETBYTE(ipa,2), 
			YGETBYTE(ipa,1), 
			YGETBYTE(ipa,0) );
	return 0;
}

static	const char	*tcpu_peername( void *P )
{
	_TY_tcpu	*t = (_TY_tcpu *)P;

	return t->peer;
}

static	int	tcpu_receive( void *P, unsigned char *buffer, unsigned int count, unsigned int sofar )
{
	_TY_tcpu	*t = (_TY_tcpu *)P;
	int	n;

	if( (n = recv(t->asd, (char *)(buffer + sofar), (count - sofar), 0 )) <= 0 ) {
		errorabort("recv");
	}

	return sofar + n;
}

static	void	tcpu_cleanup( void *P )
{
	_TY_tcpu	*t = (_TY_tcpu *)P;

	t->peer[0] = 0;
	close( t->asd );
	t->asd = (-1);
}

static	void	tcpu_addsearch(void *P, char *toname, LAPI *how)
{
	_TY_tcpu	*t = (_TY_tcpu *)P;
	_SR_tcpu	*s = (_SR_tcpu *)qvSysMalloc( sizeof(_SR_tcpu) );
	char	ipa[64], mask[64];

	if( !strcmp(toname, "*") ) {
		strcpy(ipa, "*");
		strcpy(mask, "*");
	} else if( strchr( toname, '/') == 0 ) {
		if( sscanf( toname, "%s", ipa ) != 1 ) {
			qvPanic("Cannot process TCP-U address <%s>", toname );
			return;
		}
		strcpy(mask, "*");
	} else {
		if( sscanf( toname, "%[^/]/%s", ipa, mask ) != 2 ) {
			qvPanic("Cannot process TCP-U address <%s>", toname );
			return;
		}
	}

	if( !strcmp(mask, "*") ) {
		s->mask = ~0;
	} else {
		s->mask = resolve(mask);
	}

	if( !strcmp(ipa, "*") ) {
		s->ipa = ntohl(INADDR_ANY);
		s->mask = 0;
	} else {
		s->ipa = resolve(ipa);
	}

	s->how = how;
	ylPushTail( &(t->fromlist), (YLNODE *)s );
}

static	void	*tcpu_hash( void *P, char *name )
{
	return (void *) name;
}

static	int	tcpu_send( void *P, unsigned char *buffer, int count, const void *todata )
{
	struct		sockaddr_in to;
	int		nsd;
	unsigned short	port;
	char		ipa[64];

	if( sscanf((char *)todata, "%[^:]:%hu", ipa, &port) != 2 ) {
		qvPanic("Cannot process TCP-U address <%s>", (const char *)todata );
	}

	if( (nsd = socket( PF_INET, SOCK_STREAM, 0 )) < 0 ) {
		errorabort("socket");
	}
	
	memset( &to, 0, sizeof(to) );
	to.sin_family = AF_INET;
	to.sin_port = htons( port );
	to.sin_addr.s_addr = resolve(ipa);

	if( connect( nsd, (struct sockaddr *)&to, sizeof(to) ) < 0 ) {
		errorabort("connect");
	}

	if ( send( nsd, (char *)buffer, count, 0 ) < 0 ) {
		errorabort("send");
	}

	close(nsd);
	return 0;
}

static	const LSOCK tcpu_methods = {
	tcpu_newsocket,
	tcpu_waitfor,
	tcpu_find,
	tcpu_peername,
	tcpu_receive,
	tcpu_cleanup,
	tcpu_addsearch,
	tcpu_hash,
	tcpu_send
};

/** The `CSPL' API Handling Method ******************************************/
typedef	struct	{
	unsigned int	size;
} _TY_cspl;

static	void	*cspl_newstyle(char *data)
{
	_TY_cspl	*t = (_TY_cspl *)qvSysMalloc( sizeof(_TY_cspl) );
	unsigned int	size = 0;

	if( data ) {
		size = atoi(data);
	}

	if( size ) {
		t->size = SH_HDRSIZE + size;
	} else {
		t->size = SH_HDRSIZE + 56*1024;
	}

	return (void *)t;
}

static	unsigned int	cspl_approve( void *P, unsigned char *buffer, unsigned int count, LINFO *info, LBUF *bd, const char *peer )
{
	unsigned int	payloadlen, paramlen;

	if( count < SH_HDRSIZE ) {
		return SH_HDRSIZE;
	}

	paramlen = SH_GETATTRSIZE( buffer );
	payloadlen = SH_GETPAYLOADSIZE( buffer );

	if( count < (SH_HDRSIZE + paramlen + payloadlen) ) {
		return (SH_HDRSIZE + paramlen + payloadlen);
	}
	
	info->from = SH_GETSOURCE( buffer );
	info->to = SH_GETDESTINATION( buffer );
	info->paramlen = paramlen;
	info->payloadlen = payloadlen;
	info->priority = SH_GETPRIORITY( buffer );

	bd->buffer = buffer + SH_HDRSIZE;
	bd->size = paramlen + payloadlen;

	return 0;
}

static	void	cspl_space( void *P, unsigned int *headroom, unsigned int *space )
{
	_TY_cspl	*t = (_TY_cspl *)P;

	*headroom = SH_HDRSIZE;
	*space = t->size;
}

static	const char	*cspl_prepare( void *P, unsigned char *buffer, unsigned int size, const LINFO *info, LBUF *bd )
{
	unsigned char	*header = buffer - SH_HDRSIZE;

	bd->buffer = header;
	bd->size = size + SH_HDRSIZE;

	SH_SETATTRSIZE(header, info->paramlen); 
	SH_SETPAYLOADSIZE(header, info->payloadlen); 
	SH_SETSOURCE(header, info->from); 
	SH_SETDESTINATION(header, info->to); 
	SH_SETPRIORITY(header, info->priority); 

	return 0;
}

static	const LSTYLE cspl_methods = {
	cspl_newstyle,
	cspl_approve,
	cspl_space,
	cspl_prepare
};

/** The `CV' API Handling Method *****88*************************************/
typedef	struct	{
	unsigned int	size;
} _TY_cv;

static	void	*cv_newstyle(char *data)
{
	_TY_cv	*t = (_TY_cv *)qvSysMalloc( sizeof(_TY_cv) );
	unsigned int	size = 0;

	if( data ) {
		size = atoi(data);
	}

	if( size ) {
		t->size = size;
	} else {
		t->size = 56*1024;
	}

	return (void *)t;
}

static	void	cv_space( void *P, unsigned int *headroom, unsigned int *space )
{
	_TY_cv	*t = (_TY_cv *)P;

	*headroom = 0;
	*space = t->size;
}


static	unsigned int	cv_approve( void *P, unsigned char *buffer, unsigned int count, LINFO *info, LBUF *bd, const char *peer )
{
	unsigned int	size;
	STACKAPIHDR	H;

	if( count < CV_HDRSIZE ) {
		return CV_HDRSIZE;
	}

	size = cvApiParseHeader( buffer, count, &H );

	if( count < size ) {
		return size;
	}
	
	info->from = H.from;
	info->to = H.to;
	info->paramlen = H.paramlen + CV_HDRSIZE;
	info->payloadlen = H.payloadlen;
	info->priority = H.priority;

	bd->buffer = buffer;
	bd->size = size;

	return 0;
}

static	const char	*cv_prepare( void *P, unsigned char *buffer, unsigned int size, const LINFO *info, LBUF *bd )
{
	bd->buffer = buffer;
	bd->size = size;
	return 0;
}

static	const LSTYLE cv_methods = {
	cv_newstyle,
	cv_approve,
	cv_space,
	cv_prepare
};

/** Method Lists ************************************************************/
#define	MAXMETHOD	64
static	LPAIR	sockmethods[MAXMETHOD] = {
#ifdef UNIXDOMAIN
	{"unix", &unix_methods},
#endif
	{"udp", &udp_methods},
	{"tcps", &tcps_methods},
	{"tcpc", &tcpc_methods},
	{"tcpu", &tcpu_methods},
	{0, 0}
};

static	LPAIR	apimethods[MAXMETHOD] = {
	{"cspl", &cspl_methods},
	{"cv", &cv_methods},
	{0, 0}
};

/** Utilities ***************************************************************/
static	void	__registerstyle( const char *name, const LSTYLE *style )
{
	LPAIR	*P;
	int	i;

	for( i=0, P = apimethods; P->name; i++, P++ ) {
		if( !strcmp(P->name, name) ) {
			qvPanic("Method %s exists", name );
			return;
		}
	}

	if( i < MAXMETHOD ) {
		P->name = name;
		P->handler = (const void *)style;
	} else {
		qvPanic("Method %s cannot be added -- too many methods", name );
	}
}

static	const void	*search( const LPAIR *set, const char *name )
{
	const LPAIR	*P;

	for( P = set; P->name; P++ ) {
		if( !strcmp(P->name, name) ) {
			return P->handler;
		}
	}
	return 0;
}

static	void	errorabort( const char *s )
{
	char	*e = strerror(errno);
	qvPanic("%s: %s", s, e );
}

/** Listener Methods ********************************************************/
static	void	s_timedwait( void *S, const QTIME *T )
{
	LISTENER	*u = (LISTENER *)S;
	struct timeval
		timeout;
	int	n;
	fd_set	F;
	LENTRY	*L;

	FD_ZERO( &F );
	for( L = (LENTRY *)ylFirst( &(u->list) ); L; L = (LENTRY *)ylNext( (YLNODE *)L ) ) {
		FD_SET( L->handler->waitfor(L->data), &F );
	}
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
	int		n;
	struct timeval
		zero;
	fd_set	F;
	unsigned int
		attrsize, payloadsize;
	LENTRY	*L;

	zero.tv_sec = zero.tv_usec = 0;
	FD_ZERO( &F );
	for( L = (LENTRY *)ylFirst( &(u->list) ); L; L = (LENTRY *)ylNext( (YLNODE *)L ) ) {
		FD_SET( L->handler->waitfor(L->data), &F );
	}

	n = select( FD_SETSIZE - 1, &F, 0, 0, &zero );

	if( n < 0 ) {
		errorabort("select");
	}

	for( L = (LENTRY *)ylFirst( &(u->list) ); L; L = (LENTRY *)ylNext( (YLNODE *)L ) ) {
		if( FD_ISSET( L->handler->waitfor(L->data), &F) ) {
			int		size;
			void		*buffer;
			LAPI		*A;
			LINFO		info;
			LBUF		bd;
			unsigned char	*temporary;
			unsigned int	headroom = 0,
					space;
			const char	*peer;

			A = L->handler->find(L->data);
			peer = L->handler->peername(L->data);
			A->operations->space( A->data, &headroom, &space );
			temporary = (unsigned char *)qvAlloc( headroom + space, 0 );

			size = 0;

			for( ; ; ) {
				n = A->operations->approve(A->data, temporary + headroom, size, &info, &bd, peer); 
				if( n == 0 ) {
					break;
				} 
				size = L->handler->receive(L->data, temporary + headroom, n, size);
				if( size < 0 ) {
					break;
				}
			}

			L->handler->cleanup(L->data);
			if ( size < 0 ) {
				qvFree( temporary );
				continue;
			}

			*sender = qvGetService(info.from); 
			*receiver = qvGetService(info.to);
			attrsize = info.paramlen;
			payloadsize = info.payloadlen;
			*priority = info.priority;

			buffer = qvMsgAllocFromModule(*receiver, attrsize);

			memcpy( buffer, bd.buffer, attrsize );
			qvMsgInsert( buffer, 0, bd.buffer + attrsize, payloadsize );
			qvFree( temporary );

			return buffer;
		}
	}

	return 0;
}

static	void	s_send( void *S, const void *message, QMODULE sender, QMODULE receiver, void *data, unsigned int size )
{
	LSEND		*L = (LSEND *)data;
	unsigned int	payloadsize;
	unsigned int	headroom, space;
	unsigned char	*temporary;
	LBUF		bd;
	LINFO		info;
	const char	*newdest;

	payloadsize = qvMsgSize(message, 0);

	L->how->api->operations->space( L->how->api->data, &headroom, &space );
	temporary = (unsigned char *)qvAlloc( headroom + space, 0 );
	memcpy( temporary + headroom, message, size );
	qvMsgExtract( message, 0, temporary + headroom + size, payloadsize );

	info.paramlen = size;
	info.payloadlen = payloadsize;
	info.priority = qvPriority( message );
	info.from = qvGetServiceName( sender );
	info.to = qvGetServiceName( receiver );
	newdest = L->how->api->operations->prepare( L->how->api->data, temporary + headroom, payloadsize + size, &info, &bd );

	L->how->socket->handler->send( L->how->socket->data, bd.buffer, bd.size, newdest?newdest:L->data );
	qvFree( temporary );
}

static void *s_open( void *data )
{
	LISTENER	
		*L = (LISTENER *)qvSysMalloc(sizeof(LISTENER));
	char	*string = (char *)data;
	char	*s, *t, *u, *v;
	const LSOCK	
		*ls;
	LENTRY	*le;
	const LSTYLE	
		*style;
	LAPI	*la;
	LNAMED	*ln;
	const LPAIR	*p = esGetApiStyles();
	
	if( p ) {
		for( ; p->name; p++ ) {
			__registerstyle( p->name, (const LSTYLE *)(p->handler) );
		}
	}

	qvPrintf("Registered socket handler methods: ");
	for( p = sockmethods; p->name; p++ ) {
		qvPrintf("%s ", p->name);
	}
	qvPrintf("\n");
	qvPrintf("Registered API handler methods: ");
	for( p = apimethods; p->name; p++ ) {
		qvPrintf("%s ", p->name);
	}
	qvPrintf("\n");

	ylInit( &(L->list) );
	ylInit( &(L->namelist) );

	s = string;

	for( ; *s; ) {
		char	*name,
			*param,
			*process,
			*cond,
			*how,
			*howparam,
			*called;
		int	n;

		s += strspn( s, " \t" );	
		if( *s == 0 ) {
			break;
		}

		n = strcspn( s, " \t" );
		t = s; 
		s += n;
		if( *s ) {
			*s = 0;
			s++;
		}

		name = t;
		u = strchr(t, '(');
		if( !u ) {
			qvPanic( "Error processing <%s> -- expecting name", t );
			return 0;
		} 
		*u = 0;
		param = t = u + 1;
		if( *param == 0 ) {
			param = 0;
		}

		u = strchr(t, ')');
		if( !u ) {
			qvPanic( "Error locating for parameter <%s>", name );
			return 0;
		} 
		*u = 0;
		process = t = u + 1;
		
		qvPrintf("Listener method: %s\n", name );
		qvPrintf("    Bound to: %s\n", param?param:"(none)" );

		if( (ls = (const LSOCK *)search( sockmethods, name)) == 0 ) {
			qvPanic( "Socket handling method <%s> is not implemented", name );
			return 0;
		}

		le = (LENTRY *)qvSysMalloc( sizeof(LENTRY) );
		le->handler = ls;
		le->data = ls->newsocket( param );
		ylPushTail( &(L->list), (YLNODE *)le );

		for( u = t; u; ) {
			cond = t;
			u = strchr(t, ';');
			if( u ) {
				*u = 0;
				t = u + 1;
			} 

			if( *cond == 0 ) {
				continue;
			}


			v = strchr(cond, '|');
			if( !v ) {
				qvPanic( "Error locating API handler for <%s:%s>", name, cond );
				return 0;
			}
			*v = 0;
			how = v + 1;

			v = strchr(how, '=');
			if( !v ) {
				called = 0;
			} else {
				*v = 0;
				called = v + 1;
			}

			v = strchr(how, '(');
			if( !v ) {
				howparam = 0;
			} else {
				*v = 0;
				howparam = v + 1;
				v = strchr(howparam, ')');
				if( v ) {
					*v = 0;
				} else {
					qvPanic( "Error locating parameters for <%s:%s:%s>", name, cond, how );
					return 0;
				}
			}

			qvPrintf("    For messages from %s %s%s%s\n", cond, called?"[":"", called?called:"", called?"]":"");
			qvPrintf("        Process with %s (%s)\n", how, howparam?howparam:"" );

			if( (style = (const LSTYLE *)search(apimethods, how)) == 0 ) {
				qvPanic( "API handling method <%s> is not implemented", how );
				return 0;
			}

			la = (LAPI *)qvSysMalloc( sizeof(LAPI) );
			la->operations = style;
			la->data = style->newstyle?style->newstyle( howparam ):0;
			ls->addsearch( le->data, cond, la );

			if( called ) {
				ln = (LNAMED *)qvSysMalloc(sizeof(LNAMED));
				ln->name = called;
				ln->api = la;
				ln->socket = le;
				ylPushTail( &(L->namelist), (YLNODE *)ln );
			}
		}
	}

	return L;
}

static void s_close( void *data )
{
}

static void *s_hash( void *data, void *what )
{
	LISTENER	*L = (LISTENER *)data;
	char		*name = (char *)what,
			*param = 0,
			*s;
	LSEND		*D;
	LNAMED		*N;

	s = strchr(name, '(');
	if( s ) {
		*s = 0;
		s++;
		param = s;
		s = strchr(param, ')');

		if( !s ) {
			qvPanic( "Error processing destination <%s>", name );
			return 0;
		} else {
			*s = 0;
		}
		
	}

	for( N = (LNAMED *)ylFirst(&(L->namelist)); N; N = (LNAMED *)ylNext((YLNODE *)N) ) {
		if( !strcmp( N->name, name ) ) {
			D = (LSEND *)qvSysMalloc( sizeof(LSEND) );
			D->how = N;
			D->data = (void *)param;
			return D;
		}
	}

	qvPanic( "Cannot find server port <%s>", name );
	return 0;
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

