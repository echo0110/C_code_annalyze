/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Simple/Unix -- Standard Single thread execution flow                   **/
/**                                                                        **/
/****************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>

#include	<shell.h>

int	main(int C, char **V )
{
	ESPARAM
		M;

	M.argc = C - 1;
	M.argv = V + 1;

	esMain( &M, esGetStackList(), esGetPorting(), 0 );

	/** Should never reach here ... **/
	return 0;
}
