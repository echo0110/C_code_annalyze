/*******************************************************************************
* File Name   : qcli.c
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
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#include <unistd.h>

#include <assert.h>

#include "qcli.h"
#include "qutil.h"

typedef struct	errorcontext_t {
	const char	
		*original,
		*copy;
	char	**arguments;
	int	nargs;
}	errorcontext_t;

typedef struct	state_t {
	struct state_t	
		*previous, 
		*next;

	struct	{
		char	*commands,
			*mode;
	}	context;

	struct	{
		char	**item;
		int	count;
	}	object;

	const command_t
		*tree;
}	state_t;

typedef struct result_t {
	struct result_t
		*next;
	char	*value;
}	result_t;

struct	interpret_t {
	const command_t
		*tree;

	state_t
		*state;

	result_t
		*result;

	int	(*redirect)(int, char *, void *);

	void	*arg;
};

#define	TERMINATOR	"#?>|"
#define	WHITESPACE	" \t\n"
#define	TAGSTART	"-/"


static int	errprintf( char *format, ... )
{
	va_list	A;
	int	i;

	va_start( A, format );
	i = vfprintf( stderr, format, A );
	va_end( A );

	return i;
}

static void	common( char *s, const char *t )
{
	while( *s && *t && (*s == *t) ) {
		s++;
		t++;
	}

	*s = 0;
}

static char	*join( const char *s, const char *t )
{
	if( s[0] ) {
		if( t[0] ) {
			return qasprintf(0, "%s %s", s, t );
		} else {
			return strdup( s );
		}
	} else {
		if( t[0] ) {
			return strdup( t );
		} else {
			return strdup( "" );
		}
	}
}

static void trim( char *t, const char *remove )
{
	for( ; ; ) {
		int n = strlen(t);
	
		if( !n ) {
			return;
		}

		if( !strchr(remove, t[n-1]) ) {
			return;
		}

		t[n-1] = 0;
	}
}

static const char	*any( const char *t )
{
	return t?t:"any";
}

static const char	*typehelp( const parameter_t *P )
{
	return P->generate ? "dynamic" : any(P->range);
}

static char	*quote( const char *s )
{
	int	n, d, i;
	char	*t, *u;

	if( s[0] && !strpbrk(s, " \t\n\"")  ) {
		return strdup(s);
	}

	n = strlen(s);
	d = 2;	/* The quotes */

	for( i=0; i<n; i++ ) {
		if( s[i] == '"' ) {
			d++;	/* The backslash */
		}
	}

	u = t = (char *) malloc( n + d + 1 );
        /* klockwork 12493 coverity 35443 */
        if(u){
	*u++ = '"';
	for( i=0; i<n; i++ ) {
		if( s[i] == '"' ) {
			*u++ = '\\';
		}
		*u++ = s[i];
	}
	*u++ = '"';
	*u = 0;
        }
	return t;
}

static char	*addto( char *s, const char *t )
{
	char	*u = quote(t);

	s = qasprintf( s, " %s", u );
	free( u );

	return s;
}

static int	match(const char *s, const char *t)
{
	while( *s && (tolower(*s) == tolower(*t)) ) {
		s++;
		t++;
	}

	if( *s == *t ) {
		return 1;
	} else if ( *s == 0 ) {
		return -1;
	} else {
		return 0;
	}
}

static int	matchcase(const char *s, const char *t)
{
	while( *s && (*s == *t) ) {
		s++;
		t++;
	}

	if( *s == *t ) {
		return 1;
	} else if ( *s == 0 ) {
		return -1;
	} else {
		return 0;
	}
}

static int	matchany(const char *s, ... )
{
	va_list	A;
	const char
		*t;
	int	r = 0;

	va_start( A, s );

	while( (t = va_arg(A, const char *)) != 0 ) {
		int	n = match( s, t );

		if( n > 0 ) {
			va_end(A);
			return n;
		} else if ( n < 0 ) {
			r = n;
		}
	}

	va_end(A);
	return r;
}

typedef struct	pair_s	{
	const char	
		*key;
	char	*value;
}	pair_t;

pair_t	*zlalloc( void )
{
	pair_t	*Q = (pair_t *)malloc( sizeof(pair_t) );
        /* klockwork 12494 coverity 35447 36028 36023 36019*/
        if( !Q ){
         return Q;
        }  	
	Q[0].key = Q[0].value = 0;
       
	return Q;
}

void	zlfree( pair_t *Q )
{
	pair_t	*Z = Q;

	while( Z->key || Z->value ) {
		if( Z->value ) {
			free( Z->value );
		}
		Z++;
	}

	free( Q );
}

int	zllen( pair_t *Q )
{
	int	i = 0;

	while( Q->key || Q->value ) {
		Q++;
		i++;
	}

	return i;
}

pair_t	*zladd( pair_t *Q, const char *s, char *t )
{
	int	size = zllen( Q );

	Q[size].key = s;
	Q[size].value = t;

	size++;
	Q = (pair_t *) realloc( Q, (size + 1) * sizeof(pair_t) );
        /* coverity 35446 */
        if(Q){  	
	 Q[size].key = Q[size].value = 0;
        }
	return Q;
}


static int	optional( const parameter_t *P )
{
	if( P->defaultvalue ) {
		return 1;
	} else {
		return 0;
	}
}

static int	flag( const parameter_t *P )
{
	if( P->defaultvalue && !(P->validate || P->generate) ) {
		return 1;
	} else {
		return 0;
	}
}

static int	keyword( const parameter_t *P )
{
	if( !P->defaultvalue && !(P->validate || P->generate) ) {
		return 1;
	} else {
		return 0;
	}
}

static int	tagged( const parameter_t *P )
{
	if( !P->tag || keyword(P) || optional(P) ) {
		return 0;
	} else {
		return 1;
	}
}

static int	empty( const char *P )
{
	if( P && !P[0] ) {
		return 1;
	} else {
		return 0;
	}
}

static void	freestate( state_t *S )
{
	qslfree( S->object.item );
	free( S->context.commands );
	free( S->context.mode );
	free( S );
}

static void	freeresult( result_t *Q )
{
	free( Q->value );
	free( Q );
}

