/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMsgPool.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Header file for msgpool library wrapper APIs
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMsgPool.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.4.10.2  2009/06/19 14:29:12  gur19836
 * Changes Done for Execution Scheduler
 *
 * Revision 1.4.10.1  2009/06/09 11:24:46  gur19140
 * first wave of Integration
 *
 * Revision 1.5  2009/04/22 04:24:36  gur19836
 * getMsgPoolStats added
 *
 * Revision 1.4  2008/09/09 08:34:23  gur19836
 * File Description Added
 *
 * Revision 1.3  2008/09/09 07:27:13  gur19836
 * Added Comments
 *
 * Revision 1.2  2008/09/09 06:47:46  ssinghal
 * Added comments and removed tabs from the file
 *
 * Revision 1.1  2008/09/08 09:38:06  gur19836
 * new wrapper added for message pool
 *
 *
 *
 *
 ****************************************************************************/

#ifndef _LTE_MSG_POOL_
#define _LTE_MSG_POOL_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include    "msgpool.h"
#include    "lteTypes.h"


/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef void        *MSGPOOL;
typedef QSEGMENT     MSGSEGMENT;
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 *  function(): Description.
 *      This API initialises the message pool.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    initMsgPool(void);


/****************************************************************************
 *  function(): Description.
 *       This function creates a message pool with blockSize as size of
 *       of each buffer & nBuf, number of  buffers
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define msgPoolCreate(blockSize, nBuf) qvMsgPoolCreateEx(blockSize, nBuf, 0)
#else
MSGPOOL    msgPoolCreate(UInt32 blockSize, UInt32 nBuf);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function provides the size of pool in terms of number of buffers
 *      in the pool. If the second argument is not null, it is set to the
 *      number of buffers already allocated in the pool.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define msgPoolSize(mPool, p_alloc) (mPool?qvMsgPoolSize(mPool, p_alloc) : 0)
#else
UInt32    msgPoolSize(MSGPOOL mPool, UInt32 *p_alloc);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function allocates a buffer for a message from pool.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#ifdef CSPL_LEAK_DEBUG_LITE
#define msgAlloc(pool, headroom, tailroom, blockSize) \
	qvMsgAllocEx(pool, headroom, tailroom, blockSize, 0, __func__, __LINE__)
#else
#define msgAlloc(pool, headroom, tailroom, blockSize) \
	qvMsgAllocEx(pool, headroom, tailroom, blockSize, 0)
#endif
#else
void    *msgAlloc(MSGPOOL pool, UInt32 headroom,
		  UInt32 tailroom, UInt32 blockSize);
#endif

#if !defined(UE_SIM_TESTING)
#define msgAllocAndReserve(headroom, tailroom, blockSize) \
	qvMsgAllocAndReserve(headroom, tailroom, blockSize)
#endif


/****************************************************************************
 *  function(): Description.
 *      This function requests de-allocation of memory pointed by the buffer
 *      argument. This function is used in conjunction  with msgAlloc.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#ifdef CSPL_LEAK_DEBUG_LITE
#define msgFree(buffer) (buffer?qvMsgFree(buffer, __func__, __LINE__):(void)0)
#else
#define msgFree(buffer) (buffer?qvMsgFree(buffer):(void)0)
#endif
#else
void    msgFree(void *buffer);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function returns the size of buffer. 'blockSize' will be set to
 *      the size of memory that was actually allocated at the time of msgAlloc
 *      call. While the function will return the size of payload attached to
 *      the message.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define msgSize(buffer, blockSize) (buffer?qvMsgSize(buffer, blockSize) : 0)
#else
UInt32 msgSize(const void *buffer, UInt32 *blockSize);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function can be used for reserving a specified area of memory
 *      in a message buffer.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#ifdef CSPL_LEAK_DEBUG_LITE
#define msgReserve(buffer, where, count) \
        (buffer?qvMsgReserveEx(buffer, where, count, 0, __func__, __LINE__) : 0)
#else
#define msgReserve(buffer, where, count) \
        (buffer?qvMsgReserveEx(buffer, where, count, 0) : 0)
#endif
#else
UInt8 * msgReserve(void *buffer, SInt32 where, UInt32 count);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function is used for inserting data at a specified location
 *      in a message.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#ifdef CSPL_LEAK_DEBUG_LITE
#define msgInsert(buffer, where, data, count) \
        ((buffer && data) ? qvMsgInsertEx(buffer, where, data, count, 0, __func__, __LINE__) : PNULL)
#else
#define msgInsert(buffer, where, data, count) \
        ((buffer && data) ? qvMsgInsertEx(buffer, where, data, count, 0) : PNULL)
#endif
#else
UInt8 * msgInsert(void *buffer, SInt32 where, UInt8 * data, UInt32 count);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function can be used for removing part of data at a specified
 *      location in a message.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#ifdef CSPL_LEAK_DEBUG_LITE
#define msgRemove(buffer, where, count) \
        (buffer?qvMsgRemoveEx(buffer, where, count, 0, __func__, __LINE__) : 0)
#else
#define msgRemove(buffer, where, count) \
        (buffer?qvMsgRemoveEx(buffer, where, count, 0) : 0)
#endif
#else
SInt32 msgRemove(void *buffer, SInt32 where, UInt32 count);
#endif
/****************************************************************************
 *  function(): Description.
 *      This function can be used for reserving a specified area of memory
 *      in a message buffer.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#ifdef CSPL_LEAK_DEBUG_LITE
#define msgReserveHeadTailRoom(buffer, where, count) \
        (buffer?qvMsgReserveHeadTailRoomEx(buffer, where, count, 0, __func__, __LINE__) : 0)
#else
#define msgReserveHeadTailRoom(buffer, where, count) \
        (buffer?qvMsgReserveHeadTailRoomEx(buffer, where, count, 0) : 0)
#endif
#else
UInt8 * msgReserveHeadTailRoom(void *buffer, SInt32 where, UInt32 count);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function can be used for extracting a part of data at a specified
 *      location in a message.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define msgExtract(buffer, where, data, count) \
        ((buffer && data) ? qvMsgExtractEx(buffer, where, data, count, 0) : 0)
#else
SInt32 msgExtract(const void *buffer, SInt32 where, UInt8 *data, UInt32 count);
#endif

/* ECN chnages start*/
/*****************************************************************************
 * function(): Description:
 *			This function is called to get the Pointer of the 'location'
 *			present in the message pointed by q.
 * ARGS:
 *
 * RETURNS:
 *
 ******************************************************************************/
