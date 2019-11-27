/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpEncSap.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Definitions for Interface between the PDCP layer
 *                     and Security engine
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 ****************************************************************************/

#ifndef _LIB_I_ENC_SAP_DEF_
#define _LIB_I_ENC_SAP_DEF_

#ifdef PDCP_ASYNC_INTERFACE
/* SPR 20899 Start */
#define MULTIPLY_BY_FOUR(x) ((x) << 2)
/* SPR 20899 Stop */
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include "lteSQueue.h"
#include "lteCircQueue.h"
/*SPR 16529 fix start*/
#include "ltePdcpLteDefs.h"
/*SPR 16529 fix end*/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
typedef enum secStatusT
{
    SEC_STATUS_SUCCESS = 0, /*SEC processed packet without error*/
    SEC_STATUS_ERROR,       /*SEC packet processing returned with error*/
    SEC_STATUS_MAC_I_CHECK_FAILED, /*Integrity check failed for this packet.*/

    SEC_STATUS_MAX_VALUE,
}secStatus;

typedef enum secErrorT
{
    SEC_SUCCESS          = 0,
    SEC_OUT_OF_MEMORY,
    SEC_PROCESSING_ERROR,
    SEC_JR_IS_FULL,
    
    SEC_RETURN_CODE_MAX_VALUE
}secError;

/* SPR 15236 fix start */
#define SEC_RB_DIRECTION 2
/* SPR 15236 fix end */

/* Pointer to a meta-data (LP_PdcpDataReq at TX side and LP_PdcpDataInd at RX
 * side ) 
 */
typedef void* uaContextHandler;

/****************************************************************************
 * Exported Types
 ****************************************************************************/
    
/* Queue to store input packets */
/* + SPR 14179 */
extern LTE_CIRCQUEUE pdcpSecInputTxQ_g;
extern LTE_CIRCQUEUE pdcpSecInputRxQ_g;
/* - SPR 14179 */
/* + SPR 17073 */
extern LTE_CIRCQUEUE pdcpSecSrbInputTxQ_g;
extern LTE_CIRCQUEUE pdcpSecSrbInputRxQ_g;
#ifdef PDCP_ASYNC_INTR_TESTING
extern LTE_SQUEUE secIntrInputRxQ_g;
extern LTE_SQUEUE secIntrInputTxQ_g;
/* Changes End for ASYN*/

#define QINIT_SEC_INTR_INPUT_TX_Q(numOfNodes)\
   sQueueInit (&secIntrInputTxQ_g);

#define QCOUNT_SEC_INTR_INPUT_TX_Q()\
    sQueueCount ((const LTE_SQUEUE *)&secIntrInputTxQ_g);

#define ENQUEUE_SEC_INTR_INPUT_TX_Q(pdcpSecData_p)\
    pushNode( &secIntrInputTxQ_g, &(pdcpSecData_p)->sNode );

#define DEQUEUE_SEC_INTR_INPUT_TX_Q(pdcpSecData_p)\
    pdcpSecData_p = (LP_PdcpSecOutPacket)popNode( \
                                        (LTE_SQUEUE *)&secIntrInputTxQ_g);

#define QINIT_SEC_INTR_INPUT_RX_Q(numOfNodes)\
   sQueueInit (&secIntrInputRxQ_g);

#define QCOUNT_SEC_INTR_INPUT_RX_Q()\
    sQueueCount ((const LTE_SQUEUE *)&secIntrInputRxQ_g);

#define ENQUEUE_SEC_INTR_INPUT_RX_Q(pdcpSecData_p)\
    pushNode( &secIntrInputRxQ_g, &(pdcpSecData_p)->sNode );

#define DEQUEUE_SEC_INTR_INPUT_RX_Q(pdcpSecData_p)\
    pdcpSecData_p = (LP_PdcpSecOutPacket)popNode( \
                                        (LTE_SQUEUE *)&secIntrInputRxQ_g);
#endif


