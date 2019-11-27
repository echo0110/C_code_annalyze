/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Interface Definition                    **/
/**                                                                        **/
/****************************************************************************/

#ifndef	__HSS_MSGPOOL_H__
#define	__HSS_MSGPOOL_H__

#include <ylib.h>
#include <stdarg.h>
#include <mempool.h>
#ifdef UE_SIM_TESTING
#define LTE_MSG_POOL_8B_MAX_BUF         70000
#define LTE_MSG_POOL_16B_MAX_BUF        10000
#define LTE_MSG_POOL_32B_MAX_BUF        9000
#define LTE_MSG_POOL_64B_MAX_BUF        90000
#define LTE_MSG_POOL_128B_MAX_BUF       10000
#define LTE_MSG_POOL_256B_MAX_BUF       10000
#define LTE_MSG_POOL_512B_MAX_BUF       40000
#define LTE_MSG_POOL_1KB_MAX_BUF        70000
#define LTE_MSG_POOL_2KB_MAX_BUF        80000
#define LTE_MSG_POOL_4KB_MAX_BUF        1000
#define LTE_MSG_POOL_8KB_MAX_BUF        1000
#define LTE_MSG_POOL_16KB_MAX_BUF       1000
#define LTE_MSG_POOL_32KB_MAX_BUF       1000
#define LTE_MSG_POOL_64KB_MAX_BUF       400
#define LTE_MSG_POOL_128KB_MAX_BUF      400
#define LTE_MSG_POOL_256KB_MAX_BUF      0
#define LTE_MSG_POOL_512KB_MAX_BUF      0
#define LTE_MSG_POOL_1MB_MAX_BUF        0
#define LTE_MSG_POOL_2MB_MAX_BUF        0
#define LTE_MSG_POOL_4MB_MAX_BUF        0
#define LTE_MSG_POOL_8MB_MAX_BUF        0
#define LTE_MSG_POOL_16MB_MAX_BUF       0
#define LTE_MSG_POOL_32MB_MAX_BUF       0
#define LTE_MSG_POOL_64MB_MAX_BUF       0
#define LTE_MSG_POOL_128MB_MAX_BUF      0
#define LTE_MSG_POOL_256MB_MAX_BUF      0
#define LTE_MSG_POOL_512MB_MAX_BUF      0
#define LTE_MSG_POOL_QPCTL_MAX_BUF      600000
#define LTE_MSG_POOL_QMSG_MAX_BUF       400000

#else

#ifdef GTP_SPLIT
#define LTE_MSG_POOL_8B_MAX_BUF         0
#define LTE_MSG_POOL_16B_MAX_BUF        0
#else
#define LTE_MSG_POOL_8B_MAX_BUF         100
#define LTE_MSG_POOL_16B_MAX_BUF        1000
#endif
/* SPR 5038 changes stop */
/* - SPR 20736 */

#ifdef GTP_SPLIT
#define LTE_MSG_POOL_32B_MAX_BUF        100
#define LTE_MSG_POOL_64B_MAX_BUF        60
#define LTE_MSG_POOL_128B_MAX_BUF       60
#define LTE_MSG_POOL_256B_MAX_BUF       50
#define LTE_MSG_POOL_512B_MAX_BUF       50
#else
#define LTE_MSG_POOL_32B_MAX_BUF        60000
#define LTE_MSG_POOL_64B_MAX_BUF        1000
#define LTE_MSG_POOL_128B_MAX_BUF       1000
#define LTE_MSG_POOL_256B_MAX_BUF       500
#define LTE_MSG_POOL_512B_MAX_BUF       3500
#endif
#define LTE_MSG_POOL_1KB_MAX_BUF        7000
/* SPR 22075 Changes Start */
/* These buffers are used for ciphering/deciphering of data at PDCP */
#define LTE_MSG_POOL_2KB_MAX_BUF        5500
/* These buffers are used for signaling messages like at cell setup when MAC
 * reads cell configuration message etc */
#define LTE_MSG_POOL_4KB_MAX_BUF        100
/* These buffers are also used for handling the large signalling
 * messages */
