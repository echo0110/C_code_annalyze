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
#include	<sample-stack.h>

/** The Main Program ********************************************************/
int	main(int C, char **V )
{
	void	*listener;

	if( C < 2 ) {
		fprintf( stderr, "Usage: %s port\n", V[0] );
		exit(1);
	}

	qvSimpleInit( &my_os );
	listener = qvOpen( &my_shell, V[1] );

	qvRegister( 0, 0, 0, &mystack, 0 );

	qvSetDestination( 0, &my_shell, listener, 0 );
	qvRun( &my_shell, listener, &my_waitstruct, 0 );

	return 0;
}

