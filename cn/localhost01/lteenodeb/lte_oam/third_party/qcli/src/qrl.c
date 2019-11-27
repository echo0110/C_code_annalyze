/*******************************************************************************
* File Name   : qrl.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include <unistd.h>

#include <signal.h>

#include <sys/ioctl.h>

#if defined(__sun__) && defined(__svr4__)
#include <termio.h>
#endif

#include <termios.h>

#ifdef __CYGWIN__
#include <ncurses/ncurses.h>
#include <ncurses/term.h>
#else
#include <curses.h>
#include <term.h>
#endif

#include <assert.h>

#include "qrl.h"
#include "qutil.h"

#define	CTL(n)	( (n) - 'A' + 1 )
#define DELETE	127

#define	C_CLEAR		0
#define	C_UP		1
#define	C_DOWN		2
#define	C_LEFT		3
#define	C_RIGHT		4
#define	C_INSERT	5
#define	C_DELETE	6
#define	C_CLRTOEOL	7
#define	C_CLRTOEOS	8

#define	K_U	0x8001
#define	K_D	0x8002
#define	K_L	0x8003
#define	K_R	0x8004
#define	K_H	0x8005
#define	K_E	0x8006
#define	K_Q	0x8007
#define	K_X	0x8008

#define	X_REDRAW	0
#define	X_CURSOR	1
#define	X_INSERT	2
#define	X_DELETE	4
#define	X_NEWLINE	5
#define	X_NEWPROMPT	6

typedef struct	sequence_s {
	const char
		*string;
	int	value;
}	sequence_t;

#define	LEADERS	"\033\217\233\377\200"

static const sequence_t	__keyfallback[] = {

	{ "\033[A", K_U },
	{ "\033[B", K_D },
	{ "\033[C", K_R },
	{ "\033[D", K_L },

	{ "\033OA", K_U },
	{ "\033OB", K_D },
	{ "\033OC", K_R },
	{ "\033OD", K_L },

	{ "\233A", K_U },
	{ "\233B", K_D },
	{ "\233C", K_R },
	{ "\233D", K_L },

	{ 0, 0 }
};

static const sequence_t	__screenfallback[] = {
	{ "\033[A", C_UP },
	{ "\033[B", C_DOWN },
	{ "\033[D", C_LEFT },
	{ "\033[C", C_RIGHT },
	{ "\033[H\033[J", C_CLEAR },
	{ 0, 0 }
};

static const sequence_t	__capability[] = {
	{ "clear",	C_CLEAR },
	{ "el",		C_CLRTOEOL },
	{ "ed",		C_CLRTOEOS },
	{ "cuu1",	C_UP },
	{ "cud1",	C_DOWN },
	{ "cuf1",	C_RIGHT },
	{ "cub1",	C_LEFT },
	{ "ich1",	C_INSERT },
	{ "dch1",	C_DELETE },
	{ 0, 0 }
};

static const sequence_t	__keypad[] = {
	{ "kcuu1",	K_U },
	{ "kcud1",	K_D },
	{ "kcub1",	K_L },
	{ "kcuf1",	K_R },
	{ "kdch1",	K_X },
	{ "khome",	K_H },
	{ "kend",	K_E },
	{ 0, 0 }
};

#define	NFIFO		16
#define	__LEN(x)	((int)(sizeof(x)/sizeof(x[0])))
#define	NCAPABILITY	(__LEN(__capability))
#define	NKEYCAP		( __LEN(__keyfallback) + __LEN(__keypad) )

typedef struct	buffer_s {
	char	*buffer;
	int	size,
		count,
		fd;
}	buffer_t;

typedef struct item_t {
	struct item_t	
		*previous,
		*next;
	char	*line;
}	item_t;

typedef struct keycap_s {
	char	*string;
	int	value;
}	keycap_t;

struct	edit_t {
	struct	{
		struct sigaction
			handler;
		struct termios	
			state;
		int	width;
		char	*capability[NCAPABILITY];
		keycap_t
			keytable[NKEYCAP];
	}	terminal;

	struct	{
		int	in, out;
	} fd;

	char	*buffer,
		*prompt;

	unsigned int
		cursor,
		size,
		done,
		possible;

	int	echo;

	struct	{
		item_t
			*head,
			*tail,
			*current;
		int	count,
			size;
	}	history;

	struct	{
		char	space[NFIFO];
		int	head;
		int	tail;
	}	fifo;

	struct	{
		char	*buffer;
		int	cursor;
	}	saved;

	char	*(*completion)(void *, const char *, unsigned int, int *);
	int	(*choices)(void *, const char *, unsigned int, int);
	int	(*permission)(void *, const char *, int); 
	void	*arg;
};

static void	__handler( int n )
{
	/* Do nothing -- just here to break the read */
}

