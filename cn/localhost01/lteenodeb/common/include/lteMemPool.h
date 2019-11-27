/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMemPool.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description :
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMemPool.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.10.10.1  2009/06/09 11:24:46  gur19140
 * first wave of Integration
 *
 * Revision 1.12  2009/05/27 04:49:09  gur19479
 * added one wrapper funtion
 *
 * Revision 1.11  2009/05/12 06:06:00  gur19479
 * pdcp merging
 *
 * Revision 1.10  2008/12/29 10:05:57  gur19836
 * Name changed from poolStats to getMemPoolStats
 *
 * Revision 1.9  2008/10/13 12:39:01  gur19836
 * Enhancements Added
 *
 * Revision 1.8  2008/09/19 10:34:58  gur19836
 * Review comments incorporated for cleanupMemPool
 *
 * Revision 1.7  2008/09/19 05:58:00  gur19836
 * MemPool cleanup functionality added
 *
 * Revision 1.6  2008/09/18 05:25:46  gur19836
 * Review comments incorporated
 *
 * Revision 1.5  2008/09/09 11:11:30  ssinghal
 * int replaced with SInt32
 *
 * Revision 1.4  2008/09/09 06:47:46  ssinghal
 * Added comments and removed tabs from the file
 *
 * Revision 1.3  2008/09/08 10:02:56  ssinghal
 * memory and string compare and copy APIs added
 *
 * Revision 1.2  2008/08/25 10:04:35  ssinghal
 * Template applied and minor changes done
 *
 *
 *
 ****************************************************************************/

#ifndef _LTE_MEM_POOL_
#define _LTE_MEM_POOL_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include    "mempool.h"
#include    "lteTypes.h"
#if !defined(UE_SIM_TESTING)
/* Rel 5.3.1: Memory related changes start*/
#include    "logging.h"
/* Rel 5.3.1: Memory related changes End*/
#endif

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
extern void ltePanic(const Char8 * format, ...);
/****************************************************************************
 *  function(): Description.
 *      This API initialises the memory pool.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    initMemPool(void);
/****************************************************************************
 *  function(): Description.
 *      This API creates memory of specified block sizes and no. of buffers.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
/* Rel 5.3.1: Memory related changes start*/
/* SPR 11109 16-JUN-2014 fix start */
void    createMemPool(UInt32 blockSize, UInt32 nBuf);


/****************************************************************************
 *  function(): Description.
 *      This API frees all the memory allocated for pools.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define cleanupMemPool qvCleanupMemPool
#else
void    cleanupMemPool(void);
#endif


/****************************************************************************
 *  function(): Description.
 *      This API displays all memory pool statistics.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define getMemPoolStats qvPoolStats
#define resetMemPoolPeakVal qvResetMemPoolPeakVal
#else
void    getMemPoolStats(void);
void    resetMemPoolPeakVal(void);
#endif


/****************************************************************************
 *  function(): Description.
 *      This API gets the meory from the memory pool.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
#define getMemFromPoolDebug(size, pSize) _getMemFromPoolDebug(size, pSize, __func__, __LINE__)
void *_getMemFromPoolDebug(unsigned int size, unsigned int *psize, char *func, unsigned int line);
#else
void *getMemFromPoolDebug(unsigned int size, unsigned int *psize);
#endif
#if !defined(UE_SIM_TESTING)
#ifdef MEM_CHECK_DOUBLE_FREE
#define getMemFromPool(size, pSize) malloc(size)
#else
#ifdef CSPL_LEAK_DEBUG_LITE
#define getMemFromPool(size, pSize)  getMemFromPoolDebug(size, pSize)
#else
#define getMemFromPool(size, pSize) qvAllocEx(size, pSize, 0)
#endif
#endif /*MEM_CHECK_DOUBLE_FREE*/
#else
void    *getMemFromPool(UInt32 size, UInt32 *pSize);
#endif

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
#if !defined(UE_SIM_TESTING)
#define createAndReturnMemPool(blockSize, nBuf) \
	qvPoolCreateAndReturn(blockSize, nBuf)
#else
void *createAndReturnMemPool(UInt32 blockSize, UInt32 nBuf);
#endif

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
#if !defined(UE_SIM_TESTING)
#define allocateMemFromPool(pool) qvPoolAlloc(pool)
#else
void    *allocateMemFromPool(void *pool);
#endif

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
#if !defined(UE_SIM_TESTING)
#define extendPool(pool, size) qvPoolExtend(pool, size)
#else
UInt32 extendPool(void *pool, UInt32 size);
#endif

/****************************************************************************
 *  function(): Description.
 *      This API returns the memory back to memory pool.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#ifdef MEM_CHECK_DOUBLE_FREE
#define freeMemPool(p) free(p)
#elif defined(CSPL_LEAK_DEBUG_LITE)
#define freeMemPool(p) qvFree(p, __func__, __LINE__)
#else
#define freeMemPool(p) qvFree(p)
#endif /*MEM_CHECK_DOUBLE_FREE*/
#else
void    freeMemPool(void *p);
#endif


/****************************************************************************
 * Function Name  : getMemFromSys
 * Inputs         : size - memory required by the application.
 * Outputs        : None.
 * Returns        : Returns the memory to be allocated from the system.
 * Variables      : None.
 * Description    : Returns the memory allocated from the system and not
 *                  from the memory pool.
****************************************************************************/

#if !defined(UE_SIM_TESTING)
#define getMemFromSys(size) qvSysMalloc(size)
#else
void    *getMemFromSys(UInt32 size);
#endif



/****************************************************************************
 *  function(): Description.
 *      This API retunrs the memory taken from the system.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define freeMemSys(p) qvSysFree(p)
#else
void    freeMemSys(void *p);
#endif

/****************************************************************************
 * Function Name  : freeMemPoolAndReturn
 * Inputs         : pool - pool to be returned to system.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : return the allocated memory to the system.
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define freeMemPoolAndReturn(pool) \
	if (PNULL != (pool)) \
	{\
	    qvAllFreeFromMemPool(pool);\
	    pool = PNULL;\
	}
#else
void freeMemPoolAndReturn(void *pool);
#endif

#if !defined(UE_SIM_TESTING)
#define getPoolSequence(p) qvPoolSequence(p)
#else
UInt16 getPoolSequence(void *);
#endif

#if !defined(UE_SIM_TESTING)
#define getPoolIndex(pool, buffer) qvPoolIndex(pool, buffer)
#else
UInt32 getPoolIndex(void *, void *);
#endif

#if !defined(UE_SIM_TESTING)
#define getPoolLocate(pool, idx) qvPoolLocate(pool, idx)
#else
void *getPoolLocate(void *, UInt32);
#endif

#if !defined(UE_SIM_TESTING)
#define deletePool(p) qvPoolDelete(p)
#else
void  deletePool(void *);
#endif



#endif /* _LTE_MEM_POOL_ */
