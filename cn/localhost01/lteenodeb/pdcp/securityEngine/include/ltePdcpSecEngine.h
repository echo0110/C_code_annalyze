/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpSecEngine.h $
 ****************************************************************************
 *
 *  File Description : Definations for Security engine
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpSecEngine.h,v $
 *
 *
 ****************************************************************************/
#ifndef _LIB_I_SEC_ENG_DEF_
#define _LIB_I_SEC_ENG_DEF_
#ifdef PDCP_ASYNC_INTERFACE

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "ltePdcpEncSap.h"
#include "lteMsgPool.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*wrapper macros definitions for security engine Input TX queues */
/* + SPR 14179 */
#define QINIT_PDCP_SEC_INPUT_TX_Q()\
    circQInit( &pdcpSecInputTxQ_g, MAX_NODES );

#define QCOUNT_PDCP_SEC_INPUT_TX_Q()\
    circQueueCount( &pdcpSecInputTxQ_g )

#define ENQUEUE_PDCP_SEC_INPUT_TX_Q(pdcpSecData_p)\
    pushNodeCircQ( &pdcpSecInputTxQ_g, pdcpSecData_p )

#define DEQUEUE_PDCP_SEC_INPUT_TX_Q(pdcpSecData_p){\
    popNodeCircQ( \
            &pdcpSecInputTxQ_g, (void **)&pdcpSecData_p); \
}
/* - SPR 14179 */

/* + SPR 17073 */
#define UPPER_POW_OF_TWO(num) \
    num--; \
    num |= num >> 1; \
    num |= num >> 2; \
    num |= num >> 4; \
    num |= num >> 8; \
    num |= num >> 16; \
    num++; 

/*wrapper macros definitions for security engine Input SRB TX queues */
#define QINIT_PDCP_SEC_SRB_INPUT_TX_Q(srbQueueLimit)\
    circQInit( &pdcpSecSrbInputTxQ_g, srbQueueLimit );

#define QCOUNT_PDCP_SEC_SRB_INPUT_TX_Q()\
    circQueueCount( &pdcpSecSrbInputTxQ_g )

#define ENQUEUE_PDCP_SEC_SRB_INPUT_TX_Q(pdcpSecData_p)\
    pushNodeCircQ( &pdcpSecSrbInputTxQ_g, pdcpSecData_p )

#define DEQUEUE_PDCP_SEC_SRB_INPUT_TX_Q(pdcpSecData_p){\
    popNodeCircQ( \
            &pdcpSecSrbInputTxQ_g, (void **)&pdcpSecData_p); \
}

/*wrapper macros definitions for security engine Input SRB RX queues */
#define QINIT_PDCP_SEC_SRB_INPUT_RX_Q(srbQueueLimit)\
    circQInit( &pdcpSecSrbInputRxQ_g, srbQueueLimit );

#define QCOUNT_PDCP_SEC_SRB_INPUT_RX_Q()\
    circQueueCount( &pdcpSecSrbInputRxQ_g )

#define ENQUEUE_PDCP_SEC_SRB_INPUT_RX_Q(pdcpSecData_p)\
    pushNodeCircQ( &pdcpSecSrbInputRxQ_g, pdcpSecData_p )

#define DEQUEUE_PDCP_SEC_SRB_INPUT_RX_Q(pdcpSecData_p){\
    popNodeCircQ( \
            &pdcpSecSrbInputRxQ_g, (void **)&pdcpSecData_p); \
}
/* - SPR 17073 */

/*wrapper macros definitions for security engine Input RX queues */
/* + SPR 14179 */
#define QINIT_PDCP_SEC_INPUT_RX_Q()\
    circQInit( &pdcpSecInputRxQ_g, MAX_NODES );

#define QCOUNT_PDCP_SEC_INPUT_RX_Q()\
    circQueueCount( &pdcpSecInputRxQ_g )

#define ENQUEUE_PDCP_SEC_INPUT_RX_Q(pdcpSecData_p)\
    pushNodeCircQ( &pdcpSecInputRxQ_g, pdcpSecData_p )

#define DEQUEUE_PDCP_SEC_INPUT_RX_Q(pdcpSecData_p){\
    popNodeCircQ( \
            &pdcpSecInputRxQ_g, (void **)&pdcpSecData_p); \
}
/* - SPR 14179 */

/*wrapper macros definitions for security engine Output TX queues */
/* + SPR 14179 */
#define QINIT_PDCP_SEC_OUTPUT_TX_Q(pdcpSecConfig_p)\
    circQInit( &(pdcpSecConfig_p.pdcpSecOutputTxQ), MAX_NODES );

#define QCOUNT_PDCP_SEC_OUTPUT_TX_Q(pdcpSecConfig_p)\
    circQueueCount( &(pdcpSecConfig_p.pdcpSecOutputTxQ) );

