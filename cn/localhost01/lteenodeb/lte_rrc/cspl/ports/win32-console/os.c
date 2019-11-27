/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include	<cspl.h>

static	void	*__qvc;

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
	return 0;
}

static	void	*s_getkey( void *key )
{
	return __qvc;
}

static	void	s_setkey( void *key, void *value )
{
	__qvc = value;
}

static	void	*s_newlock( void )
{
	return	0;
}

static	void	s_lock( void *lock )
{
}

static	void	s_unlock( void *lock )
{
}

static	void	s_destroylock( void *lock )
{
}

static	int	s_vprintf( const char *format, va_list A )
{
	return vfprintf( stderr, format, A );
}

static	void	s_abort(void)
{
	abort();
}

extern const QSYSOP __os;
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

const QSYSOP *os = &__os;

static	void	s_walltime( QTIME *now )
{
	DWORD	ms = GetTickCount();

	now->s = ms / 1000;
	now->us = 1000 * (ms % 1000);
}

extern const QWAIT __waitstruct;
const QWAIT __waitstruct = {
	0,
	0,
	0,
	s_walltime
};


const QWAIT *waitstruct = &__waitstruct;
