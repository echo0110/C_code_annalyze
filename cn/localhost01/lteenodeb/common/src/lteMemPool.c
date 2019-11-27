/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMemPool.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Wrapper library for providing memory pool
 *                     functionality.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMemPool.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.9.10.1  2009/06/09 11:24:51  gur19140
 * first wave of Integration
 *
 * Revision 1.12  2009/05/27 04:48:47  gur19479
 * updated for pools
 *
 * Revision 1.11  2009/05/12 06:06:06  gur19479
 * pdcp merging
 *
 * Revision 1.10  2009/05/06 05:39:24  gur20548
 * pdcp common merged with lte common
 *
 * Revision 1.1  2009/05/04 14:52:04  gur20548
 * pdcp files added
 *
 * Revision 1.9  2008/12/29 10:07:52  gur19836
 * Done changes for getMemPoolStats function name
 *
 * Revision 1.8  2008/10/13 12:39:01  gur19836
 * Enhancements Added
 *
 * Revision 1.7  2008/09/19 10:34:58  gur19836
 * Review comments incorporated for cleanupMemPool
 *
 * Revision 1.6  2008/09/19 05:57:59  gur19836
 * MemPool cleanup functionality added
 *
 * Revision 1.5  2008/09/18 05:25:08  gur19836
 * Review comments incorporated
 *
 * Revision 1.4  2008/09/09 11:11:53  ssinghal
 * comments added to functions
 *
 * Revision 1.3  2008/09/08 10:02:56  ssinghal
 * memory and string compare and copy APIs added
 *
 * Revision 1.2  2008/08/25 10:05:02  ssinghal
 * Template applied and minor changes done
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "mempool.h"
#include "lteMemPool.h"
#include "lteMemPool.h"

/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern QSYSOP my_os;

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Function Name  : initMemPool
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Initilises the memory pool.
 ****************************************************************************/
void initMemPool(void)
{
	qvInit(&my_os);
	return;
}
/****************************************************************************
 * Function Name  : getMemFromPoolDebug
 * Inputs         : size,psize
 * Outputs        : pointer to allocated memory
 * Returns        : None.
 * Variables      : None.
 * Description    : allocates the memory.
 ****************************************************************************/
#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
void *_getMemFromPoolDebug(unsigned int size, unsigned int *psize, char *func, unsigned int line)
{
	void *data_p = PNULL;

	data_p = qvAllocEx(size, psize, 0, func, line);
	if (data_p == PNULL)
		ltePanic("Memory not allocated for buffer of size [%u]\n", size);

	return  data_p;
}
#endif

/* Cyclomatic_complexity_changes_start */
/****************************************************************************
 * Function Name  : createMemPool
 * Inputs         : blockSize - size of block to be formed.
 *                  nBuf - number of buffers to be formed for the given size.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Creates the memory pool and allocates memory for the
 *                  indicated block size and number of buffers are formed
 *                  based on the nBuf parameter.
 ****************************************************************************/
#if ((!defined(UE_SIM_TESTING)) && ((defined(MEM_CHECK_DOUBLE_FREE) && defined(MAC_AUT_TEST)) || defined(MEM_LEAK_DEBUG)))
void createMemPool(UInt32 blockSize, UInt32 nBuf)
{
	UInt32 cnt = nBuf;

	if (cnt)
	    qvPoolCreate(blockSize, 0);
}
#else
void createMemPool(UInt32 blockSize, UInt32 nBuf)
{
    qvPoolCreate(blockSize, nBuf);
    return;
}
#endif

#ifdef UE_SIM_TESTING
/* Cyclomatic_complexity_changes_end */
/****************************************************************************
 * Function Name  : createAndReturnMemPool
 * Inputs         : blockSize - size of block to be formed.
 *                  nBuf - number of buffers to be formed for the given size.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Creates the memory pool and allocates memory for the
 *                  indicated block size and number of buffers are formed
 *                  based on the nBuf parameter.
 ****************************************************************************/
void *createAndReturnMemPool(UInt32 blockSize, UInt32 nBuf)
{
    return (qvPoolCreateAndReturn(blockSize, nBuf));
}

/****************************************************************************
 * Function Name  : freeMemPoolAndReturn
 * Inputs         : pool - pool to be returned to system.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : return the allocated memory to the system.
 ****************************************************************************/
void freeMemPoolAndReturn(void *pool)
{
   if (pool != PNULL) {
       qvAllFreeFromMemPool(pool);
       pool = PNULL;
   }
}

/****************************************************************************
 * Function Name  : cleanupMemPool
 * Inputs         : None
 * Outputs        : Cleans the memory allocated for pools.
 * Returns        : None.
 * Variables      : None.
 * Description    : Frees all the memory allocated for pools.
 ****************************************************************************/
