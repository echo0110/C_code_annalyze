/*******************************************************************************
* File Name   :qval.c
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
#include <sys/types.h>
#include <regex.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <regex.h>

#include "qutil.h"
#include "qval.h"
#include "qcli.h"

static void trimwhite( char *s )
{
	int	n;

	while( ((n = strlen(s)) != 0) && isspace((int)(s[n-1])) ) {
		s[n-1] = 0;
	}
}

static char *lowercase( char *s )
{
	int	i;

	for( i=0; s[i]; i++ ) {
		s[i] = isupper( (int) s[i]) ? tolower( (int) s[i]) : s[i];
	}

	return s;
}

static char *uppercase( char *s )
{
	int	i;

	for( i=0; s[i]; i++ ) {
		s[i] = islower( (int) s[i]) ? toupper( (int) s[i]) : s[i];
	}

	return s;
}

typedef	struct {
	char	*W,
		*F;
	int	E,
		negative;
}	number_t;

static void parse( const char *A, number_t *V )
{
	const char
		*s = A, *t;
	int	n;

	if( *s == '-' ) {
		s++;
		V->negative = 1;
	} else {
		V->negative = 0;
	}

	s += strspn( s, "0" );
	if( *s == 0 ) {
		/* When dealing with integers, `-0' is same as `0' */
		V->negative = 0;
	}

	t = s;
	s += strspn( s, "0123456789" );

	n = s - t;
	V->W = (char *) malloc( n + 1 );
	strncpy( V->W, t, n );
	V->W[n] = 0;


	if( *s == '.' ) {
		s++;
		t = s;
		s += strspn( s, "0123456789" );

		n = s - t;
		V->F = (char *) malloc( n + 1 );
		strncpy( V->F, t, n );
		V->F[n] = 0;
	} else {
		V->F = strdup( "" );
	}

	if( (*s == 'E') || (*s == 'e') ) {
		s++;
		V->E = atoi( s );
	} else {
		V->E = 0;
	}
}

static void normalize( number_t *V )
{
	char	*s, *t;

	if( !V->E ) {
		return;
	}

	if( V->E < 0 ) {
		int	n = -V->E,
			k = strlen(V->W);

		if( n > k ) {
			int	q = n - k, i;

			s = (char *)malloc( q + 1 );

			for( i=0; i<q; i++ ) {
				s[i] = '0';
			}
			s[q] = 0;

			s = qasprintf( s, "%s%s", V->W, V->F );
			V->W[0] = 0;
		} else {
			s = strdup( V->W + (k - n) );

			s = qasprintf( s, "%s", V->F );
			V->W[k-n] = 0;
		}

		free(V->F);
		V->F = s;
	} else {
		int	i,
			n = V->E,
			k = strlen(V->F),
			m = n + strlen(V->W);

		s = (char *) malloc( m + 1 );

		i = 0;
		t = V->W;
		while( *t ) {
			s[i++] = *t++;
		}

		t = V->F;
		while( (i < m) && *t ) {
			s[i++] = *t++;
		}

		while( i < m ) {
			s[i++] = '0';
		}

		s[i] = 0;

		free( V->W );
		V->W = s;
		if( n < k ) {
			t = strdup( V->F + (k - n) );
			free( V->F );
			V->F = t;
		} else {
			V->F[0] = 0;
		}
	}
}

static void expand( number_t *V, unsigned int precision )
{
	if( strlen(V->F) < precision ) {
		unsigned int	
			i = 0;
		char	*s = V->F, *t;

		t = (char *) malloc(precision + 1);
		while( *s ) {
			t[i++] = *s++;
		}

		while( i < precision ) {
			t[i++] = '0';
		}

		t[i] = 0;

		free( V->F );
		V->F = t;
	}
}

static int compare( const char *a, const char *b )
{
	int	r = 0;
	number_t
		A, B;

	parse( a, &A );
	parse( b, &B );

	normalize( &A );
	normalize( &B );

	if( strlen(A.F) > strlen(B.F) ) {
		expand(&B, strlen(A.F));
	} else {
		expand(&A, strlen(B.F));
	}

	if( A.negative != B.negative ) {
		r = A.negative ? -1 : 1;
	} else {
		int	nA = strlen(A.W), nB = strlen(B.W);
		if( nA == nB ) {
			r = strcmp(A.W, B.W);

			if( !r ) {
				r = strcmp(A.F, B.F);
			} 
		} else {
			r = (nA > nB) ? 1 : -1;
		}

		if( A.negative ) {
			r = -r;
		}
	}

	free( A.W );
	free( A.F );
	free( B.W );
	free( B.F );

	return r;
}

