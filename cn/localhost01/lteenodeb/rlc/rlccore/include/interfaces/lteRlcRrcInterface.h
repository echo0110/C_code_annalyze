/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcRrcInterface.h,v 1.1.1.1.6.2.4.2.2.1 2010/09/21 15:49:49 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description :
 *  This file describes APIs for RRC to controls the configuration of RLC entity. 
 *  The presence of configuration parameters depends on the mode of operation 
 *  each RLC entity is configured with.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcRrcInterface.h,v $
 * Revision 1.1.1.1.6.2.4.2.2.1  2010/09/21 15:49:49  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.2.2.1  2010/08/10 12:16:30  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.13.2.5  2010/07/17 12:52:54  gur21148
 * RLC_UM_MERGES
 *
 * Revision 1.13.2.4  2010/07/15 06:31:01  gur23054
 * Review comments fixes
 *
 * Revision 1.13.2.3  2010/06/22 14:00:29  gur19836
 * Feature Enhancement: Re-establish UE Entity Support Added
 *
 * Revision 1.13.2.2  2010/06/22 09:13:50  gur19836
 * Reconfig API Support Feature Enhancement
 *
 * Revision 1.13.2.1  2010/06/17 10:59:46  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.13  2010/06/14 11:59:35  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.12  2009/06/04 05:20:34  gur19836
 * Optimization Changes
 *
 * Revision 1.11  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.10  2009/03/02 10:12:08  gur19836
 * Compilation warnings removed
 *
 * Revision 1.9  2009/03/02 05:34:08  gur19836
 * Updated for OAM Enhancement
 *
 * Revision 1.8  2009/02/11 10:29:26  gur19836
 * UT Defects Fixed
 *
 * Revision 1.7  2009/02/11 07:31:52  gur19836
 * UT Defects Fixed
 *
 * Revision 1.6  2009/02/09 09:19:43  gur19140
 * Update to make rlc init successufully
 *
 * Revision 1.5  2009/02/09 05:00:02  gur20294
 * update
 *
 * Revision 1.4  2009/02/06 15:39:33  gur20294
 * updated
 *
 * Revision 1.3  2009/02/06 15:03:49  gur20294
 * updated
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef RRCINTERFACE_H
#define RRCINTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteTypes.h"
#include "lteRlcTypes.h"
#include "lteRlcRrcExtInterface.h"
#include "lteRlcEntityDef.h"
#include "lteRlcUmRxOperation.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
 /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
/* SPR 3805 Fix Start */
#define UE_DEL_CIC_Q_SIZE 128

#define QINIT_RLC_DEL_UE_ENTITY(rlcUependingDelQ_g)\
            circQInit(&(rlcUependingDelQ_g),UE_DEL_CIC_Q_SIZE)

#define ENQUEUE_DEL_UE_ENTITY_Q(ueEntity_p,rlcUependingDelQ_g)\
        pushNodeCircQ(&(rlcUependingDelQ_g),ueEntity_p)  
 
#define DEQUEUE_DEL_UE_ENTITY_Q(ueEntity_p,rlcUependingDelQ_g);\
        popNodeCircQ(&(rlcUependingDelQ_g),(void *)&(ueEntity_p));\
        ueEntity_p = (RlcUeDelQNode *)ueEntity_p;

#define EMPTY_DEL_UE_ENTITY_Q(ueEntity_p,rlcUependingDelQ_g)\
        ueEntity_p = RLC_NULL;\
        DEQUEUE_DEL_UE_ENTITY_Q(ueEntity_p,rlcUependingDelQ_g);\
        while(ueEntity_p != RLC_NULL ){\
        freeMemPool(ueEntity_p);\
        ueEntity_p = RLC_NULL;\
        DEQUEUE_DEL_UE_ENTITY_Q(ueEntity_p,rlcUependingDelQ_g);\
        }

#define QDEINIT_RLC_DEL_UE_ENTITY(ueEntity_p,rlcUependingDelQ_g)\
        EMPTY_DEL_UE_ENTITY_Q(ueEntity_p,rlcUependingDelQ_g)\
        circQDeInit(&(rlcUependingDelQ_g));

#define GET_UE_DEL_ENTITY_Q_COUNT(rlcUependingDelQ_g)\
        circQueueCount(&(rlcUependingDelQ_g))

#define CREATE_RLC_UE_DEL_Q_NODE(rlcDelQNode_p){\
        if((rlcDelQNode_p = \
            (RlcUeDelQNode *)getMemFromPool(sizeof(RlcUeDelQNode),RLC_NULL))\
            == RLC_NULL ){\
            ltePanic("Memory Allocation Failure");\
            }\
        }