static int	taglocate( const char *s, const pair_t *tags )
{
	int	j;

	for( j=0; tags[j].key; j++ ) {
		if( match(s, tags[j].key) > 0 ) {
			break;
		}
	}

	if( tags[j].key ) {
		return j + 1;
	} 

	return 0;
}

static char	*dynamic( const char *s, char **(*function)(const void *), const void *arg )
{
	char	**list = function( arg ), **t;
	char	*r = 0;
	int	count = 0, q, exact;

	if( !list ) {
		return 0;
	}

	/* Order -- exact case sensitive match, exact match, prefix match */
	for( exact=0, t=list; *t; t++ ) {
		q = match(s, *t);

		if( q > 0 ) {
			r = *t;
			if( matchcase(s, *t) > 0 ) {
				count = 1;
				break;
			} else {
				count = exact ? (count + 1) : 1;
				exact = 1;
			}
		} else if ( !exact && (q < 0) ) {
			r = *t;
			count++;
		}
	}

	if( count == 1 ) {
		r = strdup( r );
	} else {
		r = 0;
	}

	qslfree(list);
	return r;
}

static void	suggestion( const parameter_t *P )
{
	errprintf( "... should be a value of type <%s %s>\n", P->type, typehelp(P) ); 
}

static void	errorcontext( const errorcontext_t *E, const char *s, int inclusive )
{
	int	i = 0;
	const char	
		*current = 0, *previous = 0, *which;
	char	*t;

	while( (i < E->nargs) && ((current = E->arguments[i]) != s) ) {
		i++;
		previous = current;
	}

	if( i == E->nargs ) {
		return;
	}

	which = inclusive ? current : previous;

	if( !which ) {
		return;
	}

	which += strlen( which );
	i = which - E->copy;
	if( E->original[i] == '"' ) {
		i++;
	}

	t = (char *) malloc( i + 1 );
        /* klockwork 12495 12496 coverity 35440 */
        if(!t){
         return;
        }
	strncpy( t, E->original, i );
	t[i] = 0;

	trim( t, WHITESPACE );
	if( t[0] ) {
		errprintf( "... after `%s'\n", t );
	}

	free(t);
}

static const char *possibletag( const char *s )
{
	if( strstr( s, "--" ) == s ) {
		return s + strlen("--");
	}

	if( strstr( s, "/" ) == s ) {
		return s + strlen("/");
	}

	return 0;
}

static const char *howtag( const char *s )
{
	if( strstr( s, "--" ) == s ) {
		return "--";
	}

	if( strstr( s, "/" ) == s ) {
		return "/";
	}

	return 0;
}

static const parameter_t *matchtag( const parameter_t *P, const char *s )
{
	const parameter_t *Q, *last = 0;
	int nmatch = 0, exact;

	if( (s = possibletag(s)) == 0 ) {
		return 0;
	}

	for( Q = P, exact = 0; Q->more; Q++ ) {
		int	q;

		if( !tagged(Q) ) {
			continue;
		}

		q = match( s, Q->tag );

		if( q > 0 ) {
			if( matchcase(s, Q->tag) > 0 ) {
				return Q;
			} else {
				last = Q;
				nmatch = exact ? (nmatch + 1) : 1;
				exact = 1;
			}
		}

		if( !exact && (q < 0) ) {
			last = Q;
			nmatch++;
		}
	}

	return (nmatch == 1) ? last : 0;
}

