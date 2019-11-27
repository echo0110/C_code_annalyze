/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcRing.c,v 1.1.1.1.12.2.2.2 2010/10/07 13:35:08 gur23054 Exp $
 *
 ****************************************************************************
 *
 *  File Description : this file describes the Ring operations.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcRing.c,v $
 * Revision 1.1.1.1.12.2.2.2  2010/10/07 13:35:08  gur23054
 * Fix for SPR 698
 *
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:10  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:24:21  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.2.28.2  2010/07/20 05:47:46  gur19836
 * Code Review Comment: Indentation and 80 column width: Incorporated
 *
 * Revision 1.2.28.1  2010/07/17 11:43:35  gur23054
 * Review Comments fixed
 *
 * Revision 1.2  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.1  2009/01/29 05:35:22  gur20294
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

#include "lteRlcRing.h"
#include "logging.h"
#include "alarm.h"
#include "lteRlcErrorCode.h"

/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
#ifdef MAC_AUT_TEST
    extern UInt16 rlcAutFlag_g;
#endif

/****************************************************************************
 * Function Name  : rlcRingInit
 * Inputs         : ring_p - a pointer to Ring,
 *                  sz -  an integer having size of ring
 * Outputs        : None
 * Returns        : RLC_SUCCESS/LTE_RLC_MEM_ALLOC_FAIL
 * Description    : Initialize the RLC Ring.
 ****************************************************************************/
UInt32 rlcRingInit(Ring *ring_p,RingSz sz)
{
    RingSz lvar = 0 ;
    ring_p->size = sz ;
    if(sz > 0)
    {
       /*As per SPR 698, ltePanic takes care of the returning value, no change done*/
        ring_p->node_p = (RingN*)getMemFromPool(sz*sizeof(RingN),RLC_NULL);
#ifdef LTE_EMBMS_SUPPORTED        
#ifdef MAC_AUT_TEST 
        if(rlcAutFlag_g == RLC_FOUR)
        {
            ring_p->node_p = RLC_NULL;
        }
#endif
#endif        
        if(ring_p->node_p == RLC_NULL)
        {
            return LTE_RLC_MEM_ALLOC_FAIL;
        }
        for(;lvar < sz ;lvar++ )
        {
            ring_p->node_p[lvar].nodeSlotStatus = RING_SLOT_FREE ;
        }
    }
    return RLC_SUCCESS;
}

