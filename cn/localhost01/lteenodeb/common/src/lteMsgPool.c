/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMsgPool.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains wrapper APIs for msgpool library
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMsgPool.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.4.10.2  2009/06/19 14:29:04  gur19836
 * Changes Done for Execution Scheduler
 *
 * Revision 1.4.10.1  2009/06/09 11:24:51  gur19140
 * first wave of Integration
 *
 * Revision 1.5  2009/04/22 04:25:14  gur19836
 * getMsgPoolStats added
 *
 * Revision 1.4  2008/09/18 05:25:08  gur19836
 * Review comments incorporated
 *
 * Revision 1.3  2008/09/09 08:34:23  gur19836
 * File Description Added
 *
 * Revision 1.2  2008/09/09 07:27:13  gur19836
 * Added Comments
 *
 * Revision 1.1  2008/09/08 09:38:06  gur19836
 * new wrapper added for message pool
 *
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMsgPool.h"
/*******Memory Handling changes 64UE Start******************************/
#include "lteLog.h"
/*******Memory Handling changes 64UE End******************************/

#define NVARPOOL    27
#define MINBUF      8
/****************************************************************************
  Private Definitions
 ****************************************************************************/

/* coverity fix 27-JUN-2014 fix start */
#ifndef MEM_LEAK_DEBUG
/* coverity fix 27-JUN-2014 fix end */
static UInt32 lteMsgPoolSize[NVARPOOL] = {
	LTE_MSG_POOL_8B_MAX_BUF,
	LTE_MSG_POOL_16B_MAX_BUF,
	LTE_MSG_POOL_32B_MAX_BUF,
	LTE_MSG_POOL_64B_MAX_BUF,
	LTE_MSG_POOL_128B_MAX_BUF,
	LTE_MSG_POOL_256B_MAX_BUF,
	LTE_MSG_POOL_512B_MAX_BUF,
	LTE_MSG_POOL_1KB_MAX_BUF,
	LTE_MSG_POOL_2KB_MAX_BUF,
	LTE_MSG_POOL_4KB_MAX_BUF,
	LTE_MSG_POOL_8KB_MAX_BUF,
	LTE_MSG_POOL_16KB_MAX_BUF,
	LTE_MSG_POOL_32KB_MAX_BUF,
	LTE_MSG_POOL_64KB_MAX_BUF,
	LTE_MSG_POOL_128KB_MAX_BUF,
	LTE_MSG_POOL_256KB_MAX_BUF,
	LTE_MSG_POOL_512KB_MAX_BUF,
	LTE_MSG_POOL_1MB_MAX_BUF,
	LTE_MSG_POOL_2MB_MAX_BUF,
	LTE_MSG_POOL_4MB_MAX_BUF,
	LTE_MSG_POOL_8MB_MAX_BUF,
	LTE_MSG_POOL_16MB_MAX_BUF,
	LTE_MSG_POOL_32MB_MAX_BUF,
	LTE_MSG_POOL_64MB_MAX_BUF,
	LTE_MSG_POOL_128MB_MAX_BUF,
	LTE_MSG_POOL_256MB_MAX_BUF,
	LTE_MSG_POOL_512MB_MAX_BUF
};
/* coverity fix 27-JUN-2014 fix start */
#endif
/* coverity fix 27-JUN-2014 fix end */

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
extern QSYSOP msg_os;

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
* Function Name  : initMsgPool
* Inputs         : None
* Outputs        : Msg Pool library initialization
* Returns        : None
* Variables      :
* Description    : This function indirectly performs the initilization of
*                  the CSPL library and threads, registers with the OS
*                  services and allocates memory for various pools
****************************************************************************/
void initMsgPool(void)
{
	/* coverity fix 27-JUN-2014 fix start */
#ifndef MEM_LEAK_DEBUG
	/* coverity fix 27-JUN-2014 fix end */
	QMPOOL  msgPool;
	UInt32  i, n;
	/* coverity fix 27-JUN-2014 fix start */
#endif
	/* coverity fix 27-JUN-2014 fix end */

	qvMsgPoolInit(&msg_os);
	/* SPR 11109 16-JUN-2014 Fix start */
#ifndef MEM_LEAK_DEBUG
	/* SPR 11109 16-JUN-2014 Fix end */
	for (i = 0, n = MINBUF; i < NVARPOOL; i++, n *= 2) {
		msgPool = qvMsgPoolForSize(n);
		if (!msgPool)
		{
		    lteWarning("Message pool for size %d not present\n", n);
		
			/* SPR 4010 Fix Start */
		}else if (!qvMsgPoolExtend(msgPool, lteMsgPoolSize[i])) {
			/* SPR 4010 Fix End */
		    lteWarning("Failed to extend pool %d\n", i);
		}
	}
	/* SPR 11109 16-JUN-2014 Fix start */
#endif
	/* SPR 11109 16-JUN-2014 Fix end */
	return;
}


