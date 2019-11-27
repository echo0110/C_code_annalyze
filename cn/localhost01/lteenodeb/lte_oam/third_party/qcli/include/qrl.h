/*******************************************************************************
* File Name   : qrl.h
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
#ifndef	__QRL_H___
#define	__QRL_H___

#include	<stdio.h>

typedef	struct	edit_t	edit_t;

extern edit_t *qrlOpen( 
		int history, 
		const char *terminal,
		FILE *in, 
		FILE *out );
extern void qrlPrepare( 
		edit_t *edit, 
		const char *prompt, 
		char *(*completion)(void *A, const char *L, unsigned int n, int *p), 
		int (*choices)(void *A, const char *L, unsigned int n, int p), 
		int (*permission)(void *A, const char *L, int c), 
		void *arg );
extern void qrlPrepareSecret( 
		edit_t *edit, 
		const char *prompt, 
		int how );
extern int qrlProcess( 
		edit_t *edit );
extern char *qrlDone( 
		edit_t *edit );
extern char *qrlGetLine( 
		edit_t *edit, 
		const char *prompt, 
		char *(*completion)(void *A, const char *L, unsigned int n, int *p), 
		int (*choices)(void *A, const char *L, unsigned int n, int p), 
		int (*permission)(void *A, const char *L, int c), 
		void *arg );
extern char *qrlGetSecret( 
		edit_t *edit, 
		const char *prompt, 
		int how );
extern void qrlClose( 
		edit_t *edit );
extern void qrlLogBegin( 
		const edit_t *edit );
extern void qrlLogEnd( 
		const edit_t *edit );
extern void qrlSetPrompt( 
		edit_t *edit, 
		const char *prompt );
extern void qrlDirty( 
		edit_t *edit );

#endif