/* SPR 3805 Fix End */

#define QINIT_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p)\
    sQueueInit(rlcDelReEstabQ_p)

#define QDEINIT_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p){\
    LTE_SNODE * sN = RLC_NULL;\
    while((sN = popNode(rlcDelReEstabQ_p)) != RLC_NULL) \
        freeMemPool(MEMBEROF(RlcDelReEstabQNode,sNode,sN));\
}                

#define QCOUNT_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p)\
    sQueueCount(rlcDelReEstabQ_p)

#define DEQUEUE_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p, rlcDelReEstabQNode_p){ \
    LTE_SNODE *sN = popNode(rlcDelReEstabQ_p);\
    rlcDelReEstabQNode_p = MEMBEROF(RlcDelReEstabQNode,sNode,sN);\
}    

#define ENQUEUE_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p,rlcDelReEstabQNode_p) \
    pushNode(rlcDelReEstabQ_p,&(rlcDelReEstabQNode_p->sNode))

#else 

#define QINIT_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p)\
    /* + SPR 9253 */\
    circQInit(rlcDelReEstabQ_p, MAX_UE_SUPPORTED)\
    /* - SPR 9253 */

#define QDEINIT_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p){\
    RlcDelReEstabQNode *node_p;\
    DEQUEUE_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p, node_p);\
    while(node_p != RLC_NULL){\
        freeMemPool(node_p);\
        node_p = RLC_NULL;\
        DEQUEUE_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p, node_p);\
    }\
    circQDeInit(rlcDelReEstabQ_p);\
    rlcDelReEstabQ_p = RLC_NULL;\
}            

#define QCOUNT_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p)\
    circQueueCount(rlcDelReEstabQ_p)

#define DEQUEUE_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p, rlcDelReEstabQNode_p){ \
    popNodeCircQ(rlcDelReEstabQ_p, (void *)(&(rlcDelReEstabQNode_p)));\
    rlcDelReEstabQNode_p = (RlcDelReEstabQNode *)rlcDelReEstabQNode_p;\
}

#define ENQUEUE_RLC_DEL_RE_ESTABQ(rlcDelReEstabQ_p,rlcDelReEstabQNode_p) \
    pushNodeCircQ(rlcDelReEstabQ_p, rlcDelReEstabQNode_p)

/* CirQ introduced for Reconfig Tx & Rx RLC Entities from DL/UL RLC threads */
#define QINIT_RLC_RECONFIG_ENTITY_Q(rlcReconfigQ_p)\
    /* + SPR 9253 */ \
    circQInit(rlcReconfigQ_p, MAX_UE_SUPPORTED)\
    /* - SPR 9253 */

 #define QCOUNT_RLC_RECONFIG_ENTITY_Q(rlcReconfigQ_p)\
    circQueueCount(rlcReconfigQ_p)

#define ENQUEUE_RLC_RECONFIG_ENTITY_Q(rlcReconfigQ_p, rlcReconfigQNode_p)\
    pushNodeCircQ(rlcReconfigQ_p, rlcReconfigQNode_p)

#define DEQUEUE_RLC_RECONFIG_ENTITY_Q(rlcReconfigQ_p, rlcReconfigQNode_p)\
    popNodeCircQ(rlcReconfigQ_p, (void *)(&(rlcReconfigQNode_p)))
    
#endif


#define CREATE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p){ \
    if((rlcDelReEstabQNode_p = \
    (RlcDelReEstabQNode *)getMemFromPool(sizeof(RlcDelReEstabQNode),RLC_NULL))\
            == RLC_NULL ){\
        ltePanic("Memory Allocation Failure");\
    }\
}
 
#define FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p) \
    freeMemPool(rlcDelReEstabQNode_p)

/* + Layer2 NON CA Changes */

#ifndef UE_SIM_TESTING

#define RLC_API_HEADER_LEN INTERFACE_API_HEADER_LEN
#define RRC_RLC_API_HEADER_CELL_POS API_HEADER_CELL_INDEX_OFFSET 

#else

#define RLC_API_HEADER_LEN  10

#endif

#define OAM_RLC_API_HEADER_LEN  RLC_API_HEADER_LEN
#define RRC_RLC_API_HEADER_LEN  RLC_API_HEADER_LEN

/* SPR 5178 changes starts here */
/* SPR 5178 changes ends here */