#if defined(UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : msgPoolCreate
 * Inputs         : blockSize -  size of each buffer in the pool
 *                  nbuf      -  number of buffers in the pool
 * Outputs        : MsgPool created
 * Returns        : Pointer to the allocated pool
 * Variables      :
 * Description    : This function creates a message pool
 ****************************************************************************/
MSGPOOL msgPoolCreate(UInt32 blockSize, UInt32 nBuf)
{
    return qvMsgPoolCreate(blockSize, nBuf);
}


/****************************************************************************
 * Function Name  : msgPoolSize
 * Inputs         : mPool    -  Pointer to Pool.
 *                  p_alloc  -  Out param, returns number of buffers
 *                              already allocated in the pool.
 * Outputs        : None
 * Returns        : Size of the pool
 * Variables      :
 * Description    : This function provides the size of pool in terms of number
 *                  of buffers in the pool. If the second argument is
 *                  not null, it is set to the number of buffers already
 *                  allocated in the pool.
 ****************************************************************************/
UInt32 msgPoolSize(MSGPOOL mPool, UInt32 *p_alloc)
{
    return ((mPool != PNULL)?qvMsgPoolSize(mPool, p_alloc) : 0);
}

/****************************************************************************
 * Function Name  : msgAlloc
 * Inputs         : pool      - Pointer to Pool.
 *                  headroom  - size of memory to be allocated before payload
 *                  tailroom  - size of memory to be allocated after payload
 *                  blockSize - size of data block to be allocated
 * Outputs        : None
 * Returns        : void * - pointer to the allocated buffer, if successful
*                            return 0, if unsuccessful
 * Variables      :
 * Description    : This function allocates a buffer for a message from pool.
 ****************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
#define msgAlloc(pool, headroom, tailroom, blockSize) _msgAlloc(pool, headroom, tailroom, blockSize, __func__, __LINE__)
void *_msgAlloc(MSGPOOL pool, UInt32 headroom, UInt32 tailroom, UInt32 blockSize, char *func, int line)
#else
void *msgAlloc(MSGPOOL pool, UInt32 headroom, UInt32 tailroom, UInt32 blockSize)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
    return qvMsgAlloc(pool, headroom, tailroom, blockSize, func, line);
#else
    return qvMsgAlloc(pool, headroom, tailroom, blockSize);
#endif
}

#ifdef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : msgFree
 * Inputs         : buffer - Pointer to the buffer to be freed.
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function requests de-allocation of memory pointed by
 *                  the buffer argument. This function is used in conjunction
 *                  with msgAlloc.
 ****************************************************************************/
void msgFree(void *buffer)
{
	if (buffer)
		qvMsgFree(buffer);

	return;
}
#endif

/****************************************************************************
 * Function Name  : msgSize
 * Inputs         : buffer - Pointer to buffer whose size is to be retrieved
 *                  blockSize - Returns size of 'non-zero copy buffer' data
 * Outputs        : None
 * Returns        : Size of zero copy buffer data present in the message
 * Variables      :
 * Description    : This function returns the size of buffer. 'blockSize' will
 *                  be set to the size of memory that was actually allocated
 *                  at the time of msgAlloc call. While the function will
 *                  return the size of payload attached to the message.
 ****************************************************************************/
UInt32 msgSize(const void *buffer, UInt32 *blockSize)
{
    return ((buffer != PNULL) ? qvMsgSize(buffer, blockSize) : 0);
}

#ifndef CSPL_LEAK_DEBUG_LITE
/****************************************************************************
 * Function Name  : msgReserve
 * Inputs         : buffer - Pointer to the message
 *                  where  - Nth byte where data is to be reserved
 *                  count  - Size of buffer to be reserved
 * Outputs        : None
 * Returns        : Pointer to the start of reserved area
 * Variables      :
 * Description    : This function can be used for reserving a specified area
 *                  of memory in a message buffer.
 ****************************************************************************/
UInt8 *msgReserve(void *buffer, SInt32 where, UInt32 count)
{
    return ((buffer != PNULL)?qvMsgReserve(buffer, where, count) : 0);
}

/****************************************************************************
 * Function Name  : msgInsert
 * Inputs         : buffer - Pointer to the message
 *                  where - Nth byte where data is to be inserted
 *                  data  - Pointer to external buffer to be added as segment
 *                  count - Size of data in ext buffer
 * Outputs        : None
 * Returns        : Pointer to data which was inserted.
 * Variables      :
 * Description    : This function is used for inserting data at a specified
 *                  location in a message.
 ****************************************************************************/
