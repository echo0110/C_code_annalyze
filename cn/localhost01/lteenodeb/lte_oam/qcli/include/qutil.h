/*******************************************************************************
* File Name   : qutil.h
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
#ifndef	__QUTIL_H_
#define	__QUTIL_H_

#include	<stdio.h>
#include	<stdarg.h>

extern char *qvasprintf( char *to, const char *format, va_list A, va_list B );  /* 64bit Fix */
extern char *qasprintf( char *to, const char *format, ... );
extern char *qfgetline( FILE *F );
extern char *qtokenize(
	char *string, 
	const char *delim, 
	const char *terminator, 
	int quote, 
	int escape, 
	char **plast,
	int *why );

extern char **qslalloc( void );
extern void qslfree( char **Q );
extern int qsllen( char **Q );
extern char **qslstr( char **Q, const char *s );
extern char **qsladdstr( char **Q, char *s );
extern char **qsladdstrcopy( char **Q, const char *s );
extern char **qsljoin( char **Q, char **Z );
extern char *qsllookup( char **Q, const char *const *Z, const char *s );

#endif