/* - Layer2 NON CA Changes */
#define CLEANUP_LAYER2          0x2FFF

/* SPR 21778 Fix */
/* The macro has been used to delay the process or reestablishment to avoid
 * Race conditons of DL and UL */
#define REESTABLISH_PROCESS_DELAY 4
/* SPR 21778 Fix */
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
 /* SPR 3805 Fix Start */
extern LTE_CIRCQUEUE rlcRxUePendingDelQ_g;
extern LTE_CIRCQUEUE rlcTxUePendingDelQ_g;
/* SPR 3805 Fix End */
 /* this queue will hold the deletion request for rx ue entity. */
extern LTE_SQUEUE rlcRxPendingDelQ_g;
 /* this queue will hold the deletion req for tx ue entity. */
extern LTE_SQUEUE rlcTxPendingDelQ_g;
 /* this queue will hold the reEstablish req for rx ue entity. */
extern LTE_SQUEUE rlcRxReEstabQ_g;
 /* this queue will hold the reEstablish req for tx ue entity. */
extern LTE_SQUEUE rlcTxReEstabQ_g;
#else
 /* this queue will hold the deletion request for rx ue entity. */
extern LTE_CIRCQUEUE rlcRxPendingDelQ_g;
 /* this queue will hold the deletion req for tx ue entity. */
extern LTE_CIRCQUEUE rlcTxPendingDelQ_g[MAX_NUM_CELL];
 /* this queue will hold the reEstablish req for rx ue entity. */
extern LTE_CIRCQUEUE rlcRxReEstabQ_g;
 /* this queue will hold the reEstablish req for tx ue entity. */
extern LTE_CIRCQUEUE rlcTxReEstabQ_g[MAX_NUM_CELL];
#endif

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef enum RlcNodeTypeT{
    RLC_UE_CONTEXT = 0,
    RLC_UE_ENTITY,
    RLC_COM_CH_ENTITY
} RlcNodeType ;

/* SPR 4994 Changes Start */
typedef enum RlcStateT{
    RLC_NO_ACTION = 0,
    RLC_FREEZE 
}RlcState;
/* SPR 4994 Changes End */

typedef struct RlcUeDelQNodeT{
    UInt16 ueIndex;
    UInt16 transactionId;
    RlcNodeType rlcNodeType;
}RlcUeDelQNode;

typedef struct RlcDelReEstabQNodeT{
 /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    LTE_SNODE sNode;
#else
    LTE_CNODE sNode;
#endif
    UInt16 ueIndex;
    UInt16 lcId;
    UInt8 comChType;
    /* + Layer2 NON CA Changes */
    RrcCellIndex cellId;
    /* - Layer2 NON CA Changes */
    UInt16 transactionId;
    RlcNodeType rlcNodeType;
    RlcEntityType entityType;
    RlcEntityType cnf;
    /* SPR 4994 Change Start */
    RlcEntityState   rlcState;
    /* SPR 4994 Change End */
    /* SPR 21778 Fix Start*/
    tickType_t reEstablishProcessTick;
    /* SPR 21778 Fix End*/

} RlcDelReEstabQNode ;

typedef struct RlcDeleteReqT {
    UInt16  ueId ;      /* user equipment Id.  */
    UInt16  ueIndex ;   /* user equipment index.  */
} RlcDeleteReq ;

typedef struct RlcDeleteCnfT {
    UInt16  ueId ;      /* user equipment Id.  */
    UInt16  ueIndex ;   /* user equipment index.  */
    UInt8   result ;    /* Result of the operation */
} RlcDeleteCnf ;

typedef struct TxAmRlcT {
    TPollRetransmit tPollRetransmit ;
    PollPDU pollPDU ;
    /*--coverity--530--54183*/
    UInt32 pollByte ;
    /*--coverity--530--54183*/
    MaxRetxThreshold maxRetxThreshold ;
    /** Rel 4.0 changes Start **/
    /* SPR# 3372 Changes Starts */
    UInt32 bitRateTx;
    UInt32 rlcQueueSize;
    /* SPR# 3372 Changes Ends */
    /** Rel 4.0 changes End **/
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESING)
    /*Indication of SCG or SCG_SPLIT bearer*/
    DCBearerType dcBearerType;
#endif

} TxAmRlc;

typedef struct RxAmRlcT {
    TReordering tReordering ;
    TStatusProhibit tStatusProhibit ;
    /** Rel 4.0 changes Start **/
    /* SPR# 3372 Changes Starts */
    UInt32 bitRateRx;
    /* SPR# 3372 Changes Ends */
    /** Rel 4.0 changes End **/
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESING)
    /*Indication of SCG or SCG_SPLIT bearer*/
    DCBearerType dcBearerType;
