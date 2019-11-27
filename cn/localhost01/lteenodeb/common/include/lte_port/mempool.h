/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Interface Definition                    **/
/**                                                                        **/
/****************************************************************************/

#ifndef	__HSS_MEMPOOL_H__
#define	__HSS_MEMPOOL_H__

#include <ylib.h>
#include <stdarg.h>
#ifndef OAM_IPR
#define	MAX_POOLS	64
#endif
#define NVARMEMPOOL	27
#ifndef OAM_IPR
typedef	void *QLOCK;

typedef	struct {
	void    *(*malloc)(unsigned int);
	void    (*free)(void *);
	void    *(*newlock)(void);
	void    (*destroylock)(void *);
	void    (*lock)(void *);
	void    (*unlock)(void *);
	int     (*vprintf)(const char *, va_list);
	int     (*iprintf)(const char *, va_list);
	void    (*abort)(void);
} QSYSOP;

/* Start 128 UE: Memory Changes */
#define LTE_CREATE_MEMPOOL_EXTRA_BUF_FACTOR (1.2)

#define LTE_DL_MULTIPLY_FACTOR(maxNumUe, dl_factor_perc) \
{ \
    if (maxNumUe >= 1 && maxNumUe <= 4) \
		dl_factor_perc = 100; \
    else if (maxNumUe >= 5 && maxNumUe <= 8) \
		dl_factor_perc = 50; \
    else if (maxNumUe >= 9 && maxNumUe <= 16) \
		dl_factor_perc = 25; \
    else if (maxNumUe >= 17 && maxNumUe <= 64) \
		dl_factor_perc = 15; \
    else if (maxNumUe >= 65 && maxNumUe <= 128) \
		dl_factor_perc = 10; \
    else \
		dl_factor_perc = 10; \
}

#define LTE_UL_MULTIPLY_FACTOR(maxNumUe, ul_factor_perc) \
{ \
    if (maxNumUe >= 1 && maxNumUe <= 4) \
		ul_factor_perc = 100; \
    else if (maxNumUe >= 5 && maxNumUe <= 8) \
		ul_factor_perc = 50; \
    else if (maxNumUe >= 9 && maxNumUe <= 16) \
		ul_factor_perc = 25; \
    else if (maxNumUe >= 17 && maxNumUe <= 64) \
		ul_factor_perc = 15; \
    else if (maxNumUe >= 65 && maxNumUe <= 128) \
		ul_factor_perc = 5; \
    else \
		ul_factor_perc = 5; \
}
/* End 128 UE: Memory Changes */
#ifdef MAC_AUT_TEST
#define STATIC
#else
#define STATIC static
#endif

/* Start AM and UM LC parameters changes */
extern UInt16 rlcNumOfSupportedUmLC_g;
extern UInt16 rlcNumOfSupportedAmLC_g;
extern UInt16 pdcpNumOfSupportedLC_g;
/* End AM and UM LC parameters changes */

#define LOGGER_MODULE_ID	999
#define LOGGER_API_SIZE		6

/** Library Initialization **************************************************/
void qvInit(const QSYSOP *op);
void qvInitEx(const QSYSOP *op, int *cErr);
void qvPoolCreate(unsigned int size, unsigned int nbuf);
#endif
void qvCleanupMemPool(void);
void qvPoolStats(void);

/* Task common memory pools *************************************************/

/** Constants **/
#define	__POOLMAGIC	0xCA98362D

/** Malloc Emulation ********************************************************/
void *qvPoolCreateAndReturn(unsigned int size, unsigned int nbuf);
#ifndef OAM_IPR
void *qvAlloc(unsigned int size, unsigned int *psize);
#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
void *qvAllocEx(unsigned int size, unsigned int *psize, int *cErr, unsigned char *func, unsigned int line);
void qvFree(void *p, char *func, int line);
void qvPoolFree(void *buff, char *func, int line);
#else
void *qvAllocEx(unsigned int size, unsigned int *psize, int *cErr);
void qvFree(void *p);
void qvPoolFree(void *buff);
void *qvPoolAlloc(void *pool);
#endif
int qvPoolExtend(void *qpool, unsigned int nbuf);

/** Access to System Memory Allocation **************************************/
void *qvSysMalloc(unsigned int size);
void qvSysFree(void *p);


