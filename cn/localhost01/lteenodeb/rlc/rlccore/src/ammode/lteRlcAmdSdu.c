/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmdSdu.c
 *
 ****************************************************************************
 *
 *  File Description : this file describes the operations on SDU queue.
 *
 ****************************************************************************
 *
 *
 ****************************************************************************/
/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcAmdSdu.h"
#include "lteRlcPdcpInterface.h"
/******************************************************************************
 * Exported Functions
 ******************************************************************************/
extern UInt32 rlcCompare(void *arg, void *data);
/******************************************************************************/

/****************************************************************************
 * Function Name  : rlcSearchNdeleteAmdSduFromQ
 * Inputs         : amdRawSduQ_p - Pointer to AmdRawSduQ,
 *                  transId - Transaction ID
 * Outputs        : None
 * Returns        : size of deleted sdu
 * Description    : search and delete the node from queue.
 ****************************************************************************/
/* SPR 2812 changes start */
UInt32 rlcSearchNdeleteAmdSduFromQ(AmdRawSduQ * amdRawSduQ_p,void *transId)
/* SPR 2812 changes end */
{ 
    UInt32 sduSize = 0;
    RlcUeDataReq * AmdRawSdu_p = RLC_NULL ;
    SInt32 buffIdx = 0;
        /** SPR 3558 Changes Start **/
        /* SPR 2812 changes start */
    if(( buffIdx = qLinearSearch(amdRawSduQ_p->pQ, &rlcCompare, transId)) >= 0)
    /* SPR 2812 changes end */
    {
        /** SPR 3558 Changes End **/
        AmdRawSdu_p = (RlcUeDataReq *)amdRawSduQ_p->pQ->ring[buffIdx].data;
        sduSize = AmdRawSdu_p->size;
        if(RLC_SUCCESS == delNodeCircQ(amdRawSduQ_p->pQ,(UInt32)buffIdx))
        {
            msgFree(AmdRawSdu_p->data_p);
            AmdRawSdu_p->data_p = RLC_NULL;
            freeMemPool(AmdRawSdu_p);
            return sduSize;
        }
        /* SPR 3406 changes start */
        else
        {
            return 0;
        }
        /* SPR 3406 changes end */
    }
    return sduSize;
}