#endif

} RxAmRlc ;

typedef struct TxUmRlcT {
    SnFieldLength snFieldLength ;
    /** Rel 4.0 changes Start **/
    /* SPR# 3372 Changes Starts */
    UInt32 bitRateTx;
    UInt32 rlcQueueSize;
    /* SPR# 3372 Changes Ends */
    /** Rel 4.0 changes End **/
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESING)
    /*Indication of SCG or SCG_SPLIT bearer*/
    DCBearerType dcBearerType;
#endif

} TxUmRlc ;

typedef struct RxUmRlcT{
    SnFieldLength snFieldLength ;
    TReordering tReordering ;
    /** Rel 4.0 changes Start **/
    /* SPR# 3372 Changes Starts */
    UInt32 bitRateRx;
    /* SPR# 3372 Changes Ends */
    /** Rel 4.0 changes End **/

#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESING)
    /*Indication of SCG or SCG_SPLIT bearer*/
    DCBearerType dcBearerType;
#endif
} RxUmRlc ; 

typedef union RlcConfigParamT {
    struct {
        TxAmRlc txAmRlc ;
        RxAmRlc rxAmRlc ;
    } am ;

    struct {
        TxUmRlc txUmRlc ;
        RxUmRlc rxUmRlc ;
    } umBiDirectional ;

    struct {
        TxUmRlc txUmRlc ;
    } umUniDirectionalTX ;

    struct {
        RxUmRlc rxUmRlc ;
    } umUniDirectionalRx ;
} RlcConfigParam ;
/** Rel 4.0 changes Start **/ //shreya_qos
typedef struct RlcQciInfoT
{
        UInt8 qci;
}RlcQciInfo;

typedef struct RlcUeBitRateInfoT
{
        UInt64 bitRateTx;
            UInt64 bitRateRx;
}RlcUeBitRateInfo;

extern RlcUeBitRateInfo rlcUeBitRateInfo_g[MAX_UE_SUPPORTED];

/** Rel 4.0 changes End **/

/* RLC_SPS_INFO in global context for each UE */
typedef struct RlcUeSpsInfoT
{
    UInt8    dlRequestType;
    UInt8    semiPersistentSchedIntervalDl;
    UInt8    ulRequestType;
    UInt8    semiPersistentSchedIntervalUl;
} RlcUeSpsInfo, *LP_RlcUeSpsInfo;

typedef struct RlcEntityCfgInfoT {
    /*This field indicates the logical channel identifier 
    corresponding to this RLC entity.*/
    RlcLcId lcId ;
    /* this field indicates the type fo comman channel */
    UInt8 comChType ;
    /***********************************************
    RLC_ENTITY_TM - for TM entity ,
    RLC_ENTITY_UM - for UM entity ,
    RLC_ENTITY_AM - for AM entity
    ************************************************/
    RlcEntityMode entityMode ;
    /***********************************************
      RLC_TX_ENTITY - for transmitting entity ,
      RLC_RX_ENTITY - for receiving entity
     *************************************************/
    RlcEntityType entityType ;
    /********************************************* 
      RLC_ADD_ENTITY - to add entity, 
      RLC_DELETE_ENTITY - to delete entity ,
      RLC_RECONFIG_ENTITY - to  re-config entity 
     ***********************************************/
    RlcEntityOpCode opCode ;
    /* Mode specific variables */
    RlcConfigParam configParam ;
    /*Result of the operation performed at this LC*/
    RlcResult result;
    /** Rel 4.0 Changes Start **/
    RlcQciInfo rlcQciInfo;
    /** Rel 4.0 Changes End **/
} RlcEntityCfgInfo ;

 /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
typedef struct RlcRxEntityCfgInfoT
{
    RlcUeIndex  ueIndex ;
    RlcLcId lcId ;
    /* Layer 2 NON CA Changes */
    UInt16 transactionId;
    RrcCellIndex cellId;
    /* Layer 2 NON CA Changes */
    RlcEntityMode entityMode ;
    RlcQciInfo rlcQciInfo;
    union {    
        RxUmRlc rxUmRlc ;
        RxAmRlc rxAmRlc ;
    }rxRlc;
}RlcRxEntityCfgInfo;