static	int	__visible( const edit_t *M, int c )
{
	return M->echo?M->echo:c;
}

static	int	__echo( const edit_t *M )
{
	return (M->echo < 0)?0:1;
}

static	buffer_t	*__newbuffer( const edit_t *M )
{
	buffer_t	*B = (buffer_t *)malloc( sizeof(buffer_t) );

	B->size = B->count = 0;
	B->buffer = (char *)malloc( B->size + 1);
	B->buffer[0] = 0;
	B->fd = M->fd.out;

	return B;
}

static	void	__flush( buffer_t *B )
{
	write( B->fd, B->buffer, B->count );
	free( B->buffer );
	free( B );
}

static	void	__addchar( buffer_t *B, int c )
{
	while( B->count + 1 > B->size ) {
		B->size = B->size?(2*B->size):1;
		B->buffer = (char *) realloc( B->buffer, B->size + 1 );
	}
	B->buffer[B->count] = (char) c;
	B->count++;
	B->buffer[B->count] = 0;
}

static	void	__addstr( buffer_t *B, const char *s )
{
	int	n = strlen(s);

	while( B->count + n > B->size ) {
		B->size = B->size?(2*B->size):1;
		B->buffer = (char *) realloc( B->buffer, B->size + 1 );
	}
	strcat( B->buffer, s);
	B->count += n;
}

typedef struct	position_s {
	int	length,
		cursor;
}	position_t;

static void	__beep( buffer_t *B )
{
	__addstr( B, "\a" );
}

static void	__clear( buffer_t *B, const edit_t *M )
{
	__addstr( B, M->terminal.capability[C_CLEAR] );
}

static void	__down( buffer_t *B, const edit_t *M )
{
	__addstr( B, M->terminal.capability[C_DOWN] );
}

static void	__up( buffer_t *B, const edit_t *M )
{
	__addstr( B, M->terminal.capability[C_UP] );
}

static void	__left( buffer_t *B, const edit_t *M )
{
	__addstr( B, M->terminal.capability[C_LEFT] );
}

static void	__right( buffer_t *B, const edit_t *M )
{
	__addstr( B, M->terminal.capability[C_RIGHT] );
}

static void	__insert( buffer_t *B, const edit_t *M, int c )
{
	__addstr( B, M->terminal.capability[C_INSERT] );
	__addchar( B, c );
}

static void	__delete( buffer_t *B, const edit_t *M )
{
	__addstr( B, M->terminal.capability[C_DELETE] );
}

static void	__move( buffer_t *B, const edit_t *M, int nL, int nC, int *pL, int *pC )
{
	if( *pL < nL ) {
		while( *pL < nL ) {
			__down( B, M );
			(*pL)++;
		}
	} else if ( *pL > nL ) {
		while( *pL > nL ) {
			__up( B, M );
			(*pL)--;
		}
	}

	if( *pC < nC ) {
		while( *pC < nC ) {
			__right( B, M );
			(*pC)++;
		}
	} else if ( *pC > nC ) {
		while( *pC > nC ) {
			__left( B, M );
			(*pC)--;
		}
	}
}

static void	__redraw( buffer_t *B, const edit_t *M, int np, int nb, int limit, int *pL, int *pC )
{
	int	i, cursor = 0;

	while( (*pL) ) {
		__up( B, M );
		(*pL)--;
	}

	while( (*pC) ) {
		__left( B, M );
		(*pC)--;
	}

	for( i=0; i<np; i++ ) {
		if( isprint( (int)(M->prompt[i]) ) ) {
			__addchar( B, M->prompt[i] );
		} else {
			__addchar( B, ' ' );
		}
		cursor++;
	}

	for( i=0; i<nb; i++ ) {
		__addchar( B, __visible(M, M->buffer[i]) );
		cursor++;
	}

	for( i=(np + nb); i<limit; i++ ) {
		__addchar( B, ' ' );
		cursor++;
	}

	if( !(cursor % M->terminal.width) ) {
		/* Guard against 'xenl' glitch */
		__addchar( B, ' ' );
		__left( B, M );
	}

	(*pL) = cursor / M->terminal.width;
	(*pC) = cursor % M->terminal.width;
}