static state_t	*walk( int C, char **V, state_t *S, const errorcontext_t *E, interpret_t *Z )
{
	int	i, j, exact;
	char	*object = 0,
		*canonical = 0;
	const command_t
		*L = S->tree;
	state_t
		*N = 0,
		*R,
		*returning = S;

	int	last = -1, nmatch = 0;
	int	nC = 0, e = 0;
	char	**nV;
	pair_t	*tags;

	/* Order -- exact case sensitive match, exact match, prefix match */
	for( exact=0, i=0; L[i].name; i++ ) {
		int q = match(V[0], L[i].name); 
		if ( q > 0 ) {
			if( matchcase(V[0], L[i].name) > 0 ) {
				break;
			} else {
				last = i;
				nmatch = exact ? (nmatch + 1) : 1;
				exact = 1;
			}
		}

		if( !exact && (q < 0) ) {
			last = i;
			nmatch++;
		}
	}

	if( L[i].name == 0 ) {
		switch( nmatch ) {
		case 0:
			errprintf( "No such command `%s'\n", V[0] );
			errorcontext( E, V[0], 0 );
			return S;
		case 1:
			i = last;
			break;
		default:
			errprintf( "Ambiguous command `%s', could be\n", V[0] );
			for( j=0; L[j].name; j++ ) {
				if( match( V[0], L[j].name) < 0 ) {
					errprintf( "\t%s\n", L[j].name );
				} 
			}
			errorcontext( E, V[0], 0 );
			return S;
		}
	}

	tags = zlalloc();
	nV = (char **) malloc( sizeof(char *));  
	nV[nC] = 0;
        
	nV[nC++] = join( S->context.commands, L[i].name );
	nV = (char **)realloc( nV, (nC+1)*sizeof(char *) );
	nV[nC] = 0;
        
	canonical = join( S->context.commands, L[i].name );

	e++;	/* Accept the command name */

	if( L[i].parameters ) {
		const parameter_t	*P = L[i].parameters;

		/* Handle mandatory parameters first */
		for( P = L[i].parameters; P->more; P++ ) {
			const parameter_t *Q;

			/* First, look for any named mandatory parameters */
			while( (e < C) && ((Q = matchtag(P, V[e])) != 0) ) {
				char	*t;

				if( taglocate(Q->tag, tags) ) {
					errprintf( "Syntax error for `%s', duplicate tag `%s'\n", L[i].name, Q->tag );
					errorcontext( E, V[e-1], 1 );
					goto cleanup;
				}

				e++;

				if( e == C ) {
					errprintf( "Syntax error for `%s', missing value\n", L[i].name );
					errorcontext( E, V[e-1], 1 );
					suggestion(Q);
					goto cleanup;
				}

				if( Q->validate ) {
					t = Q->validate(V[e], Q->range);
				} else if ( Q->generate ) {
					t = dynamic(V[e], Q->generate, Z->arg );
				} else {
					t = 0;
				}

				if( !t ) {
					errprintf( "Invalid parameter `%s' for `%s'\n", 
						V[e], L[i].name );
					errorcontext( E, V[e-1], 1 );
					suggestion(Q);
					goto cleanup;
				}

				tags = zladd( tags, Q->tag, t );
				e++;
			}

			/* Now start looking for positionals */
			if( !optional(P) ) {
				char	*t = 0;

				if( keyword(P) ) {
					if( e == C ) {
						errprintf( "Syntax error for `%s', expecting `%s'\n", L[i].name, P->tag );
						errorcontext( E, V[e-1], 1 );
						goto cleanup;
					}

					if( !match(V[e], P->tag) ) {
						errprintf( "Syntax error for `%s', expecting `%s', found `%s'\n", 
							L[i].name, P->tag, V[e] );
						errorcontext( E, V[e-1], 1 );
						goto cleanup;
					}

					canonical = addto( canonical, P->tag );
					e++;
				} else {
					if( P->tag && ((j = taglocate(P->tag, tags)) != 0) ) {
						j--;
						t = tags[j].value;
						tags[j].value = 0;
					} else {
						if ( e == C ) {
							errprintf( "Not enough parameters for `%s'\n", L[i].name );
							errorcontext( E, V[e-1], 1 );
							goto cleanup;
						} 

						if( P->validate ) {
							t = P->validate(V[e], P->range);
						} else if ( P->generate ) {
							t = dynamic(V[e], P->generate, Z->arg );
						} else {
							t = 0;
						}

						if( !t ) {
							errprintf( "Invalid parameter `%s' for `%s'\n", 
								V[e], L[i].name );
							errorcontext( E, V[e-1], 1 );
							suggestion(P);
							goto cleanup;
						}
						e++;
					}

					canonical = addto( canonical, t );
					nV[nC++] = t;
					nV = (char **)realloc( nV, (nC+1)*sizeof(char *) );
					nV[nC] = 0;
				    }
			} 
		}

                /* coverity 55276 */
                if(tags){
		 zlfree(tags);
                }
		tags = zlalloc();

		/* Now the optionals */
		while( e < C ) {
			char	*s = V[e], *t = 0;
			const parameter_t	
				*lp = 0;
			int nparam = 0;

			/* Order -- exact case sensitive match, exact match, prefix match */
			for( exact = 0, P = L[i].parameters; P->more; P++ ) {
				int 	q;

				if( !optional(P) ) {
					continue;
				}

				q = match( s, P->tag );
				if(q == 0) {
					continue;
				} else if (!exact && (q < 0)) {
					lp = P;
					nparam++;
				} else {
					lp = P;

					if( matchcase(s, P->tag) > 0 ) {
						nparam = 1;
						break;
					} else {
						nparam = exact ? (nparam + 1) : 1;
						exact = 1;
					}
				}
			}

			/* Handle - more than one partial matches */
			if( nparam > 1 ) {
				errprintf( "Ambiguous tag for `%s' for `%s', could be\n", s, L[i].name );
				for( P = L[i].parameters; P->more; P++ ) {
					if( optional(P) && (match(s, P->tag) < 0) && !taglocate(P->tag, tags)) {
						errprintf( "\t%s\n", P->tag );
					}
				}
				errorcontext( E, V[e-1], 1 );
				goto cleanup;
			}

			/* Handle - no matches */
			if( !lp ) {
				break;
			}
			
			/* Handle - tag already found */
			if( taglocate(lp->tag, tags) ) {
				break;
			}

			if( flag(lp) ) {
				assert( lp->range );
				t = strdup(lp->range);
				canonical = addto( canonical, lp->tag );
				e++;
			} else {
				/* Handle - not enough parameters */
				if ( (e + 2) > C ) {
					errprintf( "Not enough parameters for tag `%s' in `%s'\n", 
						lp->tag, L[i].name );
					errorcontext( E, V[e], 1 );
					goto cleanup;
				} 

				if( lp->validate ) {
					t = lp->validate(V[e + 1], lp->range);
				} else if ( lp->generate ) {
					t = dynamic(V[e + 1], lp->generate, Z->arg );
				} else {
					t = 0;
				}

				/* Handle - invalid parameters */
				if( !t ) {
					errprintf( "Invalid parameter `%s' for tag `%s' in `%s'\n", 
						V[e + 1], lp->tag, L[i].name );
					errorcontext( E, V[e], 1 );
					suggestion(lp);
					goto cleanup;
				}

				canonical = addto( canonical, lp->tag );
				canonical = addto( canonical, V[e + 1] );
				e += 2;
			}

			tags = zladd( tags, lp->tag, t );
		}

		/* Now all tags have been found. */
		for( P = L[i].parameters; P->more; P++ ) {
			char	*t;

			if( !optional(P) ) {
				continue;
			}

			if( (j = taglocate(P->tag, tags)) != 0 ) {
				j--;

				/* NOTE - transferring ownership of malloc-ed buffer here */
				t = tags[j].value;
				tags[j].value = 0;	
			} else {
				t = strdup( P->defaultvalue );
			}

			nV[nC++] = t;
			nV = (char **)realloc( nV, (nC+1)*sizeof(char *) );
			nV[nC] = 0;
		}
	} else {
		/* No parameter list... */
		if( !L[i].subcommand ) {
			/* If there are no subcommands, consume all parameters */
			while( e < C ) {
				nV[nC++] = strdup( V[e++] );
				nV = (char **)realloc( nV, (nC+1)*sizeof(char *) );
				nV[nC] = 0;
			}
		} else {
			/* If there are subcommands, consume none */
		}
	}

	if( !L[i].subcommand && (e < C) ) {
		errprintf( "Extra parameter `%s' for `%s'\n", V[e], L[i].name );
		errorcontext( E, V[e], 0 );
		goto cleanup;
	}

	if( L[i].action ) {
		char	**U = 0;

		if( L[i].parameters ) {
			const parameter_t	*P = L[i].parameters;

			U = qslalloc();
			U = qsladdstrcopy(U, "");
	
			/* Add tags (if they exist) for mandatory parameters first */
			for( P = L[i].parameters; P->more; P++ ) {
				if( !optional(P) && !keyword(P) ) {
					U = qsladdstrcopy(U, P->tag ? P->tag : "");
				}
			}

			for( P = L[i].parameters; P->more; P++ ) {
				if( optional(P) ) {
					U = qsladdstrcopy(U, P->tag);
				}
			}
		}

		object = (*(L[i].action))( nC, nV, (const char *const *)(S->object.item), (const char *const *)U, Z->arg );

		if( U ) {
			qslfree(U);
		}
	}

	if( L[i].subcommand ) {
		char	**Q, **W;
		int	nobj = 0;

		if( L[i].action && !object ) {
			/* An object selection gone wrong */
			errprintf( "Nothing selected by `%s'\n", L[i].name );
			errorcontext( E, V[0], 1 );
			goto cleanup;
		}

		N = (state_t *) malloc( sizeof(state_t) );
     
		N->previous = S; 
		S->next = N; 
		N->next = 0;
		N->tree = L[i].subcommand;
    
		if( L[i].mode == 0 ){ 
			N->context.mode = join( S->context.mode, L[i].name );
		} else if ( empty(L[i].mode) ) {
			N->context.mode = strdup(S->context.mode);
		} else {
			N->context.mode = strdup(L[i].mode);
		}

		/* NOTE - transfer ownership of 'canonical', which is already malloc-ed */
		N->context.commands = canonical;
		canonical = 0;
        
		Q = qslalloc();

		if( object ) {
			/* NOTE - transfer ownership of 'object', which is already malloc-ed */
			Q = qsladdstr( Q, object );
			nobj++;
		}

		for( W = S->object.item; *W; W++ ) {
			Q = qsladdstrcopy( Q, *W );
			nobj++;
		}

		N->object.item = Q;
		N->object.count = nobj;

		if( C == e ) {
			if( empty(L[i].mode) ) {
				errprintf( "Syntax error for `%s' -- is not a mode\n", L[i].name );
				errorcontext( E, V[e-1], 1 );
				S->next = 0;
				freestate( N );
				returning = S;
			} else {
				returning = N;
			}
		} else {
			R = walk( C - e, V + e, N, E, Z );
			if( R == N ) {
				S->next = 0;
				freestate( N );
				returning = S;
			} else {
				returning = R;
			}
		}
        
	} else {
		if( object ) {
			result_t
				*X = (result_t *)malloc( sizeof(result_t) );

                       /* klockwork 12501 */
                        if(X){
			/* NOTE - Transfer of ownership here -- object was malloc-ed previously */
			X->value = object;
			X->next = Z->result;
			Z->result = X;
                        }
		}
	}
     
cleanup:
	for( j=0; j<nC; j++ ) {
		free( nV[j] );
	}
        /* coverity 55277 */
        if( nV ){   
	 free( nV );
        }
        if( tags ){
	 zlfree( tags );
        }
	if( canonical ) {
		free( canonical );
	}

	return returning;
}

