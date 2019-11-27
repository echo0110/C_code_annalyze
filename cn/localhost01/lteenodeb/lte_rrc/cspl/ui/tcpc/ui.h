/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Main Program -- Interface Definition                            **/
/**                                                                        **/
/****************************************************************************/
#ifndef	__CSPL_UI_INTERFACE_HEADER__
#define __CSPL_UI_INTERFACE_HEADER__

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<strings.h>
#include	<time.h>
#include	<limits.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/times.h>
#include	<sys/time.h>
#include	<sys/stat.h>
#include	<pthread.h>

#include	<sys/un.h>
#include	<netinet/in.h>

#ifdef	__cplusplus
extern "C" {
#endif
#include	<readline/readline.h>
#include	<readline/history.h>
#ifdef	__cplusplus
}
#endif

#include	<ylib.h>
#include	<stacklayer.h>
#include	<shell.h>
#include	<util.h>

#define	MAXMSGSIZE	4096

int	ui_send( void *server, void *destination, unsigned char *message, unsigned int size, char *peername );
int	ui_receive( void *server, unsigned char *message, unsigned int maxsize, char *peername );
void	*ui_makeserver( char * );
void	*ui_hash( char * );
void	ui_free( void * );

#endif