static void	__update(buffer_t *B, const edit_t *M, const position_t *P, int op, ... )
{
	va_list	A;
	int	L, C, nb, np, nL, nC, limit = 0;

	assert( M->buffer );
	if( __echo(M) ) {
		nb = strlen( M->buffer );
		np = strlen( M->prompt );
		nL = (M->cursor + np) / M->terminal.width;
		nC = (M->cursor + np) % M->terminal.width;
	} else {
		nb = 0;
		np = strlen( M->prompt );
		nL = np / M->terminal.width;
		nC = np % M->terminal.width;
	}

	if( !P ) {
		int	n;

		switch( op ) {
		case X_NEWPROMPT:
			va_start( A, op );
			n = va_arg( A, int );
			va_end( A );

			if( __echo(M) ) {
				L = (M->cursor + n) / M->terminal.width;
				C = (M->cursor + n) % M->terminal.width;
				limit = n + strlen( M->buffer );
			} else {
				L = n / M->terminal.width;
				C = n % M->terminal.width;
				limit = n;
			}
			break;
		case X_NEWLINE:
			L = C = 0;
			break;
		case X_REDRAW:
		default:
			__clear( B, M );
			L = C = 0;
		}
	} else {
		L = (P->cursor + np) / M->terminal.width;
		C = (P->cursor + np) % M->terminal.width;
		limit = P->length;
	}

	switch( op ) {
	case X_INSERT:
		if( !__echo(M) ) {
			/* Nothing to do */
		} else if( M->terminal.capability[C_INSERT] ) {
			int	c, d, e, i, fL;//, fC;

			va_start( A, op );
			c = __visible(M, va_arg(A, int));
			va_end( A );

			fL = (strlen(M->buffer) + np) / M->terminal.width;
			//fC = (strlen(M->buffer) + np) % M->terminal.width;

			if( C != (M->terminal.width - 1) ) {
				__insert( B, M, c );
				C++;
			}

			while( L < fL ) {
				/* Consider the following characters (*before* the insertion) 
				**      |     ...cd| 
				**      |e...      | 
				** We emit 'c', emit 'e' & backspace (to scroll & handle xenl) & 
				** finally insert 'd' and backspace again -- ending up in column zero of the next line 
				*/

				i = (M->terminal.width * (L + 1)) - np;
				
				c = __visible(M, M->buffer[i - 1]);
				if( (d = __visible(M, M->buffer[i])) == 0 ) {
					d = e = ' ';
				} else if ( (e = __visible(M, M->buffer[i + 1])) == 0 ) {
					e = ' ';
				}

				__move( B, M, L, (M->terminal.width - 1), &L, &C );
				__addchar( B, c );
				__addchar( B, e );
				__addchar( B, '\b' );
				__insert( B, M, d );
				__addchar( B, '\b' );

				L++;
				C = 0;
			}

			__move( B, M, nL, nC, &L, &C );
			return;
		} else {
			__redraw( B, M, np, nb, limit, &L, &C );
			__move( B, M, nL, nC, &L, &C );
			return;
		}

	case X_DELETE:
		if( !__echo(M) ) {
			/* Nothing to do */
		} else if( M->terminal.capability[C_DELETE] ) {
			int	mode, capacity, c;

			va_start( A, op );
			mode = va_arg(A, int);
			va_end( A );

			if( mode == -1 ) {
				/* Backspace -- that is, move left by one, then delete current. */
				if( C ) {
					__move( B, M, L, C - 1, &L, &C );
				} else {
					__move( B, M, L - 1, M->terminal.width - 1, &L, &C );
				}
			}

			if( C != (M->terminal.width - 1) ) {
				__delete( B, M );
			}

			while( (capacity = (L+1)*M->terminal.width) < limit ) {
				c = __visible(M, M->buffer[(capacity - 1) - np]);

				__move( B, M, L, M->terminal.width - 1, &L, &C );
				__addchar( B, c );
				__addchar( B, c );
				__addchar( B, '\b' );
				__delete( B, M );

				L++;
				C = 0;
			}

			/** If we are at the end... **/
			if( (L+1)*M->terminal.width == limit ) {
				__move( B, M, L, M->terminal.width - 1, &L, &C );
				__addchar( B, ' ' );
				__addchar( B, ' ' );
				__addchar( B, '\b' );
				__delete( B, M );

				L++;
				C = 0;
			}

			__move( B, M, nL, nC, &L, &C );
			return;
		} else {
			__redraw( B, M, np, nb, limit, &L, &C );
			__move( B, M, nL, nC, &L, &C );
			return;
		}

	default:
		/* Default to be treated as REDRAW, so fall through... */

	case X_NEWLINE:
	case X_NEWPROMPT:
	case X_REDRAW:
		__redraw( B, M, np, nb, limit, &L, &C );
		__move( B, M, nL, nC, &L, &C );
		break;
	case X_CURSOR:
		__move( B, M, nL, nC, &L, &C );
		break;
	}
}