static pair_t	*__fullchoices( const interpret_t *Z, int C, char **V, const command_t *L, pair_t *r, int definite )
{
	int	i, j, exact;
	int	e = 0;
	pair_t	*tags;
	int	last = -1, nmatch = 0;


	if ( e == (C - 1) ) {
		/* Potential command... */
		for( i=0; L[i].name; i++ ) {
			if( match( V[0], L[i].name) ) { 
				if( L[i].subcommand && empty(L[i].mode) ) {
					r = zladd( r, "-- keyword --", strdup(L[i].name) );
				} else {
					r = zladd( r, L[i].help, strdup(L[i].name) );
				}
			}
		}

		return r;
	}

	/* Order -- exact case sensitive match, exact match, prefix match */
	for( exact=0, i=0; L[i].name; i++ ) {
		int q = match(V[0], L[i].name); 

		if ( q > 0 ) {
			if( matchcase(V[0], L[i].name) > 0 ) {
				break;
			} else {
				last = i;
				nmatch = exact ? (nmatch + 1) : 1;
				exact = 1;
			}
		}

		if( !exact && (q < 0) ) {
			last = i;
			nmatch++;
		}
	}

	if( L[i].name == 0 ) {
		switch( nmatch ) {
		case 0:
			/* No Match */
			zlfree( r );
			return 0;
		case 1:
			i = last;
			break;
		default:
			/* Ambiguous */
			zlfree( r );
			return 0;
		}
	}

	tags = zlalloc();

	e++;	/* Eat up the command name */

	if( L[i].parameters ) {
		const parameter_t	*P = L[i].parameters;

		/* Handle mandatory parameters First */
		for( P = L[i].parameters; P->more; P++ ) {
			const parameter_t	*Q;

			/* First, look for any named mandatory parameters */
			while( (e < (C - 1)) && ((Q = matchtag(P, V[e])) != 0) ) {
				char	*t;

				if ( (e + 2) > C ) {
					/* Not enough parameters for tag */
					zlfree( r );
					r = 0;
					goto cleanup;
				} 

				if( taglocate(Q->tag, tags) ) {
					/* Duplicate tag */
					zlfree( r );
					r = 0;
					goto cleanup;
				}

				if( e == (C - 2) ) {
					/* Potential parameter to a tag... */
					if( Q->generate ) {
						int	k = 0;
						char **list = Q->generate( Z->arg ), **u;

						if( list ) {
							for( u = list; 	*u; u++ ) {
								if( match(V[e + 1], *u) ) {
									r = zladd( r, Q->help, strdup(*u) );
									k++;
								}
							}

							qslfree(list);
						}

						if( !k ) {
							if (!definite ) {
								r = zladd( r, Q->help, qasprintf(0, "<%s dynamic>", Q->type) );
							} else {
								zlfree( r );
								r = 0;
							}
						}
					} else if( definite ) {
						zlfree( r );
						r = 0;
					} else {
						r = zladd( r, Q->help, qasprintf(0, "<%s %s>", Q->type, any(Q->range) ) );
					}
					goto cleanup;
				}

				e++;	/* Eat the tag */

				if( Q->validate ) {
					t = Q->validate(V[e], Q->range);
				} else if ( Q->generate ) {
					t = dynamic(V[e], Q->generate, Z->arg );
				} else {
					t = 0;
				}

				if( !t ) {
					zlfree(r);
					r = 0;
					goto cleanup;
				}

				tags = zladd( tags, Q->tag, t );
				e++;	/* Eat the value */
			}

			/* Next, are we starting a tag? */
			if( (e == (C - 1)) && possibletag(V[e]) ) {
				int	npossible = 0;

				for( Q = P; Q->more; Q++ ) {
					const char	*s = possibletag(V[e]); 
					if( tagged(Q) && match(s, Q->tag) && !taglocate(Q->tag, tags) ) {
						r = zladd( r, Q->help, qasprintf(0, "%s%s", howtag(V[e]), Q->tag) );
						npossible++;
                                        }
				}

				if( npossible ) {
					/* 
					** If there was atleast one match for a tag, assume it is a tag being entered,
					** and ignore any possible values
					*/
					goto cleanup;
				}
			}

			/* Now start looking for positionals */
			if( !optional(P) ) {
				if( keyword(P) ) {
					if( !match( V[e], P->tag ) ) {
						/* Keyword expected, but didn't match! */
						zlfree( r );
						r = 0;
						goto cleanup;
					} 

					if( e == (C - 1) ) {
						/* Matches a keyword... */
						r = zladd( r, "-- keyword --", strdup(P->tag) );
						goto cleanup;
					}

					e++;
				} else {
					char	*t = 0;

					if( e == (C - 1) ) {
						/* Potential mandatory parameter. Note that it may be in the tag list */
						while( P->more && (optional(P) || (tagged(P) && taglocate(P->tag, tags))) ) {
							P++;
						}

						if( !P->more ) {
							/* All mandatory parameters have been served. Go look for something else */
							break;
						}

						if( P->generate ) {
							int	k = 0;
							char **list = P->generate( Z->arg ), **u;

							if( list ) {
								for( u = list; 	*u; u++ ) {
									if( match(V[e], *u) ) {
										r = zladd( r, P->help, strdup(*u) );
										k++;
									}
								}

								qslfree(list);
							}

							if( !k ) {
								if (!definite ) {
									r = zladd( r, P->help, qasprintf(0, "<%s dynamic>", P->type) );
								} else {
									zlfree( r );
									r = 0;
								}
							}
						} else if( definite ) {
							zlfree( r );
							r = 0;
						} else {
							r = zladd( r, P->help, qasprintf(0, "<%s %s>", P->type, any(P->range) ) );
						}
						goto cleanup;
					}

					if( P->tag && ((j = taglocate(P->tag, tags)) != 0) ) {
						j--;
						t = tags[j].value;
						tags[j].value = 0;
					} else {
						if( P->validate ) {
							t = P->validate(V[e], P->range);
						} else if ( P->generate ) {
							t = dynamic(V[e], P->generate, Z->arg );
						} else {
							t = 0;
						}

						if( !t ) {
							/* Invalid Parameter */
							zlfree( r );
							r = 0;
							goto cleanup;
						}

						e++;
					}

					free( t );
				}
			} 
		}
                if(tags){
	 	 zlfree(tags);
                }
		tags = zlalloc();

		/* Now the optionals */
		while( e < C ) {
			char	*s = V[e], *t = 0;
			const parameter_t	
				*lp = 0;
			int nparam = 0;

			if( e == (C - 1) ) {
				/* Potential tag... */
				for( P = L[i].parameters; P->more; P++ ) {
					if( optional(P) && match(s, P->tag) && !taglocate(P->tag, tags) ) {
						r = zladd( r, P->help, strdup(P->tag) );
					}
				}

				/* NOTE - a tag, being optional, is not necessarily the only option,  
				** So don't return, and go try subcommands if there are any... 
				*/
				break; 
			}

			/* Order -- exact case sensitive match, exact match, prefix match */
			for( P = L[i].parameters; P->more; P++ ) {
				int 	q;

				if( !optional(P) ) {
					continue;
				}

				q = match( s, P->tag );
				if(q == 0) {
					continue;
				} else if (!exact && (q < 0)) {
					lp = P;
					nparam++;
				} else {
					lp = P;

					if( matchcase(s, P->tag) > 0 ) {
						nparam = 1;
						break;
					} else {
						nparam = exact ? (nparam + 1) : 1;
						exact = 1;
					}
				}
			}

			/* Handle - more than one partial matches */
			if( nparam > 1 ) {
				/* Ambiguous tag */
				zlfree( r );
				r = 0;
				goto cleanup;
			}

			/* Handle - no matches */
			if( !lp ) {
				break;
			}
			
			/* Handle - tag already found */
			if( taglocate(lp->tag, tags) ) {
				break;
			}

			if( flag(lp) ) {
				assert( lp->range );
				t = strdup(lp->range);
				e++;
			} else {
				/* Handle - not enough parameters */
				if ( (e + 2) > C ) {
					/* Not enough parameters for tag */
					zlfree( r );
					r = 0;
					goto cleanup;
				} 

				if( e == (C - 2) ) {
					/* Potential parameter to a tag... */
					if( lp->generate ) {
						int	k = 0;
						char **list = lp->generate( Z->arg ), **u;

						if( list ) {
							for( u = list; 	*u; u++ ) {
								if( match(V[e + 1], *u) ) {
									r = zladd( r, lp->help, strdup(*u) );
									k++;
								}
							}

							qslfree(list);
						}

						if( !k ) {
							if (!definite ) {
								r = zladd( r, lp->help, qasprintf(0, "<%s dynamic>", lp->type) );
							} else {
								zlfree( r );
								r = 0;
							}
						}
					} else if( definite ) {
						zlfree( r );
						r = 0;
					} else {
						r = zladd( r, lp->help, qasprintf(0, "<%s %s>", lp->type, any(lp->range) ) );
					}
					goto cleanup;
				} 

				if( lp->validate ) {
					t = lp->validate(V[e + 1], lp->range);
				} else if ( lp->generate ) {
					t = dynamic(V[e + 1], lp->generate, Z->arg );
				} else {
					t = 0;
				}

				/* Handle - invalid parameters */
				if( !t ) {
					/* Invalid parameter for tag */
					zlfree( r );
					r = 0;
					goto cleanup;
				}

				e += 2;
			}

			tags = zladd( tags, lp->tag, t );
		}
	} else {
		/* No parameter list... */
		if( !L[i].subcommand ) {
			/* We have a command with '...' arguments here. Anything is allowed, nothing is predictable */
			if( definite ) {
				zlfree( r );
				r = 0;
			} else {
				r = zladd( r, "[free format]", strdup("...") );
				r = zladd( r, L[i].help, strdup("<cr>") );
			}
			goto cleanup;
		} else {
			/* Consume none */
		}
	}

	if( (e == (C - 1)) && !V[e][0] ) {
		/* Eaten all mandatory parameters, now considering the last empty one -- 'nothing more' may be an option here! 
		*/
		if( !L[i].subcommand || !empty(L[i].mode) ) {
			if( definite ) {
				zlfree( r );
				r = 0;
				goto cleanup;
			} else {
				r = zladd( r, L[i].help, strdup("<cr>") );
			}
		} else {
			assert( L[i].subcommand );
			/* Not an allowed mode, but there is a subcommand. So <cr> isn't an option. 
			** Just carry on, presumably the subcommands will provide an option 
			*/
		}
	}

	if( L[i].subcommand ) {
		r = __fullchoices( Z, C - e, V + e, L[i].subcommand, r, definite );
	} else {
		if( e != (C - 1) ) {
			/* Haven't consumed all parameters -- that is an error in the line */
			zlfree( r );
			r = 0;
			goto cleanup;
		}

		/* We are a command, and have something -- V[e] -- we are trying to match. It may have matched
		** something (a tag) before  -- in which case we can just return that. If it hasn't matched anything yet,
		** we have an 'extra' parameter here, which is an error */
		if( V[e][0] ) {
			if( !zllen( r ) ) {
				zlfree( r );
				r = 0;
			}
		}
	}

cleanup:
        /* coverity 55226 */
        if(tags){
	 zlfree( tags );
        }

	return r;
}

