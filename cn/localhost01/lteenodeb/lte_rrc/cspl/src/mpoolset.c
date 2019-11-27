/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Implementation of Context Pools         **/
/**                                                                        **/
/****************************************************************************/

#include	<cspl.h>

static void    qvMsgPoolSetSort( QMPOOL *Q, int n );


/*****************************************************************************
* FUNCTION: qvMsgPoolSetCreate
*
* DESCRIPTION: Create a task specific memory pool
*
* ARGS:
* 		t		QPOOLSETTUNING parameter which describes the buffer sizes alongwith the
*				number of buffers in each member of the pool .
*
* RETURNS:
*		Pool id of the pool 
*
******************************************************************************/
QMPOOLSET qvMsgPoolSetCreate(QPOOLSETTUNING *t, int *cErr)
{
    unsigned int    i,k;
    unsigned int    size, nbuf;
    qmpool	    *q;
    unsigned int    iserror = 0;
    qmpoolset          *rpd;       

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

    if (!rpd ) {
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
    /* SES-488 Coverity-110284 */
    for( i=0; i<MAX_POOLS; i++ ) {
        rpd->alloc[i] = 0;
    }
    rpd->magic = 0;
    rpd->opt = 0;
    rpd->npools = 0;
    /* SES-488 Coverity-110284 */

    for( i=0; i<t->npools; i++ ) {
        size = t->qpt[i].bufsize;
        nbuf = t->qpt[i].nbufs;

        /*
        *Create a memory pool of a particular size
        */
        if (( q = (qmpool*)qvMsgPoolCreateEx( size, nbuf, cErr)) !=0) {
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
		qvMsgPoolDelete(rpd->alloc[k]);

	qvUnlock(rpd->lock);
        qvDestroyLock(rpd->lock); 
	qvSysFree(rpd);

	return 0 ; 
    }


    rpd->npools = t->npools;

    /*
    *Sort the pool in order of their buffer sizes
    */
    qvMsgPoolSetSort( rpd->alloc, rpd->npools );
    qvUnlock(rpd->lock);

    return (QMPOOLSET *)rpd ;
}
/*****************************************************************************
* FUNCTION: qvMsgPoolSetAlloc
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
void    *qvMsgPoolSetAlloc(QMPOOLSET *rpd, unsigned int size,unsigned int *psize, int *cErr)
{
    unsigned int        i, n ;
    unsigned int        npools ;
    qmpoolset           *rvp;
    
	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

    rvp =  (qmpoolset *)rpd ;

    if (!rpd) {
		if(cErr)
		{
			*cErr = QVERROR_INVALID_ARGUMENT;
		}
	        return 0 ;
    }
      
    /*
       Increment size to get the actual size of the buffer allocated 
    */
    size += qvGetQMessageSize();
        

    npools = rvp->npools ;

	if ( size > qvMsgPoolBufferSize(rvp->alloc[npools-1]) )
	{
		if(cErr)
		{
			*cErr = QVERROR_MEM_ALLOC_ILLEGAL_SIZE;
		}
		return 0;
	}

    /* 
    *Search the pools for best fit size
    *Start from the smallest pool .
    *The pools are sorted in order of sizes 
    */
    for( i=0 ; i<npools; i++ ) {

        /*
        * Find the size of the pool 
        */
        n=qvMsgPoolBufferSize(rvp->alloc[i]);


    	/*
    	*Check for the best fit memory
    	*/
    	if ( size <= n ) {


            void    *p = qvMsgAllocFromPoolEx(rvp->alloc[i], cErr);

            if( p == 0 ) {
                continue;
            }

                /*
        	* Update the pointer with the pool size from which 
                * memory was allocated 
        	*/
        	if( p && psize ) {
                *psize = (n - qvGetQMessageSize());
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
* FUNCTION: qvMsgPoolSetExtend
*
* DESCRIPTION: Extend the task specific memory 
*              Pool with the parameters specified
* ARGS:
* 		rpd 	Pool id
*		t	QPOOLSETTUNING parameter which describes the buffer sizes alongwith the
*			number of buffers in each member of the pool .
*
* RETURNS:
*		0/1
*
******************************************************************************/
unsigned int qvMsgPoolSetExtend(QMPOOLSET *rpd, QPOOLSETTUNING *t, int *cErr)
{
    unsigned int        i,k;
    unsigned int        size, nbuf;
    qmpoolset           *rvp;
    unsigned int 	iserror = 0 ,npools ;
    QLOCK               lock ;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

       /*
	* Find the context pool
	*/

    rvp =  (qmpoolset *)rpd ;

    if (!rpd || !t || t->opt || (t->npools > MAX_POOLS) ) {
		if(cErr)
		{
			*cErr = QVERROR_INVALID_ARGUMENT;
		}
		return 0;
    }
    lock = rvp->lock ;
    qvLock(lock);

    for( i=0; i<t->npools; i++ ) {

        unsigned int j =0 ;

        size = t->qpt[i].bufsize ; 
        size +=  qvGetQMessageSize(); /* Add the size of QMESSAGE */
        nbuf = t->qpt[i].nbufs;

        npools = rvp->npools ;

        /*
        * Find the pool which needs to be updated 
        */
        while (j < npools) {
                        
			if ( (qvMsgPoolBufferSize(rvp->alloc[j])) == size) {
                                /*
                                 * Extend the specified pool 
                                 */
				if (!qvMsgPoolExtendEx( rvp->alloc[j], nbuf, cErr) ) {

                                        iserror = 1 ;
                                        break ;

					} else  {

					break ;
				}
			}
			j++;
		}

        if(iserror)
          break ;
    }

    if(iserror)
    {
      unsigned int j =0 ;
      for (k=0;k<i;k++) {
         j=0;
         size = t->qpt[k].bufsize;
         size +=  qvGetQMessageSize(); /* Add the size of QMESSAGE */
         nbuf = t->qpt[k].nbufs;
         while (j < rvp->npools) {
                if ( qvMsgPoolBufferSize(rvp->alloc[j]) == size) {
	                __qvMsgPoolDecrease( rvp->alloc[j], nbuf ); /* Update this function */
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
* FUNCTION: qvMsgPoolSetDelete
*
* DESCRIPTION: Delete a pool
* ARGS:
* 		rpd  	Pool id
*
* RETURNS:
*		0/1
*
******************************************************************************/
unsigned int qvMsgPoolSetDelete( QMPOOLSET *rpd, int *cErr)
{
    qmpoolset           *rvp;
    QLOCK 		lock ;
    unsigned int        i =0;
    unsigned int        nalloc,npools;
    
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
        qvMsgPoolSize(rvp->alloc[i],&nalloc);

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
    
    for(i=0;i<npools;i++) {
                  /* 
                   * Delete the memory pools from the MM pool context 
                   */
                  qvMsgPoolDelete( rvp->alloc[i] );
     }


     /* 
     * First Unlock should be done then Destroy 
     * the lock and then Free rvp
      */

     /* SES-488 Coverity 48357 */ 
     qvUnlock(lock);     
     qvDestroyLock(lock);
     qvSysFree(rvp);
     /* SES-488 Coverity 48357 */
     return 1 ;
}


/*****************************************************************************
* FUNCTION: qvMsgPoolSetSize
*
* DESCRIPTION:  Return the size of the mm pools and 
*               and the numbers allocated in each of them 
*
* ARGS:*        rpd - Pool id of the pool whose size needs to be determined
*               t   - QPOOLSETTUNINGINFO structure that contains size of pool 
*                     buffers , number of buffers in each pool and number 
*                     allocated buffers in each pool 
*
* RETURNS:  
*              0/1
******************************************************************************/
unsigned int qvMsgPoolSetSize(QMPOOLSET *rpd, QPOOLSETTUNINGINFO *t, int *cErr)
{
    unsigned int    i ;
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
        t->qpt[i].bufsize = qvMsgPoolBufferSize(rvp->alloc[i]) - qvGetQMessageSize() ;
        t->qpt[i].nbufs = qvMsgPoolSize(rvp->alloc[i],&nalloc);
        t->qpt[i].nalloc = nalloc;
    }

    t->npools = rvp->npools ;

    return 1;
}

/*****************************************************************************
* FUNCTION: qvMsgPoolSetSort
*
* DESCRIPTION: Rearranging array of qmpool pointers 
*              in asc. order based on bufsize of each pool
*
* ARGS:
*
* RETURNS:  NOTHING
*
******************************************************************************/
static void    qvMsgPoolSetSort( QMPOOL *Q, int n )
{
    int lcv, rh, where;
    QMPOOL *temp=NULL;
      

    for( rh=n-1; rh>0; rh-- ) {
        where = 0;
        for ( lcv=1; lcv<=rh; lcv++ )
            if ( qvMsgPoolBufferSize(*(Q + lcv)) > qvMsgPoolBufferSize(*(Q + where)))
                where = lcv;
        temp = (QMPOOL*)*(Q + where);
        *(Q + where) = *(Q + rh);
        *(Q + rh) = temp;
      }
}

