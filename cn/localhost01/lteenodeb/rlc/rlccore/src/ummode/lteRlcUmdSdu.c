/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmdSdu.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:34 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : this file describes the operations on SDU queue.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcUmdSdu.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:34  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:24:29  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.7.2.2  2010/07/17 11:51:09  gur23054
 * Review Comments fixed
 *
 * Revision 1.7.2.1  2010/06/18 07:31:04  gur19836
 * Corrected Indentation
 *
 * Revision 1.7  2010/06/14 12:04:29  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.6  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.5  2009/03/06 12:44:42  gur19140
 * Review comments fixes
 *
 * Revision 1.4  2009/03/02 05:50:58  gur19140
 * *** empty log message ***
 *
 * Revision 1.3  2009/02/12 12:28:52  gur19140
 * timer related fixes
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

#include "lteRlcUmdSdu.h"
#include "lteRlcPdcpInterface.h"
#include "lteCircQueue.h"
/******************************************************************************
 * Exported Functions
 ******************************************************************************/
extern UInt32 rlcCompare(void *arg, void *data);
/******************************************************************************/

/****************************************************************************
 * Function Name  : rlcSearchNdeleteUmdSduFromQ
 * Inputs         : umdRawSduQ_p - Pointer to UmdRawSduQ,
 *                  transId - Transaction ID
 * Outputs        : None
 * Returns        : sduSize - size of deleted sdu 
 * Description    : search and delete the node from queue.
 ****************************************************************************/
UInt32 rlcSearchNdeleteUmdSduFromQ(UmdRawSduQ * umdRawSduQ_p,
        /* SPR 2812 changes start */
        void *transId)
        /* SPR 2812 changes end */
{ 
    UInt32 sduSize = 0;
    RlcUeDataReq * UmdRawSdu_p = RLC_NULL ;
    SInt32 buffIdx = 0;
    /* SPR 2812 changes start */
    if((buffIdx = qLinearSearch(umdRawSduQ_p->pQ, &rlcCompare, transId)) >= 0)
    /* SPR 2812 changes end */
    {
       UmdRawSdu_p = (RlcUeDataReq *)umdRawSduQ_p->pQ->ring[buffIdx].data; 
       sduSize = UmdRawSdu_p->size;
        if(RLC_SUCCESS == delNodeCircQ(umdRawSduQ_p->pQ, (UInt32)buffIdx))
        {
            msgFree(UmdRawSdu_p->data_p);
            UmdRawSdu_p->data_p = RLC_NULL;
            freeMemPool(UmdRawSdu_p);
            return sduSize;
        }
        /*SPR 10678 Fix Starts*/
        else
        {
            sduSize = 0;
        }
        /*SPR 10678 Fix Ends*/
    }
    return sduSize;
}