static pair_t	*fullchoices( const interpret_t *Z, int C, char **V, const command_t *L, pair_t *r, int definite )
{
	const char 
		*s;
	pair_t	*z, *Q;
	int	n, m;

	assert( C );

	s = V[C-1];

	z = __fullchoices(Z, C, V, L, r, definite );

	if( !z ) {
		return 0;
	}

	m = zllen(z);

	if( m == 1 ) {
		return z;
	}

	for( n=0, Q=z; Q->value; Q++ ) {
		/* 
		** We are looking for all that 
		**   EITHER prefix-matches `s' case-sensitively, 
		**   OR doesn't match `s' at all -- so is there for some other reason
		*/
		if( matchcase(s, Q->value) || !match(s, Q->value) ) {
			n++;
		}
	}

	if( n == 0 ) {
		return z;
	} else if (n == m) {
		return z;
	} else {
		pair_t	*y = zlalloc();
                /* coverity 55227 */
                if( !y ){
                   return 0;
                } 
		
		for( Q=z; Q->value; Q++ ) {
			if( matchcase(s, Q->value) || !match(s, Q->value) ) {
				y = zladd( y, Q->key, strdup(Q->value) );
			}
		}

		zlfree(z);
		return y;
	}
}

static char	**choices( const interpret_t *Q, int C, char **V, const command_t *L, char **r )
{
	pair_t	*z = zlalloc(), *q; 
        /* coverity 55230 */
        if ( !z ){
         return 0;
        }

	z = fullchoices(Q, C, V, L, z, 1 );

	if( !z ) {
		qslfree(r);
		return 0;
	} else {
		q = z;
		while( q->value ) {
			/* NOTE - Transferring ownership of malloc-ed buffer */
			r = qsladdstr(r, q->value);
			q->value = 0;
			q++;
		}
		zlfree( z );
		return r;
	}
}