static void	__logbegin( buffer_t *B, const edit_t *M )
{
	int	L, C, np, nb;

	if( __echo(M) ) {
		nb = strlen( M->buffer );
		np = strlen( M->prompt );
		L = (M->cursor + np ) / M->terminal.width;
		C = (M->cursor + np ) % M->terminal.width;
	} else {
		nb = 0;
		np = strlen( M->prompt );
		L = np / M->terminal.width;
		C = np % M->terminal.width;
	}

	while( L ) {
		__up( B, M );
		L--;
	}

	while( C ) {
		__left( B, M );
		C--;
	}

	if( M->terminal.capability[C_CLRTOEOS] ) {
		__addstr( B, M->terminal.capability[C_CLRTOEOS] );
	} else {
		int	i, cursor = 0;

		for( i = 0; i<(np+nb); i++ ) {
			__addchar( B, ' ' );
			cursor++;
		}

		L = (cursor + np ) / M->terminal.width;
		C = (cursor + np ) % M->terminal.width;

		if( !C ) {
			/* Handling 'xenl' */
			__addchar( B, ' ' );
			__addchar( B, '\b' );
		}

		while( L ) {
			__up( B, M );
			L--;
		}

		while( C ) {
			__left( B, M );
			C--;
		}
	}
}

static void	__logend( buffer_t *B, const edit_t *M )
{
	__update( B, M, 0, X_NEWLINE );
}

static void	__push( edit_t *M, char c )
{
	if( ((M->fifo.head + 1) % NFIFO) == M->fifo.tail ) {
		/* FIFO full! */
		return;
	}

	M->fifo.space[M->fifo.head] = c;
	M->fifo.head = (M->fifo.head + 1) % NFIFO;
}

static int	__pop( edit_t *M )
{
	int	i, c, partial = 0;
	const keycap_t
		*S;

	if( M->fifo.head == M->fifo.tail ) {
		return -1;
	}

	c = M->fifo.space[M->fifo.tail];

	if( strchr(LEADERS, c) ) {
		for( S = M->terminal.keytable; S->string; S++ ) {
			const char	
				*t;
			int	q;

			t = S->string;	

			if( t[0] != (char) c ) {
				continue;
			}

			i = 0; 
			q = M->fifo.tail; 
			while( t[i] && (q != M->fifo.head) && (M->fifo.space[q] == t[i]) ) {
				i++; 
				q = (q+1) % NFIFO;
			}

			if( !t[i] ) {
				/* Exact match! */
				M->fifo.tail = q;
				return S->value;
			}

			if (q == M->fifo.head) {
				/* Partial match, mark & carry on (don't return in case there is an exact match later) */
				partial++;
			}

			/* If both checks above fail, then we have a mismatch -- in which case we continue the loop */
		}
	}

	if( partial ) {
		return -1;
	} else {
		M->fifo.tail = (M->fifo.tail + 1) % NFIFO;
		return c;
	}
}

static void	__probewidth( edit_t *M )
{
	struct winsize
		W;

	if( ioctl( M->fd.in, TIOCGWINSZ, &W ) < 0 ) {
		/* Error -- probably not a window? */
		return;
	} else {
		M->terminal.width = W.ws_col;
	}
}

static void	__deletehistorytail( edit_t *M )
{
	item_t	*T = M->history.tail;

	assert( M->history.count );
	assert( M->history.tail );

	if( M->history.head == M->history.tail ) {
		M->history.head = M->history.tail = 0;
	} else {
		M->history.tail = T->previous;
		M->history.tail->next = 0;
	}

	free( T->line );
	free( T );
	M->history.count--;
}

static void	__addhistory( edit_t *M, const char *s )
{
	item_t	*T;

	if( M->history.size < 0 ) {
		return;
	}

	s += strspn(s, " \t\n");

	if( !s[0] ) {
		return;
	}

	if( M->history.size && (M->history.count == M->history.size) ) {
		__deletehistorytail(M);
	}

	T = (item_t *) malloc( sizeof(item_t) );
	T->line = strdup( s );	

	T->previous = 0;

	if( !M->history.head ) {
		M->history.tail = T;
	}

	T->next = M->history.head;
	M->history.head = T;

	if( T->next ) {
		T->next->previous = T;
	}

	M->history.count++;
}

