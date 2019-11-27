/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>

#include <assert.h>

#include <ylib.h>
#include <util.h>

static const int	yes = 1, no = 0;

static void	err(int C, char **V) {
	errprintf( "Invalid Command: " );
	errprintf( "%s", V[0] );
	if( C > 1 ) {
		int	i;

		for( i=1; i<C; i++ ) {
			errprintf( " <%s>", V[i] );
		}
	}
	errprintf("\n");
}

/* How to help ... */
static void	help(command_t *cmdlist) {
	int	i;

	for( i=0; cmdlist[i].name; i++ ) {
		errprintf(
			"%12s     %s\n", 
			cmdlist[i].name, 
			cmdlist[i].helptext );
	}	
}

/* How to log command execution */
static void	logcommand(int C, char **V) {
	/* Default - Nothing ... */
}

/* How to run scripts ... */
static void	script(char *filename, command_t *cmdlist) {
	FILE	*fp;

	fp = fopen( filename, "r" );
	if( fp == 0 ) {
		perror( filename );
	} else {
		char	line[512];
		while( fgets(line, sizeof(line), fp) ) {
			char	*s;

			/* Eat any ending newline */
			while ((s = strrchr(line,'\n')) != 0){
				*s = 0;
				commandprocess( line, cmdlist );
			}
		}

		fclose( fp );
	}
}

/* Process a command ... */
void	commandprocess( char *buffer, command_t *cmdlist ) 
{
	char	*args[512];
	keyval_t
		named[512];
	char	*argument;
	int	argcount,
		namedcount,
		i;
	char	*save, *key;

	namedcount = argcount = 0;
	argument = tokenize( buffer, " \t", 0, 0, 0, '#', 0, 0, &save );
	if( argument ) {
		while( argument ) {
			args[argcount] = argument;
			argcount++;
			for( ; ; ) {
				argument = tokenize( 0, " \t", '"', 0, '\\', '#', '=', &key, &save );
				if( !argument ) {
					break;
				}
				if( key ) {
					int j;

					if( *key == 0 ) {
						errprintf("Invalid key-value pair\n");
						return;
					}
					for( j=0; j<namedcount; j++ ) {
						if( !strcmp(key, named[j].key) ) {
							errprintf("Duplicate key <%s>\n", key);
							return;
						}
					}
					named[namedcount].key = key;
					named[namedcount].value = argument;
					namedcount++;
				} else {
					break;
				}
			}
		}

		if( argcount == 0 ) {
			/* No arguments ... */
		} else if( !strcmp( (char *)args[0], "?" ) ) {
			/* Help ... */
			help(cmdlist);
		} else if ( args[0][0] == '@' ) {
			/* Scripting ... */
			char	*filename;
			if ( args[0][1] ) {
				filename = &(args[0][1]);
			} else if (argcount > 1) {
				filename = args[1];
			} else {
				filename = 0;
			}
				
			if( filename ) {
				script( filename, cmdlist );
			} else {
				errprintf( "Filename required\n" ); 
			}
		} else {
			/* Not a builtin, search the commandlist ... */
			for( i=0; cmdlist[i].name; i++ ) {
				if( !strcmp(cmdlist[i].name, (char *)args[0]) )
					break;
			}
			if( cmdlist[i].name == 0 ) {
				err( argcount, args );
			} else {
				if( cmdlist[i].func ) {
					logcommand( argcount, args );
					(*(cmdlist[i].func))( argcount, args, namedcount, named );
				}
			}
		}
	}
}

/* Read in hexadecimal bytes from string ... */
int	hexbuf( char *D, unsigned char *buffer, int buflen ) {
	int	i = 0,
		n = 0,
		len = strlen(D);
	char	local[3];

	while( (i < len) && (n < buflen) ) {
		unsigned int	c;
		int		ret;

		local[0] = D[i++];
		local[1] = D[i++];
		local[2] = 0;

		ret = sscanf( local, "%x", &c );
		if( ret != 1 ) {
			break;
		}

		buffer[n] = c;
		n++;
	}

	return n;
}

/* Dump bytes in hexadecimal and character ... */
void	dump( unsigned char *D, unsigned long count, int (*printf)(char *, ...) ) 
{
	unsigned long	i,
			base = 0;

	(*printf)(  "-------- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- ----------------\n" );
	for( ; base+16 < count; base += 16 ) {
		(*printf)(  "%08lX ", base );

		for( i=0; i<16; i++ ) {
			(*printf)(  "%02X ", D[base+i] );
		}

		for( i=0; i<16; i++ ) {
			(*printf)(  "%c", isprint(D[base+i])?D[base+i]:'.' );
		}
		(*printf)(  "\n" );
	}

	(*printf)(  "%08lX ", base );
	for( i=base; i<count; i++ ) {
		(*printf)(  "%02X ", D[i] );
	}
	for( ; i<base+16; i++ ) {
		(*printf)(  "   " );
	}
	for( i=base; i<count; i++ ) {
		(*printf)(  "%c", isprint(D[i])?D[i]:'.' );
	}
	for( ; i<base+16; i++ ) {
		(*printf)(  " ");
	}
	(*printf)(  "\n" );
}

