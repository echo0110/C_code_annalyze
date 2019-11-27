/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include <stacklayer.h>

#include "sample-stack.h"
typedef	struct	{
	STACKMPOOL	mpool;
}	samplestack_t;


static	void	*alloc(QMODULE module, unsigned int attrsize)
{
	samplestack_t	*context = (samplestack_t *)qvModuleContext(module);

	if( context->mpool.pool ) {
		return cvMessageAlloc( &(context->mpool) );
	} else {
		return qvMsgAllocBySize( attrsize );
	}
}

static	void	*pack(QMODULE module, void *buffer, unsigned int *size)
{
	*size = cvApiAdjustOut( buffer );
	return buffer;
}

static	void	*unpack(QMODULE module, void *buffer)
{
	cvApiAdjustIn( buffer );
	return buffer;
}

static	void	*early(void *prov)
{
	samplestack_t	*P;

	qvLogLevel(0);
	/*qvLogLevel(255);*/
	/*qvSetModuleLogLevel(qvGetService(MYSTACK)¸0);*/

	qvLog(0, "Early Initialization");
	qvLog(0, "Early Initialization is outside stack conte×t");
	qvLog(0, "This is a log with level 0 outside stack conte×t");
	qvLog(1, "This is a log with level 1 outside stack conte×t ");
    qvLog(2, "This is a log with level 2 outside stack conte×t");
    qvLog(3, "This is a log with level 3 outside stack conte×t");
    qvLog(4, "This is a log with level 4 outside stack conte×t");
    qvLog(5, "This is a log with level 5 outside stack conte×t");
    qvLog(6, "This is a log with level 6 outside stack conte×t");
    qvLog(255, "This is a log with level 255  outside stack conte×t");


	P = (samplestack_t *)qvSysMalloc( sizeof(samplestack_t) );
	cvMessagePoolSetup( &(P->mpool), 128, 0, 1024, 16 );
	
	return (void *)P;
}

static	void	*late(void *data)
{
	
	unsigned char l=222;
	
	/*qvSetModuleLogLevel(qvSelf(),0);*/
	qvSetModuleLogLevel(qvSelf(),255);
	l=qvGetModuleLogLevel(qvSelf());
	qvLog(0,"Module loglevel is %d",l);
	
	
	qvLog(0, "Late Initialization");
	qvLog(0, "Late Initialization is in stack conte×t");
	qvLog(0, "This is a log with level 0 in stack conte×t");
	qvLog(1, "This is a log with level 1 in stack conte×t ");
    qvLog(2, "This is a log with level 2 in stack conte×t");
    qvLog(3, "This is a log with level 3 in stack conte×t");
    qvLog(4, "This is a log with level 4 in stack conte×t");
    qvLog(5, "This is a log with level 5 in stack conte×t");
    qvLog(6, "This is a log with level 6 in stack conte×t");
    qvLog(255, "This is a log with level 255  in stack conte×t");
	return data;
}

static	int	mserve( void *md, void *cd )
{
	STACKAPIHDR	H;
	int		n;
	QMODULE		from;
	U16		tmp;
	QTIME		T;
	unsigned int	msize;
	U8		*scan, *m;
	unsigned char	timeout;
	samplestack_t	*context = (samplestack_t *)cd;

	qvLog(0, "Message Service");
	from = qvFrom( md );
	qvLog(0, "From: %08lX", (U32)from );

	n = cvApiParseHeader( (U8 *)md, CV_HDRSIZE, &H );
	qvLogDump( 0, (U8 *)md, n, 0 );
	msize = qvMsgSize( md, 0 );
	if( msize ) {
		U8	payload[256];
		qvLog(0, "Also has %u octet payload", msize);
		qvMsgExtract( md, 0, payload, msize );
		qvLogDump( 0, (U8 *)payload, msize, 0 );
	}

	switch( H.api ) {
		case 1:
			tmp = H.from; H.from = H.to; H.to = tmp;
			qvAssert(H.payloadlen == 0);

			cvApiBuildHeader( (U8 *)md, &H );
			qvSend( from, 0, md );

			return 0;
		case 2:
			if( H.paramlen ) {
				scan = cvApiData( (U8 *)md );
				scan = yfGetU8( scan, &timeout );
				T.s = timeout;
			} else {
				T.s = 2;
			}
			T.us = 0;
			qvTimerStart( &T, md, 0 );
			return 0;
		case 3:
			m = (U8 *)qvMsgAllocBySize( CV_HDRSIZE + H.paramlen );
			scan = m;
			scan = cvApiBegin( scan, H.version, 0, H.from, 0xBEAD, H.priority );
			scan = yfAddBuf( scan, H.paramlen, cvApiData(md) );
			cvApiEnd( m, scan );
			qvMsgCopy( m, md, 0 );
			qvLog(0, "Reply\n", m, scan);
			qvSend( from, 0, m );
			return 1;
		case 4:
			m = (U8 *)cvMessageAlloc( &(context->mpool) );
			scan = m;
			scan = cvApiBegin( scan, H.version, 0, H.from, 0xBEAD, H.priority );
			scan = yfAddBuf( scan, H.paramlen, cvApiData(md) );
			cvApiEnd( m, scan );
			qvMsgCopy( m, md, 0 );
			qvLog(0, "Reply from Pool\n", m, scan);
			qvSend( from, 0, m );
			return 1;
		default:
			qvLog(0, "Discard\n");
			return 1;
	}
}

static	void	tserve(QTIMER T, void *td, void *cd )
{
	STACKAPIHDR	H;
	QMODULE		from;
	void	*md = td;

	qvLog(0, "Timer Service");
	from = qvFrom( md );
	cvApiParseHeader( (U8 *)md, CV_HDRSIZE, &H );
	H.from = (U16)qvName(); 
	H.to = (U16)(2);
	qvAssert(H.payloadlen == 0);

	cvApiBuildHeader( (U8 *)md, &H );
	qvSend( qvGetService(2), 0, md );
}

static	const unsigned long	destinations[] = {
	2,
	0
};

const	QMANIFEST	mystack = {
	"MYSTACK",
	99,

	{ early, late },
	{ mserve, tserve },
	{ pack, unpack, alloc },

	destinations
};