#define LTE_MSG_POOL_8KB_MAX_BUF        100
/* SPR 22075 Changes End */
 /* SPR 4895 FIX START */
/* SPR 5038 changes start */
/*******Memory Handling changes 64UE Start****************************/
/* SPR 22075 Changes Start */
/* These buffers are used in 20 MHz while processing UL data packets. These
 * buffers are sustain until all the packets are reassemble. */
#define LTE_MSG_POOL_16KB_MAX_BUF       500
/* SPR 22075 Changes End */
/* SPR 5038 changes stop */
/* SPR 4895 FIX END */
#ifdef UL_MU_MIMO_BUILD_ENABLED
#define LTE_MSG_POOL_32KB_MAX_BUF      300
#else
#define LTE_MSG_POOL_32KB_MAX_BUF       0
#endif
/*******Memory Handling changes 64UE End****************************/
#define LTE_MSG_POOL_64KB_MAX_BUF       0
#define LTE_MSG_POOL_128KB_MAX_BUF      0
#define LTE_MSG_POOL_256KB_MAX_BUF      0
#define LTE_MSG_POOL_512KB_MAX_BUF      0
#define LTE_MSG_POOL_2MB_MAX_BUF        0
#define LTE_MSG_POOL_1MB_MAX_BUF        0
#define LTE_MSG_POOL_4MB_MAX_BUF        0
#define LTE_MSG_POOL_8MB_MAX_BUF        0
#define LTE_MSG_POOL_16MB_MAX_BUF       0
#define LTE_MSG_POOL_32MB_MAX_BUF       0
#define LTE_MSG_POOL_64MB_MAX_BUF       0
#define LTE_MSG_POOL_128MB_MAX_BUF      0
#define LTE_MSG_POOL_256MB_MAX_BUF      0
#define LTE_MSG_POOL_512MB_MAX_BUF      0
/* 255 UE changes start */
#ifdef GTP_SPLIT
#define LTE_MSG_POOL_QPCTL_MAX_BUF      200
#define LTE_MSG_POOL_QMSG_MAX_BUF       100
#else
#define LTE_MSG_POOL_QPCTL_MAX_BUF      60000
/*SPR 21366 +*/
#define LTE_MSG_POOL_QMSG_MAX_BUF       60000
/*SPR 21366 -*/
#endif
/* 255 UE changes end */
#endif

#define NVARMSGPOOL   27
#ifndef OAM_IPR
/* Defining Version of CSPL Release */
#define qvRelVer    htonl((1<<24) | (4<<16) | (7<<8))


typedef	struct	{
	unsigned long	s;
	unsigned long	us;
} QTIME;

typedef	struct	{
	unsigned char	*base;
	unsigned int	size;
} QSEGMENT;

typedef	void	*QMODULE;
typedef	void	*QTIMER;
typedef	void	*QMPOOL;
typedef	void	*QDRIVER;
typedef void	**QVMPOOLSET;

typedef	struct	{
	unsigned long	ntimers;
	unsigned long	nmodules;
	unsigned long	npctls;
	unsigned long	nqmsg;
} QTUNING;

typedef struct {
	unsigned int	bufsize;
	unsigned int	nbufs;
} QPOOLTUNING;

typedef struct {
	unsigned int	opt;
	unsigned long	npools;
	QPOOLTUNING	qpt[MAX_POOLS];
} QPOOLSETTUNING;


struct QSHELL;
typedef	struct	QSHELL QSHELL;
struct	QSHELL {
	void  (*timedwait)(void *, const QTIME *);
	void  (*send)(void *, const void *, QMODULE, QMODULE, void *, unsigned int);
	void  *(*receive)(void *, QMODULE *, QMODULE *, signed char *);
	void  *(*open)(void *);
	void  (*close)(void *);
	void  *(*hash)(void *, void *);
};

struct QWAIT;
typedef	struct	QWAIT QWAIT;
struct	QWAIT {
	void *(*newchannel)(void);
	void (*sleep)(void *, const QTIME *);
	void (*wakeup)(void *);
	void (*walltime)(QTIME *);
};