/** Locks *******************************************************************/
QLOCK qvNewLock(void);
void qvLock(QLOCK lock);
void qvUnlock(QLOCK lock);
void qvDestroyLock(QLOCK lock);

/** Tracing *****************************************************************/
int	qvPrintf(const char *format, ...);
int	qvLog(int level, const char *format, va_list A);
void qvLogDump(int level, const unsigned char *buf, unsigned int n, unsigned long style);
void qvLogLevel(unsigned char level);

int	qvPrintfVa(const char *format, va_list A);
int	qvLogVa(int level, const char *format, va_list A);
#endif

void qvAllFreeFromMemPool(void *qv);
#ifndef OAM_IPR
enum {
	QVLOG_NONE = 0,
	QVLOG_CRITICAL = 1,
	QVLOG_WARNING = 2,
	QVLOG_MAJOR = 3,
	QVLOG_MINOR = 4,
	QVLOG_INFO = 5,
	/* SPR 20585 Fix Start */
	QVLOG_BRIEF = 6,
	QVLOG_DETAIL = 7,
	QVLOG_DUMP = 8,
	/* SPR 20585 Fix End */
	QVLOG_LIBRARY = 255
};

/** Error Handling **********************************************************/
void qvPanic(const char *format, va_list A);
void qvWarning(const char *format, va_list A);
void qvAbort(void);
#define qvAssert(d) (void)((d) || (__qvAssert__(__FILE__, __LINE__, #d), 0))
#define __qvAssert__(a, b, c) qvPrintf("[%s:%u] Assertion Failed <%s>", a, b, c)

/** Error Codes and some description ********************************************/
/* Each API functions, Initializes 'cErr' to QVERROR_NONE.
	So, Internally called functions need not reset this again.
	This is done to avoid unnecesary checks in the system. */
enum {
	QVERROR_NONE = 0,
	QVERROR_GENERIC,
	QVERROR_INVALID_SRV,
	QVERROR_TIMER_NOT_OWNER,
	QVERROR_TIMER_NOT_RUNNING,
	QVERROR_SYS_ALLOC_FAIL,
	QVERROR_MEM_POOL_EXHAUSTED,
	QVERROR_TIMER_ALLOC_FAIL,
	QVERROR_MODULE_ALLOC_FAIL,
	QVERROR_PCTL_ALLOC_FAIL,
	QVERROR_MEM_ALLOC_ILLEGAL_SIZE,
	QVERROR_MEM_ILLEGAL_LOCATION,
	QVERROR_MEM_ILLEGAL_INDEX,
	QVERROR_MEM_UNALLOCATED_BUFFER,
	QVERROR_MEM_ALLOCATED_BUFFER,
	QVERROR_MEM_ILLEGAL_BUFFER,
	QVERROR_MEM_ILLEGAL_POOL,
	QVERROR_INVALID_ARGUMENT
};

typedef struct  {
	unsigned int bufsize, min, max, step;
	void *pool;
} STACKCPOOL;

unsigned short qvPoolSequence(void *);
unsigned int qvPoolIndex(void *, void *);
unsigned int qvPoolIndexEx(void*, void *, int *);
void *qvPoolLocate(void*, unsigned int);
void *qvPoolLocateEx(void*, unsigned int, int *);
void qvPoolDelete(void  *);
int cvContextPoolSetup(STACKCPOOL *, unsigned int,
						unsigned int, unsigned int,
						unsigned int);
void *cvContextAlloc(STACKCPOOL *);
unsigned int qvPoolSize(void*, unsigned int *);
#define cvContextPoolDelete(p) (qvPoolDelete((p)->pool))
#define cvContextIndex(p, c) (qvPoolIndex((p)->pool, (c)))
#define cvContextLocate(p, n)    (qvPoolLocate((p)->pool, (n)))
#define cvContextFree   qvPoolFree
#endif

/*spr 22474 start*/
/*CSPL_STATS flag removed*/
extern int getMemPoolStatsInShm(unsigned int, unsigned int *, unsigned int *);
unsigned int getQpctlSize(void);
unsigned int getQMessageSize(void);
/*spr 22474 end*/
unsigned int qvMemPoolGetUsagePercentage(void);

#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
void qvPrintMemPoolAllocBuffers(void);
#endif

#ifdef CSPL_LEAK_DEBUG
void qvPrintMemAllocBuf(void);
void qvPrintMemCurrentBt(void);
#endif

#endif /* __HSS_MEMPOOL_H__ */
