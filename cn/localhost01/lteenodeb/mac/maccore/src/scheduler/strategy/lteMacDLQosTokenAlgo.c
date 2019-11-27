/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDLRoundRobinScheduler.c,v 1.3.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file includes the functionality of distributing the 
                       Downlink RBs in round robin fashion.The number of RBs 
                       distributed to particular UE will be depending upon the 
                       configurale parameters 
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacDLRoundRobinScheduler.h"
#include "lteMacDLQosScheduler.h"
#include "lteMacDLQosTokenAlgo.h"
#include "lteMacParseUtil.h"
#include "lteMacCellConfMgr.h"
/* SPR 23402 Changes Start */ 
/* MOCN CHANGES START */
#include "lteMacOAMInterface.h"
 /* MOCN CHANGES END */
/* SPR 23402 Changes End */

/******************************************************************************
  Private Definitions
 *****************************************************************************/
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
extern mocnOperatorParams mocnOperatorParams_g;
/* MOCN Changes End */
/* SPR 23402 Changes End */
/* This array holds the prority values for the MCS 
 * the same will be mapped by UE MCS values*/

static UInt16 dlLcMcsPriMap_g[MAX_MCS_INDEX_FOR_PRI] = {
        1,/* MCS Index 0 */
        1,/* MCS Index 1 */
        1,/* MCS Index 2 */
        2,/* MCS Index 3 */
        2,/* MCS Index 4 */
        2,/* MCS Index 5 */
        3,/* MCS Index 6 */
        3,/* MCS Index 7 */
        3,/* MCS Index 8 */
        4,/* MCS Index 9 */
        4,/* MCS Index 10 */
        4,/* MCS Index 11 */
        5,/* MCS Index 12 */
        5,/* MCS Index 13 */
        5,/* MCS Index 14 */
        6,/* MCS Index 15 */
        6,/* MCS Index 16 */
        6,/* MCS Index 17 */
        7,/* MCS Index 18 */
        7,/* MCS Index 19 */
        7,/* MCS Index 20 */
        8,/* MCS Index 21 */
        8,/* MCS Index 22 */
        8,/* MCS Index 23 */
        9,/* MCS Index 24 */
        9,/* MCS Index 25 */
        9,/* MCS Index 26 */
       10,/* MCS Index 27 */
       10,/* MCS Index 28 */
};






/* This array holds the prority values for the QCI
 * the same will be mapped by LC QCI values*/
static UInt16 dlLcQciPriMap_g[MAX_QCI_PRIORITIES] = {
        10, /* QCI priority 1 */
        9,  /* QCI priority 2 */
        8,  /* QCI priority 3 */
        7,  /* QCI priority 4 */
        6,  /* QCI priority 5 */ 
        5,  /* QCI priority 6 */
        4,  /* QCI priority 7 */
        3,  /* QCI priority 8 */
        2   /* QCI priority 9 */
};

/* This array holds the prority values for the Available Token  
 * the same will be mapped by ratio of 
 * scheduled tokens and availed tokens of LC, during last 
 * Scheduling window(MAX_TKN_SCHD_WINDOW)
 * i.e for last N * TOKEN_UPDATE_PERIOD duration 
 */

static UInt16 dlLcTknPriMap_g[MAX_TOKEN_PRIORITIES] = {
       10,  /* 0 < alloc opp < 0.75 expected alloc   */ 
        8,  /* 0.75 < alloc opp < 0.9 expected alloc */
        5,  /* 0.90 < alloc opp < 1 expected alloc */
        2,  /* 1  < alloc opp < 1.25 expected alloc    */
        0   /*  expected alloc  <=   alloc opp      */
};



/* This array holds the prority values for the  Qload
 * the same will be mapped by ratio of qload by QSize of LC */
static UInt16 dlLcQloadPriMap_g[MAX_QLOAD_PRIORITIES] = {
        2, /* 0< LC Q load < 0.25 Q Size */
        5, /* 0.25 Q Size < LC Q load < 0.5 Q Size */
        8, /* 0.5 Q Size < LC  Q load < 0.75 Q Size */
        10, /* 0.75 Q Size < LC Q load */  
        10  /* for MAX value */
};

/* This array holds the prority values for the PDB 
 * the same will be mapped by ratio of 
 * delay of head of line packet and PDB
 */
static UInt16 dlLcPdbPriMap_g[MAX_PDB_PRIORITIES] = {
        3,  /*  0< Delay < 0.25 PDB */
        5,  /*  0.25 < Delay < 0.5 PDB */
        8,  /*  0.5 < Delay < 0.75 PDB */
        10, /*  0.75 < Delay < 1.2 PDB */
        /* SPR 6430 changes start */
        20  /*  1.2 PDB<Delay */
        /* SPR 6430 changes end */
};
 UInt16 dlQosGetHighestQosPriorityAmongCells(
                                              DLUEContext* ueDLContext_p,
                                              DlLcQosInfo *dlLcQosInfo_p);

STATIC  void  dlQosUpdateQosPriorityInAllCells( DLUEContext* ueDLContext_p, 
              			                      DlLcQosInfo* dlLcQosInfo_p, 
                                		      UInt32 lcSpecificWt,
		                                      UInt8 mcsWeight);

extern  UInt8 dlQosGetPCellMcs(  DLUEContext* ueDLContext_p);

extern  UInt8 dlQosGetSCellMcs(  DLUEContext* ueDLContext_p,
                                       InternalCellIndex sCellIndex);

/* SPR 16116 fix start */
 UInt32 dlQosGetLowestQosPriorityAmongCells(
                                              DLUEContext* ueDLContext_p,
                                              DlLcQosInfo *dlLcQosInfo_p);
/* SPR 16116 fix end */
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
 * Private Variables (Must be declared static)
 *****************************************************************************/


/*****************************************************************************
 * Functions
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : dlQosInitLcTree
 * Inputs         : tree_p - pointer to UL Priortised LC Tree,
 *                  compare_p - pointer to compare function for Priortised LC
 *                              Tree,
 *                  keyOf_p   - pointer to key function
 * Outputs        : None 
 * Returns        : None
 * Description    : This function initialises the UL priortised LC Tree.
 *****************************************************************************/

void dlQosInitLcTree (
        /* Pointer to Tree */
        MACPriorityLcTree *tree_p, 
        /* Pointer to compare function :
         * it returns 0 if A==B, 1 if A<B and -1 if A>B
         */
        SInt32 (*compare_p)(const void *, const void *),

        /* Pointer to function returning Key
         *        */
        const void *(*keyOf_p)( const LTE_TREE_NODE *))
{
    ytInit(tree_p,compare_p,keyOf_p);
}

/*****************************************************************************
 * Function Name  : dlQosGetFirstLcNode
 * Inputs         : tree_p - pointer to UL Priortised LC Tree
 * Outputs        : Node 
 * Returns        : Pointer to first node of UL Priortised LC Tree according 
 *                  to INORDER Traversal
 * Description    : This function returns the pointer to the first node of 
 *                  UL priortised LC Tree.
 *****************************************************************************/
DlPriorityLcNode * dlQosGetFirstLcNode (
        /* Pointer to Tree */
        MACPriorityLcTree *tree_p)
{
    return ytFirst (tree_p, T_INORDER);
}


/*****************************************************************************
 * Function Name  : dlQosGetNextLcNode
 * Inputs         : entry_p - pointer to DlPriorityLcNode
 * Outputs        : Node  
 * Returns        : Pointer to next node of DL Priortised LC Tree
 * Description    : This function returns the pointer to the Next node of 
 *                  DL priortised LC Tree Node passed as an arguement.
 *****************************************************************************/
DlPriorityLcNode * dlQosGetNextLcNode (
        /* Pointer to node being inserted */
        DlPriorityLcNode *entry_p)
{
    return ytNext (entry_p, T_INORDER);
}