#define ENQUEUE_PDCP_SEC_OUTPUT_TX_Q(pdcpSecConfig_p, pdcpSecData_p)\
    pushNodeCircQ( &(pdcpSecConfig_p.pdcpSecOutputTxQ), pdcpSecData_p )

#define DEQUEUE_PDCP_SEC_OUTPUT_TX_Q(pdcpSecConfig_p, pdcpSecData_p){\
    popNodeCircQ( \
            &(pdcpSecConfig_p.pdcpSecOutputTxQ), (void **)&pdcpSecData_p); \
}
/* - SPR 14179 */

/*wrapper macros definitions for security engine Output RX queues */
/* + SPR 14179 */
#define QINIT_PDCP_SEC_OUTPUT_RX_Q(pdcpSecConfig_p)\
    circQInit( &(pdcpSecConfig_p.pdcpSecOutputRxQ), MAX_NODES );

#define QCOUNT_PDCP_SEC_OUTPUT_RX_Q(pdcpSecConfig_p)\
    circQueueCount( &(pdcpSecConfig_p.pdcpSecOutputRxQ) );

#define ENQUEUE_PDCP_SEC_OUTPUT_RX_Q(pdcpSecConfig_p, pdcpSecData_p)\
    pushNodeCircQ( &(pdcpSecConfig_p.pdcpSecOutputRxQ), pdcpSecData_p );

#define DEQUEUE_PDCP_SEC_OUTPUT_RX_Q(pdcpSecConfig_p, pdcpSecData_p){\
    popNodeCircQ( \
            &(pdcpSecConfig_p.pdcpSecOutputRxQ), (void **)&pdcpSecData_p); \
}
/* - SPR 14179 */

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* Init structure at PDCP and security engine */
typedef struct PdcpSecConfigT
{
    UInt8 workMode;

    /* Output queue for processed packets */
    /* + SPR 14179 */
    LTE_CIRCQUEUE pdcpSecOutputTxQ;
    LTE_CIRCQUEUE pdcpSecOutputRxQ;
    /* - SPR 14179 */

}PdcpSecConfig, *LP_PdcpSecConfig;

/* Structure to enqueue input packets in security engine */
typedef struct PdcpSecPacketT
{
    /* SPR 15236 fix start */
    /*SPR_16529_review_commet +*/
    /* The count is maintained so that ASYNC thread need not access the
     * dataReq_p which may become dangling due to UE delete */
    /*SPR_16529_review_commet -*/
    UInt32               count; 
    /* SPR 15236 fix end */
   /* +- SPR 14179*/
    LP_PdcpSecContextInfo secContext_p;
    void                  *inData_p;
    void                  *outData_p;
    uaContextHandler      uaCtxHandle ;
/*SPR 16529 fix start*/
    /* + Coverity 64114, 64115 */
    UInt16                ueIndex;
    UInt16                lcId;
    /* - Coverity 64114, 64115 */
/*SPR 16529 fix end*/
} PdcpSecPacket, *LP_PdcpSecPacket ;

/* Structure to enqueue output packets in security engine */
typedef struct PdcpSecOutPacketT
{
    /* +- SPR 14179*/
    void             *inData_p;
    void             *outData_p;
    uaContextHandler uaCtxHandle;
    secStatus        status;
    UInt32           errInfo;
    pdcpSecOutCbk    secOutCbk;
    /* SPR 15236 fix start */
    UInt16                ueIndex;
    UInt16                lcId;
    /* SPR 15236 fix end */
}PdcpSecOutPacket, *LP_PdcpSecOutPacket;

/****************************************************************************
 * Exported Constants
****************************************************************************/
#define PDCP_NUM_OF_BEARERS 2

/****************************************************************************
 * Exported Variables
 ***************************************************************************/
extern PdcpSecConfig pdcpSecConfig_g;
/* SPR 15342 fix start */
/* SPR 15342 fix end */

/****************************************************************************
 * Exported Functions
 ***************************************************************************/

/****************************************************************************
 * Function Name  : pdcpSecProcessPacketTx
 * Inputs         : secDataRx_p - Pointer to input queue node
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This function will process TX processing to be done by
 *                  security engine.
 ****************************************************************************/
UInt32 pdcpSecProcessPacketTx(LP_PdcpSecPacket secDataRx_p);

/****************************************************************************
 * Function Name  : pdcpSecProcessPacketRx
 * Inputs         : secDataRx_p - Pointer to input queue node
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This function will process RX processing to be done by
 *                  security engine.
 ****************************************************************************/
UInt32 pdcpSecProcessPacketRx(LP_PdcpSecPacket secDataRx_p);

#endif



#endif /* _LIB_I_SEC_ENG_DEF_ */
