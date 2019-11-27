/*******************************************************************************
* File Name   : qcli.h
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
#ifndef	__QCLI_H__
#define	__QCLI_H__

/* SPR 17777 fix start
 * This Macro is added to avoid warnings generated due to unused parameters
 * for qcli module.
 */
/* SPR 17928 Start*/
/* Code Deleted */
/* SPR 17928 End */
/* SPR 17777 fix end */

typedef struct	parameter_t {
	int	more;
	const char	
		*type;
	char	*(*validate)(const char *input, const char *range);
	const char	
		*range;
	char	**(*generate)(const void *A);
	const char	
		*tag;
	const char	
		*defaultvalue;
	const char	
		*help;
}	parameter_t;

typedef struct	command_t {
	const char	
		*name;
	char	*(*action)(
			int C, 
			char *const *V, 
			const char *const *E, 
			const char *const *T, 
			void *R );
	const struct command_t 
		*subcommand;
	const struct parameter_t
		*parameters;
	const char	
		*mode;
	const char	
		*help;
}	command_t;

typedef struct	interpret_t interpret_t;

extern interpret_t *qclOpen( 
		const command_t *tree,
		int (*redirect)(int op, char *tail, void *arg),
		void *arg);
extern void qclClose( 
		interpret_t *interpreter );

extern int qclExecute( 
		interpret_t *interpreter, 
		const char *line );
extern char *qclComplete( 
		const interpret_t *interpreter, 
		const char *line, 
		unsigned int index,
		int *partial );
extern int qclChoices( 
		const interpret_t *interpreter, 
		const char *line, 
		unsigned int index,
		int print );
extern char **qclList( 
		const interpret_t *interpreter, 
		const char *line, 
		unsigned int index );
extern int qclQuoting( 
		const char *line);

extern int qclObjectCount( 
		const interpret_t *interpreter );
extern const char *qclGetObject( 
		const interpret_t *interpreter, 
		int index );
extern const char *qclGetMode( 
		const interpret_t *interpreter );
extern const char *qclGetCanonical( 
		const interpret_t *interpreter );

extern char *qclResult( 
		interpret_t *interpreter );

#endif
