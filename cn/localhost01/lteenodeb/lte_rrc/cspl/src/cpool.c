/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Implementation of Context Pools         **/
/**                                                                        **/
/****************************************************************************/

#include	<cspl.h>

/* Code corres to SPR No: 2667*/
#ifdef MEM_OVERWRITE
unsigned char __CBUFPADMAGIC[]={0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x3a}; 
#endif


/** Memory Pool Management **************************************************/
/*****************************************************************************
* FUNCTION: qvPoolCreate
*
* DESCRIPTION:  
* 			This function creates a context pool of specified size of buffer
* 			and the number of buffers in the pool.
*
* ARGS:	
* 			size				size of each buffer in the pool
* 			nbuf				number of buffers in the pool
*
* RETURNS:		
* 			Pointer to the allocated pool
*
******************************************************************************/
QMPOOL	qvPoolCreate( unsigned int size, unsigned int nbuf)
{
	return qvPoolCreateEx(size, nbuf, 0);
}
QMPOOL	qvPoolCreateEx( unsigned int size, unsigned int nbuf, int *cErr)
{
	qmpool		*pool;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	/** Allocate the memory **/
	pool = (qmpool *)qvSysMalloc( YALIGN(sizeof(qmpool),8) );
	if( !pool ) {
		if(cErr)
		{
			*cErr = QVERROR_SYS_ALLOC_FAIL;
		}
		return 0;
	}

	ylInit( &(pool->list) );
	ylInit( &(pool->chunks) );
	pool->size = size;
	pool->nbuf = 0;
	pool->alloc = 0;
	pool->magic = __POOLMAGIC;
	pool->lock = qvNewLock();
#ifdef MEM_DEBUG
	pool->ref_ctr = 0;
#endif

	if( nbuf ) {
		if( !qvPoolExtendEx((QMPOOL)pool, nbuf, cErr) ) {
            /* SES-488 Coverity 48250 */
            qvDestroyLock( (void *)(pool->lock));
            /* SES-488 Coverity 48250 */
			qvSysFree( (void *)pool );
			return 0;
		}
	}

	return (QMPOOL)pool;
}
/*****************************************************************************
* FUNCTION: qvPoolDelete
*
* DESCRIPTION:  
* 			This function deletes the memory allocated for the pool.
* ARGS:	
* 			Q					Pointer to Pool.
*
* RETURNS:			
*
******************************************************************************/
void	qvPoolDelete( QMPOOL Q )
{
	qmpool		*pool = (qmpool *)Q;
	qmchunk		*chunk;
	QLOCK		lock = pool->lock;

	qvLock( lock );
	while( (chunk=(qmchunk *)ylPopHead(&(pool->chunks))) != 0 ) {
		qvSysFree( (void *)chunk );
	}

	qvSysFree( (void *)pool );
	qvUnlock( lock );

	/* Changes corres to SPR No: 2339 */	
	qvDestroyLock( lock );

}
/*****************************************************************************
* FUNCTION: qvPoolExtend
*
* DESCRIPTION:  
*			This function extends an existing pool by 'nbuf' number 
*			of buffers. 
* ARGS:	
* 			Q					Pointer to Pool.
* 			nbuf				number of buffers in the pool
*
* RETURNS:			
* 			1 if successful
* 			0 if unsuccessful
*
******************************************************************************/
int	qvPoolExtend( QMPOOL Q, unsigned int nbuf)
{
	return qvPoolExtendEx(Q, nbuf, 0);
}
int	qvPoolExtendEx( QMPOOL Q, unsigned int nbuf, int *cErr)
{
	qmpool		*pool = (qmpool *)Q;
	qmchunk		*chunk;
	unsigned int	size = pool->size;
	unsigned int	realsize, i;
	unsigned char	*p;
	
    /* Start SPR 5045 Fix */
	double total_size = 0 ,size_alloc = 0;
	/* End SPR 5045 Fix*/
	
/* Code corres. to SPR No: 2667*/
#ifdef MEM_OVERWRITE
	unsigned char	*r = NULL;
    unsigned int    j=0;
#endif

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

#ifdef MEM_DEBUG
	if ( nbuf > 0)
		nbuf = 1;
#endif

	/** Compute the memory required for each buffer**/
	realsize = YALIGN(sizeof(qmbuf),8) + YALIGN(size,8);

    /* Start SPR 5045 Fix */
	size_alloc= nbuf * (double)realsize;
	total_size= YALIGN(sizeof(qmchunk),8) + size_alloc ;
	if (total_size > MAX_VAL_U32)
	{
	  if(cErr)
	  {
		*cErr = QVERROR_SYS_ALLOC_FAIL;
	  }
	  return 0;
	}
	/* End SPR 5045 Fix*/	
	/** Allocate the memory **/
	/*Memory is allocated/extended in chunks of nbuf buffers.
	 * This list of chunks and actual buffers is maintained in the pool.*/
	p = (unsigned char *)qvSysMalloc( YALIGN(sizeof(qmchunk),8) + (nbuf * realsize) );
	if( !p ) {
		if(cErr)
		{
			*cErr = QVERROR_SYS_ALLOC_FAIL;
		}
		return 0;	/** Failed **/
	}

	chunk = (qmchunk *)(void *)p;
	chunk->nbuf = nbuf;

	qvLock( pool->lock );

	ylPushTail( &(pool->chunks), (YLNODE *)chunk );
	for( i=0, p += YALIGN(sizeof(qmchunk),8); i<nbuf; i++, p += realsize ) {
		qmbuf	*q = (qmbuf *)(void *)p;

		q->allocated = 0;
		q->incarnation = 0;

/* Start SPR 4283 Changes. */ 	
#ifdef MEM_DEBUG
	q->ref_id = 0;
#endif	
/* End SPR 4283 Changes. */ 

/* Code corres. to SPR No: 2667*/
#ifdef MEM_OVERWRITE
		    r = (unsigned char *)((unsigned char *)q)+(YALIGN(sizeof(qmbuf),8));
			for( j=0; j<(YALIGN(size,8)-size); j++) 
			{	  
		  		r[size+j]= __CBUFPADMAGIC[j];	
						
			}
#endif	
		ylPushHead( &(pool->list), (YLNODE *)q );
	}
	pool->nbuf += nbuf;

	qvUnlock( pool->lock );
	return 1;
}
/*****************************************************************************
* FUNCTION: qvPoolSize
*
* DESCRIPTION:  
* 			This function provides the size of pool in terms of number 
* 			of buffers in the pool. If the second argument is
* 			not null, it is set to the number of buffers already 
* 			allocated in the pool.
* ARGS:	
* 			Q					Pointer to Pool.
* 			p_alloc				Out param, returns number of buffers
* 								already allocated in the pool.
*
* RETURNS:			
* 			Size of the pool
*
******************************************************************************/
unsigned int	qvPoolSize(QMPOOL Q, unsigned int *p_alloc)
{
	qmpool		*pool = (qmpool *)Q;
	unsigned int	nbuf, nalloc;

	qvLock( pool->lock );
	nbuf = pool->nbuf;
	nalloc = pool->alloc;
	qvUnlock( pool->lock );

	if( p_alloc ) {
		*p_alloc = nalloc;
	}
	return nbuf;
}
/*****************************************************************************
* FUNCTION: qvPoolAlloc
*
* DESCRIPTION:  
*			This function allocates a buffer from the pool.
* ARGS:	
* 			pool					Pointer to Pool.
*
* RETURNS:			
* 			Pointer to the buffer allocated
*
******************************************************************************/
void	*qvPoolAlloc(QMPOOL pool)
{
	return qvPoolAllocEx(pool, 0);
}
void	*qvPoolAllocEx(QMPOOL pool, int *cErr)
{
	void	*p;
#ifdef MEM_DEBUG
	void	*k;
	/* Start SPR 4283 Changes. */ 
	unsigned int      pool_ref_ctr = 0;
    unsigned int      pool_temp_counter=1;
	/* End SPR 4283 Changes. */ 
#endif

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	qvLock( ((qmpool *)pool)->lock );
	if( (p = ylPopHead( &((qmpool *)pool)->list )) != 0 ) {
		((qmpool *)pool)->alloc++;
	}
	qvUnlock( ((qmpool *)pool)->lock );

	if( p ) {
		unsigned char	*c;
		qmbuf	*q = (qmbuf *)p;

		q->allocated = 1; /*If this buffer is allocated*/
		q->incarnation++; /*Number of times this buffer is allocated*/
		q->u.pool = (qmpool *)pool;
#ifdef MEM_DEBUG
        /* Start SPR 4283 Changes. */ 
		/* pool->ref_ctr stores the number of buffer a pool has.Since
		 * buffer could have been allocated from qvPoolAllocSpecific 
		 * function with any pool index we check whether the given index
		 * is occupied or not if it is not occupied we set the ref_id of
		 * that buffer to the next free index available
		 */
        pool_ref_ctr = ++(((qmpool *)pool)->ref_ctr) ;
        while(qvPoolLocate((void *)pool,pool_temp_counter))
        {
            pool_temp_counter++;
        }
		q->ref_id = pool_temp_counter;
        ((qmpool *)pool)->nbuf = ((qmpool *)pool)->ref_ctr;
        (((qmpool *)pool)->list).count = ((qmpool *)pool)->ref_ctr;
		/* End SPR 4283 Changes. */ 
#endif
		c = (unsigned char *)q;
		c += YALIGN(sizeof(qmbuf),8);

		return (void *)c;
	} else {
#ifdef MEM_DEBUG
		if( !qvPoolExtendEx(pool, 1, cErr) ) {
			return 0;
		}
		k = qvPoolAllocEx(pool, cErr);
		return k;
	}
#else	
		if(cErr)
		{
			*cErr = QVERROR_MEM_POOL_EXHAUSTED;
		}
		return 0;
	}