/*****************************************************************************
 * Function Name  : dlQosDeleteLcNode
 * Inputs         : tree_p - pointer to UL Priortised LC Tree,
 *                  entry_p - pointer to Strategy node to be deleted
 * Outputs        : None  
 * Returns        : None
 * Description    : This function deletes the DL strategic LC node from the 
 *                  priortised LC Tree.
 *****************************************************************************/
 void dlQosDeleteLcNode (
        /* Pointer to Tree */
        MACPriorityLcTree *tree_p,

        /* Pointer to node being inserted */
        DlPriorityLcNode *entry_p)
{
    /* SPR 13572 fix start */
    if (entry_p != NULL)
    {
        if ( (entry_p->child[0] == NULL) && (entry_p->child[1] == NULL) && (entry_p->parent == NULL) && entry_p->red == 0)
        {
            /* Node has already been freed */
            return;
        }

        ytDelete (tree_p, entry_p);
	/* Reset the tree node */
        entry_p->child[0] = NULL;
        entry_p->child[1] = NULL;
        entry_p->parent = NULL;
        entry_p->red = 0;
    }
    /* SPR 13572 fix end */
}




/*****************************************************************************
 * Function Name  : dlQosInsertLcNode
 * Inputs         : tree_p - pointer to UL Priortised LC Tree,
 *                  entry_p - pointer to Strategy node to be inserted
 * Outputs        : None  
 * Returns        : None
 * Description    : This function inserts the strategic LC node in priortised 
 *                  LC Tree.
 *****************************************************************************/
 void dlQosInsertLcNode (
        /* Pointer to Tree */
        MACPriorityLcTree *tree_p,

        /* Pointer to node being inserted */
        DlPriorityLcNode *entry_p)
{
    ytInsertWithDuplicate(tree_p, entry_p);
}

/*****************************************************************************
 * Function Name  : dlQosCompareLcNode
 * Inputs         : firstElem_p - Pointer to first element being compared,
 *                  secondElem_p - Pointer to second element being compared
 * Outputs        : None  
 * Returns        : 0 if A == B, 1 if A < B and -1 if A > B
 * Description    : This function compares keys of two strategic LC node in 
 *                  priortised LC Tree.
 *****************************************************************************/

SInt32 dlQosCompareLcNode (
        /* Pointer to first element being compared */
        const void * firstElem_p ,

        /* Pointer to second element being compared */
        const void * secondElem_p
        )
{
    if (firstElem_p == PNULL || secondElem_p == PNULL)
    {
        return ERROR_IN_COMPARE;
    }

    if ( * ((UInt32 *) firstElem_p) == * ((UInt32 *) secondElem_p))
    {
        return FIRST_IS_EQUAL_TO_SECOND;
    }
    else if ( * ((UInt32 *) firstElem_p) < * ((UInt32 *) secondElem_p))
    {
        return FIRST_IS_LESS_THAN_SECOND;
    }
    else /*( * ((UInt32 *) firstElem_p) > * ((UInt32 *) secondElem_p))*/
    {
        return FIRST_IS_GREATER_THAN_SECOND;
    }
}

/*****************************************************************************
 * Function Name  : dlQosKeyOfLcNode
 * Inputs         : node_p - pointer to Node of Tree
 * Outputs        : None  
 * Returns        : pointer to totLCWeight
 * Description    : This function returns pointer to the key if LC node exists 
 *                  in priortised LC Tree.
 *****************************************************************************/



const void * dlQosKeyOfLcNode (
        /* Pointer to Tree node */
        const LTE_TREE_NODE * node_p
        )
{
    return (node_p) ? &((( DLQosStrategyLCNode *)node_p)->totLCWeight) : NULL;
}


/*****************************************************************************
 * Function Name  : dlGetTreeNodeCount
 * Inputs         : p_tree -  pointer to Tree
 * Outputs        : Node
 * Returns        : Node count of the tree
 * Description    : The function returns Number of nodes present in a tree.
 *****************************************************************************/
UInt32 dlGetTreeNodeCount (
        const MACPriorityLcTree *p_tree)
{
    return ytCount(p_tree);
}



/*****************************************************************************
 * Function Name  : dlQosCalcLCWeightAndInsert
 * Inputs         : lcInfo_p - pointer to DLLogicalChannelInfo,
 *                  mcsPriority - MCS Priority,
 *                  drxOnDurationDelay - Delay duration when UE is in DRX on 
 *                                       state,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Calculates aggregate weight of the LC and insert node
 * Returns        : None
 * Description    : It calculates aggregate weight of an LC by formula
 *                  AggWeight = Pqci * Wqci + Ppdb * Wpdb + Ptkn * Wtkn + 
 *                  Pql * Wql + Pmcs * Wmcs
 *                  and inserts the node in UL Priortised LC Tree.
 *****************************************************************************/
 void dlQosCalcLCWeightAndInsert(
    DLLogicalChannelInfo *lcInfo_p,
    UInt8 mcsPriority,
    UInt16 drxOnDurationDelay,
    InternalCellIndex internalCellIndex)
{

    /*Calculate the PDB priority*/
    UInt32 qload = 0;
    /* SPR 9203 Fix starts*/
    DLUEContextInfo *ueDLContextInfo_p = PNULL;
    DLUEContext *ueDLContext_p =PNULL;
    /* SPR 9203 Fix Ends*/
    InitParams *initParams_p = cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->initParams_p;

    UInt16 ueIndex = INVALID_UE_INDEX;
    UInt32 lcSpecificWt = 0;
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    mocnOperatorParams *mocnOperatorParams_p = &mocnOperatorParams_g;
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
    if(SRB != lcInfo_p->dlLcQosInfo.channelType)
    {
        /* QOS_UT */
        lcInfo_p->dlLcQosInfo.pdbPriority   = 
            dlQosGetPdbPriority(
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    lcInfo_p->dlLcQosInfo.lcPdbTable,
                    lcInfo_p->pktArrivalTS,
                    drxOnDurationDelay /*it will be zero in this case */
                    );
        qload = lcInfo_p->queueLoad + lcInfo_p->dlLcQosInfo.ctrlQload + 
            lcInfo_p->dlLcQosInfo.reTxnQueueLoad;

        lcInfo_p->dlLcQosInfo.qLoadPriority = 
            dlQosGetQloadPriority(
                    lcInfo_p->dlLcQosInfo.lcQlTable,
                    qload
                    );
        /*SPR 4017 PH2 Start*/
        if(lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p)
        {
            lcSpecificWt = 
                (lcInfo_p->dlLcQosInfo.pdbPriority * 
                 initParams_p->dlSchdConfig.dlQosStrategyWeights.pdbWeight ) 
                            +
                (lcInfo_p->dlLcQosInfo.qciPriority * 
                 initParams_p->dlSchdConfig.dlQosStrategyWeights.qciWeight ) 
                            +
                (lcInfo_p->dlLcQosInfo.tokenPriority * 
                 initParams_p->dlSchdConfig.dlQosStrategyWeights.tokenWeight ) 
                            +
                (lcInfo_p->dlLcQosInfo.qLoadPriority * 
                 initParams_p->dlSchdConfig.dlQosStrategyWeights.qLoadWeight );
                            
            ueIndex = lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->ueIndex; 
            ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
            
            /*SPR 21948 Changes Start */
            /* Changes under spr 21762 have been reverted */
            /*SPR 21948 Changes End */
            if((IS_CA_ENABLED()) && (isUeEligibleForCaScheduling(ueDLContext_p)))
            {
                dlQosUpdateQosPriorityInAllCells( ueDLContext_p, 
                                                  &lcInfo_p->dlLcQosInfo, 
                                                  lcSpecificWt,
                                                  initParams_p->dlSchdConfig.\
                                                  dlQosStrategyWeights.mcsWeight);

                lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeight = 
                        dlQosGetHighestQosPriorityAmongCells(ueDLContext_p, &(lcInfo_p->dlLcQosInfo));
                /* SPR 16116 fix start */
                lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeightWithLP =
                    dlQosGetLowestQosPriorityAmongCells(ueDLContext_p, &(lcInfo_p->dlLcQosInfo));
                /* SPR 16116 fix end */
            }
            else
            {
                lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeight = 
                        lcSpecificWt +
                        (mcsPriority * initParams_p->dlSchdConfig.dlQosStrategyWeights.mcsWeight);    

                lcInfo_p->dlLcQosInfo.qosPriority[PCELL_SERV_CELL_IX] =
                            lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeight;
                ueDLContext_p->highestMcsIndex = dlQosGetPCellMcs( ueDLContext_p );
            }
            
           /* SPR 9203 Fix starts*/
           ueDLContextInfo_p = &dlUECtxInfoArr_g[lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->ueIndex];
           if(!(ueDLContextInfo_p->pendingDeleteFlag))
           {
                /* Get UE Context */
               ueDLContext_p       = ueDLContextInfo_p->dlUEContext_p;
                /* SPR 23402 Changes Start */   
                /* MOCN Changes Start */
               if (ueDLContext_p)
               {
                   if (mocnOperatorParams_p->isMocnEnabled)
                   {
                       lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeight += 
                      mocnOperatorParams_p->dlPrbUsageDRBPriorityFactor[ueDLContext_p->operatorId];
                   }    

               }
                   /* MOCN Changes End */
                       /* SPR 23402 Changes End */
               if( (PNULL != ueDLContext_p) && (ueDLContext_p->prioritySchedulingReqDL == LTE_TRUE))
               {
                   /* 
                    * Fixing in advance -> 5.3.3 has wrong fix 
                    * need to define srb weight as a macro
                    */
                   lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeight = 600;
                   /* SPR 23402 Changes Start */
                   /* MOCN Changes Start */
                   if (mocnOperatorParams_p->isMocnEnabled)
                   {
                       lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeight += 
                       mocnOperatorParams_p->prbUsageSRBPriorityFactor;
                   }
                   /* MOCN Changes End */
                   /* SPR 23402 Changes End */
               }
           }
            /* SPR 9203 Fix Ends*/
    
            /*Insert LC statergic node in the Priortised LC Tree*/
            dlQosPutEntryInPrioritisedLcTree (
                    lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p,
                    internalCellIndex,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex));
        }
         /*SPR 4017 PH2 End*/
    }
    else
    {
        /*SPR 4017 PH2 Start*/
        if(lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p)
        {
            lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeight = 
                lcInfo_p->dlLcQosInfo.srbWeight;
            /* SPR 23402 Changes Start */
            /* MOCN Changes Start */
            if (mocnOperatorParams_p->isMocnEnabled)
            {
                lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeight += 
                    mocnOperatorParams_p->prbUsageSRBPriorityFactor;
            }
            /* MOCN Changes End */
            /* SPR 23402 Changes End */
            /*Insert LC statergic node in the Priortised LC Tree*/
            dlQosPutEntryInPrioritisedLcTree (
                    lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p,
                    internalCellIndex,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex));
        }
        /*SPR 4017 PH2 End*/
    }
}/* End of Function dlQosCalcLCWeightAndInsert */


