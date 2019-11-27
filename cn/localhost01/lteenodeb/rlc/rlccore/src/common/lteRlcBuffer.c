/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcBuffer.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:08 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcBuffer.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:08  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:43  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.13.2.3  2010/07/20 05:47:46  gur19836
 * Code Review Comment: Indentation and 80 column width: Incorporated
 *
 * Revision 1.13.2.2  2010/07/17 11:43:06  gur23054
 * Review Comments fixed
 *
 * Revision 1.13.2.1  2010/06/18 07:30:57  gur19836
 * Corrected Indentation
 *
 * Revision 1.13  2010/06/14 12:02:40  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.12  2009/06/04 05:20:41  gur19836
 * Optimization Changes
 *
 * Revision 1.11  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.10  2009/03/06 12:44:42  gur19140
 * Review comments fixes
 *
 * Revision 1.9  2009/02/18 13:22:09  gur19140
 * tx fixes
 *
 * Revision 1.8  2009/02/17 14:29:58  gur19140
 *
 * Revision 1.7  2009/02/13 14:07:51  gur19140
 * bug fixes
 *
 * Revision 1.6  2009/02/12 16:45:46  gur20294
 * update for making correct some common channel entity configuration related actions
 *
 * Revision 1.5  2009/02/10 08:45:39  gur19140
 * Changes regarding RM40 comments and LTE_RLC_DEINIT_1 issue
 *
 * Revision 1.4  2009/02/09 05:00:02  gur20294
 * update
 *
 * Revision 1.3  2009/02/05 10:58:21  gur19836
 * CR comments fixed
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcBuffer.h"
#include "logging.h"
#include "alarm.h"

void *zeroPool_g = RLC_NULL;
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */

/****************************************************************************
 * Function Name  : rlcCreateBuffer
 * Inputs         : size - size of buffer to be created
 * Outputs        : pointer of RlcBuffer
 * Returns        : pointer of buffer created
 * Description    : This API is used to create zero copy buffer.
 ****************************************************************************/

RlcBuffer *rlcCreateBuffer(UInt32 size) 
{
    RlcBuffer *rlcBuff_p = RLC_NULL;
    if((rlcBuff_p = 
        (RlcBuffer *)getMemFromPool(sizeof(RlcBuffer), RLC_NULL)) == RLC_NULL)
    {
	/* Changes for Rel 3.1 Start */
	LOG_RLC_MSG(LTE_RLC_MEM_ALLOC_FAILURE, LOGFATAL,(RLC_RX), globalTTITickCount_g, __LINE__,
                 sizeof(RlcBuffer),0, 0,0, 0,0, __func__, "creating buffer");
	/* Changes for Rel 3.1 End */
	ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("Memory Allocation Failure");

        return RLC_NULL;
    }
    rlcBuff_p->data_p = RLC_NULL;
    if((rlcBuff_p->data_p = (UInt8*)msgAlloc(zeroPool_g,0,0,size)) == RLC_NULL)
    {
	LOG_RLC_MSG(LTE_RLC_MSG_ALLOC_FAIL, LOGFATAL,(RLC_RX), globalTTITickCount_g, __LINE__,
                 0,0, 0,0, 0,0, __func__, "");

	ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("Memory Allocation Failure");
        return RLC_NULL;
    }
    rlcBuff_p->copyType = RLC_ZERO_COPY_BUFFER;
    rlcBuff_p->size = size; 
    rlcBuff_p->szUsed = 0;
    return rlcBuff_p;
}

/****************************************************************************
 * Function Name  : rlcSetBuffer
 * Inputs         : size - size of buffer to be created,
 *                  rlcBuff_p - pointer of RlcBuffer
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to create zero copy buffer.
 ****************************************************************************/

void rlcSetBuffer(UInt32 size, RlcBuffer* rlcBuff_p) 
{
    rlcBuff_p->data_p = RLC_NULL;
    if((rlcBuff_p->data_p = (UInt8*)msgAlloc(zeroPool_g,0,0,size)) == RLC_NULL)
    {
	LOG_RLC_MSG(LTE_RLC_MSG_ALLOC_FAIL, LOGFATAL,(RLC_RX), globalTTITickCount_g, __LINE__,
                 0,0, 0,0, 0,0, __func__, "");
	ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("Memory Allocation Failure");

    }
    rlcBuff_p->copyType = RLC_ZERO_COPY_BUFFER;
    rlcBuff_p->size = size; 
    rlcBuff_p->szUsed = 0;
}

/****************************************************************************
 * Function Name  : rlcCopyBuffer
 * Inputs         : desRlcBuff_p - pointer to RlcBuffer,
 *                  src_p - void pointer,
 *                  size - size of buffer
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to create zero copy buffer also copy the
 *                  data into data field.
 ****************************************************************************/

void rlcCopyBuffer(RlcBuffer *desRlcBuff_p,void *src_p, UInt32 size)
{
    msgCopy(desRlcBuff_p->data_p,src_p, 0);

    desRlcBuff_p->szUsed = size;
    desRlcBuff_p->size = size;
}

/****************************************************************************
 * Function Name  : rlcFreeBuffer
 * Inputs         : rlcBuff_p - pointer to RlcBuffer
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to free the zero copy buffer.
 ****************************************************************************/

void rlcFreeBuffer(RlcBuffer* rlcBuff_p) 
{
    if(rlcBuff_p != RLC_NULL)
    {
        if(rlcBuff_p->data_p != RLC_NULL)
        {
            msgFree(rlcBuff_p->data_p);
            rlcBuff_p->data_p = RLC_NULL;
        }
        freeMemPool(rlcBuff_p);
    }
}