UInt8 *msgInsert(void *buffer, SInt32 where, UInt8 *data, UInt32 count)
{
    return (((buffer != PNULL) && (data != PNULL)) ?
                 qvMsgInsert(buffer, where, data, count) : PNULL);
}


/****************************************************************************
 * Function Name  : msgRemove
 * Inputs         : buffer - Pointer to the message
 *                  where  - Nth byte from where data is to be removed
 *                  count  - Size of data to be removed.
 * Outputs        : None
 * Returns        : Number of bytes removed.
 * Variables      :
 * Description    : This function can be used for removing part of data at
 *                  a specified location in a message.
 ****************************************************************************/
SInt32 msgRemove(void *buffer, SInt32 where, UInt32 count)
{
    return ((buffer != PNULL) ?  qvMsgRemove(buffer, where, count) : 0);
}


/****************************************************************************
 * Function Name  : msgExtract
 * Inputs         : buffer - Pointer to the message
 *                  where  - Nth byte from where data is to be extracted
 *                  data  - Pointer of location where extracted data is copied
 *                  count - Size of data to be extracted.
 * Outputs        : None
 * Returns        : Number of bytes extracted.
 * Variables      :
 * Description    : This function can be used for extracting a part of data
 *                  at a specified location in a message.
 ****************************************************************************/
SInt32 msgExtract(const void *buffer, SInt32 where, UInt8 *data, UInt32 count)
{
    return (((buffer != PNULL) && (data != PNULL)) ?
                     qvMsgExtract(buffer, where, data, count) : 0);
}


/****************************************************************************
 * Function Name  : msgClone
 * Inputs         : buffer - Pointer to the message
 * Outputs        : None
 * Returns        : Poinetr to the zero-copy buffer where msg is cloned
 * Variables      :
 * Description    : This function clone the message in a zero copy buffer
 *                  and returns the pointer to it.
 ****************************************************************************/
void *msgClone(const void *buffer)
{
    return ((buffer != PNULL) ? qvMsgClone(buffer) : PNULL);
}


/****************************************************************************
 * Function Name  : msgCopy
 * Inputs         : tobuffer - Pointer to destination buffer for copy operation
 *                  frombuffer - Pointer to source buffer for copy operation
 *                  blockSize  - size of 'non-zero copy buffer'
 * Outputs        : None
 * Returns        : Pointer to the 'tobuffer'
 * Variables      :
 * Description    : This function is used for creating a copy of message.
 *                  Contents of 'frombuffer' are copied to contents of
 *                  'tobuffer'. Both 'non-zero copy' and 'zero copy' buffers
 *                  of the message are copied. The zero copy buffers, if any,
 *                  in the 'tobuffer' are freed prior to the copy operation.
 ****************************************************************************/
void *msgCopy(void *tobuffer, const void *frombuffer, UInt32 blockSize)
{
    return (((tobuffer != PNULL) && (frombuffer != PNULL)) ?
		    qvMsgCopy(tobuffer, frombuffer, blockSize) : PNULL);
}


/****************************************************************************
 * Function Name  : msgSplit
 * Inputs         : buffer  - Pointer to message whose buffers are to be split
 *                  offset  - Location at which buffers are to be split
 *                  tobuffer - Pointer to destination buffer where buffers
 *                             after the split point are moved
 * Outputs        : None
 * Returns        : Pointer to the tobuffer
 * Variables      :
 * Description    : This function can be used for splitting 'buffer' message
 *                  at 'offset' position. The buffers after the split position
 *                  ('offset') are moved to 'tobuffer'. The zero copy buffers,
 *                  if any, in the 'tobuffer' are freed prior to the moving
 *                  split buffers.
 ****************************************************************************/
void *msgSplit(void *buffer, UInt32 offset, void *tobuffer)
{
    return (((buffer != PNULL) && (tobuffer != PNULL)) ?
		    qvMsgSplit(buffer, offset, tobuffer):PNULL);
}


/****************************************************************************
 * Function Name  : msgJoin
 * Inputs         : buffer - Pointer to message in which the list of buffers
 *                           shall be appended
 *                  append - Pointer to message whose list of buffers are to
 *                           be appended in 'buffer'
 * Outputs        : None
 * Returns        : Pointer to the 'buffer'
 * Variables      :
 * Description    : This function is used for joining list of 'zero copy
 *                  buffers' in 'append' message to the list of 'zero copy
 *                  buffers' in 'buffer' message.'Append' remains unaltered
 ****************************************************************************/
void *msgJoin(void *buffer, void *append)
{
    return (((buffer != PNULL) && (append != PNULL)) ?
                    qvMsgJoin(buffer, append) : PNULL);
}