/*****************************************************************************
 * Function Name  : dlQosUpdateScheduledTokens
 * Inputs         : ueDLContext_p - Pointer to DLUEContext 
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the token and weight for each DL strategic node
 *                  of the UE scheduled in the current tick.
 *****************************************************************************/
void dlQosUpdateScheduledTokens(DLUEContext *ueDLContext_p,
                                InternalCellIndex internalCellIndex)
{

    /*
     * 1.   calculate AMBR token priority
     * 2.   Increment the  Stkn (t) for AMBR
     * 3.   For each scheduled LC of the UE
     * Note: The scheduled LC list was earlier maintaine in the UE Strategy TX Node,
     * Since the processing of the scheduled LC list is done after processing the 
     * Tx opportunity and in MUX.
     * The scheduled LC list in the maintained in the UE context
     *      a. If GBR 
     *          i.   Increment the  Stkn (t)
     *          ii.  calculate the token priority of  LC  
     *      b. Calculate the total weight of LC and update the node in the Priority
     *         Tree.
     *          i.   Remove the node from tree 
     *          ii.  dlQosCalcLCWeightAndInsert (dlLogicalChannelInfo_p, 
     mcsPriority, drxOnDurationDelay)
     *          iii. Remove the Strategy LC Node from Scheduled LC List
     * 4.   Update the token values for non-GBR LCs. dlQosUpdateNonGbrLCTknValue()
     *
     */

    SInt8 nonGbrLcCount = ueDLContext_p->nonGbrLcCount;
    UInt8 *nonGbrLcInfo_p = ueDLContext_p->nonGbrLcInfo;
    /* spr 2882 fix start */
    /*UInt32 availAmbrToken = 0;TDD Warning Fix */
    /* spr 2882 fix end */
    DLAmbrInfo *dlAmbrInfo_p = &ueDLContext_p->dlAmbrInfo;
    void *dlQosStrategyLCNodeVoid_p = PNULL;
    DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
    DLLogicalChannelInfo *lcInfo_p = PNULL;
    DlLcQosInfo *dlLcQosInfo_p = PNULL;


    /* availAmbrToken = dlAmbrInfo_p->availAmbrToken; TDD Warning Fix */

    /* calculate AMBR token priority */
    /*   Calculate the sum of all Non-GBR LC weight 
     */

    lcInfo_p = &ueDLContext_p->logicalChannel[0];        
    if(dlAmbrInfo_p->ambrTokenPriority != 
            lcInfo_p[nonGbrLcInfo_p[nonGbrLcCount]].dlLcQosInfo.tokenPriority)
    {
        while(nonGbrLcCount--)
        {
            lcInfo_p[nonGbrLcInfo_p[nonGbrLcCount]].dlLcQosInfo.tokenPriority = 
                dlAmbrInfo_p->ambrTokenPriority;        

            LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOKEN_PRIORITY
                (ueDLContext_p->ueIndex, nonGbrLcInfo_p[nonGbrLcCount],  
                 dlAmbrInfo_p->ambrTokenPriority);

        }
    }

    /* For each node in the scheduled LC list 
     * Calculate the total weight of LC 
     * and update the node in the Tree
     */

    
    /*CA_CHG*/

    dlQosStrategyLCNodeVoid_p = (void *)
                              getFirstListNode(&ueDLContext_p->dlScheduledLcList);
    while(dlQosStrategyLCNodeVoid_p)
    {
        DL_QOS_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(dlQosStrategyLCNodeVoid_p,
                dlQosStrategyLCNode_p);


        dlLcQosInfo_p = &(dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo);

        if(dlLcQosInfo_p->channelType == QOS_GBR)
        {
            /* Calculate Priority*/

            dlLcQosInfo_p->tokenPriority = 
                dlQosGetTknPriority(
                        dlLcQosInfo_p->scheduledToken[dlLcQosInfo_p->curTknWindowInd],
                        dlLcQosInfo_p->lcTknTable,
                        dlLcQosInfo_p->totSchdTknInWindow);
            LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOKEN_PRIORITY
                (ueDLContext_p->ueIndex, dlQosStrategyLCNode_p->lcId,  
                 dlLcQosInfo_p->tokenPriority);

            dlLcQosInfo_p->dlQosStrategyLCNode_p->schedulableTokensPcell = 
                dlLcQosInfo_p->availToken;
        }

        /* Check for Qload */
        if(( dlQosStrategyLCNode_p->lcInfo_p->queueLoad ) ||
                ((RLC_AM_MODE == dlQosStrategyLCNode_p->lcInfo_p->rlcMode) &&
                 ((dlLcQosInfo_p->ctrlQload) || (dlLcQosInfo_p->retxnPduSize))))
        {
            /* QOS_UT */
            dlQosDeleteEntryFromPrioritisedLcTree( dlQosStrategyLCNode_p, 
                                                   internalCellIndex,
                                                   GET_CURRENT_TICK_FOR_CELL(internalCellIndex));
            dlQosCalcLCWeightAndInsert(dlQosStrategyLCNode_p->lcInfo_p, 
                    ueDLContext_p->mcsPriority, 
                    ZERO_DRX_DELAY,/* Zero DRX Delay */
                    internalCellIndex);

            /*NEW_LOG_REQ*/
            LOG_MAC_MSG(MAC_DL_QOS_DL_METRIC_CALC_PER_USER,LOGBRIEF,
                    MAC_DL_Strategy,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueDLContext_p->ueIndex,
                    dlQosStrategyLCNode_p->lcId,
                    dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.\
                    dlQosStrategyLCNode_p->totLCWeight, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE, __func__,"");

        }

        LOG_MAC_MSG(MAC_DELETE_SCH_LIST_NODE,LOGBRIEF,
                MAC_MUX,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                __LINE__,
                (UInt32)ueDLContext_p->ueIndex, 
                dlQosStrategyLCNode_p->lcId,
                ueDLContext_p->scheduledLcCount,
                DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

        dlQosStrategyLCNode_p->schedulableBytes = 0;
        /* + SPR 12349 Changes */
        dlQosStrategyLCNode_p->requestedDataSize = 0;
        /* - SPR 12349 Changes */
        /*CA_CHG*/
        dlQosStrategyLCNodeVoid_p = (void *)
                    getNextListNode(&dlQosStrategyLCNode_p->scheduledLcAnchor);

    }
    /* Distribute the AMBR tokens among the Non-GBR LC's */
    dlQosUpdateNonGbrLCTknValue(ueDLContext_p);

    /* Distribute total available tokens among both serving cells
     * only if UE's Scell is in Active state */
    if( (IS_CA_ENABLED()) &&
        /*SPR 21948 Changes Start */
         /* Changes under spr 21762 have been reverted */
          /*SPR 21948 Changes End */
        (isUeEligibleForCaScheduling(ueDLContext_p)))
    {
        lteMacDLQoSDistributeTokenAmongCells(ueDLContext_p);
    }
}/* End of Funtion dlQosUpdateScheduledTokens */

/*****************************************************************************
 * Function Name  : dlQosUpdateTokens
 * Inputs         : ulUEContext_p - Pointer to DLUEContext
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the tokens of the each GBR LC and for AMBR  
 *                  And calculate the weight for each Eligible LC 
 *                  & update the DL LC strategic node in the Priority LC tree.
 *****************************************************************************/
void dlQosUpdateTokens(DLUEContext *ueDLContext_p,
                       InternalCellIndex internalCellIndex)
{
    SInt8 gbrLcCount = ueDLContext_p->gbrLcCount;
    UInt8 *gbrLcInfo_p = ueDLContext_p->gbrLcInfo;
    SInt8 nonGbrLcCount = ueDLContext_p->nonGbrLcCount;
    UInt8 *nonGbrLcInfo_p = ueDLContext_p->nonGbrLcInfo;
    void *dlQosStrategyLCNodeVoid_p = PNULL;
    DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
    DLAmbrInfo *dlAmbrInfo_p = &ueDLContext_p->dlAmbrInfo;
    DlLcQosInfo *dlLcQosInfo_p = PNULL;
    DLLogicalChannelInfo *lcInfo_p = PNULL;


    /* Update the AMBR tokens */

    /* DESIGN REVIEW CHANGES INCORPORATION  START */
    if(dlAmbrInfo_p->eTokenUpdateFlag)
    {
        dlAmbrInfo_p->availAmbrToken += dlAmbrInfo_p->ambrTokenRate;
    }
    else
    {
        dlAmbrInfo_p->availAmbrToken -= dlAmbrInfo_p->ambrTokenRate;
    }

    /* SPR 6149 Changes Starts */
    if(dlAmbrInfo_p->availAmbrToken > (SInt32)dlAmbrInfo_p->ambrTokenDepth)
    {
        /* SPR 6149 Changes Ends */
        dlAmbrInfo_p->availAmbrToken = dlAmbrInfo_p->ambrTokenDepth;

        if(!dlAmbrInfo_p->queueLoad)
        {
            dlAmbrInfo_p->eTokenUpdateFlag = DECREMENT_TOKENS;
        }

        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_AMBR_TOKEN_OVERFLOW
            (ueDLContext_p->ueIndex);
    } 
    /* SPR 6149 Changes Starts */
    /* SPR 11115 Fix Start */
    else if(dlAmbrInfo_p->availAmbrToken <= dlAmbrInfo_p->ambrTokenLimit)
    {
        /* SPR 6149 Changes Ends */
        dlAmbrInfo_p->availAmbrToken = dlAmbrInfo_p->ambrTokenLimit;
        /* SPR 11115 Fix End */

        if(dlAmbrInfo_p->queueLoad)
        {
            dlAmbrInfo_p->eTokenUpdateFlag = INCREMENT_TOKENS;
        }
    }
    /* DESIGN REVIEW CHANGES INCORPORATION  END */

    /* Update the total schedule tokens with the current 
     * TOKEN_UPDATE_PERIOD tokens
     */
    dlAmbrInfo_p->totSchdTknInWindow += 
        dlAmbrInfo_p->scheduledToken[dlAmbrInfo_p->curTknWindowInd];

    /* Increment the current window Index 
     * Advance the token window
     */
    dlAmbrInfo_p->curTknWindowInd = 
        ((dlAmbrInfo_p->curTknWindowInd + 1 )% MAX_TKN_SCHD_WINDOW);

    /* Ttkn(t-1) - Stkn(t-N) 
     * decrement the Stkn(t-N) from total schedule tokens 
     */
    dlAmbrInfo_p->totSchdTknInWindow -=
        dlAmbrInfo_p->scheduledToken[dlAmbrInfo_p->curTknWindowInd];

    /* Initialize the S(t) */
    dlAmbrInfo_p->scheduledToken[dlAmbrInfo_p->curTknWindowInd] = 0;

    /* calculate AMBR token priority */
    /*Calculate MCS priority*/
            
    /* SPR# 10042 fix start */
    if (ueDLContext_p->dlMIMOInfo.dlRi == 1)
    {
        /* SPR 18122 Changes Start*/
        ueDLContext_p->mcsPriority =
            dlQosGetMcsPriority((DIVIDE_BY_TWO(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne)));
        /* SPR 18122 Changes End*/

    }
    else
    {
        /* SPR 18122 Changes Start*/
        ueDLContext_p->mcsPriority =
            dlQosGetMcsPriority(((DIVIDE_BY_TWO(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne+ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo))));
        /* SPR 18122 Changes End*/
    }
    /* SPR# 10042 fix end */
          
    lcInfo_p = &ueDLContext_p->logicalChannel[0];        
    if(dlAmbrInfo_p->ambrTokenPriority != 
            lcInfo_p[nonGbrLcInfo_p[nonGbrLcCount]].dlLcQosInfo.tokenPriority)
    {
        while(nonGbrLcCount--)
        {
            lcInfo_p[nonGbrLcInfo_p[nonGbrLcCount]].dlLcQosInfo.tokenPriority = 
                dlAmbrInfo_p->ambrTokenPriority;        

            LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOKEN_PRIORITY
                (ueDLContext_p->ueIndex, nonGbrLcInfo_p[nonGbrLcCount],  
                 dlAmbrInfo_p->ambrTokenPriority);

        }
    }


    while(gbrLcCount--)
    {
        /* SPR# 3110 Changes Starts */
        if(ueDLContext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].logicalChannelId != INVALID_LCID)
        {
            /* SPR# 3110 Changes Ends */
            dlLcQosInfo_p = 
                &ueDLContext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].dlLcQosInfo;

            /* DESIGN REVIEW CHANGES INCORPORATION  START */
            if(dlLcQosInfo_p->eTokenUpdateFlag)
            {
                dlLcQosInfo_p->availToken += dlLcQosInfo_p->tokenRate;
            }
            else
            {
                dlLcQosInfo_p->availToken -= dlLcQosInfo_p->tokenRate;
            }

            /* SPR 6149 Changes Starts */
            if(dlLcQosInfo_p->availToken > (SInt32)dlLcQosInfo_p->tokenDepth)
            {
                /* SPR 6149 Changes Ends */
                dlLcQosInfo_p->availToken = dlLcQosInfo_p->tokenDepth;
                if(!dlLcQosInfo_p->dlQosStrategyLCNode_p->lcInfo_p->queueLoad)
                {
                    dlLcQosInfo_p->eTokenUpdateFlag = DECREMENT_TOKENS;
                }
                LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOKEN_OVERFLOW
                    (ueDLContext_p->ueIndex, gbrLcInfo_p[gbrLcCount]);
            }
            /* SPR 6149 Changes Starts */
                /* SPR 11115 Fix Start */
            else if(dlLcQosInfo_p->availToken <= dlLcQosInfo_p->tokenLimit)
            {
                /* SPR 6149 Changes Ends */
                dlLcQosInfo_p->availToken = dlLcQosInfo_p->tokenLimit;
                /* SPR 11115 Fix End */
                if(dlLcQosInfo_p->dlQosStrategyLCNode_p->lcInfo_p->queueLoad)
                {
                    dlLcQosInfo_p->eTokenUpdateFlag = INCREMENT_TOKENS;
                }
            }
            /* DESIGN REVIEW CHANGES INCORPORATION  END */

            /* Update the total schedule tokens with the current
             * TOKEN_UPDATE_PERIOD tokens
             */

            dlLcQosInfo_p->totSchdTknInWindow +=
                dlLcQosInfo_p->scheduledToken[dlLcQosInfo_p->curTknWindowInd];

            /* Increment the current window Index 
             * Advance the token window
             */
            dlLcQosInfo_p->curTknWindowInd=
                ((dlLcQosInfo_p->curTknWindowInd + 1 )% MAX_TKN_SCHD_WINDOW);

            /* Ttkn(t-1) - Stkn(t-N) 
             * decrement the Stkn(t-N) from total schedule tokens 
             */
            dlLcQosInfo_p->totSchdTknInWindow -=
                dlLcQosInfo_p->scheduledToken[dlLcQosInfo_p->curTknWindowInd];

            /* Initialize the S(t) */
            dlLcQosInfo_p->scheduledToken[dlLcQosInfo_p->curTknWindowInd] = 0;

            /* calculate the token priority of  LC */
            dlLcQosInfo_p->tokenPriority =
                dlQosGetTknPriority(
                        dlLcQosInfo_p->scheduledToken[dlLcQosInfo_p->curTknWindowInd],
                        dlLcQosInfo_p->lcTknTable,
                        dlLcQosInfo_p->totSchdTknInWindow);

#ifdef UTFWK_SIMULATION
            if ( dlLcQosInfo_p->tokenPriority == 10 )
            {
                gMacStats.pStats->perfStats.gbrTputFactor_0_75++;
            }
            else if ( dlLcQosInfo_p->tokenPriority == 8 )
            {
                gMacStats.pStats->perfStats.gbrTputFactor_75_90++;
            }
            else if ( dlLcQosInfo_p->tokenPriority == 5 )
            {
                gMacStats.pStats->perfStats.gbrTputFactor_90_100++;
            }
            else if ( dlLcQosInfo_p->tokenPriority == 2 )
            {
                gMacStats.pStats->perfStats.gbrTputFactor_100_125++;
            }
            else if ( dlLcQosInfo_p->tokenPriority == 0 )
            {
                gMacStats.pStats->perfStats.gbrTputFactor_125++;
            }
            gMacStats.pStats->perfStats.totalNumberOfSamples++;

#endif
            LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOKEN_PRIORITY
                (ueDLContext_p->ueIndex, gbrLcInfo_p[gbrLcCount],  
                 dlLcQosInfo_p->tokenPriority);

            dlLcQosInfo_p->dlQosStrategyLCNode_p->schedulableTokensPcell = 
                dlLcQosInfo_p->availToken;
        }
    }
    if ( (DL_QOS_NEW_TRANSMISSION == ueDLContext_p->scheduleFlag) ||
         ( (IS_SCELL_ACTIVE(ueDLContext_p->ueIndex, START_SCELL_INDEX)) && 
           (DL_QOS_NEW_TRANSMISSION == ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag)))
    {
        /* QOS_UT */
        dlQosStrategyLCNodeVoid_p =  (void *)
            getFirstListNode(&(ueDLContext_p->dlEligibleLcList));

        while(dlQosStrategyLCNodeVoid_p)
        {
            DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(dlQosStrategyLCNodeVoid_p,
                    dlQosStrategyLCNode_p);

            dlQosDeleteEntryFromPrioritisedLcTree( dlQosStrategyLCNode_p,
                                                   internalCellIndex,
                                                   GET_CURRENT_TICK_FOR_CELL(internalCellIndex));


            dlQosCalcLCWeightAndInsert(dlQosStrategyLCNode_p->lcInfo_p, 
                    ueDLContext_p->mcsPriority, 
                              ZERO_DRX_DELAY,/* Zero DRX Delay */
                              internalCellIndex);
            /*NEW_LOG_REQ*/
            LOG_MAC_MSG(MAC_DL_QOS_DL_METRIC_CALC_PER_USER,LOGBRIEF,
                    MAC_DL_Strategy,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex), 
                    ueDLContext_p->ueIndex,
                    dlQosStrategyLCNode_p->lcId,
                    dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.\
                    dlQosStrategyLCNode_p->totLCWeight, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE, __func__,"");

            /* QOS_UT */
            dlQosStrategyLCNodeVoid_p = (void *)
                getNextListNode(&(dlQosStrategyLCNode_p->eligibleLcAnchor));

        }
    }

    /* Distribute the AMBR tokens among the Non-GBR LC's */
    dlQosUpdateNonGbrLCTknValue(ueDLContext_p);

    /* Distribute total available tokens among both serving cells
     * only if UE's Scell is in Active state */
    if( (IS_CA_ENABLED()) &&
        /*SPR 21948 Changes Start */
         /* Changes under spr 21762 have been reverted */
          /*SPR 21948 Changes End */
        (isUeEligibleForCaScheduling(ueDLContext_p)))
    {
        lteMacDLQoSDistributeTokenAmongCells(ueDLContext_p);
    }
}
/*****************************************************************************
 * Function Name  : dlQosCalcGbrLCTknRate
 * Inputs         : dlLcQosInfo_p - Pointer to DlLcQosInfo
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Updates the LC Info with token rate
 * Returns        : None
 * Description    : It updates the token rate for a GBR LC,
 *                  This function will be called when an LC is created 
 *                  to update.
 *****************************************************************************/


 void dlQosCalcGbrLCTknRate (DlLcQosInfo *dlLcQosInfo_p
                   ,InternalCellIndex internalCellIndex)
{
    /*Calculate the token rate */
        
    /* The token rate(Token Value) = (GBR /  8) * TOKEN_UPDATE_PERIOD; 
     * as token is updated at a granularity of BYTES value 8 is used.
     */

    dlLcQosInfo_p->tokenRate = (dlLcQosInfo_p->GBR *
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
        initParams_p->dlSchdConfig.dlTokenPeriodicity) / NUM_TICKS_PER_SECOND ; 


}