typedef struct QMANIFEST {
	const char *name;
	unsigned long service;

	struct {
		void *(*early)(void *);
		void *(*late)(void *);
	} init;

	struct {
		int	(*message)(void *, void *);
		void (*timer)(QTIMER, void *, void *);
	} handler;

	struct	{
		void *(*pack)(QMODULE, void *, unsigned int *);
		void *(*unpack)(QMODULE, void *);
		void *(*alloc)(QMODULE, unsigned int);
	} method;

	const unsigned long *destinations;
} QMANIFEST;


typedef struct	QADAPTER {
	void *(*init)(QMODULE, void *);
	void *(*pack)(QMODULE, void *, unsigned int *);
	void *(*unpack)(QMODULE, void *);
	void *(*alloc)(QMODULE, unsigned int);
	void *(*in)(void *, QMODULE *, QMODULE *);
	void *(*out)(void *, QMODULE *, QMODULE *);

	const unsigned long *destinations;
} QADAPTER;
#endif

#define		LOGGER_MODULE_ID	999
#define		LOGGER_API_SIZE		6

/** Library Initialization **************************************************/
void qvMsgPoolInit(const QSYSOP *op);
void qvMsgPoolInitEx(const QSYSOP *op, int *cErr);
QDRIVER	qvNewDriver(void);
QDRIVER	qvNewDriverEx(int *cErr);
QDRIVER	qvDriverStart(QDRIVER driver, void (*wakeupfn)(void *), void *arg);
#define	qvDriver(w, a)	(qvDriverStart(qvNewDriver(), (w), (a)))

/** Memory Pool Management **************************************************/

/* Task common memory pools *************************************************/

/** Constants **/
#define	__BUFMAGIC	0xACE12E92
#define	__POOLMAGIC	0xCA98362D

#ifndef OAM_IPR
typedef void *QMPOOLSET;

typedef struct {
	unsigned int bufsize;
	unsigned int nbufs;
	unsigned int nalloc;
} QPOOLTUNINGINFO;

typedef struct {
	unsigned long   npools;
	QPOOLTUNINGINFO qpt[MAX_POOLS];
} QPOOLSETTUNINGINFO;

typedef	struct	qmpool {
	unsigned long magic;
	unsigned int size, nbuf, alloc;
	YLIST chunks;
	YLIST list;

	QLOCK lock;
#ifdef MEM_DEBUG
	unsigned long ref_ctr;
#endif
} qmpool;

typedef	struct qmbuf {
	union {
		YLNODE	__header__;
		qmpool	*pool;
	} u;

	unsigned short	incarnation;
	unsigned char	allocated;
#ifdef MEM_DEBUG
	unsigned long   ref_id;
#endif
} qmbuf;

typedef	struct	qmchunk {
	YLNODE	__header__;
	unsigned int	nbuf;
} qmchunk;

typedef struct qmpoolsetContext {
	unsigned int	magic;
	unsigned int	opt;
	unsigned int	npools;
	QMPOOL			alloc[MAX_POOLS];
	QLOCK			lock;

} qmpoolset;
#endif

/** Message Buffer Management ***********************************************/
QMPOOL qvMsgPoolCreate(unsigned int attrsize, unsigned int nbuf);
QMPOOL qvMsgPoolCreateEx(unsigned int attrsize, unsigned int nbuf, int *cErr);
unsigned int qvMsgPoolSize(QMPOOL Q, unsigned int *p_alloc);
int	qvMsgPoolExtend(QMPOOL Q, unsigned int nbuf);
int	qvMsgPoolExtendEx(QMPOOL Q, unsigned int nbuf, int *cErr);
QMPOOL qvMsgPoolOf(const void *buffer);
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgAlloc(QMPOOL pool, unsigned int headroom, unsigned int tailroom, unsigned int attrsize, char *func, int line);
#else
void *qvMsgAlloc(QMPOOL pool, unsigned int headroom, unsigned int tailroom, unsigned int attrsize);
#endif

#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgAllocEx(QMPOOL pool, unsigned int headroom, unsigned int tailroom, unsigned int attrsize, int *cErr, char *func, int line);
#else
void *qvMsgAllocEx(QMPOOL pool, unsigned int headroom, unsigned int tailroom, unsigned int attrsize, int *cErr);
#endif

