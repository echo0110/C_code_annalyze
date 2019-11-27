/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Utility Functions for Drivers           **/
/**                                                                        **/
/****************************************************************************/

#ifndef	__HSS_SHELL_H_
#define	__HSS_SHELL_H_

#include	<cspl.h>

extern const QSYSOP  __os;
extern const QSYSOP  *os;
extern const QWAIT   __waitstruct;
extern const QWAIT   *waitstruct;
extern const QSHELL   __operations;
extern const QSHELL   *operations;

typedef	struct	ESLIST {
	const QMANIFEST
		*stack;
	void	*data;
	unsigned long 
		service;
	const char 
		*name;
	unsigned int	
		thread;
} ESLIST;

typedef	struct	ESPORT {
	const QSHELL	*ipc;
	const QSYSOP	*os;
	const QWAIT	*wait;
} ESPORT;

typedef	struct	ESPARAM {
	int	argc;
	char	**argv;
} ESPARAM;

/** Macros for accessing the IPC header *************************************/
#define	SH_HDRSIZE	16

#define	SH_GETPRIORITY(b)	((signed char)yfGetU8At((b), 1))

#ifdef HDRTYPE_EXT

#define	SH_GETATTRSIZE(b)	\
		(unsigned int)(((unsigned int)yfGetU16At((b), 6)) << 16) | \
		((unsigned int)yfGetU16At((b), 2))
#define SH_GETPAYLOADSIZE(b)	0	
#define	SH_SETATTRSIZE(b,n)	\
	(yfSetU16At((b), 6, (unsigned short)(n >> 16)));	\
	(yfSetU16At((b), 2, (unsigned short)(n)));	
#define	SH_SETPAYLOADSIZE(b,n)

#else
	
#define	SH_GETATTRSIZE(b)	((unsigned int)yfGetU16At((b), 2))
#define	SH_GETPAYLOADSIZE(b)	((unsigned int)yfGetU16At((b), 4))
#define	SH_SETATTRSIZE(b,n)	(yfSetU16At((b), 2, (unsigned short)(n)))
#define	SH_SETPAYLOADSIZE(b,n)	(yfSetU16At((b), 4, (unsigned short)(n)))

#endif

#define	SH_GETSOURCE(b)		((unsigned long)yfGetU32At((b), 8))
#define	SH_GETDESTINATION(b)	((unsigned long)yfGetU32At((b), 12))

#define	SH_SETPRIORITY(b,n)	(yfSetU8At((b), 1, (unsigned char)(n)), yfSetU8At((b), 0, 0xEE))

#define	SH_SETSOURCE(b,n)	(yfSetU32At((b), 8, (unsigned long)(n)))
#define	SH_SETDESTINATION(b,n)	(yfSetU32At((b), 12, (unsigned long)(n)))

/** Callback - For the sample encapsulating shell ***************************/
const ESLIST 
	*esGetStackList(void);
const ESPORT 
	*esGetPorting(void);

/** Generic Execution Flow **************************************************/
void	esMain( ESPARAM *data, const ESLIST *stacklist, const ESPORT *port, 
			void (*createthread)(QDRIVER, const QWAIT *) );

/** Support for Generic Multiplexed Sockets *********************************/
typedef	struct	LPAIR {
	const char	*name;
	const void	*handler;
} LPAIR;

typedef	struct	LINFO {
	unsigned long	from, to;
	unsigned int	paramlen;
	unsigned long	payloadlen;
	signed char	priority;
} LINFO;

typedef	struct	LBUF {
	unsigned char	*buffer;
	unsigned int	size;
} LBUF;

typedef	struct	LSTYLE {
	void	*(*newstyle)( char * );	
	unsigned int	
		(*approve)( void *, unsigned char *, unsigned int, LINFO *, LBUF *, const char * );
	void	(*space)( void *, unsigned int *, unsigned int * );
	const char	
		*(*prepare)( void *, unsigned char *, unsigned int, const LINFO *, LBUF * );
} LSTYLE;

const LPAIR *esGetApiStyles(void);	/** Callback **/

#endif