static int	casematch(const char *s, const char *t)
{
	while( *s && (tolower(*s) == tolower(*t)) ) {
		s++;
		t++;
	}

	if( *s == *t ) {
		return 1;
	} else {
		return 0;
	}
}

static int patternmatch( const char *pattern, const char *s, int ignorecase )
{
	regex_t R;
	int n;

	if( regcomp( &R, pattern, REG_EXTENDED|REG_NOSUB|(ignorecase?REG_ICASE:0) ) != 0 ) {
		return 0;
	}

	n = regexec( &R, s, 0, 0, 0 );
	regfree( &R );
	return (n == 0) ? 1 : 0;
}

static char *validatenumber( const char *value, const char *range )
{
	char	*s, *t, *r, *min, *max;

	s = strdup(range);
	if( patternmatch( "^-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?:-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?$", range, 1 ) ) {
		min = s;
		t = strchr( s, ':' );
		*t = 0;
		max = t + 1;

		if( (compare(value, min) < 0) || (compare(value, max) > 0) ) {
			r = 0;
		} else {
			r = strdup(value);
		}
	} else if( patternmatch( 
			"^-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?"
			"[ \t]*\\.\\."
			"[ \t]*-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?$", range, 1 ) ) {
		min = s;
		t = strstr( s, ".." );
		*t = 0;
		trimwhite(min);
		max = t + 2;
		max += strspn(max, " \t");

		if( (compare(value, min) < 0) || (compare(value, max) > 0) ) {
			r = 0;
		} else {
			r = strdup(value);
		}
	} else if( patternmatch( 
			"^-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?"
			"[ \t]*\\.\\."
			"[ \t]*-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?"
			"[ \t]*EXCLUSIVE$", range, 1 ) ) {
		min = s;
		t = strstr( s, ".." );
		*t = 0;
		trimwhite(min);
		max = t + 2;
		max += strspn(max, " \t");
		t = strstr( max, "EXCLUSIVE" );
		*t = 0;
		trimwhite(max);

		if( (compare(value, min) <= 0) || (compare(value, max) >= 0) ) {
			r = 0;
		} else {
			r = strdup(value);
		}
	} else if( patternmatch( "^-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?:$", range, 1 ) ) {
		min = s;
		t = strchr( s, ':' );
		*t = 0;

		if( compare(value, min) < 0 ) {
			r = 0;
		} else {
			r = strdup(value);
		}
	} else if( patternmatch( "^>=[ \t]*-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?$", range, 1 ) ) {
		min = s + 2;
		min += strspn(min, " \t");

		if( compare(value, min) < 0 ) {
			r = 0;
		} else {
			r = strdup(value);
		}
	} else if( patternmatch( "^>[ \t]*-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?$", range, 1 ) ) {
		min = s + 1;
		min += strspn(min, " \t");

		if( compare(value, min) <= 0 ) {
			r = 0;
		} else {
			r = strdup(value);
		}
	} else if( patternmatch( "^:-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?$", range, 1 ) ) {
		max = s + 1;

		if( compare(value, max) > 0 ) {
			r = 0;
		} else {
			r = strdup(value);
		}
	} else if( patternmatch( "^<=[ \t]*-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?$", range, 1 ) ) {
		max = s + 2;
		max += strspn(max, " \t");

		if( compare(value, max) > 0 ) {
			r = 0;
		} else {
			r = strdup(value);
		}
	} else if( patternmatch( "^<[ \t]*-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?$", range, 1 ) ) {
		max = s + 1;
		max += strspn(max, " \t");

		if( compare(value, max) >= 0 ) {
			r = 0;
		} else {
			r = strdup(value);
		}
	} else {
		/* Malformed range value */
		r = 0;
	}

	free(s);
	return r ? lowercase(r) : 0;
}