void qvCleanupMsgPool(void);

#define	qvMsgAllocFromPool(pool)	qvMsgAlloc((pool), 0, 0, 0)
#define	qvMsgAllocFromPoolEx(pool, cErr)	qvMsgAllocEx((pool), 0, 0, 0, cErr)
#ifdef CSPL_LEAK_DEBUG_LITE
#define	qvMsgAllocBySize(size)		qvMsgAlloc(0, 0, 0, (size), __func__, __LINE__)
#else
#define	qvMsgAllocBySize(size)		qvMsgAlloc(0, 0, 0, (size))
#endif
#define	qvMsgAllocBySizeEx(size, cErr)	qvMsgAllocEx(0, 0, 0, (size), cErr)
#if defined(CSPL_LEAK_DEBUG_LITE)
void qvMsgFree(void *buffer, char *func, int line);
#else
void qvMsgFree(void *buffer);
#endif

#ifndef OAM_IPR
unsigned int qvMsgSize(const void *buffer, unsigned int *attrsize);
#endif

#ifdef CSPL_LEAK_DEBUG_LITE
#define qvMsgReserve(buffer, where, count) _qvMsgReserve(buffer, where, count, func, line)
unsigned char *_qvMsgReserve(void *buffer, int where, unsigned int count, char *func, int line);
#else
unsigned char *qvMsgReserve(void *buffer, int where, unsigned int count);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *qvMsgReserveEx(void *buffer, int where, unsigned int count, int *cErr, char *func, int line);
#else
unsigned char *qvMsgReserveEx(void *buffer, int where, unsigned int count, int *cErr);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
#define qvMsgInsert(buffer, where, data, count) _qvMsgInsert(buffer, where, data, count, func, line);
unsigned char *_qvMsgInsert(void *buffer, int where, unsigned char *data, unsigned int count, char *func, int line);
#else
unsigned char *qvMsgInsert(void *buffer, int where, unsigned char *data, unsigned int count);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *qvMsgInsertEx(void *buffer, int where, unsigned char *data,
					unsigned int count, int *cErr, char *func, int line);
#else
unsigned char *qvMsgInsertEx(void *buffer, int where, unsigned char *data,
					unsigned int count, int *cErr);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *qvMsgInsertExternal(void *buffer, int where, unsigned char *data,
		unsigned int count, void (*freefn)(void *, void *), void *freearg, char *func, int line);
#else
unsigned char *qvMsgInsertExternal(void *buffer, int where, unsigned char *data,
		unsigned int count, void (*freefn)(void *, void *), void *freearg);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *qvMsgInsertExternalEx(void *buffer, int where, unsigned char *data,
		unsigned int count, void (*freefn)(void *, void *), void *freearg, int *cErr, char *func, int line);
#else
unsigned char *qvMsgInsertExternalEx(void *buffer, int where, unsigned char *data,
		unsigned int count, void (*freefn)(void *, void *), void *freearg, int *cErr);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