void cleanupMemPool(void)
{
    qvCleanupMemPool();
}

/****************************************************************************
 * Function Name  : getMemPoolStats
 * Inputs         : None
 * Outputs        : Displays pool statistics.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function displays the statistics of all pools.
 ****************************************************************************/
void getMemPoolStats(void)
{
    qvPoolStats();
}

/****************************************************************************
 * Function Name  : getMemFromPool
 * Inputs         : size - size of memory required by the application.
 *                  pSize - Pointer to integer indicating the allocated
 *                  memory of this size in the pool.
 * Outputs        : None.
 * Returns        : Returns the memory allocated.
 * Variables      : None.
 * Description    : Returns the memory allocated by the memory pool.
 ****************************************************************************/
void *getMemFromPool(UInt32 size, UInt32 *pSize)
{
    return qvAlloc(size, pSize);
}

/****************************************************************************
 * Function Name  : allocateMemFromPool
 * Inputs         : size - size of memory required by the application.
 *                  pSize - Pointer to integer indicating the allocated
 *                  memory of this size in the pool.
 * Outputs        : None.
 * Returns        : Returns the memory allocated.
 * Variables      : None.
 * Description    : Returns the memory allocated by the memory pool.
 ****************************************************************************/
void *allocateMemFromPool(void *pool)
{
    return ((void *)qvPoolAlloc(pool));
}

/****************************************************************************
 * Function Name  : extendPool
 * Inputs         : pool - Pointer to pool.
 *                : size - Size of memory required by the application.
 *                  memory of this size in the pool.
 * Outputs        : None.
 * Returns        : Returns the memory allocated.
 * Variables      : None.
 * Description    : This function extends an existing pool by nbuf number.
 ****************************************************************************/
UInt32 extendPool(void *pool, UInt32 size)
{
    return qvPoolExtend(pool, size);
}


/****************************************************************************
 * Function Name  : freeMemPool
 * Inputs         : p - Pointer to the memory to be freed.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This frees the memory back to the pool that was
 *                  already allocated.
 ****************************************************************************/
void freeMemPool(void *p)
{
    qvFree(p);
}


/****************************************************************************
 * Function Name  : getMemFromSys
 * Inputs         : size - memory required by the application.
 * Outputs        : None.
 * Returns        : Returns the memory to be allocated from the system.
 * Variables      : None.
 * Description    : Returns the memory allocated from the system and not
 *                  from the memory pool.
 ****************************************************************************/
void *getMemFromSys(UInt32 size)
{
    return qvSysMalloc(size);
}


/****************************************************************************
 * Function Name  : freeMemSys
 * Inputs         : p - pointer to memory to be returned to the system.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Returns the memory back to the system.
 ****************************************************************************/
void freeMemSys(void *p)
{
    qvSysFree(p);
}

/****************************************************************************
 * Function Name  : getPoolSequence
 * Inputs         : Pointer to the buffer.
 * Outputs        : None.
 * Returns        : Unique number.
 * Variables      : None.
 * Description    : This function returns the unique number depending upon
 *                  the number of times 'buffer' has been allocated.
 ****************************************************************************/
UInt16 getPoolSequence(void *buffer)
{
    return qvPoolSequence(buffer);
}

/****************************************************************************
 * Function Name  : getPoolIndex
 * Inputs         : Pointer to the pool
 *                  Pointer to the buffer
 * Outputs        : None.
 * Returns        : index of buffer if successful, else 0
 * Variables      : None.
 * Description    : This function returns the index of 'buffer' in the pool 'Q'
 *                  only if the buffer is allocated.Range of index is from 1
 *                  to nbuf where nbuf is the number of buffers in the pool.
 ****************************************************************************/
UInt32 getPoolIndex(void *pool, void *buffer)
{
    return qvPoolIndex(pool, buffer);
}

/****************************************************************************
 * Function Name  : getPoolLocate
 * Inputs         : Pointer to the pool,
 *                  Index of buffer in a pool
 * Outputs        : None.
 * Returns        : Pointer to the buffer
 * Variables      : None.
 * Description    : This function Used to retrieve the pointer to a buffer in
 *                  a pool based on the index of this buffer in that pool.
 *                  Buffer returned only if it is allocated.
 ****************************************************************************/
void *getPoolLocate(void *pool, UInt32 indexParam)
{
    return qvPoolLocate(pool, indexParam);
}

/****************************************************************************
 * Function Name  : deletePool
 * Inputs         : p - Pointer to the memory to be freed.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function deletes the memory allocated for the pool.
 ****************************************************************************/
void deletePool(void *p)
{
    qvPoolDelete(p);
}

#endif