const char __qnam_INTEGER[] = "INTEGER";
char *__qval_INTEGER( const char *value, const char *range )
{
	if( !patternmatch( "^-?[0-9]+$", value, 0 ) ) {
		return 0;
	}

	if( !range ) {
		return strdup( value );
	}

	return validatenumber( value, range );
}


const char __qnam_FLOATINGPOINT[] = "FLOATING-POINT";
char *__qval_FLOATINGPOINT( const char *value, const char *range )
{
	if( !patternmatch( "^-?[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?$", value, 1 ) ) {
		return 0;
	}

	if( !range ) {
		return lowercase(strdup(value));
	}

	return validatenumber( value, range );
}

const char __qnam_IDENTIFIER[] = "IDENTIFIER";
char *__qval_IDENTIFIER( const char *value, const char *range )
{
	if( !patternmatch( "^[A-Za-z][A-Za-z0-9_]*(-[A-Za-z0-9_]*)*$", value, 1 ) ) {
		return 0;
	}

	return lowercase(strdup(value));
}

const char __qnam_CHOICE[] = "CHOICE";
char *__qval_CHOICE( const char *value, const char *range )
{
	char	*pattern = 0, *s, *t, *u;

	if( !patternmatch( "^[A-Za-z0-9_-]+$", value, 1 ) ) {
		return 0;
	}

	if( !range ) {
		return strdup( value );
	}

	pattern = strdup( range );
	s = t = pattern;

	while( t ) {
		u = s;
		t = strchr( s, '|' );
		if( t ) {
			*t = 0;
			s = t + 1;
		}

		if( casematch(value, u) ) {
			u = strdup(u);
			free(pattern);
			return u;
		}
	}

	free(pattern);
	return 0;
}

const char __qnam_STRING[] = "STRING";
char *__qval_STRING( const char *value, const char *range )
{
	if( !range ) {
		return strdup(value);
	}

	if( patternmatch( "^PATTERN[ \t]+[`'\"].*['\"]$", range, 1 ) ) {
		char	*pattern = 0, *s;
		int	i, n;

		range += strcspn( range, " \t" );	/* Skip over `PATTERN' */
		range += strspn( range, " \t" );	/* Skip over whitespace */
		range++;	/* Skip the first quote */

		n = strlen(range);
		n--;

		pattern = (char *)malloc( n + 2 + 1 );	
		s = pattern;
		*s++ = '^';
		for( i=0; i<n; i++ ) {
			*s++ = range[i];
		}
		*s++ = '$';
		*s = 0;
		
		if( patternmatch( pattern, value, 0 ) ) {
			free(pattern);
			return strdup(value);
		} else {
			free(pattern);
			return 0;
		}
	} else if( patternmatch( "^MAXLEN[ \t]+[0-9]+$", range, 1 ) ) {
		int	length;

		range += strcspn( range, " \t" );
		range += strspn( range, " \t" );
		length = atoi( range );
		if( (int)strlen(value) > length ) {
			return 0;
		} else {
			return strdup(value);
		}
	} else {
		return 0;
	}
}

const char __qnam_HEXBUF[] = "HEX-BUFFER";
char *__qval_HEXBUF( const char *value, const char *range )
{
	char	*pattern = 0;
	int	n;

	if( !range ) {
		if( !patternmatch( "^[0-9A-F]{2}([-:]?[0-9A-F]{2})*$", value, 1 ) ) {
			return 0;
		}
	} else if( patternmatch( "^LENGTH[ \t]+[0-9]+$", range, 1 ) ) {
		range += strcspn( range, " \t" );	/* Skip over `LENGTH' */
		range += strspn( range, " \t" );	/* Skip over whitespace */
		n = atoi(range);

		pattern = (n > 1) 
			? qasprintf( 0, "^[0-9A-F]{2}([-:]?[0-9A-F]{2}){%s}$", n - 1 )
			: qasprintf( 0, "^[0-9A-F]{2}$" );

		if( !patternmatch( pattern, value, 1 ) ) {
			free(pattern);
			return 0;
		}

		free(pattern);
	} else if( patternmatch( "^MAXLEN[ \t]+[0-9]+$", range, 1 ) ) {
		range += strcspn( range, " \t" );	/* Skip over `MAXLEN' */
		range += strspn( range, " \t" );	/* Skip over whitespace */
		n = atoi(range);

		pattern = (n > 1) 
			? qasprintf( 0, "^[0-9A-F]{2}([-:]?[0-9A-F]{2}){0,%s}$", n - 1 )
			: qasprintf( 0, "^[0-9A-F]{2}$" );

		if( !patternmatch( pattern, value, 1 ) ) {
			free(pattern);
			return 0;
		}

		free(pattern);
	} else {
		return 0;
	}

	return uppercase(strdup(value));
}

