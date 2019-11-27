/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include	<pthread.h>
#include	<sys/time.h>
#include	<cspl.h>

static	void	*s_malloc( unsigned int size )
{
	return malloc( size );
}

static	void	s_free( void *m )
{
	free(m);
}

static	void	*s_newkey( void )
{
	pthread_key_t	*key = (pthread_key_t *)malloc( sizeof(pthread_key_t) );

	pthread_key_create( key, 0 );
	return (void *)key;
}

static	void	*s_getkey( void *key )
{
	return	pthread_getspecific( *((pthread_key_t *)key) );
}

static	void	s_setkey( void *key, void *value )
{
	pthread_setspecific( *((pthread_key_t *)key), value );
}

static	void	*s_newlock( void )
{
	pthread_mutex_t	*lock = (pthread_mutex_t *)malloc( sizeof(pthread_mutex_t) );

	pthread_mutex_init( lock, 0 );
	return	(void *)lock;
}

static	void	s_lock( void *lock )
{
	pthread_mutex_lock( (pthread_mutex_t *)lock );
}

static	void	s_unlock( void *lock )
{
	pthread_mutex_unlock( (pthread_mutex_t *)lock );
}

static	void	s_destroylock( void *lock )
{
	pthread_mutex_destroy( (pthread_mutex_t *)lock );
	free( lock );
}

static	int	s_vprintf( const char *format, va_list A )
{
	return vfprintf( stderr, format, A );
}

static	void	s_abort(void)
{
#ifndef MODE_PROFILER_DEFINED
#ifdef RRC_DEBUG
    abort();
#endif

/*  #GS: NEW_BUILD_SYS: The exit() shell be used here instead of abort() to allow gcov generate code coverage information.
 *                      Now there is no way to stop CSPL application execution, so the Fatal error situation simulated to interrupt it.
 */
#else
    exit(1);
#endif
}

extern const QSYSOP	__os;
const QSYSOP	__os = {
	s_malloc,
	s_free,
	s_newkey,
	s_getkey,
	s_setkey,
	s_newlock,
	s_destroylock,
	s_lock,
	s_unlock,
	s_vprintf,
	s_vprintf,
	s_abort
};

const QSYSOP	*os = &__os;

typedef	struct __waitdata {
	pthread_cond_t	condition;
	pthread_mutex_t	lock;
	int		value;
} __waitdata;

static	void	s_walltime( QTIME *now )
{
    /* SPR 15822 Fix Start */
    struct timespec currentTimeStamp = {0,0};   
/* SPR 19288 change start */
    clock_gettime(CLOCK_MONOTONIC_RAW,&currentTimeStamp);
/* SPR 19288 change end */
    now->s = currentTimeStamp.tv_sec;
    now->us = currentTimeStamp.tv_nsec / 1000;
    /* SPR 15822 Fix End */
}

static	void	*s_newchannel( void )
{
	__waitdata	*w = (__waitdata *)malloc( sizeof(__waitdata) );

    pthread_cond_init(&(w->condition),0); /* SPR 11435 */
	pthread_mutex_init( &(w->lock), 0 );
	w->value = 0;

	return (void *)w;
}

static	void	s_sleep( void *channel, const QTIME *timeout )
{
	__waitdata	*w = (__waitdata *)channel;
	struct timespec	T;

	pthread_mutex_lock( &(w->lock) );
	if( !w->value ) {
		if( !timeout ) {
			pthread_cond_wait( &(w->condition), &(w->lock) );
		} else if ( timeout->s || timeout->us ) {
			struct timeval	_T;

			gettimeofday( &_T, 0 );

			_T.tv_sec += timeout->s;
			_T.tv_usec += timeout->us;
			while( _T.tv_usec >= 1000000L ) {
				_T.tv_usec -= 1000000L;
				_T.tv_sec++;
			}

			T.tv_sec = _T.tv_sec;
			T.tv_nsec = 1000 * _T.tv_usec;
			pthread_cond_timedwait( &(w->condition), &(w->lock), &T );
		} else {
			/** Nothing -- zero timeout was specified **/
		}
	}
	w->value = 0;
	pthread_mutex_unlock( &(w->lock) );
}

static	void	s_wakeup( void *channel )
{
	__waitdata	*w = (__waitdata *)channel;

	pthread_mutex_lock( &(w->lock) );
	w->value = 1;
	pthread_cond_signal( &(w->condition) );
	pthread_mutex_unlock( &(w->lock) );
}

extern const QWAIT __waitstruct;
const QWAIT __waitstruct = {
	s_newchannel,
	s_sleep,
	s_wakeup,
	s_walltime
};


const QWAIT *waitstruct = &__waitstruct;
