/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** vxWorks     -- Standard Single thread execution flow                   **/
/**                                                                        **/
/****************************************************************************/

#include <shell.h>
#include <stdarg.h>


/* Changes regarding compilation on vxworks */
/* Start SPR 5921 Changes. */
int emain(char *,...);
/* End SPR 5921 Changes. */

int	emain(char *first,...)
{
	ESPARAM	M;
	va_list	A;
	int	i = 0;

	if( !first ) {
		M.argc = 0;
		M.argv = 0;
	} else {
		int	N = 0;
		char	*c;
		char	**arg;

		c = first;
		va_start( A, first );
		while( c ) {
			N++;
			c = va_arg(A, char *);
		}
		va_end(A);

		arg = (char **) malloc( N * sizeof(char *) );

		i = 0;
		c = first;
		va_start( A, first );
		while( c ) {
			arg[i++] = c;
			c = va_arg(A, char *);
		}
		va_end(A);
		M.argc = N;
		M.argv = arg;
	}

	esMain( &M, esGetStackList(), esGetPorting(), 0 );

	/** Should never reach here ... **/
	return 0;
}