const char __qnam_IP4ADDRESS[] = "IP-ADDRESS/v4";
char *__qval_IP4ADDRESS( const char *value, const char *range )
{
	unsigned int
		a, b, c, d;

	if( !patternmatch( "^[0-9]{1,3}(\\.[0-9]{1,3}){3}$", value, 0 ) ) {
		return 0;
	}

	sscanf( value, "%u.%u.%u.%u", &a, &b, &c, &d );

	if( (a > 255) || (b > 255) || (c > 255) || (d > 255) ) {
		return 0;
	}

	return qasprintf(0, "%u.%u.%u.%u", a, b, c, d);
}

static const char *__nwmapping[] = {
	"0.0.0.0",
	"128.0.0.0",
	"192.0.0.0",
	"224.0.0.0",
	"240.0.0.0",
	"248.0.0.0",
	"252.0.0.0",
	"254.0.0.0",
	"255.0.0.0",
	"255.128.0.0",
	"255.192.0.0",
	"255.224.0.0",
	"255.240.0.0",
	"255.248.0.0",
	"255.252.0.0",
	"255.254.0.0",
	"255.255.0.0",
	"255.255.128.0",
	"255.255.192.0",
	"255.255.224.0",
	"255.255.240.0",
	"255.255.248.0",
	"255.255.252.0",
	"255.255.254.0",
	"255.255.255.0",
	"255.255.255.128",
	"255.255.255.192",
	"255.255.255.224",
	"255.255.255.240",
	"255.255.255.248",
	"255.255.255.252",
	"255.255.255.254",
	"255.255.255.255",
	0
};

	
const char __qnam_IP4NETWORK[] = "IP-NETWORK/v4";
char *__qval_IP4NETWORK( const char *value, const char *range )
{
	unsigned int
		a, b, c, d, e, f, g, h;

	if( patternmatch( "^[0-9]{1,3}(\\.[0-9]{1,3}){3}/[0-9]{1,2}$", value, 0 ) ) {
		sscanf( value, "%u.%u.%u.%u/%u", &a, &b, &c, &d, &e );

		if( (a > 255) || (b > 255) || (c > 255) || (d > 255) ) {
			return 0;
		}

		if( (e > 32) ) {
			return 0;
		}

		return qasprintf(0, "%u.%u.%u.%u/%s", a, b, c, d, __nwmapping[e]);
	} else if( patternmatch( "^[0-9]{1,3}(\\.[0-9]{1,3}){3}/[0-9]{1,3}(\\.[0-9]{1,3}){3}$", value, 0 ) ) {
		sscanf( value, "%u.%u.%u.%u/%u.%u.%u.%u", &a, &b, &c, &d, &e, &f, &g, &h );

		if( (a > 255) || (b > 255) || (c > 255) || (d > 255) ) {
			return 0;
		}

		if( (e > 255) || (f > 255) || (g > 255) || (h > 255) ) {
			return 0;
		}

		return qasprintf(0, "%u.%u.%u.%u/%u.%u.%u.%u", a, b, c, d, e, f, g, h);
	}

	return 0;
}