#if !defined(UE_SIM_TESTING)
#define msgLocate(buffer, where, data) \
        ((buffer) ? qvMsgLocate(buffer, where, data) : 0)
#else
SInt32 msgLocate(void *buffer, SInt32 where, UInt8 **data);
#endif
/* ECN chnages start*/

/****************************************************************************
 *  function(): Description.
 *      This function clone the message in a zero copy buffer and returns
 *      the pointer to it.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#ifdef CSPL_LEAK_DEBUG_LITE
#define msgClone(buffer) (buffer ? qvMsgCloneEx(buffer, 0, __func__, __LINE__) : PNULL)
#else
#define msgClone(buffer) (buffer ? qvMsgCloneEx(buffer, 0) : PNULL)
#endif
#else
void *msgClone(const void *buffer);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function is used for creating a copy of message. Contents of
 *      'frombuffer' are copied to contents of 'tobuffer'. Both 'non-zero
 *      copy' and 'zero copy' buffers of the message are copied. The zero
 *      copy buffers, if any, in the 'tobuffer' are freed prior to the copy
 *      operation.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#ifdef CSPL_LEAK_DEBUG_LITE
#define msgCopy(tobuffer, frombuffer, blockSize) \
	((tobuffer && frombuffer) ? \
		    qvMsgCopyEx(tobuffer, frombuffer, blockSize, 0, __func__, __LINE__) : PNULL)
#else
#define msgCopy(tobuffer, frombuffer, blockSize) \
	((tobuffer && frombuffer) ? \
		    qvMsgCopyEx(tobuffer, frombuffer, blockSize, 0) : PNULL)
#endif
#else
void *msgCopy(void *tobuffer, const void *frombuffer, UInt32 blockSize);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function can be used for splitting 'buffer' message at 'offset'
 *      position. The buffers after the split position ('offset') are moved
 *      to 'tobuffer'. The zero copy buffers, if any, in the 'tobuffer' are
 *      freed prior to the moving split buffers.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#ifdef CSPL_LEAK_DEBUG_LITE
#define msgSplit(buffer, offset, tobuffer) \
	((buffer && tobuffer) ? \
		    qvMsgSplitEx(buffer, offset, tobuffer, 0, __func__, __LINE__):PNULL)
#else
#define msgSplit(buffer, offset, tobuffer) \
	((buffer && tobuffer) ? \
		    qvMsgSplitEx(buffer, offset, tobuffer, 0):PNULL)
#endif
#else
void *msgSplit(void *buffer, UInt32 offset, void *tobuffer);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function is used for joining list of 'zero copy buffers' in
 *      'append' message to the list of 'zero copy buffers' in 'buffer'
 *      message.'Append' remains unaltered
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#ifdef CSPL_LEAK_DEBUG_LITE
#define msgJoin(buffer, append) \
        ((buffer && append) ? qvMsgJoinEx(buffer, append, 0, __func__, __LINE__) : PNULL)
#else
#define msgJoin(buffer, append) \
        ((buffer && append) ? qvMsgJoinEx(buffer, append, 0) : PNULL)
#endif
#else
void *msgJoin(void *buffer, void *append);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function can be used for packing all the payload buffers attached
 *      to a message into a single payload buffer.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define msgPack(buffer, headroom, tailroom) \
	(buffer ? qvMsgPackEx(buffer, headroom, tailroom, 0):PNULL)
#else
void *msgPack(void *buffer, UInt32 headroom, UInt32 tailroom);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function returns the number of payload buffers attached
 *      to a message.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define msgSegCount(buffer) (buffer ? qvMsgSegCount(buffer):0)
#else
UInt32 msgSegCount(const void *buffer);
#endif


/****************************************************************************
 *  function(): Description.
 *      This function can be used for fetching a payload buffer attached
 *      to a message.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define msgSegNext(buffer, last, segment) \
	(buffer?qvMsgSegNext(buffer, last, segment):PNULL)
#else
void *msgSegNext(const void *buffer, void *last, MSGSEGMENT *segment);
#endif

#if !defined(UE_SIM_TESTING)
#define getMsgPoolStats qvMsgPoolStats
#define resetMsgPoolPeakVal qvResetMsgPoolPeakVal
#else
void    getMsgPoolStats(void);
void    resetMsgPoolPeakVal(void);
#endif

/****************************************************************************
 *  function(): Description.
 *      This API frees all the memory allocated for message pools.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define cleanupMsgPool qvCleanupMsgPool
#else
void    cleanupMsgPool(void);
#endif
/****************************************************************************
 *  function(): Description.
 *      This function is used for joining list of 'zero copy buffers' in
 *      'append' message to the list of 'zero copy buffers' in 'buffer'
 *      message.'Append' zero copy buffer is then freed.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define msgJoinAndFree(buffer, append) \
        ((buffer && append) ? qvMsgJoinAndFreeEx(buffer, append, 0) : PNULL)
#else
void *msgJoinAndFree(void *buffer, void *append);
#endif

/*******Memory Handling changes 64UE Start****************************/
/****************************************************************************
 *  function(): Description.
 *       This function extends message pool with blockSize as size of
 *       of each buffer & nBuf, number of  buffers
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#if !defined(UE_SIM_TESTING)
#define msgPoolExtend(blockSize, nBuf)\
{\
    QMPOOL  msgPool;\
    msgPool = qvMsgPoolForSize(blockSize);\
    if (!msgPool)\
    {\
       lteWarning("Message pool for size %d not present\n", blockSize);\
    } \
    else if (!qvMsgPoolExtend(msgPool, nBuf))\
    {\
        lteWarning("Failed to extend pool for size%d\n", blockSize);\
    } \
}
#else
void msgPoolExtend(UInt32 blockSize, UInt32 nBuf);
#endif
/*******Memory Handling changes 64UE End****************************/

#endif
