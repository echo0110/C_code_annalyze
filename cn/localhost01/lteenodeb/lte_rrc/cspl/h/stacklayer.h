/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Conventions Library                     **/
/**                                                                        **/
/****************************************************************************/

#ifndef	__HSS_STACKLAYER_H__
#define	__HSS_STACKLAYER_H__

#include	<cspl.h>

/** Constants ***************************************************************/
#define CV_OF_VERSION	0
#define CV_OF_FROM	1
#define CV_OF_TO	3
#define CV_OF_API	5
#define CV_OF_BUFLEN	7
#define CV_OF_SOPL	9
#define CV_OF_LENPL	11
#define CV_OF_PRIORITY	13
#define	CV_OF_PAD	14

#define CV_HDRSIZE	16

/** Types *******************************************************************/
typedef unsigned long long  U64;
typedef struct	{
	U8	version;
	U16	from;
	U16	to;
	U16	api;
	S8	priority;
	unsigned int	paramlen;
	unsigned int	payloadlen;
} STACKAPIHDR;

typedef	struct	{
	unsigned int	
		bufsize, min, max, step;
	QMPOOL	pool;
} STACKCPOOL;

typedef	struct	{
	unsigned int	
		bufsize, min, max, step;
	QMPOOL	pool;
} STACKMPOOL;

/** Functions ***************************************************************/
unsigned int	
	cvApiParseHeader( U8 *buffer, unsigned int length, STACKAPIHDR *api );
unsigned int	
	cvApiParseHeaderEx( U8 *buffer, unsigned int length, STACKAPIHDR *api, int *cErr);
unsigned int	
	cvApiBuildHeader( U8 *buffer, STACKAPIHDR *api );
U8	*cvApiBegin( U8 *buffer, U8 version, U16 from, U16 to, U16 api, signed char priority );
U8	*cvApiEnd( U8 *buffer, U8 *end );
#define	cvApiData(p) ((U8 *)((U8 *)(p) + CV_HDRSIZE ))
void	cvApiAdjustIn( void *message );
unsigned int	
	cvApiAdjustOut( void *message );

int	cvContextPoolSetup( 
		STACKCPOOL *cpool, unsigned int bufsize, unsigned int min, 
		unsigned int max, unsigned int step);
#define cvContextPoolDelete(p) (qvPoolDelete((p)->pool))
void	*cvContextAlloc( STACKCPOOL *cpool );
#define	cvContextIndex(p,c)	(qvPoolIndex((p)->pool, (c)))
#define	cvContextLocate(p,n)	(qvPoolLocate((p)->pool, (n)))
#define	cvContextFree	qvPoolFree

int	cvMessagePoolSetup( 
		STACKMPOOL *mpool, unsigned int bufsize, unsigned int min, 
		unsigned int max, unsigned int step);
void	*cvMessageAlloc( STACKMPOOL *mpool );

void 	qvLadderDump(unsigned short api_id,
		unsigned short src_id,
		unsigned short dst_id);
void	*cvGetBufferFrom( 
		QMODULE module, U16 api, unsigned int paramlen, unsigned int payloadlen );


#endif