static void	__key( edit_t *M, int c )
{
	unsigned int	
		i, n;
	position_t
		P;
	buffer_t
		*B;

	assert( M->buffer );

	if( M->possible ) {
		if( c == '\t' ) {
			c = K_Q;
		}
		M->possible = 0;
	}

	n = strlen(M->buffer);

	if( __echo(M) ) {
		P.length = n + strlen( M->prompt );
		P.cursor = M->cursor;
	} else {
		P.length = strlen( M->prompt );
		P.cursor = 0;
	}

	/* A few special cases! */
	if( M->possible ) {
		if( c == '\t' ) {
			c = K_Q;
		}
		M->possible = 0;
	}

	if( (c == '?') && M->permission && (M->cursor == n) && M->permission(M->arg, M->buffer, c) ) {
		c = K_Q;
	}

	if( (c == CTL('D')) && n && (n == M->cursor) ) {
		c = K_Q;
	}

	if( (unsigned int)c == (unsigned int)(M->terminal.state.c_cc[VERASE]) ) {
		c = CTL('H');
	} else if( (unsigned int)c == (unsigned int)(M->terminal.state.c_cc[VKILL]) ) {
		c = CTL('X');
	}

	B = __newbuffer(M);
	switch( c ) {
	case -1:
		/* Nothing to do */
		break;
	case K_H:
	case CTL('A'):
		M->cursor = 0;
		__update( B, M, &P, X_CURSOR );
		break;

	case K_L:
	case CTL('B'):
		if( M->cursor ) {
			M->cursor--;
			__update( B, M, &P, X_CURSOR );
		} else {
			__beep( B );
		}
		break;

	case K_R:
	case CTL('F'):
		if( M->cursor < strlen(M->buffer) ) {
			M->cursor++;
			__update( B, M, &P, X_CURSOR );
		} else {
			__beep( B );
		}
		break;

	case K_E:
	case CTL('E'):
		M->cursor = strlen( M->buffer );
		__update( B, M, &P, X_CURSOR );
		break;

	case K_X:
	case DELETE:
		if( M->cursor < n ) {
			for( i=M->cursor; i<n; i++ ) {
				M->buffer[i] = M->buffer[i+1];
			}
			M->buffer[n-1] = 0;

			__update( B, M, &P, X_DELETE, 0 );
			break;
		}
		/* Otherwise (cursor was at end of line) fall-through to backspace handling  */

	case CTL('H'):
		if( M->cursor ) {
			for( i=M->cursor; i<n; i++ ) {
				M->buffer[i-1] = M->buffer[i];
			}
			M->buffer[n-1] = 0;
			M->cursor--;

			__update( B, M, &P, X_DELETE, -1 );
		} else {
			__beep( B );
		}
		break;

	case CTL('X'):
		M->buffer[0] = 0;
		M->cursor = 0;
		__update( B, M, &P, X_REDRAW );
		break;

	case CTL('R'):
		__update( B, M, &P, X_REDRAW );
		break;

	case CTL('L'):
		__update( B, M, 0, X_REDRAW );
		break;

	case CTL('D'):
		if( !strlen(M->buffer) ) {
			M->cursor = 0;
			free( M->buffer );
			M->size = 0;
			M->buffer = 0;
			M->done = 1;
		} else {
			__beep( B );
		}
		break;
	case '\r':
	case '\n':
		M->done = 1;
		if( !M->echo ) {
			__addhistory( M, M->buffer );
		}

		M->cursor = strlen( M->buffer );
		__update( B, M, &P, X_CURSOR );
		__addchar( B, '\r' );
		__addchar( B, '\n' );
		break;

	case CTL('C'):
		/* Abandon this line */
		M->cursor = strlen( M->buffer );
		__update( B, M, &P, X_CURSOR );
		__addchar( B, '\r' );
		__addchar( B, '\n' );
		M->buffer[0] = 0;
		M->done = 1;
		break;

	case CTL('\\'):
		kill( getpid(), SIGQUIT );
		break;

	case CTL('P'):
	case K_U:
		if( M->echo ) {
			__beep( B );
		} else if( M->history.current && M->history.current->next ) {
			M->history.current = M->history.current->next;

			free( M->buffer ); 
			M->buffer = strdup( M->history.current->line );
			M->size = M->cursor = strlen( M->buffer );
			__update( B, M, &P, X_REDRAW );
		} else if ( !M->history.current && M->history.head ) {
			M->history.current = M->history.head;
			M->saved.buffer = strdup( M->buffer );
			M->saved.cursor = M->cursor;

			free( M->buffer ); 
			M->buffer = strdup( M->history.current->line );
			M->size = M->cursor = strlen( M->buffer );
			__update( B, M, &P, X_REDRAW );
		} else {
			__beep( B );
		}
		break;
	case CTL('N'):
	case K_D:
		if( M->echo ) {
			__beep( B );
		} else if( M->history.current && M->history.current->previous ) {
			M->history.current = M->history.current->previous;

			free( M->buffer );
			M->buffer = strdup( M->history.current->line );
			M->size = M->cursor = strlen( M->buffer );
			__update( B, M, &P, X_REDRAW );
		} else if( M->history.current && M->saved.buffer ) {
			M->history.current = 0;

			free( M->buffer );
			M->buffer = M->saved.buffer;
			M->saved.buffer = 0;
			M->size = strlen( M->buffer );
			M->cursor = M->saved.cursor;
			__update( B, M, &P, X_REDRAW );
		} else {
			__beep( B );
		}
		break;

	case '\t':
		if( M->completion && (M->cursor == n) ) {
			unsigned int	start;
			int	partial;
			char	*s;

			if( n && M->buffer[n-1] != ' ' ) {
				s = strrchr( M->buffer, ' ' );

				if( s ) {
					start = s - M->buffer + 1;
				} else {
					start = 0;
				}
			} else {
				start = n;
			}

			s = (M->completion)(M->arg, M->buffer, start, &partial);
			
			if( s ) {
				M->buffer[start] = 0;
				while( strlen(M->buffer) + strlen(s) + 1 > M->size ) {
					M->size = M->size ? (2*M->size) : 1;
					M->buffer = (char *) realloc( M->buffer, M->size + 1 );
				}

				strcat( M->buffer, s );

				if( !partial ) {
					strcat( M->buffer, " " );
				} 

				M->cursor = strlen(M->buffer);
				free(s);
				__update( B, M, &P, X_REDRAW );

				if( partial ) {
					M->possible = 1;
					__beep( B );
				}
			} else {
				if( M->choices && ((M->choices)(M->arg, M->buffer, start, 0) != 0) ){
					M->possible = 1;
				}
				__beep( B );
			}
		} else {
			__beep( B );
		}
		break;

	case K_Q:
		if( M->choices && (M->cursor == n) ) {
			unsigned int	start;

			if( n && M->buffer[n-1] != ' ' ) {
				char	*s = strrchr( M->buffer, ' ' );

				if( s ) {
					start = s - M->buffer + 1;
				} else {
					start = 0;
				}
			} else {
				start = n;
			}

			if( (M->choices)(M->arg, M->buffer, start, 0) != 0 ){
				__logbegin( B, M );
	
				__addstr( B, M->prompt );
				__addstr( B, M->buffer );
				__addchar( B, '\n' );
				__flush(B);

				(M->choices)(M->arg, M->buffer, start, 1);

				B = __newbuffer(M);
				__logend( B, M );
			} else {
				__beep( B );
			}
		} else {
			__beep( B );
		}
		break;

	default:
		if( isprint(c) ) {
			/* Self Insert */
			char	t, u;

			if( (n + 1) > M->size ) {
				M->size = M->size ? (2*M->size) : 1;
				M->buffer = (char *) realloc( M->buffer, M->size + 1 );
			}

			t = (char) c;
			for( i=M->cursor; i<n; i++ ) {
				u = M->buffer[i];
				M->buffer[i] = t;
				t = u;
			}

			M->buffer[n] = t;
			M->buffer[n+1] = 0;
			M->cursor++;
			__update( B, M, &P, X_INSERT, c );
		} else {
			__beep( B );
		}
	}
	__flush( B );
}