/****************************************************************************
 * Function Name  : pdcpSecOutCbk 
 * Inputs         : inData_p  - pointer to input data.
 *                  outData_p  - pointer to output data.
 *                  uaCtxHandle - Pointer to meta-data.
 *                  status - status of the packet after processing by the 
 *                           security engine.
 *                  errInfo - error indications if any.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used for receiving the processed packets 
 *                  from security engine by PDCP. It will be a callback
 *                  function. 
 ****************************************************************************/
typedef Int (*pdcpSecOutCbk) (
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo); 

/* PDCP context between PDCP and security engine */
typedef struct PdcpSecContextInfoT
{
    UInt8       snSize;          /* SN size reqd for DRB */
    UInt8       bearer;          /* Radio bearer id */
    UInt8       direction;      /* Direction can be uplink(0) or downlink(1) */
    UInt8       integrityAlgoId; /* Integrity algo to be used */
    UInt8       oldCiphAlgoId;   /* Ciphering algo to be used before re-est */
    UInt8       newCiphAlgoId;   /* Ciphering algo to be used after re-est */
    UInt32      hfn;             /* HFN used for the packet */
    UInt8       *oldCiphKey_p;   /* Ciphering key to be used before re-est */
    UInt8       *newCiphKey_p;   /* Ciphering key to be used after re-est */
    UInt32      ciphKeyLen;      /* Length of ciphering key */
    UInt8       *integrityKey_p; /* Integrity key to be used */
    UInt32      integrityKeyLen; /* Length of Integrity key */
    pdcpSecOutCbk notifyPacket;  /* Callback function to be called for all 
                                    packets processed on this context. */
#ifndef UE_SIM_TESTING									
	/* SPR 8174 Changes Start */
	UInt8       reconfigUeFlag;
	/* SPR 8174 Changes End */
#endif
} PdcpSecContextInfo, *LP_PdcpSecContextInfo ;

/* SPR 15236 fix start */
/* Per entity structure for ASYNC thread */
typedef struct PdcpSecEntityContextT
{
    PdcpSecContextInfo pdcpSecurityContext ;
} PdcpSecEntityContext, *LP_PdcpSecEntityContext;

/* UEContext structure for ASYNC thread */
typedef struct PdcpSecUeContextT
{
    PdcpSecEntityContext secEntity[LTE_MAX_LC_ID][SEC_RB_DIRECTION];
} PdcpSecUeContext, *LP_PdcpSecUeContext;

/* Global Sec UE Context */
extern PdcpSecUeContext   secUeCtx[MAX_UE_SUPPORTED];
/* SPR 15236 fix end */

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define INTERRUPT_MODE 0
#define POLLING_MODE   1

#define NUM_PACKETS_RETRIEVED 50
#define RETRIEVE_ALL_PACKETS  -1
/* SPR 15236 fix start */
#define PDCP_SEC_CONTEXT( ueId, lcId, dir ) \
    &(secUeCtx[ueId].secEntity[lcId][dir].pdcpSecurityContext)
/* SPR 15236 fix end */
/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpSecInit
 * Inputs         : workMode  - mechanism of how PDCP will receive the response
 *                              from the security engine. 
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used for initializing the security engine. 
 ****************************************************************************/
UInt32 pdcpSecInit( UInt8 workMode); 

/****************************************************************************
 * Function Name  : pdcpCreateSecurityContext
 * Inputs         : secContext_p - Pointer to security context
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used for updating the security engine. 
 ****************************************************************************/
UInt32 pdcpCreateSecurityContext( 
        UInt8 *jobRingHandler_p,
        LP_PdcpSecContextInfo secContext_p); 

/****************************************************************************
 * Function Name  : pdcpDeleteSecurityContext
 * Inputs         : secContext_p - Pointer to security context
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used for deleting the security engine. 
 ****************************************************************************/
UInt32 pdcpDeleteSecurityContext( LP_PdcpSecContextInfo secContext_p); 

/****************************************************************************
 * Function Name  : pdcpSecurityProcessPacket
 * Inputs         : secContext_p - Pointer to security context
 *                  inData_p - input data
 *                  outData_p - output data
 *                  uaCtxHandle - Pointer to meta-data.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used for sending packets to the security
 *                  engine. 
 ****************************************************************************/