typedef struct RlcTxEntityCfgInfoT
{
    RlcUeIndex  ueIndex ;
    RlcLcId lcId ;
    /* Layer 2 NON CA Changes */
    UInt16 transactionId;
    RrcCellIndex cellId;
    /* Layer 2 NON CA Changes */
    RlcEntityMode entityMode ;
    RlcQciInfo rlcQciInfo;
    union {    
        TxUmRlc txUmRlc ;
        TxAmRlc txAmRlc ;
    }txRlc;
}RlcTxEntityCfgInfo;
#endif

typedef struct RlcConfigReqT {
    RlcUeIndex  ueIndex ;       /* user equipment index.  */
    RlcUeId  ueId ;       
    RlcCount  lcCount ;
    /* list of Logical channel Ids of the RLC entity to be re-established  */
    RlcEntityCfgInfo rlcEntityCfgList[MAX_LC_ID] ;   
#ifndef UE_SIM_TESTING
    UInt8 spsBitmask:2;
    /* SPS config Info received from L3 */
    RlcUeSpsInfo rlcUeSpsInfo;
#endif
} RlcConfigReq ;

/**************************************************
* RLC Re-establishment
**************************************************/
typedef struct  RlcReEstabLcInfoT {
    RlcLcId lcId;
    RlcResult  result;
    /* SPR 4994 Change start */
    RlcState state;
    /* SPR 4994 Change End */
} RlcReEstabLcInfo ;

typedef struct  RlcReEstabReqT {
    RlcUeId  ueId ;          /* user equipment Id.  */
    RlcUeIndex  ueIndex ;       /* user equipment index.  */
    RlcCount  lcCount ;
    /* list of Logical channel Ids of 
    the RLC entity to be re-established  */
    RlcReEstabLcInfo lcIdReEstList[MAX_LC_ID] ;  
} RlcReEstabReq ;

typedef struct  RlcReEstabCnfT {
    RlcUeId  ueId ;          /* user equipment Id.  */
    RlcUeIndex  ueIndex ;       /* user equipment index.  */
    RlcCount  lcCount ;
    /* list of result corresponding to Logical channel Ids of 
    the RLC entity to be re-established  */
    RlcReEstabLcInfo lcResultReEstList[MAX_LC_ID] ;  
} RlcReEstabCnf ;

/** Chnage For Change CRNTI Start **/

typedef  struct RlcChangeCrntiReqT{
    UInt16 ueIndex;
    UInt16 oldCrnti;
    UInt16 newCrnti;
}RlcChangeCrntiReq;


typedef  struct RlcChangeCrntiCnfT{
    UInt16 ueIndex;
    UInt16 responseCode;
}RlcChangeCrntiCnf;
/** Chnage For Change CRNTI End **/

/* SPR 4994 Change Start */
typedef struct RlcReEstabCompCnfT{
    UInt16 ueIndex;
    UInt16 responseCode;
}RlcReEstabCompCnf;
/* SPR 4994 Change End */



/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* Data transfer methods */
UInt32 intRrcDataReq(UInt16 comChType, UInt16 rnti, UInt8* data_p,UInt32 size);
UInt32 intRrcDataInd(UInt8 comChType, UInt16 rnti, UInt8* data_p,UInt32 size);

/* API to read msg from RRC */
void* runRRCInterfaceProcessor(void* args);
/* API to parse the TLV formated RRC Msg and call the other relevant API
   based on the requested API. */
/* SPR 5599_129119 Fix Start */
  /* Removing Inline for Compilation warning  */
UInt32 parseRRCMsg(UInt8* msgBuf, SInt32 bytesRead);
/* SPR 5599_129119 Fix End */

/* initialize the rrc interface thread */
SInt32 initRRCInterfaceThread(void* args);

/* initializes the rrc interface */

/* SPR 5599_129119 Fix Start */
  /* Removing Inline for Compilation warning  */
/* + SPR 17439 */
void  rlcInitRrcInterface(void);
/* SPR 5599_129119 Fix End */

/* de-initializes the rrc interface */
UInt32 rlcDeInitRrcInterface(void) ;
/* - SPR 17439 */
/* RLC_SUCCESS when deleted successfuly */ 
UInt32 rlcDeleteUEContext(UInt16 ueIndex, UInt16 transactionId, RrcCellIndex cellId
 /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                          , UInt8 *nodeCount
#endif
);
/* RLC_SUCCESS when created common channel entity successfuly */
UInt16 rlcCreateComChEntity (RlcEntityCfgInfo* rlcEntityCfgInfo_p);
/* RLC_SUCCESS when deleted common channel entity successfuly */
UInt16 rlcDeleteComChEntity(UInt8 comChType);
/* RLC_SUCCESS if successful error message otherwise */
UInt16 rlcReEstablishComChEntity(UInt8 comChType);