static void	__next( edit_t *M )
{
	int	c, e;
	char	__c;
	buffer_t
		*B;

	e = read( M->fd.in, &__c, 1 );

	switch( e ) {
	case 0:
		/* End of input, can't really happen in the mode selected, unless nonblocking I/O is happening */
		return;
	case -1:
		/* Could be a read interrupted by SIGWINCH? */
		__probewidth( M );
		B = __newbuffer(M);
		__update( B, M, 0, X_REDRAW );
		__flush( B );
		break;
	default:
		/* Normal handling, got a character */
		__push( M, __c );
		break;
	}

	while( (c = __pop(M)) != -1 ) {
		__key( M, c );
	}
}

void 	__prepare( edit_t *M, const char *prompt, int echo, 
		char *(*completion)(void *, const char *, unsigned int, int *), 
		int (*choices)(void *, const char *, unsigned int, int), 
		int (*permission)(void *, const char *, int), 
		void *arg )
{
	struct termios
		S;
	struct sigaction
		Z;
	char	*t;
	buffer_t
		*B;

	M->buffer = (char *) malloc (M->size + 1);
	M->buffer[0] = 0;

	if( prompt ) {
		M->prompt = strdup(prompt);;
	} else {
		M->prompt = strdup("");;
	}

	M->completion = completion;
	M->choices = choices;
	M->permission = permission;
	M->arg = arg;

	M->history.current = 0;
	M->echo = echo;

	/* Save the terminal state */
	tcgetattr( 0, &(M->terminal.state) );

	/* Now set it to 'single character input, no echo' mode */
	S = M->terminal.state;
	S.c_lflag |= IGNBRK;
	S.c_lflag &= ~(ICANON | ISIG | ECHO );
	S.c_cc[VMIN] = 1;
	S.c_cc[VTIME] = 0;
	tcsetattr( 0, TCSANOW, &S );

	/* Note the SA_RESTART has not been set -- trying hard to interrupt the 'read' call here! 
	** The old fashioned call to 'signal' didn't work for cygwin
	*/
	Z.sa_handler = __handler;
	Z.sa_flags = 0;
	sigemptyset( &(Z.sa_mask) );
	sigaction( SIGWINCH, &Z, &(M->terminal.handler) );

	B = __newbuffer(M);
	for( t = M->prompt; *t; t++ ) {
		if( isprint((int)(*t)) ) {
			__addchar( B, *t );
		} else {
			__addchar( B, ' ' );
		}
	}
	__flush( B );
}