/* Set a flag ... */
void	flagset( int N, char **V, int *flagptr) {
	if ( N == 1 ) {
		errprintf( "%s\n", (*flagptr)?"ON":"OFF" );
		return;
	} else {
		if( !strcmp(V[1], "on") )
			*flagptr = 1;
		else if ( !strcmp(V[1], "off") )
			*flagptr = 0;
	}
}

/* Set a level ... */
void	levelset( int N, char **V, int *flagptr) {
	if ( N == 1 ) {
		errprintf( "%d\n", (*flagptr) );
		return;
	} else {
		if ( sscanf( V[1], "%d", flagptr ) != 1) {
			errprintf( "Invalid Value");
		}
	}
}

/* Utility: An 'strtok' on steroids */
char *tokenize(char *string, char *delim, char sb, char se, char sx, char cc, char ck, char **pkey, char **plast ) 
{
	register char 	*spanp;
	register int 	c, sc;
	char 		*tok;
	char 	*s;

	if( pkey ) {
		*pkey = 0;
	}

	if (string) {
		/* An input string has been provided */
		s = string;
	} else if (plast) {
		/* No string has been provided, but a state has been provided */
		s = *plast;
	} else 	{
		return 0;
	}

	/* `s' is now initialized  */
	if (s == 0 ) {
		return (0); /* No place to start ...  */
	}

	/* Skip (span) leading delimiters (s += strspn(s, delim), sort of). */
	while((c = *s++) != 0) {
		if( ck && (c==ck) ) {
			/* A logical `=' all by itself! */
			s[-1] = 0;
			if( pkey ) {
				*pkey = s - 1;
			}
		}

		for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
			if (c == sc) {
				break;
			}
		}

		if (sc == 0) {
			/* Non-delimiter detected */
			break;
		}
	}

	if ( (c==0) || (cc && (c==cc)) ) {		
		/* No non-delimiter characters */
		s = 0;
		if(plast) {
			*plast = s;
		}
		return (0);
	}

	for( ; ; ) {
		if( sb && (c==sb) ) {
			/* Detected string begin, pointer now on the character AFTER the string-begin */
			tok = s;

			while( *s && (*s != (se?se:sb)) ) {
				if( sx && (*s == sx) ) {
					char	*p = tok;

					while( p != s) {
						*(p+1) = *p;
						p++;
					}
					tok++;
					s++;
					if (*s == 0 ) {
						/* skipping the escape character has brought */
						/* us to the end of the string               */
						s = 0;
						if(plast)
							*plast = s;
						return( tok );
					}
				}
				s++;
			}
	
			if (*s == 0 ) {
				/* Reached the end of the string without hitting the delimiter */
				s = 0;
				if(plast) {
					*plast = s;
				}
				return( tok );
			} else {
				/* Pointer is sitting on the string-end character */
				*s = 0;
				s++;		/* Advance to next character */
				if(plast) {
					*plast = s;
				}
				return( tok );
			}
		} else {
			/* Normal `strtok' like processing */
	
			/* We have overstepped the begining of the token, so back up one ... */
			tok = s - 1;
	
	 		/* Scan token (scan for delimiters: s += strcspn(s, delim), sort of) */
	 		/* Note that delim must have one NUL; we stop if we see that, too.   */
	
			for ( ; ; ) {
				c = *s++;
				if ( ck && (c == ck) ) {
					s[-1] = 0;
					if( pkey ) {
						/* Note - if `*pkey' is non-zero, we have an error */
						/* Handled by setting key to empty                 */
						if( *pkey == 0 ) {
							*pkey = tok;
						} else {
							**pkey = 0;
						}
					}
					break;
				}

				spanp = (char *)delim;
				do {
					sc = *spanp++;
					if ( (c==sc) || (cc && (c==cc)) ) {
						if (c == 0) {
							s = 0;
						} else if ( cc && (c==cc) ) {
							s[-1] = 0;
							s = 0;
						} else {
							s[-1] = 0;
						}

						if(plast) {
							*plast = s;
						}
						return (tok);
					} 
				} while (sc != 0);
			}
		}

		c = *s++;
	}
}

char	*keylookup( char *key, int nC, keyval_t *nV )
{
	int	i;

	for( i=0; i<nC; i++ ) {
		if( !strcasecmp( nV[i].key, key ) ) {
			return nV[i].value;
		}
	}
	return 0;
}

/* Print errors with any number of arguments ... */
int	errprintf( char *format, ... ) 
{
	va_list	A;
	int	i;

	va_start( A, format );
	i = vfprintf( stderr, format, A );
	va_end( A );
	return i;
}

#if defined(__sun__) || defined(__linux) || defined(__CYGWIN__)
#define	NAMERESOLVE
#endif

void	errorabort( const char *s )
{
	char	*e = strerror(errno);
	errprintf("%s: %s\n", s, e );
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

void	makeaddress(const char *s, struct sockaddr_in *address, unsigned long defaultaddress )
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
 
void	printaddress( char *s, const struct sockaddr_in *address )
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

