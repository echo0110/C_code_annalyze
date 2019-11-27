/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: 
 *
 ****************************************************************************
 *
 *  File Description : this file describes the Ring operations.
 *
 ****************************************************************************
 *
 *
 ****************************************************************************/
/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcAmdRing.h"
#include "logging.h"
#include "alarm.h"

/****************************************************************************
 * External Variable Includes
 ****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : rlcFreeAmdPdu
 * Inputs         : AmdDFEQ_p - a pointer to AmdPdu
 * Outputs        : None
 * Returns        : None
 * Description    : Free the PDUs and PDU segment.
 ****************************************************************************/

void rlcFreeAmdPdu(AmdPdu *AmdDFEQ_p)
{
    AmdPduSegment *amdPduSeg_p = RLC_NULL ;
    DEQUEUE_AMD_PDU_SEGMENT_Q(AmdDFEQ_p,amdPduSeg_p);
    while(amdPduSeg_p != RLC_NULL ){
        rlcFreeAmdPduSegment(amdPduSeg_p);
        amdPduSeg_p = RLC_NULL;
        DEQUEUE_AMD_PDU_SEGMENT_Q(AmdDFEQ_p,amdPduSeg_p);
    }
    /* + SPR 9646 Fix */
    semDestroy((LTE_SEM *)&(AmdDFEQ_p->LQ.sem_lock));
    /* - SPR 9646 Fix */
    freeMemPool(AmdDFEQ_p);
}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : rlcRingInitAmdPduR
 * Inputs         : pR - a pointer to Ring,
 *                  sz -  an integer having size of ring
 * Outputs        : None
 * Returns        : None
 * Description    : It initializes the ring.
 ****************************************************************************/

void rlcRingInitAmdPduR(AmdPduRing *pR,RingSz sz) 
{
    UInt32 ret = RLC_SUCCESS;
    RingNodeId idx = 0 ;
    /* SPR 4010 Fix Start */
    if(pR == RLC_NULL)
    {
        return;
    }
    /* SPR 4010 Fix End */
    ret = rlcRingInit(pR->pR,sz);
    if(ret != RLC_SUCCESS)
    {
    }
    else{
        if(sz > 0 ){
            for(;idx < sz ;idx++){
                AmdPdu *pPdu = RLC_NULL;
                CREATE_AMD_PDU(pPdu);
                if( pPdu == RLC_NULL )
                {
                    ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
                    return;
                }    
                SET_RING_DATA(pR->pR,idx,pPdu);
            }
        }
    }
}

/****************************************************************************
 * Function Name  : rlcChangeSizeAmdPduR
 * Inputs         : pR - a pointer to Ring
 *                  sz -  an integer having size of ring
 * Outputs        : None
 * Returns        : None
 * Description    : It changes size of the ring.
 ****************************************************************************/

void rlcChangeSizeAmdPduR(AmdPduRing *pR,RingSz sz)
{
    RingSz rSz     = RING_SIZE_AMD_PDU_RN(pR) ;
    RingNodeId idx = 0 ;
    AmdPdu *pPdu   = RLC_NULL;

    if ( rSz > 0 )
    {
        for(;idx < rSz ;idx++)
        {
            pPdu = GET_RING_DATA(pR->pR,idx);
            /* Cyclomatic Complexity changes - starts here */
            /* SPR S1 Link Down Fix start */
            if ( RLC_NULL != pPdu )
            {
                rlcFreeAmdPdu(pPdu);
                //FREE_AMD_PDU(pPdu);
                /* Cyclomatic Complexity changes - ends here */
                pPdu = RLC_NULL ;
            }
            /* SPR S1 Link Down Fix end */
        }
    }
    rlcRingInitAmdPduR(pR,sz);
}

