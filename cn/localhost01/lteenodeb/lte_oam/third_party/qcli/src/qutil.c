/*******************************************************************************
* File Name   : qutil.c
*
* Description : -- QCLI Toolkit
* Revision History: --
*
* Date        Author           Reference          Comments
* -----        ------           ---------          --------
*  2006        Avijit Ghosh       -              Initial Creation
*
* Copyright (c) 2006, Aricent Group.
*******************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <string.h>

#include "qutil.h"

/* Utility: An 'strtok' on steroids */
char *qtokenize(char *string, const char *delim, const char *terminator, int quote, int escape, char **plast, int *why ) 
{
	register int 	c;
	char 		*t, *s;

	if (string) {
		/* An input string has been provided*/
		s = string;
	} else if (plast) {
		/* No string has been provided, but a state has been provided */
		s = *plast;
	} else 	{
		/* No string, no state!*/
		s = 0;
	}

	/* `s' is now initialized */
	if (!s) {
		/* No place to start ... */
		if( plast ) {
			*plast = 0;
		}
		if( why ) {
			*why = 0;
		}
		return (0); 
	}

	/* Skip (span) leading delimiters (s += strspn(s, delim), sort of). */
	for( ; ; ) {
		c = *s++;

		if( (c == 0) || (terminator && strchr(terminator, c)) ) {
			/* No non-delimiter characters */
			if (plast) {
				/* If we are returning zero because of a terminator, flag it here */
				*plast = c?s:0;
			}

			if( why ) {
				*why = c;
			}

			return (0);
		}

		if( !strchr(delim, c) ) {
			break;
		}
	}
		
	if( quote && (c==quote) ) {
		/* Detected string begin, pointer now on the character AFTER the string-begin */
		t = s;

		while((c = *s++) != quote) {
			if( escape && (c == escape) ) {
				char	*p = t;

				while( p != (s-1)) {
					*(p+1) = *p;
					p++;
				}
				t++;

				/* Now pointing to whatever the 'escape' was protecting */
				if( *s ) {
					c = *s++;
				}
			}

			if( c == 0 ) {
				/* Didn't find the quote */
				break;
			}
		}

		if (c == 0) {
			/* Reached the end of the string without hitting the end quote */
			if (plast) {
				*plast = 0;
			}
			if (why) {
				*why = 0;
			}
			return( t );
		} else {
			/* Found end quote */
			s[-1] = 0;

			/* Already on next character */
			if (plast) {
				*plast = s;
			}
			if (why) {
				*why = quote;
			}
			return( t );
		}
	} else {
		/* We have overstepped the begining of the token, so back up one ... */
		s--;
		t = s;

		/* Normal `strtok' like processing 
	 	** Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 	** Note that delim must have one NUL; we stop if we see that, too.
		*/

		for ( ; ; ) {
			int	escaped;

			c = *s++;
			escaped = 0;

			if( escape && (c == escape) ) {
				char	*p = t;

				while( p != (s-1)) {
					*(p+1) = *p;
					p++;
				}
				t++;
				escaped = 1;

				if( *s ) {
					c = *s++;
				}
			}

			if (c == 0) {
				if(plast) {
					*plast = 0;
				}

				if( why ) {
					*why = 0;
				}

				return t;
			} 

			if (!escaped && terminator && strchr(terminator, c)) {
				s[-1] = 0;
				s--;	/* So next time we get "" -- and 'why' will flag the terminator */

				if(plast) {
					*plast = s;
				}

				if( why ) {
					*why = c;
				}

				return t;
			} 

			if (!escaped && strchr(delim, c)) {
				s[-1] = 0;

				if(plast) {
					*plast = s;
				}

				if( why ) {
					*why = c;
				}

				return t;
			}

			/* Otherwise, continue... */
		}
	}
}

char	*qvasprintf( char *to, const char *format, va_list A, va_list B )  /* 64bit Fix */
{
	char	z;
	int	n;
	char	*s, *t;
	
	n = vsnprintf( &z, 1, format, B );

	if( to ) {
		s = (char *) realloc( to, strlen(to) + n + 1 );
		t = s + strlen(s);
	} else {
		t = s = (char *)malloc( n + 1 );
	}

	if( !s ) {
		return 0;
	}

	vsnprintf( t, n + 1, format, A );

	return s;
}

char	*qasprintf( char *to, const char *format, ... )
{
	char	*s;
	va_list	A, B; /* 64bit Fix */

	va_start( A, format );
	va_start( B, format );
	s = qvasprintf( to, format, A, B ); /* 64bit Fix */
	va_end( A );
	va_end( B );  /* 64bit Fix */

	return s;
}

char	*qfgetline( FILE *F )
{
	char	*s = 0;
	int	size = 0, i = 0, c;

	if( feof(F) || ((c = getc(F)) == EOF) ) {
		return 0;
	}

	s = (char *) malloc( size + 1 );
	s[size] = 0;

	while( (c != EOF) && (c != '\n') ) {
		while( (i + 1) > size ) {
			size = size?(2*size):1;
			s = (char *) realloc( s, size + 1 );
		}
		s[i++] = c;
		c = getc(F);
	}

	s[i] = 0;
	return s;
}

char	**qslalloc( void )
{
	char	**Q = (char **)malloc( sizeof(char *) );
	
	Q[0] = 0;
	return Q;
}

void	qslfree( char **Q )
{
	char	**Z = Q;

	while( *Z ) {
		free( *Z );
		Z++;
	}

	free( Q );
}

int	qsllen( char **Q )
{
	int	i = 0;

	while( *Q ) {
		Q++;
		i++;
	}

	return i;
}

char	**qslstr( char **Q, const char *s )
{
	while( *Q ) {
		if( !strcmp( *Q, s ) ) {
			return Q;
		}

		Q++;
	}

	return 0;
}

char	**qsladdstr( char **Q, char *s )
{
	int	size = qsllen( Q );

	Q[size] = s;

	size++;
	Q = (char **) realloc( Q, (size + 1) * sizeof(char *) );
	Q[size] = 0;
	return Q;
}

char	**qsladdstrcopy( char **Q, const char *s )
{
	return qsladdstr( Q, strdup(s) );
}

char	**qsljoin( char **Q, char **Z )
{
	char **Y = Z;

	while( *Y ) {
		Q = qsladdstr( Q, *Y );
		Y++;
	}

	free( Z );
	return Q;
}

char	*qsllookup( char **Q, const char *const *Z, const char *s )
{
	const char *const *Y;
	char	**P;

	if( !Z ) {
		return 0;
	}

	for( Y=Z, P=Q;  *Y && *P; Y++, P++ ) {
		if( !strcmp( *Y, s ) ) {
			return *P;
		}
	}

	return 0;
}

