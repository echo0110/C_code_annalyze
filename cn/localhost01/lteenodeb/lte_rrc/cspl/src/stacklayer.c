/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Stack Layer Conventions                 **/
/**                                                                        **/
/****************************************************************************/

#include <cspl.h>
#include <stacklayer.h>
/*****************************************************************************
* FUNCTION: cvApiParseHeader
*
* DESCRIPTION: 
* 			This function parses the 'buffer' and populates the 
* 			fields of 'api' with information gathered by parsing.
* 			The buffer comprises Hdr, Paramlen and Payloadlen.
* ARGS:	
* 			buffer			Pointer to the buffer to be parsed
* 			length			Size of hdr
* 			api				Pointer to the STACKAPIHDR
* RETURNS:	
*			length of message (size of header + size of data + size 
*			of payload)
******************************************************************************/
unsigned int	cvApiParseHeader( U8 *buffer, unsigned int length, STACKAPIHDR *api )
{
	return cvApiParseHeaderEx(buffer, length, api, 0);
}
unsigned int	cvApiParseHeaderEx( U8 *buffer, unsigned int length, STACKAPIHDR *api, int *cErr)
{
	U8	*scan = buffer;
	U32 buflen = 0;
#ifdef LINUX_PC_TEST
    U32Long llen = 0;
#endif
	U16	payloadlen = 0, payloadoffset = 0;
	U16 temp_buflen = 0;
	STACKAPIHDR H;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	if( length < CV_HDRSIZE ) {
		if(cErr)
		{
			*cErr = QVERROR_MEM_ILLEGAL_BUFFER;
		}
		return 0;
	}
	scan = yfGetU8( scan, (U8 *)&(H.version) );
	scan = yfGetU16( scan, (U16 *)&(H.from) );
	scan = yfGetU16( scan, (U16 *)&(H.to) );
	scan = yfGetU16( scan, (U16 *)&(H.api) );
	scan = yfGetU16( scan, (U16 *)&temp_buflen );
#ifdef LINUX_PC_TEST
    if (0 == temp_buflen)
    {
	    scan = yfGetU32( scan, (U32 *)&llen );
        buflen = llen;
        H.priority = 0;
    }
    else
    {
#endif//LINUX_PC_TEST

	scan = yfGetU16( scan, (U16 *)&payloadoffset );
	scan = yfGetU16( scan, (U16 *)&payloadlen );
#ifdef HDRTYPE_EXT
	scan = yfGetU8( scan, (U8 *)&(H.priority) );
	scan = yfGetU16(scan, (U16 *)&buflen );
	buflen = buflen | temp_buflen;
	payloadlen = 0;
	payloadoffset = 0;
#else
	yfGetU8( scan, (U8 *)&(H.priority) );
	buflen = temp_buflen;
#endif

#ifdef LINUX_PC_TEST
    }
#endif//LINUX_PC_TEST

	if( buflen < CV_HDRSIZE ) {
		/** Oh no! This buffer is insane! **/
		if(cErr)
		{
			*cErr = QVERROR_MEM_ILLEGAL_BUFFER;
		}
		return 0;
	}

	if (!payloadoffset) {/*If no payload*/
		if( payloadlen ) {
			if(cErr)
			{
				*cErr = QVERROR_MEM_ILLEGAL_BUFFER;
			}
			return 0;
		} else {
			H.paramlen = buflen - CV_HDRSIZE;
			H.payloadlen = 0;
		}
	} else {
		if( (U32)payloadoffset > buflen ) {
			if(cErr)
			{
				*cErr = QVERROR_MEM_ILLEGAL_BUFFER;
			}
			return 0;
		} else {
			H.paramlen = payloadoffset - CV_HDRSIZE;
		}

		if( (U32)((payloadoffset + payloadlen)) > buflen ) {
			if(cErr)
			{
				*cErr = QVERROR_MEM_ILLEGAL_BUFFER;
			}
			return 0;
		} else {
			H.payloadlen = payloadlen;
		}
	}

	if( api ) {
		*api = H;
	}

	return CV_HDRSIZE + H.paramlen + H.payloadlen;
}
/*****************************************************************************
* FUNCTION: cvApiBuildHeader
*
* DESCRIPTION: 
* 			This function puts API header in the 'buffer'. The contents
* 			of API header are extracted from the second parameter 'H'.
* ARGS:	
* 			buffer			Pointer to the buffer to  be formed
* 			H				Pointer to STACKAPIHDR which has hdr info
* RETURNS:	
* 			buflen
*
******************************************************************************/
unsigned int	cvApiBuildHeader( U8 *buffer, STACKAPIHDR *H )
{
	U8	*scan = buffer;
	U32	buflen = 0;
	U16 payloadlen = 0, payloadoffset = 0;

	if( H->paramlen || H->payloadlen ) {
		buflen = CV_HDRSIZE + H->paramlen + H->payloadlen;
		if( H->payloadlen ) {
			payloadoffset = (U16)(buflen - H->payloadlen);
			payloadlen = H->payloadlen;
		}
	}

	buflen = buflen?buflen:CV_HDRSIZE;

	scan = yfAddU8( scan, (U8)H->version );
	scan = yfAddU16( scan, (U16)H->from );
	scan = yfAddU16( scan, (U16)H->to );
	scan = yfAddU16( scan, (U16)H->api );
	scan = yfAddU16( scan, (U16)buflen );
	scan = yfAddU16( scan, (U16)payloadoffset );
	scan = yfAddU16( scan, (U16)payloadlen );
	scan = yfAddU8( scan, (U8)H->priority );
#ifdef HDRTYPE_EXT
	scan = yfAddU16( scan, (U16)(buflen >> 16));
#else
	yfAddU16( scan, (U16)0 );
#endif
	return buflen;
}
/*****************************************************************************
* FUNCTION: cvApiBegin
*
* DESCRIPTION: 
* 			This function can be used for putting a header in the 
* 			API as per the information passed in the arguments. 
* ARGS:	
* 			buffer			Pointer to the buffer in which the hdr is to 
* 							be put
* 			version			Version of API
* 			from			source module id
* 			to				destination module id
* 			api				API id
* 			priority		Priority of the API
* RETURNS:	
*			Pointer to the end of hdr
******************************************************************************/
U8	*cvApiBegin( U8 *buffer, U8 version, U16 from, U16 to, U16 api, signed char priority )
{
	U8	*scan = buffer;

	if( !from ) {
        /* SES-488 Coverity 11464 */
        if((from = (U16)qvGetServiceName( qvSelf() )) == 0)
            return 0;
        /* SES-488 Coverity 11464 */
	}

	scan = yfAddU8( scan, (U8)version );
	scan = yfAddU16( scan, (U16)from );
	scan = yfAddU16( scan, (U16)to );
	scan = yfAddU16( scan, (U16)api );
	scan = yfAddU16( scan, (U16)0 );
	scan = yfAddU16( scan, (U16)0 );
	scan = yfAddU16( scan, (U16)0 );
	scan = yfAddU8( scan, (U8)priority );
	scan = yfAddU16( scan, (U16)0 );
	return scan;
}
/*****************************************************************************
* FUNCTION: cvApiEnd
*
* DESCRIPTION: 
* 			This function updates the information in API header with 
* 			the buffer length information
* ARGS:	
* 			buffer		Pointer to start of API buffer
* 			end			Pointer to end of API buffer
* RETURNS:	
* 			pointer to field after buffer length in header
*
******************************************************************************/
U8	*cvApiEnd( U8 *buffer, U8 *end )
{
	U32	buflen = (U32)(end - buffer);
#ifdef HDRTYPE_EXT
	yfSetU16At( buffer, CV_OF_BUFLEN, (U16)buflen );
	yfSetU16At( buffer, CV_OF_PAD, (U16)(buflen >> 16));
#else
	yfSetU16At( buffer, CV_OF_BUFLEN, (U16)buflen );
#endif
	return buffer;
}
/*****************************************************************************
* FUNCTION: cvApiAdjustIn
*
* DESCRIPTION: 
* 			This function modifies the header of the incoming API. 
* 			The offset and length fields for payload are set to zero.
* 			Buffer length is updated to 'size of header+size of data'.
* ARGS:	
* 			message		Pointer to incoming message
* RETURNS:	
*
******************************************************************************/
void	cvApiAdjustIn( void *message )
{
	unsigned int	payloadoffset = yfGetU16At((U8 *)message, CV_OF_SOPL);
#ifdef HDRTYPE_EXT
	unsigned int	paramlen = payloadoffset?
					(payloadoffset - CV_HDRSIZE):
					((((unsigned int)((unsigned int)(yfGetU16At((U8 *)message, CV_OF_PAD))) << 16) | ((unsigned int)yfGetU16At((U8 *)message, CV_OF_BUFLEN))) - CV_HDRSIZE);
#else
	unsigned int	paramlen = payloadoffset?
					(payloadoffset - CV_HDRSIZE):
					((unsigned int)yfGetU16At((U8 *)message, CV_OF_BUFLEN) - CV_HDRSIZE);
#endif

	yfSetU16At( (U8 *)message, CV_OF_BUFLEN, (unsigned short)(CV_HDRSIZE + paramlen ));
#ifdef HDRTYPE_EXT
	yfSetU16At((U8 *)message, CV_OF_PAD, (U16 )((CV_HDRSIZE + paramlen) >> 16));
#endif

	yfSetU16At( (U8 *)message, CV_OF_SOPL, 0 );
	yfSetU16At( (U8 *)message, CV_OF_LENPL, 0 );
#ifndef HDRTYPE_EXT
	yfSetU16At( (U8 *)message, CV_OF_PAD, 0 );
#endif
}
/*****************************************************************************
* FUNCTION: cvApiAdjustOut
*
* DESCRIPTION: 
* 			This function modifies the header of the outgoing API. 
* 			The offset and length fields for payload are calculated and
* 			update in API header. Buffer length is updated to 'size of
* 			header+size of data+size of payload'.
* ARGS:	
* 			message		Pointer to outgoing message
* RETURNS:	
*			Sum of hdr size and paramlen
******************************************************************************/
unsigned int	cvApiAdjustOut( void *message )
{
#ifdef HDRTYPE_EXT
	unsigned int	paramlen = 	 
					((((unsigned int)((unsigned int)(yfGetU16At((U8 *)message, CV_OF_PAD))) << 16) | ((unsigned int)yfGetU16At((U8 *)message, CV_OF_BUFLEN))) - CV_HDRSIZE);
#else
	unsigned int	paramlen = 	 
					((unsigned int)yfGetU16At((U8 *)message, CV_OF_BUFLEN) - CV_HDRSIZE);
#endif
	unsigned int	payloadsize = qvMsgSize(message, 0);

#ifndef HDRTYPE_EXT 
	yfSetU16At( (U8 *)message, CV_OF_SOPL, (unsigned short)(CV_HDRSIZE + paramlen) );
#endif

	yfSetU16At( (U8 *)message, CV_OF_LENPL, (unsigned short)payloadsize );

	yfSetU16At( (U8 *)message, CV_OF_BUFLEN, (unsigned short)(CV_HDRSIZE + paramlen + payloadsize ));
#ifdef HDRTYPE_EXT
	yfSetU16At((U8 *)message, CV_OF_PAD, (U16 )((CV_HDRSIZE + paramlen + payloadsize) >> 16));
#endif
	return paramlen + CV_HDRSIZE;
}
/*****************************************************************************
* FUNCTION: cvContextPoolSetup
*
* DESCRIPTION: 
* 			The function creates a context pool. 
* 			
* ARGS:	
* 			cpool			Pointer to the allocated pool
* 			bufsize			Size of buffers in pool
* 			min				Minimun buffers in the pool
* 			max				Maximum buffers in the pool
* 			step			Number of buffers by which the pool is extended
* 							when the pool is exhausted
* RETURNS:	1 if successful, else 0
*
******************************************************************************/
int	cvContextPoolSetup( 
		STACKCPOOL *cpool, unsigned int bufsize, unsigned int min, 
		unsigned int max, unsigned int step)
{
	cpool->bufsize = bufsize;
	cpool->min = min;
	cpool->max = max;
	cpool->step = step;

	if( (cpool->pool = qvPoolCreate( bufsize, min )) == 0 ) {
		return 0;
	} else {
		return 1;
	}
}
/*****************************************************************************
* FUNCTION: cvContextAlloc
*
* DESCRIPTION: 
* 			This function allocates a buffer from the 'cpool'
* ARGS:	
* 			cpool			Pointer to the pool from which buffer is to 
* 							be allocated
* RETURNS:	
* 			Pointer to the allocated buffer
*
******************************************************************************/
void	*cvContextAlloc( STACKCPOOL *cpool )
{
	void	*m = qvPoolAlloc( cpool->pool );

	if( m ) {
		return m;
	} else {/*Pool exhausted*/
		unsigned int	n = qvPoolSize( cpool->pool, 0 ), step;
		if( (n < cpool->max) || !cpool->max ) {/*If max not specified, 
												pool extension is limitless*/
			step = cpool->step;
			if( (cpool->max - n) < step ) {
				step = cpool->max - n;/*Pool can be extended only till
									   its max limit*/
			}
			if( !step ) {/*If not step specified, extended by 1 only*/
				step = 1;
			}
			if ( !qvPoolExtend( cpool->pool, step ) ) {
				return 0;
			}
		}
		m = qvPoolAlloc( cpool->pool );
		return m;
	}
}
/*****************************************************************************
* FUNCTION: cvMessagePoolSetup
*
* DESCRIPTION: 
* 			The function creates a message pool. 
* ARGS:	
* 			cpool			Pointer to the allocated pool
* 			bufsize			Size of buffers in pool
* 			min				Minimun buffers in the pool
* 			max				Maximum buffers in the pool
* 			step			Number of buffers by which the pool is extended
* 							when the pool is exhausted
* RETURNS:	1 if successful, else 0
*
******************************************************************************/
int	cvMessagePoolSetup( 
		STACKMPOOL *mpool, unsigned int bufsize, unsigned int min, 
		unsigned int max, unsigned int step)
{
	mpool->bufsize = bufsize;
	mpool->min = min;
	mpool->max = max;
	mpool->step = step;

	if( (mpool->pool = qvMsgPoolCreate( bufsize, min )) == 0 ) {
		return 0;
	} else {
		return 1;
	}
}
/*****************************************************************************
* FUNCTION: cvMessageAlloc
*
* DESCRIPTION: 
* 			This function allocates a buffer from the 'mpool'. 
* ARGS:	
* 			mpool			Pointer to the pool from which buffer is to 
* 							be allocated
* RETURNS:	
* 			Pointer to the allocated buffer
*
******************************************************************************/
void	*cvMessageAlloc( STACKMPOOL *mpool )
{
	void	*m = qvMsgAllocFromPool( mpool->pool );/*Calls qvMsgAlloc*/

	if( m ) {
		return m;
	} else {
		unsigned int	n = qvMsgPoolSize( mpool->pool, 0 ), step;
					
		if( (n < mpool->max) || !mpool->max ) {
			step = mpool->step;
			if( (mpool->max - n) < step ) {
				step = mpool->max - n;
			}
			if( !step ) {
				step = 1;
			}
			if ( !qvMsgPoolExtend( mpool->pool, step ) ) {
				return 0;
			}
		}
		m = qvMsgAllocFromPool( mpool->pool );
		return m;
	}
}
/*****************************************************************************
* FUNCTION: qvLadderDump
*
* DESCRIPTION: 
* 			This function is used for generating the ladder diagram 
* 			for inter module message exchange. 
* ARGS:	
* 			api_id			API Id which shall be depicted in the ladder
* 							diagram
* 			src_id			Service name of the module which sends the API
* 			dst_id			Service name of the module to which the API
* 							is sent
* RETURNS:	
*
******************************************************************************/
void 	qvLadderDump( unsigned short api_id, unsigned short src_id, unsigned short dst_id )
{
	unsigned char	*buf=NULL;

	buf = (unsigned char   *)qvMsgAllocBySize( LOGGER_API_SIZE );

	yfSetU16At(buf, 0, api_id);
	yfSetU16At(buf, 2, src_id);
	yfSetU16At(buf, 4, dst_id);
	/*Send the Msg which has the API to the logger utility
	 * which shall generate the ladder diagram*/	
	qvSend( qvGetService(LOGGER_MODULE_ID), 0 , (void *)buf);
}
	