static int	assist( const interpret_t *Z, int C, char **V, const command_t *L, int how )
{
	pair_t	*r;
	int	n;

	assert( C );

	r = zlalloc();
        /* coverity 55228 */
        if ( !r ){
         return 0;
        }
	r = fullchoices( Z, C, V, L, r, 0 );

	if( !r ) {
		if( how < 0 ) {
			errprintf( "No matches -- error in line\n" );
		}
		return 0;
	}

	if( (n = zllen(r)) == 0 ) {
		if( how < 0 ) {
			errprintf( "Sorry -- no matches for `%s'\n", V[C-1] );
		}
	} else {
		pair_t	*Q;

		if( how ) {
			for( Q = r; Q->value; Q++ ) {
				if( strlen(Q->value) > 20 ) {
					errprintf( "    %-20s\n", Q->value );
					errprintf( "    %-20s %s\n", "", Q->key );
				} else {
					errprintf( "    %-20s %s\n", Q->value, Q->key );
				}
			}
		}
	}

	zlfree( r );
	return n;
}

static char	*complete( const interpret_t *Z, int C, char **V, const command_t *L, int *partial )
{
	char	**r = qslalloc();
        /* coverity 36021 */
	char	*s = 0;
	int	i, n;

	if( partial ) {
		*partial = 0;
	}

	r = choices( Z, C, V, L, r );

	if( !r ) {
		return 0;
	}

	if( (n = qsllen(r)) == 1 ) {
		s = strdup(r[0]);
		qslfree(r);
		return s;
	} else if ( !n || !partial ) {
		qslfree(r);
		return 0;
	} else {
        /* coverity 35439 */
		s = strdup( r[0] );
                if(!s){
                  qslfree(r);
                  return s;
                }
		for( i=1; i<n; i++ ) {
			common( s, r[i] );
		}
		*partial = 1;
		qslfree( r );
		return s;
	}
}

static	void	details( const char *prefix, const command_t *L, const char *name )
{
	for( ; L->name; L++ ) {
		char	*s = 0;

		if( !match(name, L->name) ) {
			continue;
		}

		if( prefix ) {
			s = qasprintf(s, "%s ", prefix );
		}

		s = qasprintf( s, "%s", L->name );

		if( L->parameters ) {
			const parameter_t	*P; 

			for(P = L->parameters; P->more; P++) {
				if( !optional(P) ) {
					if( keyword(P) ) {
						s = qasprintf(s, " %s", P->tag);
					} else {
						if( tagged(P) ) {
							s = qasprintf(s, " [--%s]", P->tag );
						}
						s = qasprintf(s, " <%s %s>", P->type, typehelp(P) );
					}
				}
			}

			for(P = L->parameters; P->more; P++) {
				if( optional(P) ) {
					if( flag(P) ) {
						s = qasprintf(s, " [%s]", P->tag);
					} else {
						s = qasprintf(s, " [%s <%s %s>]", P->tag, P->type, typehelp(P) );
					}
				}
			}
		} else {
			if( !L->subcommand ) {
				s = qasprintf(s, " ...");
			}
		}

		if( L->subcommand && empty(L->mode) ) {
			details( s, L->subcommand, "" );
		} else {
			errprintf( "%s\n", s);
			errprintf( "     --\t%s\n", L->help );
		}

		free( s );
	}
}

