
/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: lteMacPorting.c
 *
 *************************************************************************
 *
 * File Description : This file contains the functions that are used for
 *                    Mac porting
 *************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/

#include "lteMacPorting.h"
#include "lteMacComPorting.h"
#include "lteMacPRACH.h"
#include "lteMacReceiver.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacPHICH.h"
#ifdef DL_UL_SPLIT_TDD
#include "lteMacTDDConfig.h"
#endif

/******************************************************************************
 Private Definitions
 *****************************************************************************/



/******************************************************************************
 Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Global variables 
 ****************************************************************************/
#ifndef DL_UL_SPLIT
/*CA Changes start  */
extern LTE_SQUEUE* ackNackQueue_gp[MAX_NUM_CELL];
extern MAC_UE_OPPOR_IND_CIRC_TYPE* macUeOpporIndCircQ_gp[MAX_NUM_CELL];
/*CA Changes end  */
/*This is List which is populated by UL Stategy for those UE for which PDCCH
 * region need to be allocated so the Uplink Grant Message can be sent */
/*CA Changes start  */
extern  LTE_SQUEUE* ulStrategyTxQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */

#endif

#if DL_UL_SPLIT_TDD
LTE_CIRCQUEUE pdcchSignalCircQueue_g;
SEM_T waitForPDCCHSignalInd_g;
#endif

/*CA Changes start  */
extern  UL_STRATEGY_TX_CIRC_TYPE* ulStrategyTxCircQueue_gp[MAX_NUM_CELL];
SEM_T waitForULStrategyTxInd_g[MAX_NUM_CELL];
/*CA Changes end  */

/*rel4-drop2 changes*/
/*16855 +-*/
#ifdef CIRC_QUEUE_IMPL
/*CA Changes start  */
extern ACK_NACK_CIRC_TYPE* ackNackCircQueue_gp[MAX_NUM_CELL];
SEM_T waitForDLHarqAckNack_g[MAX_NUM_CELL];

extern PUSCH_SIGNAL_CIRC_TYPE* puschSignalCircQueue_gp[MAX_NUM_CELL];
SEM_T waitForPUSCHSignalInd_g[MAX_NUM_CELL];
/*CA Changes end  */

/* + SPR 11209 Changes */
extern EncoderContainerNode *encoderContainerArr_gp[MAX_NUM_CELL][MAX_NUM_CELL];
/* - SPR 11209 Changes */
SEM_T waitForEncoderQueueInd_g[MAX_NUM_CELL];

/*CA Changes start  */
extern UplinkContainer* ulContainerPool_gp[MAX_NUM_CELL];
SEM_T waitForULNackQueue_g[MAX_NUM_CELL];
SEM_T waitForULNackForTempUE_g[MAX_NUM_CELL];

#endif
extern MAC_UE_OPPOR_IND_CIRC_TYPE* macUeOpporIndCircQ_gp[MAX_NUM_CELL];
SEM_T waitForMacUeOpporInd_g[MAX_NUM_CELL];
/*CA Changes end  */
/*This is List in which a delimiter node is put when CCEs are allocated
  so that the UL thread unblocks and start processing of PUSCH */
/*CA Changes start  */
SInt32 puschSignalQueueIdDL_g[MAX_NUM_CELL];
SInt32 puschSignalQueueIdUL_g[MAX_NUM_CELL];
SInt32 ulStrategyTxQueueIdDL_g[MAX_NUM_CELL];
SInt32 ulStrategyTxQueueIdUL_g[MAX_NUM_CELL];
SInt32 ackNackQueueIdDL_g[MAX_NUM_CELL];
SInt32 ackNackQueueIdUL_g[MAX_NUM_CELL];
SInt32 macUeOppnIndQIdUL_g[MAX_NUM_CELL];
SInt32 macUeOppnIndQIdDL_g[MAX_NUM_CELL];
/*CA Changes end  */

/*16855 +-*/

