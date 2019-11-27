/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Windows NT  -- Standard Single thread execution flow                   **/
/**                                                                        **/
/****************************************************************************/
#include	<windows.h>
#include	<winsock.h>
#include	<stdio.h>
#include	<errno.h>

#include	<shell.h>

int	main(int C, char **V )
{
	ESPARAM M;
	WSADATA	info;

	if( WSAStartup( MAKEWORD(1,1), &info ) != 0 ) {
		perror("WSAStartup" );
		abort();
	}

	M.argc = C - 1;
	M.argv = V + 1;

	esMain( &M, esGetStackList(), esGetPorting(), 0 );

	/** Should never reach here ... **/
	return 0;
}