/*****************************************************************************
 * Function Name  : dlQosCalcAmbrTknRate
 * Inputs         : dlAmbrInfo_p - Pointer to DLAmbrInfo,
 *                  dlTokenPeriodicity - DL Token Priodicity.
 * Outputs        : Updates the LC Info with token rate
 * Returns        : None
 * Description    : It updates the token rate for a AMBR LC.
 *                  This function will be called during the below mentioned 
 *                  scenarios:
 *                      a. when AMBR gets changed.
 *                      b. when AMBR token is updated at every token update 
 *                         periodicity.
 *****************************************************************************/

 void dlQosCalcAmbrTknRate(DLAmbrInfo *dlAmbrInfo_p,UInt16 dlTokenPeriodicity)
{

     dlAmbrInfo_p->ambrTokenRate = (dlAmbrInfo_p->AMBR * dlTokenPeriodicity) 
                           / NUM_TICKS_PER_SECOND ;
}



/*****************************************************************************
 * Function Name  : dlQosUpdateNonGbrLCTknValue
 * Inputs         : ueDLContext_p - pointer to lc info in UE context
 * Outputs        : Updates the NON-GBR LCs Info with avail Token
 * Returns        : None
 * Description    : It distributes the Available AMBR token among NON-GBR LCs,
 *                  The tokens are distribute proportionally according to their
 *                  weight.
 *****************************************************************************/
