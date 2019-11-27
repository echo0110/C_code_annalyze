/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Sample Porting Code -- For Unix Systems                                **/
/**                                                                        **/
/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include <port.h>
#include <semaphore.h>
#include <pthread.h>
#include <lteMisc.h>
/* SPR 19288 change start */
/* SPR 19288 change end */
/** The Porting Functions ***************************************************/
static void *my_malloc(unsigned int size)
{
	return malloc(size);
}

/* SPR 19288 change start */
/* SPR 19288 change end */
static void my_free(void *m)
{
	free(m);
}

static int my_vprintf(const char *format, va_list A)
{
	return vfprintf(stderr, format, A);
}

static void my_abort(void)
{
    abort_wrapper();
}

static void *my_newlock(void)
{
	SInt32 n;
	pthread_mutex_t *pMutex = malloc_wrapper(sizeof(pthread_mutex_t));

	/* Coverity 10788 */
	if (pMutex != NULL) {
		/* Coverity 10788 */
		if ((n = pthread_mutex_init((pthread_mutex_t *)pMutex, NULL)) == 0)
			errno = n;

		/* Coverity 110291 Fix End */
	}
	return pMutex;
}

static void my_destroylock(void *lock)
{
	pthread_mutex_destroy((pthread_mutex_t *)lock);
	free(lock);
}

static void my_lock(void *lock)
{
	/* Coverity 110292 Fix Start */
	SInt32 n;

	if ((n = pthread_mutex_lock((pthread_mutex_t *)lock)) != 0)
		errno = n;
	/* Coverity 110292 Fix End*/
}

static void my_unlock(void *lock)
{
	/* + coverity 111584 */
	SInt32 n;

	if (0 != (n = pthread_mutex_unlock((pthread_mutex_t *)lock)))
		errno = n;

	/* - coverity 111584 */
}

const QSYSOP my_os = {
	/* SPR 19288 change start */
	my_malloc,
	/* SPR 21001 Changes end */
	/* SPR 19288 change end */
	my_free,
	my_newlock,
	my_destroylock,
	my_lock,
	my_unlock,
	my_vprintf,
	my_vprintf,
	my_abort
};

const QSYSOP msg_os = {
	/* SPR 19288 change start */
	my_malloc,
	/* SPR 21001 Changes end */
	/* SPR 19288 change end */
	/* SPR 19288 change end */
	my_free,
	my_newlock,
	my_destroylock,
	my_lock,
	my_unlock,
	my_vprintf,
	my_vprintf,
	my_abort
};