/* RLC_SUCCESS if created UE Entity successfully */
UInt16 rlcCreateUEEntity (UInt16 ueIndex, UInt16 rnti,
                              RlcEntityCfgInfo* rlcEntityCfgInfo);
/*  RLC_SUCCESS if deleted UE entity successfully */
UInt16 rlcDeleteUEEntity(UInt16 ueIndex, RlcLcId lcId, UInt16 transactionId,
                         RrcCellIndex cellId, RlcEntityType entityType, RlcEntityMode entityMode
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                         , UInt8 *nodeCount
#endif
                         );
/*RLC Rel 2.0 Changes Start*/                         
/* RLC_SUCCESS if reconfigured UE Entity successfully */
UInt16 rlcReconfigUEEntity (UInt16 ueIndex, 
                             RlcEntityCfgInfo* rlcEntityCfgInfo);
/*RLC Rel 2.0 Changes End*/                             
/* this is to reEstablish the UE Entity */
UInt16 rlcReEstabUeEntity(UInt16 ueIndex, RlcLcId lcId,
                          /* SPR 4994 Change Start */
                          RlcState   rlcState,
                          /* SPR 4994 Change End */
                          UInt16 transactionId,
                          RrcCellIndex cellId
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                          , UInt8 *nodeCount
#endif
);
/* this is to reEstablish the UE Entity called form reEstablish Queue handler */
void reEstablishUeEntity(UInt16 ueIndex, RlcLcId lcId, 
                             RlcEntityType entityType);
/* this is to reEstablish common channel entity called from handler */
void reEstablishComChEntity(UInt8 comChType);
/* this is to delete common channel entity */
void deleteComChEntity(TMEntity* tmEntity_p);
/* This APIs is called when recieve common channel data from RRC */
void rlcComChData(UInt8 comChType, UInt16 rnti, UInt8* comChData);
/* This API is to reassemble SDUs from rxUm Entity */

/* SPR 5599_129119 Fix Start */
void rlcReEstabReassRxUMEntity(RxUMEntity* rxUMEntity_p);
/* SPR 5599_129119 Fix End */
/*RLC Rel 2.0 Changes Start*/
/* This API is to reassemble SDUs from rxAm Entity */
/* SPR 5599_129119 Fix Start */
void rlcReEstabReassRxAMEntity(RxAMEntity* rxAMEntity_p);
/* SPR 5599_129119 Fix End */
/* This API is used to send max Re-transmit indication to RRC */
UInt32 rlcSendMaxReTxThresholdIndToRrc(TxAMEntity* pAmTxE);
/*RLC Rel 2.0 Changes End*/
/* API to handle the Pending Deletion Queue */
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
UInt32 rlcHdlrPenDelQ(LTE_SQUEUE *pendDelQ_p);
/* SPR 3805 Fix start */
/* API to handle the UE delete Queue */
UInt32 rlcHdlrPenUeDelQ(LTE_CIRCQUEUE *rlcUePendingDelQ);
/* SPR 3805 Fix End */
#endif
/* API to handle the ReEstablish Queue */
/* + SPR 17439 */
UInt32 rlcHdlrReEstabQ(LTE_SQUEUE *reEstabQ_p);

SInt32 deInitRRCInterfaceThread(void);

void destroySockets(void);
/* - SPR 17439 */
/* CA changes */
void putEntryInRlcRrcCnfQUsingICC(UInt8 resp,UInt16 ueIndex, UInt8 respEvent, UInt16 transactionId, RrcCellIndex cellId);
/* CA changes */

extern inline UInt16 rlcReconfigTxRxEntity( UInt16 ueIndex,
        UInt16 transactionId, RrcCellIndex cellId,
        RlcEntityCfgInfo* rlcEntityCfgInfo_p,
        UInt8 *nodeCount);

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : rlcConfigSpsInfo
 * Inputs         : ueindex, rlcConfigReq_p
 * Outputs        :
 * Returns        : UInt32
 * Variables      :
 * Description    : This API is used to configure SPS information in the UE
 *                  and for each eligible LC.
 ****************************************************************************/
UInt32 rlcConfigSpsInfo( UInt16 ueIndex, RlcConfigReq *rlcConfigReq_p );
#endif


#endif  /* RRCINTERFACE_H */