void dlQosUpdateNonGbrLCTknValue (DLUEContext *ueDLContext_p)
{
    SInt8 nonGbrLcCount = ueDLContext_p->nonGbrLcCount;
    UInt8 *nonGbrLcInfo_p = ueDLContext_p->nonGbrLcInfo;
    UInt16 sumNonGbrLcWeight  = 0;
    /* spr 2882 fix start */
    /* SPR 6149 Changes Starts */
    SInt32 availAmbrToken = ueDLContext_p->dlAmbrInfo.availAmbrToken;
    /* SPR 6149 Changes Ends */
    /* spr 2882 fix end */
    DLLogicalChannelInfo *lcInfo_p = PNULL;

    /* Calculate the sum of all Non-GBR LC weight */

    while(nonGbrLcCount--)
    {
        lcInfo_p = &ueDLContext_p->logicalChannel[nonGbrLcInfo_p[nonGbrLcCount]];

        /* SPR_10354_Base_code Start */
        if(lcInfo_p->logicalChannelId != INVALID_LCID )
        {
            /* SPR_10354_Base_code End */
            sumNonGbrLcWeight += 
                lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeight;
        } 
    }

    nonGbrLcCount = ueDLContext_p->nonGbrLcCount;

    if(sumNonGbrLcWeight)
    {
        /* For Each Non-GBR LC distribute the tokens by proportionally */
        while(nonGbrLcCount--)
        {

            /* SPR_10354_Base_code Start */
            if(lcInfo_p->logicalChannelId == INVALID_LCID )
            {
                continue;
            }
            /* SPR_10354_Base_code Start */

            lcInfo_p = 
                &ueDLContext_p->logicalChannel[nonGbrLcInfo_p[nonGbrLcCount]];

        /* 
         *                                            LC priority weight
         * Token Value = Available Ambr token * -------------------------------
         *                                       sum of all LC priority weight
         */
            /* SPR 6149 Changes Starts */
            lcInfo_p->dlLcQosInfo.availToken = (availAmbrToken * 
                ((SInt32 )lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeight)) / 
                 ((SInt32)sumNonGbrLcWeight);
            /* SPR 6149 Changes Ends */


            /* SPR 6149 Changes Starts */
            if(lcInfo_p->dlLcQosInfo.availToken > ((SInt32)ueDLContext_p->dlAmbrInfo.ambrTokenDepth))
            {
            /* SPR 6149 Changes Ends */
                lcInfo_p->dlLcQosInfo.availToken = 
                                ueDLContext_p->dlAmbrInfo.ambrTokenDepth;

                LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOKEN_OVERFLOW
                    (ueDLContext_p->ueIndex, nonGbrLcInfo_p[nonGbrLcCount]);
            }

            /* b.   calculate the token priority of  LC */
            lcInfo_p->dlLcQosInfo.tokenPriority = 
                ueDLContext_p->dlAmbrInfo.ambrTokenPriority;

            lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->schedulableTokensPcell = 
                lcInfo_p->dlLcQosInfo.availToken;
        }
    }
}


