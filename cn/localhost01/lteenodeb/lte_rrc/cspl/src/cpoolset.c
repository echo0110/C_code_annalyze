/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Implementation of Context Pools         **/
/**                                                                        **/
/****************************************************************************/

#include	<cspl.h>

static void    qvPoolSetSort( QMPOOL *Q, int n );
static void __qvPoolDecrease(QMPOOLSET Q, unsigned int nbufs);


/*****************************************************************************
* FUNCTION: qvPoolSetCreate
*
* DESCRIPTION: Create a task specific MM pool 
*
* ARGS:
* 		t	QPOOLSETTUNING parameter which describes the buffer sizes alongwith the
*			number of buffers in each member of the pool .
*
* RETURNS:
*		Pointer to the Pool
*
******************************************************************************/
QMPOOLSET qvPoolSetCreate(QPOOLSETTUNING *t, int *cErr)
{
    unsigned int        i,k;
    unsigned int        size, nbuf;
    qmpool 		*q;
    unsigned int 	iserror = 0;
    qmpoolset           *rpd;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

    if (!t || t->opt || (t->npools > MAX_POOLS) ) {
		if(cErr)
		{
			*cErr = QVERROR_INVALID_ARGUMENT;
		}
		return 0 ;
	}
	/*
	* Allocate a pool context
	*/
    rpd = (qmpoolset *)qvSysMalloc( YALIGN(sizeof(qmpoolset),8) );
    if (!rpd) {
		if(cErr)
		{
			*cErr = QVERROR_SYS_ALLOC_FAIL;
		}
		return 0;
    }


    rpd->lock = qvNewLock();

    qvLock(rpd->lock);

    /*
    *Allocate the buffers in the pool .
    *The buffer size and the number of buffers are specified which allocating
    *the members of the pools
    */
    /* SES-488 Coverity-110287 */
    for( i=0; i<MAX_POOLS; i++ ) {
        rpd->alloc[i] = 0;
    }
    rpd->magic = 0;
    rpd->opt = 0;
    rpd->npools = 0;
    /* SES-488 Coverity-110287 */

    for( i=0; i<t->npools; i++ ) {
        size = t->qpt[i].bufsize;
        nbuf = t->qpt[i].nbufs;

        /*
         * create the memory pool of a particular size 
         */
        if (( q =(qmpool*)qvPoolCreateEx( size, nbuf, cErr)) !=0) {
			rpd->alloc[i] = (QMPOOL)q ;
		} else {
			iserror=1;
			break ;
		}
    }

	/*
	*In case of error rollback to original configuration
	*/
	if (iserror) {

			for( k=0; k<i;k++ )
	                        qvPoolDelete(rpd->alloc[k]);
			qvUnlock(rpd->lock);
                        qvDestroyLock(rpd->lock);
			qvSysFree(rpd);
			return 0 ; 
	}


    rpd->npools = t->npools;

    /*
    *Sort the pool in order of their buffer sizes
    */
    qvPoolSetSort( rpd->alloc, rpd->npools ); /* Check the types */

    qvUnlock(rpd->lock);

    return (QMPOOLSET)rpd ;
}
/*****************************************************************************
* FUNCTION: qvPoolSetAlloc
*
* DESCRIPTION: Allocates a buffer of a particular size from a specific pool
*
* ARGS:
* 		rpd		Pool id
*		size 	Size of the buffer required
*		psize 	Actual size returned
*
* RETURNS:
*		Pointer to the allocated memory
*
******************************************************************************/
void    *qvPoolSetAlloc(QMPOOLSET rpd, unsigned int size,unsigned int *psize, int *cErr)
{
    unsigned int        i, n;
    qmpoolset           *rvp;
    unsigned int        npools ;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

    
    /*
     * Find the context 
     */
    rvp =  (qmpoolset *)rpd ;

    if (!rpd) {
		if(cErr)
		{
			*cErr = QVERROR_INVALID_ARGUMENT;
		}
		return 0 ; 
    }
    

    npools = rvp->npools ;

	if ( size > ( ((qmpool *)rvp->alloc[npools-1])->size ))
	{
		if(cErr)
		{
			*cErr = QVERROR_MEM_ALLOC_ILLEGAL_SIZE;
		}
		return 0;
	}
    
    /* 
     * Search the list of pools for best possible size 
     * of buffer 
     */
    for( i=0 ; i<npools; i++ ) {

        qmpool          *q ;
        q = (qmpool *)rvp->alloc[i];
        n=((qmpool *)rvp->alloc[i])->size ;
    	/*
    	*Check for the best fit memory
    	*/
    	if ( size <= n ) {

            void    *p = qvPoolAllocEx( q, cErr);

            if( p == 0 ) {
                continue;
            }

            /*
        	* Update the pointer for actual buffer size
        	*/
        	if( p && psize ) {
                *psize = q->size;
            }

        	/*
        	*Return the allocated memory
        	*/
		if(cErr)
		{
			*cErr = QVERROR_NONE;
		}

        	return p;
        }
    }
    return 0;
}