#endif
}
/*****************************************************************************
* FUNCTION: qvPoolFree
*
* DESCRIPTION:  
* 			This function frees the buffer, which was previously 
* 			allocated from a pool. The buffer is returned back to 
* 			the list of free buffers of pool to which it belonged.
* ARGS:	
* 			buffer			Pointer to the buffer to be freed.
*
* RETURNS:			
*
******************************************************************************/
void	qvPoolFree(void *buffer)
{
	unsigned char	*c = (unsigned char *)buffer;
	qmbuf	*q;
	qmpool	*pool;

	c -= YALIGN(sizeof(qmbuf),8);
	q = (qmbuf *)(void *)c;
	pool = q->u.pool;

	qvAssert( q->allocated );

	q->allocated = 0;

	qvLock( pool->lock );
#ifdef MEM_DEBUG
	c -= YALIGN(sizeof(qmchunk),8);
	ylDelete( &(pool->chunks), (YLNODE *)c);
	qvSysFree(c);
#else
	ylPushHead( &(pool->list), (YLNODE *)q );
#endif
	((qmpool *)pool)->alloc--;
#ifdef MEM_DEBUG
	((qmpool *)pool)->nbuf--;
#endif
	qvUnlock( pool->lock );
}
/*****************************************************************************
* FUNCTION: qvPoolSequence
*
* DESCRIPTION:  
* 			This function returns the unique number depending upon 
* 			the number of times 'buffer' has been allocated.
* ARGS:	
* 			buffer			Pointer to the buffer.
*
* RETURNS:			
*
******************************************************************************/
unsigned short	qvPoolSequence( void *buffer )
{
	unsigned char	*c = (unsigned char *)buffer;
	qmbuf	*q = (qmbuf *)(void *)(c - YALIGN(sizeof(qmbuf),8));

	return q->incarnation;
}
/*****************************************************************************
* FUNCTION: qvPoolIndex
*
* DESCRIPTION:
*			This function returns the index of 'buffer' in the pool 'Q'
*			only if the buffer is allocated.
*			Range of index is from 1 to nbuf where nbuf is the number 
*			of buffers in the pool.
* 
* ARGS:
* 			Q				Pointer to the pool
*           buffer          Pointer to the buffer.
*
* RETURNS:
* 			index of buffer if successful, else 0
*
******************************************************************************/
unsigned int qvPoolIndex( QMPOOL Q, void *buffer)
{
	return qvPoolIndexEx(Q, buffer, 0);
}
unsigned int qvPoolIndexEx( QMPOOL Q, void *buffer, int *cErr)
{
	qmpool		*pool = (qmpool *)Q;
	unsigned int	var_index = 0;
	unsigned char	*c = ((unsigned char *)buffer - YALIGN(sizeof(qmbuf),8));
#ifndef MEM_DEBUG
	unsigned int	realsize = YALIGN(sizeof(qmbuf),8) + YALIGN(pool->size,8);
	qmchunk		*q;
#endif

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	qvLock( pool->lock );
#ifndef MEM_DEBUG
	/*For loop to find out in which chunk the buffer lies.*/
	for( q = (qmchunk *)ylFirst(&(pool->chunks)); q; q = (qmchunk *)ylNext((YLNODE *)q) ) {
		unsigned char	*base = ((unsigned char *)q +  YALIGN(sizeof(qmchunk),8));
		if( (c >= base) && (c < (base + (realsize * q->nbuf))) ) {
			/** Within range, is it aligned? **/
			if( ((c - base) % realsize ) != 0 ) {
				q = 0;
				if(cErr)
				{
					*cErr = QVERROR_MEM_ILLEGAL_BUFFER;
				}
				break;
			} else {
				if( !((qmbuf *)(void *)c)->allocated ) {
					q = 0;
					if(cErr)
					{
						*cErr = QVERROR_MEM_UNALLOCATED_BUFFER;
					}
					break;
				} else {
					/*Index is combination of position of buffer
					 * within various lists of chunks and its
					 * position within the chunk it exists in*/
					var_index += (c - base)/realsize;
					break;
				}
			}
		}
		var_index += q->nbuf;
	}
	var_index = q?(var_index + 1):0;
#else
	var_index = ((qmbuf *)c)->ref_id;
#endif
	qvUnlock( pool->lock );
	return	var_index;
}
/*****************************************************************************
* FUNCTION: __qvPoolLocate
*
* DESCRIPTION:	
* 			Used to retrieve the pointer to a buffer in a pool based on the
* 			index of this buffer in that pool.
* 
* ARGS:
* 			pool			Pointer to the pool
*           index           Index of buffer in a pool.
*
* RETURNS:
* 			Pointer to the buffer.
*
******************************************************************************/
static qmbuf	*__qvPoolLocate( qmpool *pool, unsigned int indexparam )
{
#ifndef MEM_DEBUG
	unsigned int	realsize = YALIGN(sizeof(qmbuf),8) + YALIGN(pool->size,8);
#endif
	qmchunk		*q;
	unsigned char	*c;

	if( !indexparam ) {
		return 0;
	}
#ifndef MEM_DEBUG
	indexparam--;
#endif
	c = 0;

	for( q = (qmchunk *)ylFirst(&(pool->chunks)); q; q = (qmchunk *)ylNext((YLNODE *)q) ) {
#ifndef MEM_DEBUG
		if( indexparam < q->nbuf ) {
			c = (unsigned char *)q;
			c += YALIGN(sizeof(qmchunk),8);
			c += (realsize * indexparam);
			break;
		}
		indexparam -= q->nbuf;
#else
        c = (unsigned char *)q + YALIGN(sizeof(qmchunk),8);
		if ( indexparam == ((qmbuf *)c)->ref_id )
			return (qmbuf *)c ;
#endif
	}

#ifndef MEM_DEBUG
	return	(qmbuf *)(void *)c;
#else
	return 0;
#endif
}
/*****************************************************************************
* FUNCTION: qvPoolLocate
*
* DESCRIPTION:	
* 			Used to retrieve the pointer to a buffer in a pool based on the
* 			index of this buffer in that pool. Buffer returned only
* 			if it is allocated.
* 
* ARGS:
* 			pool			Pointer to the pool
*           index           Index of buffer in a pool.
*
* RETURNS:
* 			Pointer to the buffer.
*
******************************************************************************/
void	*qvPoolLocate( QMPOOL Q, unsigned int indexparam)
{
	return qvPoolLocateEx(Q, indexparam, 0);
}
void	*qvPoolLocateEx( QMPOOL Q, unsigned int indexparam, int *cErr)
{
	qmbuf		*m;
	unsigned char	*c;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	qvLock( ((qmpool *)Q)->lock );
	m = __qvPoolLocate( (qmpool *)Q, indexparam );

	if( m && m->allocated ) {
		c = (unsigned char *)m;
		c += YALIGN(sizeof(qmbuf),8);
	} else {
		c = 0;
		if ( !m ) {
			if(cErr)
			{
				*cErr = QVERROR_MEM_ILLEGAL_INDEX;
			}
		} else if ( !m->allocated ) {
			if(cErr)
			{
				*cErr = QVERROR_MEM_UNALLOCATED_BUFFER;
			}
		}
	}
	qvUnlock( ((qmpool *)Q)->lock );

	return (void *)c;
}
/*****************************************************************************
* FUNCTION: qvPoolAllocSpecific
*
* DESCRIPTION:	
* 			This function can be used for allocating a buffer from a 
* 			pool from some specified index. If the pool has a free buffer
* 			available at 'index' location in pool 'Q' then it will be 
* 			allocated. 'sequence' can be used to set the sequence number
* 			of the buffer to be allocated to a desired value, otherwise 
* 			set the 'sequence' argument to zero in the function call.
* ARGS:
* 			pool			Pointer to the pool
*           indexparam           Index of buffer in a pool.
*
* RETURNS:
* 			Pointer to the buffer.
*
******************************************************************************/
void	*qvPoolAllocSpecific( QMPOOL Q, unsigned int indexparam, unsigned short sequence)
{
	return qvPoolAllocSpecificEx(Q, indexparam, sequence, 0);
}