/*****************************************************************************
 * Function Name  : dlQosGetQloadPriority
 * Inputs         : lcQlTable_p - pointer to lc Qload table,
 *                  qLoad
 * Outputs        : None
 * Returns        : Qload priority
 * Description    : It maps the qload with its priority.
 *****************************************************************************/

 UInt16 dlQosGetQloadPriority(UInt32 *lcQlTable_p, 
                      UInt32 qLoad
                      )
{
    UInt8 index = 0;
    while(qLoad > lcQlTable_p[index])
    {
        index++;

        /* SPR 20257 Changes Starts */
        /* SPR# 3110 Changes Starts */
        if((MAX_QLOAD_PRIORITIES -1) == index )
        {
            return dlLcQloadPriMap_g[index];
        }
            /* SPR# 3110 Changes Ends */
        /* SPR 20257 Changes Ends */
    }
    return dlLcQloadPriMap_g[index];
}

/*****************************************************************************
 * Function Name  : dlQosGetPdbPriority
 * Inputs         : currentTick - current global tick,
 *                  lcPdbTable_p - pointer to lc Qload table,
 *                  pktArrivalTS,
 *                  drxOnDurationDelay - delay considered for DRX on State
 * Outputs        : None
 * Returns        : Qload load priority
 * Description    : It maps the Delay of the head of the line packet 
 *                  with its priority. In case of the DRX cycle, when the 
 *                  DRX OFF trigger is received the DRX on duration delay 
 *                  is also added to the actual delay.
 *****************************************************************************/
/* SPR 21494 Start */
                                /* SPR 15909 fix start */
 UInt16 dlQosGetPdbPriority(tickType_t currentTick, 
                                  UInt16 *lcPdbTable_p, 
                                  tickType_t pktArrivalTS, 
                                  /* SPR 15909 fix end */
                                  UInt16 drxOnDurationDelay
                                  )
{
    UInt8 index = 0;
    /* + SPR 22784 */
    /* To handle delay more than 0xffff */
    UInt32 headOfLinePktDelay = currentTick - pktArrivalTS + drxOnDurationDelay;
    /* - SPR 22784 */

    /* 
     * In case we have scheduled all the qLoad for that LC,
     * RLC will update BO with 0 qload and 0 pktArrivalTS
     * In that case pdbPriority should be minimum
     */
    if(!pktArrivalTS)
    {
        headOfLinePktDelay = 0;
    }

    while(headOfLinePktDelay > lcPdbTable_p[index])
    {
        index++;
       /* SPR 20257 fix start */ 
        if((MAX_PDB_PRIORITIES - 1) == index)
        {
           return dlLcPdbPriMap_g[index];
        }
       /* SPR 20257 fix end */ 
    }
    return dlLcPdbPriMap_g[index];
}
/* SPR 21494 End */

/*****************************************************************************
 * Function Name  : dlQosGetTknPriority
 * Inputs         : currScheduledTkn - scheduled token for current Tick,
 *                  lcTknTable_p - pointer to LC Token Table,
 *                  totSchdTkn_p - Pointer to sum of scheduled tokens in 
 *                                 a Token Window
 * Outputs        : None
 * Returns        : Token priority
 * Description    : It maps the Available token of the LC with its priority.
 *****************************************************************************/

 UInt16 dlQosGetTknPriority(
                    UInt32 currScheduledTkn, /* Stkn(t) */
                    UInt32 *lcTknTable_p,
                    UInt32 totSchdTkn)  /* Ttkn(t-1) - Stkn(t-N) */
{
    UInt8 index = 0;
    UInt32 newTotSchdTkn = 0; /* Ttkn(t) */

    /*Ttkn(t )  = Ttkn(t  - 1) - Stkn(t - N) +   Stkn(t)*/

    newTotSchdTkn = totSchdTkn + currScheduledTkn;
    
    /* Where, Ttkn(t )    : Sum of the token scheduled for last 
     *                       N TOKEN_UPDATE_PERIOD's
              Ttkn(t - 1) : Ttkn(t ) of the previous TOKEN_UPDATE_PERIOD
              Stkn(t - N) : Tokens scheduled at (t-N)th TOKEN_UPDATE_PERIOD
              Stkn(t)     : Tokens scheduled at (t)th (current) 
                            TOKEN_UPDATE_PERIOD
     */

        
    while(newTotSchdTkn > lcTknTable_p[index])
    {
        /*SPR 16982 fix*/
        index++;
        /*SPR 16982 fix*/
        /* SPR 20257 fix start */
        if((MAX_TOKEN_PRIORITIES - 1) == index )
        {
            return dlLcTknPriMap_g[index];
        }
        /* SPR 20257 fix end */
    }
    return dlLcTknPriMap_g[index];
}