/*****************************************************************************
* FUNCTION: qvPoolSetExtend
*
* DESCRIPTION: Extend the task specific pool with the parameters specified
* ARGS:
* 		rpd 	Pool id
*		t       QPOOLSETTUNING parameter which describes the buffer sizes alongwith the
*			number of buffers in each member of the pool .
*
* RETURNS:
*		0/1
*
******************************************************************************/
unsigned int qvPoolSetExtend(QMPOOLSET rpd, QPOOLSETTUNING *t, int *cErr)
{
    unsigned int        i,k;
    unsigned int        size, nbuf;
    qmpoolset           *rvp;
    QLOCK               lock ;
    unsigned int        npools ;
    unsigned int        iserror = 0 ;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}


    /*
	* Find the context pool
	*/

    rvp =  (qmpoolset *)rpd ;

    if (!rpd || !t || t->opt || (t->npools > MAX_POOLS) )
	{
		if(cErr)
		{
			*cErr = QVERROR_INVALID_ARGUMENT;
		}
		return 0;
    }

    lock = rvp->lock ;

    qvLock(lock);
    for( i=0; i<t->npools; i++ ) 
	{
        unsigned int j =0 ;

        size = t->qpt[i].bufsize;   /*Size of the buffer-to be extended*/
        nbuf = t->qpt[i].nbufs;     /*Increase in the no. of buffers in the pool*/

        npools = rvp->npools ;       /*No. of pools in the poolset */
        
        /* Find the pool that needs to be extended */
        while (j < npools) {
                        unsigned int poolsize ;
                        
                        poolsize = (((qmpool *)rvp->alloc[j])->size) ;

			if ( poolsize == size) {
				if (!qvPoolExtendEx(rvp->alloc[j], nbuf, cErr) )
				   {
                                        iserror=1;
                                        break ;
				   } 
					else 		 
					     break ;   /*Extends only a single pool*/
				    
								   }
			j++;
				        	}
        
        if(iserror)
              break ;
    }
/* RollBack in case of error */
    if (iserror) {
	unsigned int j ;
        for (k=0;k<i;k++) {
                j=0;
		size = t->qpt[k].bufsize;
		nbuf = t->qpt[k].nbufs;

		while (j < rvp->npools) {
		if ( (((qmpool *)rvp->alloc[j])->size) == size) { 
				__qvPoolDecrease( rvp->alloc[j], nbuf ); 
				
		}
		j++;
	     }
	}
        qvUnlock(lock);
      return 0; 
   }

   qvUnlock(lock);

   return 1;
}