#define qvMsgRemove (buffer, where, count) _qvMsgRemove(buffer, where, count, func, line)
int	_qvMsgRemove(void *buffer, int where, unsigned int count, char *func, int line);
#else
int	qvMsgRemove(void *buffer, int where, unsigned int count);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
int	qvMsgRemoveEx(void *buffer, int where, unsigned int count, int *cErr, char *func, int line);
#else
int	qvMsgRemoveEx(void *buffer, int where, unsigned int count, int *cErr);
#endif
int	qvMsgExtract(const void *buffer, int where, unsigned char *data, unsigned int count);
int	qvMsgExtractEx(const void *buffer, int where, unsigned char *data, unsigned int count, int *cErr);
#ifdef CSPL_LEAK_DEBUG_LITE
#define qvMsgClone(buffer) _qvMsgClone(buffer, func, line)
void *_qvMsgClone(const void *buffer, char *func, int line);
#else
void *qvMsgClone(const void *buffer);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgCloneEx(const void *buffer, int *cErr, char *func, int line);
#else
void *qvMsgCloneEx(const void *buffer, int *cErr);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
#define qvMsgCopy(tobuffer, frombuffer, attrsize) _qvMsgCopy(tobuffer, frombuffer, attrsize, func, line)
void *_qvMsgCopy(void *tobuffer, const void *frombuffer, unsigned int attrsize, char *func, int line);
#else
void *qvMsgCopy(void *tobuffer, const void *frombuffer, unsigned int attrsize);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgCopyEx(void *tobuffer, const void *frombuffer, unsigned int attrsize, int *cErr, char *func, int line);
#else
void *qvMsgCopyEx(void *tobuffer, const void *frombuffer, unsigned int attrsize, int *cErr);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
#define qvMsgSplit(frombuffer, offset, tobuffer) _qvMsgSplit(frombuffer, offset, tobuffer, func, line)
void *_qvMsgSplit(void *frombuffer, unsigned int offset, void *tobuffer, char *func, int line);
#else
void *qvMsgSplit(void *frombuffer, unsigned int offset, void *tobuffer);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgSplitEx(void *frombuffer, unsigned int offset, void *tobuffer, int *cErr, char *func, int line);
#else
void *qvMsgSplitEx(void *frombuffer, unsigned int offset, void *tobuffer, int *cErr);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
#define qvMsgJoin(buffer, append) __qvMsgJoin(buffer, append, func, line)
void *_qvMsgJoin(void *buffer, void *append, char *func, int line);
#else
void *qvMsgJoin(void *buffer, void *append);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgJoinEx(void *buffer, void *append, int *cErr, char *func, int line);
#else
void *qvMsgJoinEx(void *buffer, void *append, int *cErr);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
#define qvMsgPack(buffer, headroom, tailroom) _qvMsgPack(buffer, headroom, tailroom, func, line)
void *_qvMsgPack(void *buffer, unsigned int headroom, unsigned int tailroom, char *func, int line);
#else
void *qvMsgPack(void *buffer, unsigned int headroom, unsigned int tailroom);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgPackEx(void *buffer, unsigned int headroom, unsigned int tailroom, int *cErr, char *func, int line);
unsigned char *qvMsgReserveHeadTailRoomEx(void *buffer, int where, unsigned int count, int *cErr, char *func, int line);
unsigned char *qvMsgReserveHeadTailRoom(void *buffer, int where, unsigned int count, char *func, int line);
#else
void *qvMsgPackEx(void *buffer, unsigned int headroom, unsigned int tailroom, int *cErr);
unsigned char *qvMsgReserveHeadTailRoomEx(void *buffer, int where, unsigned int count, int *cErr);
unsigned char *qvMsgReserveHeadTailRoom(void *buffer, int where, unsigned int count);
#endif
unsigned int qvMsgSegCount(const void *buffer);
void *qvMsgSegNext(const void *buffer, void *last, QSEGMENT *segment);
void qvMsgPoolStats(void);
void *qvMsgJoinAndFree(void *buffer, void *append);
void *qvMsgJoinAndFreeEx(void *buffer, void *append, int *cErr);
QMPOOL qvMsgPoolForSize(unsigned int size);
unsigned int qvMsgPoolGetUsagePercentage(void);
/* ECN changes start*/
int qvMsgLocate(void *buffer, int where, unsigned char **pLocation);
/* ECN changes  end*/

/*spr 22474 start*/
/*CSPL_STATS flag removed*/
int getMsgPoolStatsInShm(unsigned int poolIndex, unsigned int *allocBuf, unsigned int *peakAllocBuf);
int getQMsgPoolStatsInShm(unsigned int *allocBuf, unsigned int *peakAllocBuf);
int getQpctlPoolStatsInShm(unsigned int *allocBuf, unsigned int *peakAllocBuf);
/*spr 22474 end*/
#ifdef CSPL_LEAK_DEBUG_LITE
void qvPrintLeakedMsgBuf(void);
#endif

#ifdef CSPL_LEAK_DEBUG
void qvPrintAllocBuf(void);
void qvPrintCurrentBt(void);
#endif
#endif