UInt32 pdcpSecurityProcessPacket(
        LP_PdcpSecContextInfo secContext_p,
        void *inData_p, void *outData_p,
        uaContextHandler uaCtxHandle_p ); 

#ifdef POLL_JOB_RING
/****************************************************************************
 * Function Name  : pdcpSecPollJobRing
 * Inputs         : jobRingHandler_p - Pointer to output queue
 *                  limit - number of packets to be retrieved
 *                  packetsNo - Number of packets submitted
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used for polling the security engine for
 *                  a job ring. 
 ****************************************************************************/
UInt32 pdcpSecPollJobRing( 
        UInt8 *jobRingHandler_p,
        UInt32 limit,
        UInt32 *packetsNo);
#else
/****************************************************************************
 * Function Name  : pdcpSecPoll
 * Inputs         : limit - number of packets to be retrieved
 *                  weight - weight for each job ring
 *                  packetsNo - Number of packets submitted
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used for polling the security engine for
 *                  a job ring. 
 ****************************************************************************/
UInt32 pdcpSecPoll( 
    Int limit,
    UInt32 weight,
    UInt32 *packetsNo);
#endif

/****************************************************************************
 * Function Name  : pdcpProcessTxDrbPktsFrmSec
 * Inputs         : inData_p  - pointer to input data.
 *                  outData_p  - pointer to output data.
 *                  uaCtxHandle - Pointer to meta-data.
 *                  status - status of the packet after processing by the 
 *                           security engine.
 *                  errInfo - error indications if any.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used by PDCP to process TX packets that 
 *                  are received from security engine.
 ****************************************************************************/
Int pdcpProcessTxDrbPktsFrmSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo);

/****************************************************************************
 * Function Name  : pdcpProcessTxSrbPktsFrmSec
 * Inputs         : inData_p  - pointer to input data.
 *                  outData_p  - pointer to output data.
 *                  uaCtxHandle - Pointer to meta-data.
 *                  status - status of the packet after processing by the 
 *                           security engine.
 *                  errInfo - error indications if any.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used by PDCP to process TX packets that 
 *                  are received from security engine.
 ****************************************************************************/
Int pdcpProcessTxSrbPktsFrmSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo);
/****************************************************************************
 * Function Name  : pdcpProcessRxSrbPktsFrmSec
 * Inputs         : inData_p  - pointer to input data.
 *                  outData_p  - pointer to output data.
 *                  uaCtxHandle - Pointer to meta-data.
 *                  status - status of the packet after processing by the 
 *                           security engine.
 *                  errInfo - error indications if any.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used by PDCP to process RX SRB packets 
 *                  that are received from security engine.
 ****************************************************************************/
Int pdcpProcessRxSrbPktsFrmSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo);

/****************************************************************************
 * Function Name  : pdcpProcessRxDrbPktsFrmSec
 * Inputs         : inData_p  - pointer to input data.
 *                  outData_p  - pointer to output data.
 *                  uaCtxHandle - Pointer to meta-data.
 *                  status - status of the packet after processing by the 
 *                           security engine.
 *                  errInfo - error indications if any.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used by PDCP to process RX DRB packets 
 *                  that are received from security engine.
 ****************************************************************************/
Int pdcpProcessRxDrbPktsFrmSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo);
/* SPR 15236 fix start */
/* Following function frees the memory of the keys in the context. */
void freeAllMemory( UInt16 ueId, UInt8 lcId, UInt8 direction );
/* SPR 21800 Fix + */
/*SPR 16529 fix start*/
/* + Coverity 65114, 65115 */
/* Following API will be used by PDCP to free the TX data from SEC
 * when context is not located by ASYNC thread. */
Int pdcpFreeTxDataFromSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo );
/* Following API will be used by PDCP to free the RX data from SEC
 * when context is not located by ASYNC thread. */
Int pdcpFreeRxDataFromSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo );
/* - Coverity 65114, 65115 */
/*SPR 16529 fix end*/
/* SPR 15236 fix end */
/* SPR 21800 Fix - */
#endif /* PDCP_ASYNC_INTERFACE */
#endif /* _LIB_I_ENC_SAP_DEF_ */