/*****************************************************************************
* FUNCTION: qvPoolSetDelete
*
* DESCRIPTION: Delete a pool
* ARGS:
* 		rpd  	Pool id
*
* RETURNS:
*		void
*
******************************************************************************/
unsigned int    qvPoolSetDelete( QMPOOLSET rpd, int *cErr)
{
        qmpoolset       *rvp;
        QLOCK 		lock ;
        unsigned int    i=0;
        unsigned int    npools,nalloc ;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

    /*
	* Find the context pool
	*/

    rvp =  (qmpoolset *)rpd ;

    if (!rpd) {
		if(cErr)
		{
			*cErr = QVERROR_INVALID_ARGUMENT;
		}
		return 0;	
    }


    lock = rvp->lock ;
    
    npools = rvp->npools ;

    for (i=0;i<npools ;i++)
    {
        nalloc =((qmpool *)rvp->alloc[i])->alloc;

        if (nalloc > 0)
		{
			if(cErr)
			{
				*cErr = QVERROR_MEM_ALLOCATED_BUFFER;
			}
			return 0; 
		}
    }
        

    qvLock(lock);
    /*
     * Delete the individual pools 
     */
    for(i=0;i<npools;i++) {
		  qvPoolDelete( rvp->alloc[i] );
     }

    /*
     * First Unlock should be done then Destroy 
     * the lock and then Free rvp
     */
    qvUnlock(lock); /* SES-488 Coverity 48356 */
    qvDestroyLock(lock);
    qvSysFree(rvp); /* SES-488 Coverity 48356 */

    return 1;
}



/*****************************************************************************
* FUNCTION: qvPoolSetSize
*
* DESCRIPTION:  Return the size of the context pools and 
*               and the numbers allocated in each of them 
*
* ARGS:         
*               rpd - Pool id of the pool whose size needs to be determined
*               t   - QPOOLSETTUNINGINFO structure that contains size of pool 
*                     buffers , number of buffers in each pool and number 
*                     allocated buffers in each pool 
*
* RETURNS:  
*               0/1
*
******************************************************************************/
unsigned int qvPoolSetSize(QMPOOLSET rpd, QPOOLSETTUNINGINFO *t, int *cErr)
{
    unsigned int    i;
    qmpoolset *rvp;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}


    /*
	* Find the context pool
	*/

    rvp =  (qmpoolset *)rpd ;

    if (!rpd || !t ) {
		if(cErr)
		{
			*cErr = QVERROR_INVALID_ARGUMENT;
		}
		return 0;
    }

    for( i=0; i<rvp->npools; i++ ) {
        unsigned int nalloc;
        /*
         * Update the pool size , total number of buffers 
         * and number of allocated buffers 
         */
        t->qpt[i].bufsize = ((qmpool *)rvp->alloc[i])->size;
        t->qpt[i].nbufs = qvPoolSize(rvp->alloc[i],&nalloc);
        t->qpt[i].nalloc = nalloc;
    }

    t->npools = rvp->npools ;

    return 1;
}


static void __qvPoolDecrease(QMPOOL Q, unsigned int nbufs)
{
	qmpool *q=(qmpool *)Q;
	unsigned int i ;
        YLNODE	      *chunk;
        QLOCK       lock = q->lock;

        qvLock( lock );
	for (i=0;i<nbufs;i++) {
              ylPopHead(&(q->list));
			  q->nbuf--;
	}
        chunk = ylPopTail(&(q->chunks));
        qvAssert(chunk);
              qvSysFree((void *)chunk);
		        qvUnlock( lock );

	return ;

}
  
/*****************************************************************************
* FUNCTION: qvPoolSetSort
*
* DESCRIPTION: Rearranging array of qmpool pointers 
*              in asc. order based on bufsize of each pool
*
* ARGS:
*
* RETURNS:  NOTHING
*
******************************************************************************/
static void    qvPoolSetSort( QMPOOL *Q, int n )
{
    int lcv, rh, where;
    QMPOOL *temp=NULL;
      

    for( rh=n-1; rh>0; rh-- ) {
        where = 0;
        for ( lcv=1; lcv<=rh; lcv++ )
            if ( ((qmpool *)(*(Q + lcv)))->size > ((qmpool *)(*(Q + where)))->size)
                where = lcv;
        temp = (QMPOOL*)*(Q + where);
        *(Q + where) = *(Q + rh);
        *(Q + rh) = temp;
      }
}

