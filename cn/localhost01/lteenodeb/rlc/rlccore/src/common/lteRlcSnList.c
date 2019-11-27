/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcSnList.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:12 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcSnList.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:12  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:24:21  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.5.28.3  2010/07/20 05:47:46  gur19836
 * Code Review Comment: Indentation and 80 column width: Incorporated
 *
 * Revision 1.5.28.2  2010/07/17 11:43:47  gur23054
 * Review Comments fixed
 *
 * Revision 1.5.28.1  2010/07/12 09:09:03  gur23054
 * Klockwork warnings removed
 *
 * Revision 1.5  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.4  2009/02/12 12:28:52  gur19140
 * timer related fixes
 *
 * Revision 1.3  2009/02/11 13:25:19  gur19140
 * fix logging issue timer issue snList issue
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
#include "lteRlcSnList.h"
#include "lteRlcSort.h"
#include "logging.h"
#include "alarm.h"

/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */

/****************************************************************************
 * Function Name  : rlcCreateSnList 
 * Inputs         : size - size of sn list
 * Outputs        : pointer of RlcSnList
 * Returns        : snList_p -  size of SN List/RLC_FAILURE
 * Description    : This API is used create the sn list of size specified.
 ****************************************************************************/

RlcSnList *rlcCreateSnList(UInt32 size) 
{
    RlcSnList *snList_p = RLC_NULL;
    if((snList_p = (RlcSnList *)getMemFromPool
                (2*sizeof(UInt32) + size,RLC_NULL)) == RLC_NULL )
    {
        LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,(RLC_RX), globalTTITickCount_g, __LINE__,
                (2*sizeof(UInt32) + size) ,0, 0,0, 0,0, __func__, "Memry Allocation for Sn list Fail");
        return RLC_FAILURE;
    }    
    snList_p->size = size; 
    snList_p->szUsed = 0;
    return snList_p;
}

/****************************************************************************
 * Function Name  : rlcSetSnList
 * Inputs         : snList_p - pointer of RlcSnList,
 *                  sn - value of sn
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to set the sn value into sn list.
 ****************************************************************************/

void rlcSetSnList(RlcSnList* snList_p,RlcSn sn)
{
    snList_p->snList[snList_p->szUsed] = sn;
    snList_p->szUsed++;
    /* SPR 13562 changes */
    /* Code removed form here */
    /* Due to deleted code snippet, snList_p->szUsed = 0 in case of snList_p->size=512 
    pdu case and reassembly will not be done and ring will not be freed and 
    RLC will go in while loop. */
    /* SPR 13562 changes */
}

/****************************************************************************
 * Function Name  : rlcReverseSnList
 * Inputs         : snList_p - pointer of RlcSnList
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to reverse sn values.
 ****************************************************************************/

void rlcReverseSnList(RlcSnList* snList_p)
{
    RlcSn *sn_p = &snList_p->snList[0];
    UInt32 j = snList_p->szUsed - 1;
    UInt32 i = 0;
    for(;i < j; i++,j--)
    {
        RlcSn temp = sn_p[i];
        sn_p[i] = sn_p[j];
        sn_p[j] = temp;
    }
}