static	void	traverse( const command_t *L, const char *prefix )
{
	for( ; L->name; L++ ) {
		char	*t = 0;

		if( prefix ) {
			t = qasprintf( t, "%s ", prefix );
		} 

		t = qasprintf( t, "%s", L->name );
		if( L->parameters ) {
			const parameter_t	*P; 

			for(P = L->parameters; P->more; P++) {
				if( !optional(P) ) {
					if( keyword(P) ) {
						t = qasprintf( t, " %s", P->tag );
					} else {
						if( tagged(P) ) {
							t = qasprintf(t, " [--%s]", P->tag );
						}
						t = qasprintf( t, " <%s %s>", P->type, typehelp(P) );
					}
				}
			}

			for(P = L->parameters; P->more; P++) {
				if( optional(P) ) {
					if( flag(P) ) {
						t = qasprintf( t, " [%s]", P->tag );
					} else {
						t = qasprintf( t, " [%s <%s %s>]", P->tag, P->type, typehelp(P) );
					}
				}
			}
		} else if( !L->subcommand ) {
			t = qasprintf( t, " ..." );
		}

		if( !L->subcommand || !empty(L->mode) ) {
			errprintf( "%s\n", t );
		}

		if( L->subcommand ) {
			traverse( L->subcommand, t );
		}

		free( t );
	}
}

static int	script( interpret_t *interpreter, const char *filename ) 
{
	FILE	*F;
	int	n = 1;

	F = fopen( filename, "r");

	if( !F ) {
		errprintf( "Error `%s' opening `%s'\n", strerror(errno), filename );
		return -1;
	} else {
		char	*s;
		while( (s = qfgetline(F)) != 0 ) {
			n = qclExecute( interpreter, s );
			free( s );

			if( (s = qclResult(interpreter)) != 0 ) {
				/* Discard any result */
				free( s );
			}

			if( !n ) {
				break;
			}
		}

		fclose( F );
		return n;
	}
}

static char	**tokenswhy(int *argcount, char *buffer, const char *extra, int *why, char **remaining)
{
	char	**args = 0;
	char	*argument, *save = ((void *)0), *eob = buffer + strlen(buffer);
	int	w, i = 0;

	(*argcount) = 0;
	args = qslalloc();

	for( i=0; ; ) {
		argument = qtokenize( i?0:buffer, WHITESPACE, TERMINATOR, '"', '\\', &save, &w );

		if( argument ) {
			args = qsladdstr( args, argument );
			i++;

			if( w && strchr(TERMINATOR, w) ) {
				*why = w;
				*remaining = save + 1;
				break;
			}
		} else {
			if( w ) {
				if( strchr(extra, w) ) {
					args = qsladdstr( args, eob );
					i++;
				}

				*why = w;
				*remaining = save;
			} else {
				*why = 0;
				*remaining = 0;
			}
			break;
		}
	}

	(*argcount) = i;
	return args;
}

static char	**tokens(int *argcount, char *buffer, int extra)
{
	char	**args = 0;
	char	*argument, *save = ((void *)0), *eob = buffer + strlen(buffer);

	(*argcount) = 0;
	args = qslalloc();

	argument = qtokenize( buffer, WHITESPACE, TERMINATOR, '"', '\\', &save, 0 );
	while( argument ) {
		args = qsladdstr( args, argument );
		(*argcount)++;

		argument = qtokenize( NULL, WHITESPACE, TERMINATOR, '"', '\\', &save, 0 );
	}

	if (extra) {
		args = qsladdstr( args, eob );
		(*argcount)++;
	}

	return args;
}

#define	S_X	0
#define	S_Q	1
#define	S_E	2
#define	S_Z	3
#define	S_F	4

#define	E_X	0
#define	E_Q	1
#define	E_E	2
#define	E_T	3

static const int	__table[S_F+1][E_T+1] = {
	{S_X,	S_Q,	S_E,	S_F},
	{S_Q,	S_X,	S_Z,	S_Q},
	{S_X,	S_X,	S_X,	S_X},
	{S_Q,	S_Q,	S_Q,	S_Q},
	{S_F,	S_F,	S_F,	S_F},
};
	
int	qclQuoting( const char *s )
{
	int	c;
	int	state = S_X,
		event;

	while( (c = *s++) != 0 ) {
		if ( c == '\\' ) {
			event = E_E;
		} else if ( c == '"' ) {
			event = E_Q;
		} else if ( strchr(TERMINATOR, c) ) {
			event = E_T;
		} else {
			event = E_X;
		}

		state = __table[state][event];
	}

	return (state == S_X) ? 0 : 1;
}