const char __qnam_IP6ADDRESS[] = "IP-ADDRESS/v6";
char *__qval_IP6ADDRESS( const char *value, const char *range )
{
	if( patternmatch( "^[0-9A-F]{1,4}(:[0-9A-F]{1,4}){7}$", value, 1 ) ) {
		return uppercase(strdup(value));
	}

	if( patternmatch( "^[0-9A-F]{1,4}(:[0-9A-F]{1,4}){0,6}::$", value, 1 ) ) {
		return uppercase(strdup(value));
	}

	if( patternmatch( "^::[0-9A-F]{1,4}(:[0-9A-F]{1,4}){0,6}$", value, 1 ) ) {
		return uppercase(strdup(value));
	}

	if( patternmatch( "^[0-9A-F]{1,4}(:[0-9A-F]{1,4})*::[0-9A-F]{1,4}(:[0-9A-F]{1,4})*$", value, 1 ) ) {
		const char	*s;
		int	n;

		for( s = value, n = 0; *s; s++ ) {
			if( *s == ':' ) {
				n++;
			}
		}

		if( n > 7 ) {
			return 0;
		} else {
			return uppercase(strdup(value));
		}
	} else {
		return 0;
	}

	return 0;
}

const char __qnam_MACADDRESS[] = "MAC-ADDRESS";
char *__qval_MACADDRESS( const char *value, const char *range )
{
	if( !patternmatch( "^[0-9A-Fa-f]{2}([-:]?[0-9A-Fa-f]{2}){5}$", value, 0 ) ) {
		return 0;
	}

	return strdup(value);
}

const char __qnam_DOMAINNAME[] = "DOMAIN-NAME";
char *__qval_DOMAINNAME( const char *value, const char *range )
{
	if( !patternmatch( "^[A-Z0-9_-]*[A-Z][A-Z0-9_-]*(\\.[A-Z0-9_-]*[A-Z][A-Z0-9_-]*)*$", value, 1 ) ) {
		return 0;
	}

	return strdup(value);
}


static const unsigned int days[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const char __qnam_DATE[] = "DATE/Y-M-D";
char *__qval_DATE( const char *value, const char *range )
{
	unsigned int	Y, M, D;

	if( !patternmatch( "^[12][0-9]{3}-(0?[1-9]|11|12)-(0?[1-9]|1[0-9]|2[0-9]|3[01])$", value, 0 ) ) {
		return 0;
	}

	sscanf( value, "%u-%u-%u", &Y, &M, &D );
	if( D > days[M - 1] ) {
		/* `Thirty days has September...' etc */
		return 0;
	}

	if( (D == 29) && (M == 2) ) {
		/* What else? Leap Years! */
		if( Y % 4 ) {
			return 0;
		}

		if( !(Y % 100) && (Y % 400) ) {
			return 0;
		}
	}

	return qasprintf( 0, "%04u-%02u-%02u", Y, M, D );
}

const char __qnam_TIME[] = "TIME/H:M:S";
char *__qval_TIME( const char *value, const char *range )
{
	unsigned int	H = 0, M = 0, S = 0;
	if( patternmatch( "^(0?[0-9]|1[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]$", value, 0 ) ) {
		sscanf( value, "%u:%u:%u", &H, &M, &S );
	} else if( patternmatch( "^(0?[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$", value, 0 ) ) {
		sscanf( value, "%u:%u", &H, &M );
	} else {
		return 0;
	}

	return qasprintf( 0, "%02u:%02u:%02u", H, M, S );
}

const char __qnam_OID[] = "OBJECT-ID/numeric";
char *__qval_OID( const char *value, const char *range )
{
	const char	*s;
	int	n, exclude = 0;

	if( !patternmatch( "^(0|([1-9][0-9]*))(\\.(0|([1-9][0-9]*)))*$", value, 1 ) ) {
		return 0;
	}

	if( !range ) {
		return strdup( value );
	}

	
	if( patternmatch( "^BELOW[ \t]+(0|([1-9][0-9]*))(\\.(0|([1-9][0-9]*)))*$", range, 1 ) ) {
		exclude = 1;
	} else if( patternmatch( "^ROOT[ \t]+(0|([1-9][0-9]*))(\\.(0|([1-9][0-9]*)))*$", range, 1 ) ) {
		exclude = 0;
	} else {
		return 0;
	}

	s = range;
	s += strcspn( s, " \t" );
	s += strspn( s, " \t" );

	if( strstr(value, s) != value ) {
		return 0;
	}

	n = strlen(s);

	if( exclude && !value[n] ) {
		return 0;
	}

	if( value[n] && (value[n] != '.') ) {
		return 0;
	}

	return strdup( value );
}

