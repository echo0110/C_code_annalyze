/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Simple/Unix -- Posix Multithread execution flow                        **/
/**                                                                        **/
/****************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<strings.h>
#include	<time.h>
#include	<limits.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/times.h>
#include	<sys/time.h>
#include	<sys/select.h>
#include	<sys/stat.h>
#include	<sys/uio.h>
#include	<sys/un.h>
#include	<pthread.h>
#include	<errno.h>

#include	<shell.h>

typedef struct {
	QDRIVER driver;
	const QWAIT *wait;
} THREADPARAM;

static	void	errorabort( const char *s )
{
	char	*e = strerror(errno);
	qvPanic("%s: %s", s, e );
}

static void	*thread( void *T )
{
	QDRIVER	driver = ((THREADPARAM *)T)->driver;
	const QWAIT
		*wait = ((THREADPARAM *)T)->wait;

	qvSysFree(T);
	qvDriverLoop( driver, wait );
	return 0;
}

static void	__createthread( QDRIVER driver, const QWAIT *wait )
{
	THREADPARAM	*T = (THREADPARAM *)qvSysMalloc(sizeof(THREADPARAM));
	pthread_t	tid;

	T->driver = driver;
	T->wait = wait;

/*SPR 22554 Fix Start*/
        char  threadName[20] = "cspl_posix";
/*SPR 22554 Fix End*/
	if( pthread_create( &tid, 0, thread, (void *)T ) != 0 ) {
		errorabort("pthread_create");
	}
/*SPR 22554 Fix Start*/

    /* Set name of the main thread */
    	if (0 != pthread_setname_np(tid, threadName))
    	{
        	errprintf("Failed to set name of the cspl_posix thread ");
    	}

/*SPR 22554 Fix End*/
}

int	main(int C, char **V )
{
	ESPARAM M;

	M.argc = C - 1;
	M.argv = V + 1;

	esMain( &M, esGetStackList(), esGetPorting(), __createthread );

	/** Should never reach here ... **/
	return 0;
}