/*****************************************************************************
 * Function Name  : dlQosGetQciPriority
 * Inputs         : qci - qciConfigPriority
 * Outputs        : None
 * Returns        : QCI priority
 * Description    : It maps the qci with its priority.
 *****************************************************************************/
/* SPR 11081 changes start */
 UInt16 dlQosGetQciPriority (UInt8 qciConfigPriority) 
{
    return dlLcQciPriMap_g[qciConfigPriority - 1];
}
/* SPR 11081 changes end */
/*****************************************************************************
 * Function Name  : dlQosGetMcsPriority
 * Inputs         : mcs - value of Modulation Coding Scheme
 * Outputs        : None
 * Returns        : MCS priority
 * Description    : It maps the mcs with its priority.
 *****************************************************************************/
 UInt16 dlQosGetMcsPriority (UInt8 mcs)
{
    return dlLcMcsPriMap_g[mcs];
}


/*****************************************************************************
 * Function Name  : dlQosUpdateQloadTableForLcPriority
 * Inputs         : lcQlTable_p - pointer to LC Qload Table,
 *                  bitRate - value of bitrate in bps,
 *                  pktDelayBgt - packet Delay Budget
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the Qload table. The Table the contains Qload 
 *                  for different level of qSize. Each level maps to particular
 *                  priority. To calculate the priority, the Qload will be 
 *                  mapped with this table and the index for the level 
 *                  is obtained. The same Index used to get the priority 
 *                  from the priority table.
 *****************************************************************************/

 void dlQosUpdateQloadTableForLcPriority(UInt32 *lcQlTable_p, 
/* L2_PERFORMANCE_OPT_CHANGES_START */
        UInt32 bitRate, 
/* L2_PERFORMANCE_OPT_CHANGES_END */
        UInt16 pktDelayBgt)
{
    /* +COVERITY 10876 */
    UInt64 qSize = ((UInt64)bitRate * pktDelayBgt) / NUM_TICKS_PER_SECOND;
    /* -COVERITY 10876 */
    
    /*Update qload table*/
#ifdef FLOAT_OPERATIONS
    *lcQlTable_p++  =  0.25 * qSize;
    *lcQlTable_p++  =  0.5  * qSize;
    *lcQlTable_p++  =  0.75 * qSize;
    *lcQlTable_p    =  1    * qSize;
#else

    *lcQlTable_p++  =  qSize >>2;
    *lcQlTable_p++  =  qSize >>1;
    *lcQlTable_p++  =  (3 * qSize)>>2;
    *lcQlTable_p++  =  qSize;
    *lcQlTable_p    =  5400000;
#endif
}

/*****************************************************************************
 * Function Name  : dlQosUpdateTknTableForLcPriority
 * Inputs         : lcTknTable_p - pointer to LC Token Table,
 *                  bitRate - value of bitrate in bps,
 *                  dlTokenPeriodicity - DL Token priodicity.
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the Token table. The Table the contains Token 
 *                  value for different level of Token Depth. Each level maps 
 *                  to particular priority. To calculate the priority, the 
 *                  available token will be mapped with this table and the 
 *                  index for the level is obtained. The same Index used to 
 *                  get the priority from the priority table.
 *****************************************************************************/


 void dlQosUpdateTknTableForLcPriority (UInt32 *lcTknTable_p, 
/* L2_PERFORMANCE_OPT_CHANGES_START */
                                  UInt32 bitRate
                                  ,UInt16 dlTokenPeriodicity)
/* L2_PERFORMANCE_OPT_CHANGES_END */
{
    /* +COVERITY 10877 */
    UInt64 mTkn  = ((UInt64)bitRate * dlTokenPeriodicity * 
                        MAX_TKN_SCHD_WINDOW)/NUM_TICKS_PER_SECOND;
    /* -COVERITY 10877 */

#ifdef FLOAT_OPERATIONS
    *lcTknTable_p++  =  0.5 * mTkn;
    *lcTknTable_p++  =  0.8 * mTkn;
    *lcTknTable_p++  =  0.9 * mTkn;
    *lcTknTable_p    =  1   * mTkn;
#else
    *lcTknTable_p++  =  (mTkn * 3)>>2;
    *lcTknTable_p++  =  (9 * mTkn)/10;
    *lcTknTable_p++  =  mTkn;
    *lcTknTable_p++  =  (mTkn * 125)/100;
    /*+- SPR 20899 Fix*/
    *lcTknTable_p    =  MULTIPLY_BY_TWO(9422 * dlTokenPeriodicity * MAX_TKN_SCHD_WINDOW);
    /*+- SPR 20899 Fix*/

#endif

}

/*****************************************************************************
 * Function Name  : dlQosUpdatePdbTableForLcPriority
 * Inputs         : lcPdbTable_p - pointer to LC PDB Table,
 *                  pktDelayBgt - packet Delay Budget
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the PDB table. The Table the contains dealy
 *                  for different level of Paket delay budget. Each level maps 
 *                  to particular priority. To calculate the priority, the 
 *                  delay of the head of the line packet in the LC queue will 
 *                  be mapped with this table and the index for the level is 
 *                  obtained. The same Index used to get the priority from the 
 *                  PDB priority table.
 *****************************************************************************/
 void dlQosUpdatePdbTableForLcPriority (UInt16 *lcPdbTable_p, 
                                        UInt16 pktDelayBgt)
{
    
#ifdef FLOAT_OPERATIONS
    *lcPdbTable_p++  =  0.25 * pktDelayBgt;
    *lcPdbTable_p++  =  0.5  * pktDelayBgt;
    *lcPdbTable_p++  =  0.75 * pktDelayBgt;
    *lcPdbTable_p++  =  1    * pktDelayBgt;
#else
    *lcPdbTable_p++  =   pktDelayBgt >> 2;
    *lcPdbTable_p++  =   pktDelayBgt >> 1;
    *lcPdbTable_p++  =   (3 * pktDelayBgt) >> 2;
    *lcPdbTable_p++  =   pktDelayBgt;
    *lcPdbTable_p    =   65535;

#endif

}
/***********************************************************************
 * Function Name  : lteMacDLQoSDistributeTokenAmongCells
 * Inputs         : ueDLContext_p 
 * Outputs        : None
 * Returns        : None
 * Description    : This function will distribute available tokens 
 *                  for GBR & non-GBR LCs among Pcell & Scells. Also
 *                  it will distribute avaialble AMBR tokens among Cells
 *                  as per the bandWidthDistribution given by RRM.
 ***********************************************************************/
