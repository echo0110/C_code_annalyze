/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#ifndef	_HSS_CMD_UTIL_H_
#define	_HSS_CMD_UTIL_H_

typedef struct	keyval_s	{
	char	*key;
	char	*value;
} keyval_t;

typedef struct	command_s	{
	char	*name;
	void	(*func)(int, char **, int, keyval_t *);
	char	*helptext;
}	command_t;

void	commandprocess( char *buffer, command_t *cmdlist );
char	*tokenize(char *string, char *delim, char sb, char se, char sx, char cc, char ck, char **pkey, char **plast );
void	commandprocess( char *buffer, command_t *cmdlist );
int	hexbuf( char *D, unsigned char *buffer, int buflen );
void	dump( unsigned char *D, unsigned long count, int (*printf)(char *, ...) );
void	flagset( int N, char **V, int *flagptr) ;
void	levelset( int N, char **V, int *flagptr) ;
int	errprintf( char *format, ... ) ;
char	*keylookup( char *key, int nC, keyval_t *nV );
void	errorabort( const char *s );
void	makeaddress(const char *s, struct sockaddr_in *address, unsigned long defaultaddress );
void	printaddress( char *s, const struct sockaddr_in *address );

#endif
