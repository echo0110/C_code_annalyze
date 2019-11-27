/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Utility Functions for IPC Drivers (should be included)          **/
/**                                                                        **/
/****************************************************************************/

#ifndef	___CsplShellUtilitiesStaticFunctions___
#define	___CsplShellUtilitiesStaticFunctions___

#include	<cspl.h>
#include	<shell.h>

#if defined(__sun__) || defined(__linux) || defined(__CYGWIN__)
#define	NAMERESOLVE
#endif

static const int yes = 1, no = 0;

static	void	errorabort( const char *s )
{
	char	*e = strerror(errno);
	qvPanic("%s: %s", s, e );
}

#ifdef NAMERESOLVE
static unsigned long	resolve( char *name )
{
	struct hostent	
		*H;
	char	**p;
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
		address->sin_port = htons((unsigned short) atoi(s) );
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

#endif