edit_t	*qrlOpen( int history, const char *terminal, FILE *in, FILE *out )
{
	int	i, e = 0;
	const sequence_t
		*q;
	edit_t	*M;
	char	*name;

	if( !in ) {
		in = stdin;
		if( !isatty(fileno(in)) ) {
			return 0;
		}
			}

	if( !out ) {
		out = stderr;
	}

	M = (edit_t *) malloc( sizeof(edit_t) );

	M->fd.in = fileno(in);
	M->fd.out = fileno(out);

	M->possible = M->cursor = M->saved.cursor = M->done = M->size = M->fifo.head = M->fifo.tail = 0;
	M->saved.buffer = M->buffer = M->prompt = 0;
	M->completion = 0;
	M->arg = 0;
	M->history.head = M->history.tail = M->history.current = 0;
	M->history.size = history;
	M->history.count = 0;
	M->echo = 0;

	for( i=0; i<NCAPABILITY; i++ ) {
		M->terminal.capability[i] = 0;
	}

	for( i=0; i<NKEYCAP; i++ ) {
		M->terminal.keytable[i].string = 0;
		M->terminal.keytable[i].value = 0;
	}

	name = terminal? (char *) terminal : getenv( "TERM" );

	if( name ) {
		e = 0;

		/* 
		** If `curses' is really `ncurses', we should take care of the fact that it has memory leaks, especially
		** when it comes to del_curterm, so we wouldn't like to do the `setupterm ... del_curterm' cycle multiple times.
		** This can be done by initializing only once -- when cur_term is zero -- and NOT calling del_curterm
		** afterwards, so that the terminal stays initialized. Unfortunately, if `curses' is NOT ncurses, we cannot
		** depend on an uninitialized cur_term to be zero---though in such cases, the memory leak at del_curterm
		** may also not be there. So ... an ifdef.
		*/

#ifdef NCURSES_VERSION
		if( cur_term && (strcmp(name, termname()) != 0) ) {
			del_curterm( cur_term );
			cur_term = 0;
		}
#endif

		if( setupterm(name, 1, &e) != OK ) {
			free( M );
			return 0;
		}
	} 

	if( !name ) {
		/* If TERM wasn't set, use fallbacks for ANSI */
		M->terminal.width = 80;

		for( q = __screenfallback; q->string; q++ ) {
			M->terminal.capability[q->value] = strdup(q->string);
		}

		i = 0;
	} else {
		M->terminal.width = columns;

		for( q = __capability; q->string; q++ ) {
			char	*s = tigetstr( (char *)(q->string) );	/* `tigetstr' isn't const-correct sometimes */

			if( s ) {
				char	*t;

				M->terminal.capability[q->value] = strdup(s);

				/* Detect & ignore padding */
				t = strstr( M->terminal.capability[q->value], "$<" );
				if ( t ) {
					*t = 0;
				}
			}
		}

		for( q = __screenfallback; q->string; q++ ) {
			if( M->terminal.capability[q->value] == 0 ) {
#ifndef NCURSES_VERSION
				del_curterm( cur_term );
				cur_term = 0;
#else
				/* Nothing */
#endif
				qrlClose( M );
				return 0;
			}
		}

		i = 0;
		for( q = __keypad; q->string; q++ ) {
			char	*s = tigetstr( (char *)(q->string) );

			if( s && strchr(LEADERS, s[0]) ) {
				M->terminal.keytable[i].string = strdup( s );
				M->terminal.keytable[i].value = q->value;
				i++;
			}
		}
	}

	for( q = __keyfallback; q->string; q++ ) {
		const char	*s = q->string;

		if( strchr(LEADERS, s[0]) ) {
			M->terminal.keytable[i].string = strdup( s );
			M->terminal.keytable[i].value = q->value;
			i++;
		}
	}

	if( name ) {
		/* See the comment before setupterm */
#ifndef NCURSES_VERSION
		del_curterm( cur_term );
		cur_term = 0;
#else
		/* Nothing */
#endif
	}

	return M;
}