void lteMacDLQoSDistributeTokenAmongCells(DLUEContext *ueDLContext_p) 
{
    /* ****************** ASSUMPTION*************************
     * This function will distribute tokens considering 
     * one-to-one mapping b/w serving cell index used for 
     * bandWidthDistribution and the order of internal Scell index
     * stored in UeContext */

    UInt8 scellCount = ueDLContext_p->scellCount;
    UInt8 scellIndex = START_SCELL_INDEX;
    DLUESCellStartegyNode *tempDLUESCellStartegyNode_p = PNULL;
    DlLcQosInfo *dlLcQosInfo_p = PNULL;
    SInt8 gbrLcCount = ueDLContext_p->gbrLcCount;
    UInt8 *gbrLcInfo_p = ueDLContext_p->gbrLcInfo;
    SInt8 nonGbrLcCount = ueDLContext_p->nonGbrLcCount;
    UInt8 *nonGbrLcInfo_p = ueDLContext_p->nonGbrLcInfo;

    /* Distribute available tokens for all GBR LCs */
    while(gbrLcCount--)
    {
        if(ueDLContext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].logicalChannelId != INVALID_LCID)
        {
            dlLcQosInfo_p =
                        &ueDLContext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].dlLcQosInfo;

            /* Distribute available tokens for Pcell */
            dlLcQosInfo_p->dlQosStrategyLCNode_p->schedulableTokensPcell = 
                        ( dlLcQosInfo_p->availToken * dlLcQosInfo_p->
                                bandWidthDistribution[PCELL_SERV_CELL_IX] ) / 100;

            /* Distribute available tokens for all Scell */
            for(scellIndex = START_SCELL_INDEX; scellIndex <= scellCount; scellIndex++)
            {
                if(IS_SCELL_ACTIVE(ueDLContext_p->ueIndex, scellIndex))
                {
                    tempDLUESCellStartegyNode_p = dlLcQosInfo_p->
                                dlQosStrategyLCNode_p->dLUESCellStartegyNode_p[scellIndex];

                    tempDLUESCellStartegyNode_p->internalCellIndex =
                                ueDLContext_p->dlUeScellContext_p[scellIndex]->internalCellIndex;

                    tempDLUESCellStartegyNode_p->schedulableTokensScell = 
                            ( dlLcQosInfo_p->availToken * 
                                    dlLcQosInfo_p->bandWidthDistribution[scellIndex] ) / 100;
                }
            }
        }
    }
    /* Distribute available tokens for all non-GBR LCs */
    while(nonGbrLcCount--)
    {
        if(ueDLContext_p->logicalChannel[nonGbrLcInfo_p[nonGbrLcCount]].logicalChannelId != INVALID_LCID)
        {
            dlLcQosInfo_p =
                &ueDLContext_p->logicalChannel[nonGbrLcInfo_p[nonGbrLcCount]].dlLcQosInfo;

            /* Distribute available tokens for Pcell */
            /* + SPR 11473 Changes */
            dlLcQosInfo_p->dlQosStrategyLCNode_p->schedulableTokensPcell = 
                        ( dlLcQosInfo_p->availToken * 
                                ueDLContext_p->bandWidthDistribution[PCELL_SERV_CELL_IX] ) / 100;
            /* - SPR 11473 Changes */

            /* Distribute available tokens for all Scell */
            for(scellIndex = START_SCELL_INDEX; scellIndex <= scellCount; scellIndex++)
            {
                if(IS_SCELL_ACTIVE(ueDLContext_p->ueIndex, scellIndex))
                {
                    tempDLUESCellStartegyNode_p = dlLcQosInfo_p->
                                dlQosStrategyLCNode_p->dLUESCellStartegyNode_p[scellIndex];

                    tempDLUESCellStartegyNode_p->internalCellIndex =
                                ueDLContext_p->dlUeScellContext_p[scellIndex]->internalCellIndex;

                    /* + SPR 11473 Changes */
                    tempDLUESCellStartegyNode_p->schedulableTokensScell = 
                            ( dlLcQosInfo_p->availToken * 
                                    ueDLContext_p->bandWidthDistribution[scellIndex] ) / 100;
                    /* - SPR 11473 Changes */
                }
            }
        }
    }
}

/***********************************************************************
 * Function Name  : dlQosGetHighestQosPriorityAmongCells 
 * Inputs         : dlLcQosInfo_p,ueDLContext_p 
 * Outputs        : None
 * Returns        : highestQoSPriority 
 * Description    : 
 ***********************************************************************/
 UInt16 dlQosGetHighestQosPriorityAmongCells( DLUEContext* ueDLContext_p, 
                                                    DlLcQosInfo *dlLcQosInfo_p)
{
    UInt8 scellIndex = START_SCELL_INDEX;
    UInt16 highestMcsPriority = dlLcQosInfo_p->qosPriority[PCELL_SERV_CELL_IX];

    for (scellIndex = START_SCELL_INDEX ; scellIndex <= ueDLContext_p->scellCount; scellIndex++)
    {
        if(highestMcsPriority < dlLcQosInfo_p->qosPriority[scellIndex])
        {
            highestMcsPriority = dlLcQosInfo_p->qosPriority[scellIndex];
        }
    }
    return highestMcsPriority ;
}

/***********************************************************************
 * Function Name  : dlQosUpdateQosPriorityInAllCells 
 * Inputs         : ueDLContext_p,
 *                : dlLcQosInfo_p,
 *                : lcSpecificWt 
 *                : mcsWeight
 * Outputs        : dlLcQosInfo_p
 * Returns        : 
 * Description    : This function will update total priority of LC based 
 *                  on MCS reported on each cell.
 ***********************************************************************/
void  dlQosUpdateQosPriorityInAllCells( DLUEContext* ueDLContext_p, 
                                        DlLcQosInfo* dlLcQosInfo_p, 
                                        UInt32 lcSpecificWt,
                                        UInt8 mcsWeight)
{
    UInt8 pCellMcs = 0;
    UInt8 sCellMcs = 0;
    UInt8 scellIndex = START_SCELL_INDEX;
    UInt16 pCellMcsPriority = 0;
    UInt16 sCellMcsPriority = 0;

    pCellMcs = dlQosGetPCellMcs( ueDLContext_p );

    pCellMcsPriority = dlQosGetMcsPriority(pCellMcs);
    /* Index 0 in qosPriority[] is used only for Pcell */
    dlLcQosInfo_p->qosPriority[PCELL_SERV_CELL_IX] = (lcSpecificWt + (pCellMcsPriority * mcsWeight));
    /*loop for updating qosPriority for each scell*/ 
    for (scellIndex = START_SCELL_INDEX; scellIndex <= ueDLContext_p->scellCount; scellIndex++)
    {
        sCellMcs = dlQosGetSCellMcs(
                                 ueDLContext_p,
                                 ueDLContext_p->dlUeScellContext_p[scellIndex]->internalCellIndex);

        sCellMcsPriority = dlQosGetMcsPriority(sCellMcs);

        dlLcQosInfo_p->qosPriority[scellIndex] = (lcSpecificWt + (sCellMcsPriority * mcsWeight)); 
    }
    ueDLContext_p->highestMcsIndex = MAX_BETWEEN_TWO(pCellMcs, sCellMcs);
}

/* SPR 16116 fix start */
/***********************************************************************
 * Function Name  : dlQosGetLowestQosPriorityAmongCells 
 * Inputs         : dlLcQosInfo_p,ueDLContext_p 
 * Outputs        : None
 * Returns        : lowestQoSPriority 
 * Description    : 
 ***********************************************************************/
 UInt32 dlQosGetLowestQosPriorityAmongCells( DLUEContext* ueDLContext_p, 
                                                    DlLcQosInfo *dlLcQosInfo_p)
{
    UInt8 scellIndex = START_SCELL_INDEX;
    UInt16 lowestMcsPriority = dlLcQosInfo_p->qosPriority[PCELL_SERV_CELL_IX];

    for (scellIndex = START_SCELL_INDEX ; scellIndex <= ueDLContext_p->scellCount; scellIndex++)
    {
        if(lowestMcsPriority > dlLcQosInfo_p->qosPriority[scellIndex])
        {
            lowestMcsPriority = dlLcQosInfo_p->qosPriority[scellIndex];
        }
    }
    return lowestMcsPriority ;
}
/* SPR 16116 fix end */


    /********************** End of the File *************************************/