int	qclExecute( interpret_t *interpreter, const char *line ) 
{
	char	**args = 0, *remaining;
	int	argcount = 0, why, i, returning;
	char	*buffer = 0;
	state_t
		*state = interpreter->state;
	errorcontext_t	E;

	assert( state );

	/* Space for first */
	buffer = strdup( line );
        /* coverity 35441 */
        if(!buffer){
         return 0;
        }
	trim( buffer, "\n\r" );
	args = tokenswhy( &argcount, buffer, "?", &why, &remaining );
    
	if( why == '#' ) {
		/* Ignore comments */
		why = 0;
		remaining = 0;
	}

	if( !argcount ) {
		free( args );
		free( buffer );
		return 1;
	}

	if( why == '?' ) {
		if( remaining && remaining[strspn(remaining, WHITESPACE)] ) {
			errprintf( "'?' allowed only at the end of line\n" );
		} else {
			assist( interpreter, argcount, args, state->tree, -1 );
		}

		returning = 1;
	} else if( matchany( args[0], ".", "..", "end", "back", "<-", NULL ) > 0 ) { /* 64bit Fix */
		if( state->previous ) {
			state_t	*N = state->previous;
				
			N->next = 0;
			freestate( state );
			interpreter->state = N;
		} 

		returning = 1;
	} else if( matchany( args[0], "-", "/", NULL ) > 0 ) { /* 64bit Fix */
		state_t	*N = state, *Q;

		while( N->previous ) {
			Q = N->previous;
			Q->next = 0;
			freestate( N );
			N = Q;
		}

		interpreter->state = N;
		returning = 1;
	} else if( match( args[0], "*" ) > 0 ) {
		traverse( state->tree, 0 );
		returning = 1;
	} else if( matchany( args[0], "quit", "exit", NULL ) > 0 ) { /* 64bit Fix */
		returning = 0;
	} else if( match( args[0], "help" ) > 0 ) {
		switch( argcount ) {
		case 1:
			details( 0, state->tree, "" );
			break;
		case 2:
			if( match( args[1], "*" ) > 0 ) {
				/* Will show everything */
				details( 0, state->tree, "" );
			} else {
				details( 0, state->tree, args[1] );
			}
			break;
		default:
			errprintf( "Usage: %s [name|prefix|*]\n", args[0] );
			break;
		}
		returning = 1;
	} else if( matchany( args[0], "@", "source", NULL ) > 0 ) { /* 64bit Fix */
		int	n = 1;

		if( argcount > 1 )  {
			for( i=1; (i < argcount) && (n == 1); i++ ) {
				n = script( interpreter, args[i] );
			}
		} else {
			errprintf( "Usage: %s <file>\n", args[0] );
		}

		returning = n ? 1 : 0;
	} else if( args[0][0] == '@' ) {
		int n = script( interpreter, &args[0][1] );
		    for( i=1; (i<argcount) && (n == 1); i++ ) {
			    n = script( interpreter, args[i] );
		    }
		returning = n ? 1 : 0;
	} else {
		char	*tail = 0; 

		if( why ) {
			tail = qasprintf( 0, "%c%s", why, remaining?remaining:"" );
		}

		if( !interpreter->redirect || (interpreter->redirect)(1, tail, interpreter->arg) ) {
			E.original = line;
			E.copy = buffer;
			E.nargs = argcount;
			E.arguments = args;
			interpreter->state = walk( argcount, args, state, &E, interpreter );

			if( interpreter->redirect ) {
				(interpreter->redirect)(0, 0, interpreter->arg);
			}
		}

		if( tail ) {
			free(tail);
		}

		returning = 1;
	}

	free( args );
	free( buffer );

	return returning;
}

interpret_t *qclOpen( const command_t *L,
		int	(*redirect)(int, char *, void *),
		void	*arg)
{
	interpret_t	*N;

	N = (interpret_t *) malloc( sizeof(interpret_t) );
        /* coverity 35442 */
        if(!N){ 
         return N;
        } 
	N->result = 0;
	N->state = (state_t *) malloc( sizeof(state_t) );
        /* klockwork 12498 */
        if(!(N->state)){
         free(N);
         N=0;
         return N;
        }
	N->state->previous = N->state->next = 0;
        
	N->tree = N->state->tree = L;

	N->state->context.commands = strdup( "" );
	N->state->context.mode = strdup( "" );
	N->state->object.item = qslalloc();
	N->state->object.count = 0;
        
	N->redirect = redirect;
	N->arg = arg;
    
	return N;
}

void	qclClose( interpret_t *interpreter )
{
	state_t	*N, 
		*S = interpreter->state;
	result_t
		*Q,
		*R = interpreter->result;

	while( S ) {
		N = S;
		S = S->previous;
		freestate( N );
	}

	while( R ) {
		Q = R;
		R = R->next;
		freeresult( Q );
	}

	free( interpreter );
}

char	*qclComplete( const interpret_t *interpreter, const char *line, unsigned int index, int *partial )
{
	char	**args = 0;
	int	argcount = 0;
	char	*r,
		*t = strdup(line);
	const state_t
		*S = interpreter->state;

	if( !t ) {
		return 0;
	}

	args = tokens( &argcount, t, (index == strlen(t))?1:0 );
	if( !argcount || (args[argcount-1] != (t + index))) {
		free( args );
		free( t );
		return 0;
	}

	r = complete( interpreter, argcount, args, S->tree, partial );
	free( args );
	free( t );

	return r;
}

int	qclChoices( const interpret_t *interpreter, const char *line, unsigned int index, int how )
{
	char	**args = 0;
	int	argcount = 0;
	char	*t = strdup(line); 
	const state_t
		*S = interpreter->state;
	int	r;

	if( !t ) {
		return 0;
	}

	args = tokens( &argcount, t, (index == strlen(t))?1:0 );
	if( !argcount || (args[argcount-1] != (t + index))) {
		free( args );
		free( t );
		return 0;
	}

	r = assist( interpreter, argcount, args, S->tree, how?1:0 );

	free( args );
	free( t );

	return r;
}

char	**qclList( const interpret_t *interpreter, const char *line, unsigned int index )
{
	char	**args = 0;
	int	argcount = 0;
	char	*t = strdup(line); 
	const state_t
		*S = interpreter->state;
	char	**r;

	if( !t ) {
		return 0;
	}

	args = tokens( &argcount, t, (index == strlen(t))?1:0 );
	if( !argcount || (args[argcount-1] != (t + index))) {
		free( args );
		free( t );
		return 0;
	}

	r = qslalloc();
	r = choices( interpreter, argcount, args, S->tree, r );

	free( args );
	free( t );

	if( !r ) {
		return 0;
	} else if( qsllen(r) == 0 ) {
		qslfree(r);
		return 0;
	} else {
		return r;
	}
}

int qclObjectCount( const interpret_t *S )
{
	return  S->state->object.count;
}

const char *qclGetObject( const interpret_t *S, int n )
{
	return (n < S->state->object.count) ?S->state->object.item[n] : 0;
}

const char	*qclGetMode( const interpret_t *S )
{
	return S->state->context.mode;
}

const char	*qclGetCanonical( const interpret_t *S )
{
	return S->state->context.commands;
}

char	*qclResult( interpret_t *S )
{
	char	*z = 0;

	if( S->result ) {
		result_t
			*R = S->result;

		S->result = R->next;
		z = R->value;
		free( R );
	}

	return z;
}