#ifdef DL_DATA_SEPARATION
LTE_SEM waitForPdschInd_g[MAX_NUM_CELL];
#endif
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
SInt32 rlcRrcCnfOutputQId_g;
SInt32 rlcRrcCnfInputQId_g;
#endif

/* SPR 5531 changes start */
extern PENDING_DL_MEMORY_POOL_FREE_TYPE *pendingDLMemoryPoolFreeQ_gp[MAX_NUM_CELL];
/* SPR 5531 changes end */

extern SInt32 initMsgQueueForICC(const char *name, SInt32 oflag);
extern SInt32 closeIccQ(SInt32 mqdes);
extern SInt32 sendMsgWithIccQ(SInt32 mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);
/*****************************************************************************
 * Function Name  : initDLAckNackQueue
 * Inputs         : qId
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the Global queues and variables for DL ACK 
 *                  NACK queue.
 *****************************************************************************/

/*CA Changes start  */
/* +- SPR 17777 */
void initDLAckNackQueue(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
/*CA Changes start  */
{
#ifdef FDD_CONFIG

#ifdef DL_UL_SPLIT
#ifdef CIRC_QUEUE_IMPL
    /* CA Changes start */
    CONSTRUCT_AND_INIT_ACK_NACK_CIRC_Q(internalCellIndex);
    semInitDefault(&waitForDLHarqAckNack_g[internalCellIndex], 0);
#else
    const char qName[] = "/AckNackQueue";
    char name[33] = {0};
    sprintf(name, "%s%d",qName , internalCellIndex);
    *qId = initMsgQueueForICC(name, O_RDWR|O_CREAT);
    if (-1 == *qId)
    {
        ltePanic(" Failure in opening ACK-NACK-MESSAGE-QUEUE, Error no : %d\n",errno );
    }
#endif    
#else
    sQueueInit(ackNackQueue_gp[internalCellIndex]);
    /* CA Changes end */
#endif

#elif TDD_CONFIG
    /** CA-TDD Changes Start:06 **/
    sQueueInit(&tddAckNackQueue_g[internalCellIndex]);
    /** CA-TDD Changes End:06 **/
#endif
/* SPR 5531 changes start */
    INIT_PENDING_DL_MEMORY_POOL_FREE_Q(internalCellIndex);
/* SPR 5531 changes end */
}

/*CA Changes start  */
/*****************************************************************************
 * Function Name  : initMACUEOppertunityIndQ
 * Inputs         : qId
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the Global queues and variables for MAC and
 *                  UE.
 *****************************************************************************/
/* +- SPR 17777 */
void initMACUEOppertunityIndQ(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
{
#ifndef DL_UL_SPLIT
    CONSTRUCT_AND_INIT_MAC_UE_OPPOR_IND_CIRC_Q(internalCellIndex);
#else
#ifdef CIRC_QUEUE_IMPL
    CONSTRUCT_AND_INIT_MAC_UE_OPPOR_IND_CIRC_Q(internalCellIndex);
    semInitDefault(&waitForMacUeOpporInd_g[internalCellIndex], 0);
    /*CA Changes start  */
#else
    const char qName[] = "/MACUeOpporIndQ";
    char name[33] = {0};
    sprintf(name, "%s%d",qName , internalCellIndex);
    *qId = initMsgQueueForICC(name, O_RDWR|O_CREAT);

    if (-1 == *qId)
    {
        pError ("Msq queue creation failed \n");
        ltePanic(" Failure in opening MACUeOpporIndQ, Error no : %d\n",errno );

    }
/*CA Changes end  */
#endif
#endif
}

/*****************************************************************************
 * Function Name  : initUlStrategyQueue
 * Inputs         : qId
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the Global queues and variables for UL strategy
 *                  queue.
 *****************************************************************************/
/*CA Changes start  */
/* +- SPR 17777 */
void initUlStrategyQueue( 
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
{
#ifndef DL_UL_SPLIT
    CONSTRUCT_AND_INIT_UL_STRATEGY_TX_CIRC_Q(internalCellIndex);
#else
#ifdef CIRC_QUEUE_IMPL
    CONSTRUCT_AND_INIT_UL_STRATEGY_TX_CIRC_Q(internalCellIndex);
    semInitDefault(&waitForULStrategyTxInd_g[internalCellIndex], 0);
#else
    const char qName[] = "/ULStrategyTxQueue";
    char name[33] = {0};
    sprintf(name, "%s%d",qName , internalCellIndex);
    *qId = initMsgQueueForICC(name, O_RDWR|O_CREAT);
    /*CA Changes end  */
    if (-1 == *qId)
    {
        pError ("Msq queue creation failed \n");
        ltePanic(" Failure in opening ulStrategyTxQueueId_g, Error no : %d\n",errno );
    }
#endif
#endif
}

#ifndef DL_UL_SPLIT_TDD
/*****************************************************************************
 * Function Name  : initPUSCHSignalQueue
 * Inputs         : qId
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the Global queues and variables for PUSCH Signal 
 *                  queue.
 *****************************************************************************/
/*CA Changes start  */
/* +- SPR 17777 */
void initPUSCHSignalQueue(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
{
#ifdef DL_UL_SPLIT
#ifdef CIRC_QUEUE_IMPL
    CONSTRUCT_AND_INIT_PUSCH_SIGNAL_CIRC_Q(internalCellIndex);
    semInitDefault(&waitForPUSCHSignalInd_g[internalCellIndex], 0);
#else
    const char qName[] = "/PUSCHSignalQ";
    char name[33] = {0};
    sprintf(name, "%s%d",qName , internalCellIndex);
    *qId = initMsgQueueForICC(name, O_RDWR|O_CREAT);
    if (-1 == *qId)
    {
        pError ("Msq queue creation failed \n");
        ltePanic(" Failure in opening ulStrategyTxQueueId_g, Error no : %d\n",errno );
    }
/*CA Changes end  */
    createMemPool(sizeof(PuschSignalNodeP), 10);
#endif
/* +- SPR 17777 */
#else
  LTE_GCC_UNUSED_PARAM(internalCellIndex)
/* +- SPR 17777 */
#endif
}
#else
/*****************************************************************************
 * Function Name  : initPDCCHSignalQueue
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the Global queues and variables for PDCCH Signal 
 *                  queue
 *****************************************************************************/
void initPDCCHSignalQueue()
{
    circQInit(&pdcchSignalCircQueue_g, MAX_NODES_SPLIT);
    semInitDefault(&waitForPDCCHSignalInd_g, 0);
}

#endif

#ifdef DL_UL_SPLIT
/*****************************************************************************
 * Function Name  : putEntryInAckNackQueueUsingICC
 * Inputs         : ueIndex - Index of UE,
 *                  operation - ACK or NACK,
 *                  recvTTI,
 *                  schUsed - Shared Channel Used
 * Outputs        : Puts the AckNackNode in ackNackQueue.
 * Returns        : None
 * Description    : This function creates a new ackNackNode and inserts this
 *                  node in the ackNackQueue to be processed by worker thread.
 *****************************************************************************/
void putEntryInAckNackQueueUsingICC( UInt32 ueIndex,
            /* CA_phase2_harq_code Start */
        UInt16 operation,
            /* CA_phase2_harq_code End */
       /* SPR 15909 fix start */
        tickType_t recvTTI
       /* SPR 15909 fix end */
        /*SPR 6169 Fix Start */
        , UInt32 schUsed
        /*SPR 6169 Fix End */
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        )
{
    AckNackQueueNode *ackNackEntry_p = PNULL;
    GET_MEM_NODE_ACK_NACK_CIRC_Q(ackNackEntry_p,AckNackQueueNode);
    /* Coverity Warning : 26410 */
    if (PNULL == ackNackEntry_p)
    {
        LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
        return ; 
    }
    /* Coverity Warning : 26410 */
    ackNackEntry_p->operationType = operation;
    ackNackEntry_p->ueIndex = ueIndex;
    ackNackEntry_p->packetSubFrame = recvTTI;
    /*SPR 6169 Fix Start */
    ackNackEntry_p->schUsed = schUsed;
    /*SPR 6169 Fix End */
#ifdef CIRC_QUEUE_IMPL
    /*CA Changes start  */
    ENQUEUE_ACK_NACK_CIRC_Q(AckNackQueueNode,&(ackNackEntry_p->ackNackAnchor), internalCellIndex);
    semPostDefault(&waitForDLHarqAckNack_g[internalCellIndex]);
#else
    sendMsgWithIccQ(ackNackQueueIdUL_g[internalCellIndex], (const char *)&ackNackEntry_p, sizeof(ackNackEntry_p), 1); 
    /*CA Changes end  */
#endif

    return;
}


/*****************************************************************************
 * Function Name  : putEntryInULSchedulerNewTxQueueUsingICC
 * Inputs         : ulStrategyTxNode_p - This is pointer to ULStrategyTxNode
 *                                       structure
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This funtion will push the ulStrategyTxNode_p Node in the
 *                  ulStrategyTxQueue_g Queue.
 *****************************************************************************/
/*CA Changes start  */
 MacRetType putEntryInULSchedulerNewTxQueueUsingICC(ULStrategyTxNode *ulStrategyTxNode_p, InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    /* + coverity 32791, 32792 */
    MacRetType retval = MAC_SUCCESS;
#ifndef CIRC_QUEUE_IMPL
    SInt32     ret    = 0;
#endif
 
#ifdef CIRC_QUEUE_IMPL
    /*CA Changes start  */
    if (!ENQUEUE_UL_STRATEGY_TX_CIRC_Q(ULStrategyTxNode,(void *)&(ulStrategyTxNode_p->ulStrategyTxNodeAnchor), internalCellIndex))
    {
        retval = MAC_FAILURE;
    }
    semPostDefault(&waitForULStrategyTxInd_g[internalCellIndex]);
#else
    ret = sendMsgWithIccQ(ulStrategyTxQueueIdUL_g[internalCellIndex], (const char *)&ulStrategyTxNode_p, sizeof(ulStrategyTxNode_p), 1); 
    /*CA Changes end  */
   
    if (ret == -1)
    {
        retval = MAC_FAILURE;
    }
#endif
    return retval;
    /* - coverity 32791, 32792 */
}

/*****************************************************************************
 * Function Name  : putEntryInPuschSignalQueueUsingICC
 * Inputs         : isCCEAllocated 
 * Outputs        : Puts the node in puschSignalQueue_g.
 * Returns        : None
 * Description    : This function creates a new PuschSignalNodeP and inserts this
 *                  node in the puschSignalQueue_g to be processed by worker thread.
 *****************************************************************************/
/*CA Changes start  */
void putEntryInPuschSignalQueueUsingICC(SInt8 isCCEAllocated, InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    PuschSignalNodeP *node_p = PNULL ; 
    GET_MEM_NODE_PUSCH_SIGNAL_CIRC_Q(node_p, PuschSignalNodeP);
    /* Coverity Warning : 26411 */
    if (PNULL == node_p)
    {
        LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
        return ;
    }
    /* Coverity Warning : 26411 */
    node_p->isCCEAllocated = isCCEAllocated;
    // Put the node into ICC queue so that PUSCH can unblock
#ifdef CIRC_QUEUE_IMPL
    /*CA Changes start  */
    ENQUEUE_PUSCH_SIGNAL_CIRC_Q(PuschSignalNodeP,(void *)(node_p), internalCellIndex);
    semPostDefault(&waitForPUSCHSignalInd_g[internalCellIndex]);
    /*CA Changes end  */
#else
    /*CA Changes start  */
    sendMsgWithIccQ(puschSignalQueueIdDL_g[internalCellIndex], (const char *)&node_p, sizeof(node_p), 1);
    /*CA Changes end  */
#endif
}


/*****************************************************************************
 * Function Name  : putEntryInMACUeOppIndQ
 * Inputs         : ueOppInd_p
 *                  size
 * Outputs        : None
 * Returns        : None
 * Description    : Puts the Entry in MAC Queue.
 *****************************************************************************/
void putEntryInMACUeOppIndQ(const char* ueOppInd_p, int size,InternalCellIndex internalCellIndex)
{
    (void)size; /*SPR 18940+-*/
    sendMsgWithIccQ(macUeOppnIndQIdDL_g[internalCellIndex], (const char *)&ueOppInd_p, sizeof(ueOppInd_p), 1);
}

/*****************************************************************************
 * Function Name  : putEntryInULContainerNackQueueUsingICC
 * Inputs         : ulContainerNackQIdUL,
 *                  nackNode_p
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Puts the entry in Uplink Container of NACK Queue using ICC.
 *****************************************************************************/
 MacRetType putEntryInULContainerNackQueueUsingICC( SInt32 ulContainerNackQIdUL , ContainerNackQueueNode *nackNode_p)
{
    if (nackNode_p->ueIdx == INVALID_UE_INDEX)
    {
    }
    else
    {
    }
    sendMsgWithIccQ(ulContainerNackQIdUL, (const char *)&nackNode_p, sizeof(nackNode_p), 1);
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : putEntryInEncoderQueueUsingICC
 * Inputs         : encoderQueueIdUL,
 *                  encoderQueueNode_p
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Puts the entry in Encoder Queue using ICC.
 *****************************************************************************/
 MacRetType putEntryInEncoderQueueUsingICC(SInt32 encoderQueueIdUL, EncoderQueueNode *encoderQueueNode_p)
{
    if (encoderQueueNode_p->dummyNode == 1 )
    {
    }
    sendMsgWithIccQ(encoderQueueIdUL, (const char *)&encoderQueueNode_p, sizeof(encoderQueueNode_p), 1);
    return MAC_SUCCESS;
}

#endif 
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
/*****************************************************************************
 * Function Name  : initRlcRrcCnfQueue
 * Inputs         : qId
 * Outputs        : None
 * Returns        : None
 * Description    : This queue is used for CNF from RLC DL/UL threads for 
 *                  delete/reconfig/reEsablish UE req recived from RRC.
 *****************************************************************************/
void initRlcRrcCnfQueue ( SInt32 *qId)
{
    *qId = initMsgQueueForICC("/RlcRrcCnfQueue", O_RDWR|O_CREAT);
    if (-1 == *qId)
    {
        pError ("Msq queue creation failed \n");
        ltePanic(" Failure in opening RlcRrcCnfQueue, Error no : %d\n",errno );

    }
}
#endif

#ifdef DL_UL_SPLIT
/*****************************************************************************
 * Function Name  : initULContainerNackQueue
 * Inputs         : qId,
 *                  index
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the UL container NACK queue.
 *****************************************************************************/
/*CA Changes start  */
void initULContainerNackQueue ( SInt32 *qId, InternalCellIndex internalCellIndex, UInt8 containerIndex)
{
#ifdef CIRC_QUEUE_IMPL
    /* Coverity Warning : 60784 */
    /* Coverity Warning : 60784 */
    (void)(*qId);/*SPR 18490 +-*/
    INIT_NACK_CIRC_Q(&((ulContainerPool_gp[internalCellIndex] + containerIndex)->nackCircQueue));
    semInitDefault(&waitForULNackQueue_g[internalCellIndex], 0);
#else
    initComULContainerNackQueue (qId,((internalCellIndex * MAX_NUMBER_OF_CONTAINER) + containerIndex));
    /*CA Changes end  */
#endif
}

/*****************************************************************************
 * Function Name  : initULContainerNackQueueForTempUE
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the UL container NACK queue for Temp UE.
 *****************************************************************************/
/*CA Changes start  */
void initULContainerNackQueueForTempUE( SInt32 *qId, InternalCellIndex internalCellIndex, UInt8 containerIndex)
{
#ifdef CIRC_QUEUE_IMPL
    /* Coverity Warning : 60785 */
    /* Coverity Warning : 60785 */
    (void)(*qId);/*SPR 18490 +-*/
    INIT_NACK_CIRC_Q(&((ulContainerPool_gp[internalCellIndex] + containerIndex)->nackCircQueueForTempUE));
    semInitDefault(&waitForULNackForTempUE_g[internalCellIndex], 0);
#else
    initComULContainerNackQueueForTempUE(qId,((internalCellIndex * MAX_NUMBER_OF_CONTAINER) + containerIndex));
    /*CA Changes end  */
#endif
}

/*****************************************************************************
 * Function Name  : initEncoderQueue
 * Inputs         : qId,
 *                  encoderContainerCount,
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the encoder queue.
 *****************************************************************************/
/* CA Changes start */
/* + SPR 11209 Changes */
void initEncoderQueue ( SInt32 *qId, UInt8 encoderContainerCount, 
        InternalCellIndex internalCellIndex, InternalCellIndex internalCellIndex2)
/* - SPR 11209 Changes */
/* CA Changes end */
{
/* SPR 8712 changes */
#ifndef DL_DATA_SEPARATION 
#ifdef CIRC_QUEUE_IMPL
/* + SPR 11209 Changes */
    (void)(*qId);/*SPR 18490 +-*/
    INIT_ENCODER_CIRC_Q(&(encoderContainerArr_gp[internalCellIndex][internalCellIndex2] + \
                encoderContainerCount)->encoderNode.encoderCircQueue);
/* - SPR 11209 Changes */
    semInitDefault(&waitForEncoderQueueInd_g[internalCellIndex], 0);
#else
    (void)internalCellIndex2;/*SPR 18490 +-*/
    initComEncoderQueue(qId, encoderContainerCount, internalCellIndex);
#endif
#endif
/* SPR 8712 changes */
}
#endif

#ifdef DL_UL_SPLIT_TDD
void putEntryInUlPdcchSignalQueue(UInt32 availableCCE)
{
    PdcchSignalNodeP *node_p = PNULL ; 
    GET_MEM_FROM_POOL(PdcchSignalNodeP, node_p, sizeof(PdcchSignalNodeP), PNULL);
    /* TDD-SPLIT-COV 59913 */
    if(node_p)
    /* TDD-SPLIT-COV 59913 */
    {
        node_p->availableCCE = availableCCE;
        pushNodeCircQ(&pdcchSignalCircQueue_g,(void *)(node_p));
        semPostDefault(&waitForPDCCHSignalInd_g);
    }
}

#endif

/*Coverity-5.3.0 CID-54651*/
/*****************************************************************************
 * Function Name  : cleanupUlStrategyQueue
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Cleans up the UL strategy queue.
 ****************************i*************************************************/
/* CA Changes start */
void cleanupUlStrategyQueue(InternalCellIndex internalCellIndex)
/* CA Changes end */
{
#if !defined DL_UL_SPLIT || defined CIRC_QUEUE_IMPL
    UInt32 count =0;
    UInt32 i;
    ULStrategyTxNode *ulStrategyTxNode_p = PNULL;

    count = COUNT_UL_STRATEGY_TX_CIRC_Q(internalCellIndex);
    for(i=0;i<count;i++)
    {
        /*++530-coverity-55199*/
        DEQUEUE_UL_STRATEGY_TX_CIRC_Q(ULStrategyTxNode, (void**)&ulStrategyTxNode_p
                                ,internalCellIndex);
        /*++530-coverity-55199*/
        if(ulStrategyTxNode_p)
        {
            FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
        /*++530-coverity-55199*/
            ulStrategyTxNode_p = PNULL;
        /*++530-coverity-55199*/
        }       
    }
    QDEINIT_UL_STRATEGY_TX_CIRC_Q(internalCellIndex);
#else
    closeIccQ(ulStrategyTxQueueIdUL_g[internalCellIndex]);
#endif    
}
/*Coverity-5.3.0 CID-54651*/