int	qrlProcess( edit_t *M )
{
	if( !M->done ) {
		__next( M );
	}

	return M->done;
}

char	*qrlGetLine( edit_t *M, const char *prompt, 
		char *(*completion)(void *, const char *, unsigned int, int *), 
		int (*choices)(void *, const char *, unsigned int, int), 
		int (*permission)(void *, const char *, int), 
		void *arg )
{
	qrlPrepare( M, prompt, completion, choices, permission, arg );

	while( !M->done ) {
		__next( M );
	}

	return qrlDone( M );
}

char *qrlGetSecret( edit_t *M, const char *prompt, int how )
{
	qrlPrepareSecret( M, prompt, how );

	while( !M->done ) {
		__next( M );
	}

	return qrlDone( M );
}

void	qrlLogBegin( const edit_t *M )
{
	buffer_t
		*B;

	if( !M->buffer ) {
		return;
	}

	B = __newbuffer(M);
	__logbegin( B, M );
	__flush( B );
}

void	qrlLogEnd( const edit_t *M )
{
	buffer_t
		*B;

	if( !M->buffer ) {
		return;
	}

	B = __newbuffer(M);
	__logend( B, M );
	__flush( B );
}

void	qrlSetPrompt( edit_t *M, const char *s )
{
	int	n;
	buffer_t
		*B;

	if( !M->buffer ) {
		return;
	}

	n = strlen( M->prompt );
	free( M->prompt );
	M->prompt = strdup( s );

	B = __newbuffer(M);
	__update( B, M, 0, X_NEWPROMPT, n );
	__flush( B );
}

void 	qrlPrepare( edit_t *M, const char *prompt, 
		char *(*completion)(void *, const char *, unsigned int, int *), 
		int (*choices)(void *, const char *, unsigned int, int), 
		int (*permission)(void *, const char *, int), 
		void *arg )
{
	__prepare( M, prompt, 0, completion, choices, permission, arg );
}

void 	qrlPrepareSecret( edit_t *M, const char *prompt, int how )
{
	__prepare( M, prompt, how?how:(-1), 0, 0, 0, 0 );
}

char	*qrlDone( edit_t *M )
{
	char	*r = M->buffer;

	if ( !r && !M->done ) {
		return 0;
	}

	M->buffer = 0;

	if( M->prompt ) {
		free( M->prompt );
		M->prompt = 0;
	}

	if( M->saved.buffer ) {
		free( M->saved.buffer );
		M->saved.buffer = 0;
	}

	/* Restore terminal state */
	tcsetattr( 0, TCSANOW, &(M->terminal.state) );
	sigaction( SIGWINCH, &(M->terminal.handler), 0 );

	M->echo = M->possible = M->cursor = M->saved.cursor = M->done = M->size = M->fifo.head = M->fifo.tail = 0;

	return	r;
}

void	qrlClose( edit_t *M )
{
	int	i;
	char	*s = qrlDone( M );
	if( s ) {
		free( s );
	}

	if( M->prompt ) {
		free( M->prompt );
	}

	if( M->saved.buffer ) {
		free( M->saved.buffer );
	}

	while( M->history.head ) {
		item_t	*T = M->history.head;

		M->history.head = T->next;
		free( T->line );
		free( T );
	}

	for( i=0; i<NCAPABILITY; i++ ) {
		if( M->terminal.capability[i] ) {
			free( M->terminal.capability[i] );
		}
	}

	for( i=0; i<NKEYCAP; i++ ) {
		if( M->terminal.keytable[i].string ) {
			free( M->terminal.keytable[i].string );
		}
	}

	free( M );
}

void	qrlDirty( edit_t *M )
{
	buffer_t
		*B;

	__probewidth( M );
	B = __newbuffer(M);
	__update( B, M, 0, X_REDRAW );
	__flush( B );
}
