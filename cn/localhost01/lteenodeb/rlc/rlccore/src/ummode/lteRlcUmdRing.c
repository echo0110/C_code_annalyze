/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmdRing.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:33 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : this file describes the Ring operations.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcUmdRing.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:33  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:24:29  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.5.28.1  2010/07/17 11:51:01  gur23054
 * Review Comments fixed
 *
 * Revision 1.5  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.4  2009/02/11 11:18:46  gur19140
 * Fixed regarding ut 53and 54
 *
 * Revision 1.3  2009/02/10 08:45:39  gur19140
 * Changes regarding RM40 comments and LTE_RLC_DEINIT_1 issue
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

#include "lteRlcUmdRing.h"
#include "logging.h"
#include "alarm.h"

/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
/****************************************************************************
 * External Variable Includes
 ****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */

/****************************************************************************
 * Function Name  : rlcRingInitUmdPduR
 * Inputs         : pR - a pointer to Ring,
 *                  sz -  an integer having size of ring
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : Initializes the ring.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
int rlcRingInitUmdPduR( UmdPduRing *pR, 
                               RingSz sz
                             ) 
/* SPR 5599_129119 Fix End */
{
    UInt32 ret = RLC_SUCCESS;
    RingNodeId idx = 0 ;
    ret = rlcRingInit(pR->pR,sz);
    if(ret != RLC_SUCCESS)
    {
	return ret;
    }
    else{
        if ( sz > 0 )
        {
            for( ; idx < sz ;idx++)
            {
                UmdPdu *pPdu = RLC_NULL;
                CREATE_UMD_PDU(pPdu,0);
                if( pPdu == RLC_NULL )
                {
                    ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
                    return RLC_FAILURE;
                }    
                SET_RING_DATA(pR->pR,idx,pPdu);
            }
        }
        return ret;
    }
}
/****************************************************************************
 * Function Name  : rlcChangeSizeUmdPduR
 * Inputs         : pR - a pointer to Ring,
 *                  sz -  an integer having size of ring
 * Outputs        : None
 * Returns        : None
 * Description    : It changes size of the ring.
 ****************************************************************************/
void rlcChangeSizeUmdPduR(UmdPduRing *pR,RingSz sz)
{
    RingSz rSz     = RING_SIZE_UMD_PDU_RN(pR) ;
    RingNodeId idx = 0 ;
    UmdPdu *pPdu   = RLC_NULL;

    if ( rSz > 0)
    {
        for(;idx < rSz ;idx++)
        {
            pPdu = GET_RING_DATA(pR->pR,idx);
            FREE_UMD_PDU(pPdu);
            pPdu = RLC_NULL ;
        }
    }
    rlcRingInitUmdPduR(pR,sz);
}