/****************************************************************************
 * Function Name  : msgPack
 * Inputs         : buffer - Pointer to the buffer
 *                  headroom - Size of empty space at the start of the
 *                             payload buffer (optional)
 *                  tailroom - Size of empty space at the end of the
 *                             payload buffer (optional)
 * Outputs        : None
 * Returns        : Pointer to message with packed payload buffer
 * Variables      :
 * Description    : This function can be used for packing all the payload
 *                  buffers attached to a message into a single payload buffer.
 ****************************************************************************/
void *msgPack(void *buffer, UInt32 headroom, UInt32 tailroom)
{
    return ((buffer != PNULL) ? qvMsgPack(buffer, headroom, tailroom):PNULL);
}

#endif

/****************************************************************************
 * Function Name  : msgSegCount
 * Inputs         : buffer - Pointer to the message
 * Outputs        : None
 * Returns        : Number of payload buffers attached to the message
 * Variables      :
 * Description    : This function returns the number of payload buffers
 *                  attached to a message.
 ****************************************************************************/
UInt32 msgSegCount(const void *buffer)
{
    return ((buffer != PNULL) ? qvMsgSegCount(buffer):0);
}


/****************************************************************************
 * Function Name  : msgSegNext
 * Inputs         : buffer - Pointer to the message
 *                  last   - Pointer to control structure for payload
 *                           buffer accessed last time
 *                  segment- OutParam, returns data and size of the payload
 * Outputs        : None
 * Returns        : Pointer to control structure for payload buffer whose data
 *                  is copied in 'segment'. The same can be passed as 'last'
 *                  in next call of this function for fetching the next
 *                  payload buffer.
 * Variables      :
 * Description    : This function can be used for fetching a payload buffer
 *                  attached to a message.
 ****************************************************************************/
void *msgSegNext(const void *buffer, void *last, MSGSEGMENT *segment)
{
    return ((buffer != PNULL)?qvMsgSegNext(buffer, last, segment):PNULL);
}

/****************************************************************************
 * Function Name  : getMsgPoolStats
 * Inputs         : None
 * Outputs        : Displays message pool statistics.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function displays the statistics of all message pools.
 ****************************************************************************/
void getMsgPoolStats(void)
{
    qvMsgPoolStats();
    return;
}

/****************************************************************************
 * Function Name  : cleanupMsgPool
 * Inputs         : None
 * Outputs        : Cleans the memory allocated for message pools.
 * Returns        : None.
 * Variables      : None.
 * Description    : Frees all the memory allocated for message pools.
 ****************************************************************************/
void cleanupMsgPool(void)
{
    qvCleanupMsgPool();
    return;
}

#ifdef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : msgJoinAndFree
 * Inputs         : buffer - Pointer to message in which the list of buffers
 *                           shall be appended
 *                  append - Pointer to message whose list of buffers are to
 *                           be appended in 'buffer'
 * Outputs        : None
 * Returns        : Pointer to the 'buffer'
 * Variables      :
 * Description    : This function is used for joining list of 'zero copy
 *                  buffers' in 'append' message to the list of 'zero copy
 *                  buffers' in 'buffer' message.'Append' buffer is freed.
 ****************************************************************************/
void *msgJoinAndFree(void *buffer, void *append)
{
    return (((buffer != PNULL) && (append != PNULL)) ?
                    qvMsgJoinAndFree(buffer, append) : PNULL);
}

/* ECN changes start*/
/*****************************************************************************
 * FUNCTION: MsgLocate
 *
 * DESCRIPTION:
 *			This function is called to get the Pointer of the 'location' in
 *			present in the message pointed by q( It is assumend that the
 *			 data indicated byu the location shall be in same node)
 * ARGS:
 *			q			Pointer to the message
 *			location	Nth location whose offset is to be determined
 *			pLocation	OutParam, returns the offset of location
 *
 * RETURNS:
 *			Pointer of the 'location' in the node
 *
 ******************************************************************************/
SInt32 msgLocate(void *buffer, SInt32 where, UInt8 **pLocation)
{
    return (((buffer != PNULL) && (*pLocation != PNULL)) ?
            qvMsgLocate(buffer, where, pLocation) : 0);
}
/* ECN changes End*/
#endif

/*******Memory Handling changes 64UE Start******************************/
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
void msgPoolExtend(UInt32 blockSize, UInt32 nBuf)
{
    QMPOOL  msgPool;

    msgPool = qvMsgPoolForSize(blockSize);
    if (!msgPool)
    {
        lteWarning("Message pool for size %d not present\n", blockSize);
    } else if(!qvMsgPoolExtend(msgPool,nBuf))\
    {
        lteWarning("Failed to extend pool for size%d\n", blockSize);
    }
    return;
}
/*******Memory Handling changes 64UE End****************************/
#endif