/* Start SPR 4283 Changes. */ 
#ifndef MEM_DEBUG
void	*qvPoolAllocSpecificEx( QMPOOL Q, unsigned int indexparam, unsigned short sequence, int *cErr)
{
	qmbuf		*m;
	unsigned char	*c;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	qvLock( ((qmpool *)Q)->lock );
	m = __qvPoolLocate( (qmpool *)Q, indexparam );
	if( m && !m->allocated ) {
		ylDelete( &(((qmpool *)Q)->list), (YLNODE *)m );

		((qmpool *)Q)->alloc++;

		m->incarnation = sequence;
		m->allocated = 1;
		m->u.pool = (qmpool *)Q;

		c = (unsigned char *)m;
		c += YALIGN(sizeof(qmbuf),8);
	} else {
		c = 0;
		if ( !m ) {
			if(cErr)
			{
				*cErr = QVERROR_MEM_ILLEGAL_INDEX;
			}
		} else if ( !m->allocated ) {
			if(cErr)
			{
				*cErr = QVERROR_MEM_ALLOCATED_BUFFER;
			}
		}
	}
	
	qvUnlock( ((qmpool *)Q)->lock );
	return (void *)c;
}
#endif
#ifdef MEM_DEBUG
void	*qvPoolAllocSpecificEx( QMPOOL Q, unsigned int indexparam, unsigned short sequence, int *cErr)
{
    qmbuf		*m = NULL;
    unsigned char	*c = NULL, *b = NULL;
    /* If required, Reset the CSPL Error Code. */
    if(cErr)
    {
        *cErr = QVERROR_NONE;
    }
    if (!indexparam){
        if(cErr)
        {
            *cErr = QVERROR_MEM_ILLEGAL_INDEX;
        }
        return 0;
    }


    qvLock( ((qmpool *)Q)->lock );
    m = __qvPoolLocate( (qmpool *)Q, indexparam );
    if(m)
    {
        /*That means buffer is allocated*/
        if(cErr)
        {
            *cErr = QVERROR_MEM_ALLOCATED_BUFFER;
        }
        return 0;
    }
    else
    {
        m= qvPoolAlloc(Q);
        b = (unsigned char *)m;
		b = b -YALIGN(sizeof(qmbuf),8);
        m = (qmbuf *)b;
        m->ref_id = indexparam;
       	m->incarnation = sequence;
		m->allocated = 1;
		m->u.pool = (qmpool *)Q;
		c = (unsigned char *)m;
		c += YALIGN(sizeof(qmbuf),8);
    }
	qvUnlock( ((qmpool *)Q)->lock );
	return (void *)c;
}
#endif
/* End SPR 4283 Changes. */ 
