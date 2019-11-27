/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacQosTokenAlgo.c,v 1.3.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
#include "lteMacULQosScheduler.h"
#include "lteMacDLQosTokenAlgo.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif
#include "lteMacCellConfMgr.h"
/* SPR 23402 Changes Start */ 
/* MOCN CHANGES START */
#include "lteMacOAMInterface.h"
 /* MOCN CHNAGES END */
/* SPR 23402 Changes End */

/******************************************************************************
  Private Definitions
 *****************************************************************************/


UInt16 total_ue = 0;
/*CA Changes start  */
STATIC TknUpdateUENodeMap tknUpdateUEArray_g[MAX_UE_SUPPORTED];

/* Global list of periodic array */
LTE_LIST* tokenPeriodicityListDl_gp[MAX_NUM_CELL];
LTE_LIST* tokenPeriodicityListUl_gp[MAX_NUM_CELL];

/* global List of periodicity array map */
LTE_LIST* tokenPeriodicityListMapDl_gp[MAX_NUM_CELL];
LTE_LIST* tokenPeriodicityListMapUl_gp[MAX_NUM_CELL];


STATIC QosToken qosToken_g[MAX_NUM_CELL];
/*CA Changes end  */

#ifdef TDD_CONFIG
/** CA-TDD Changes Start:06 **/
extern TddConfigInfo tddConfigInfo_g[MAX_NUM_CELL];
/** CA-TDD Changes End:06 **/
#endif
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
void populateMocnParams(MacInitReqParams *macInitReqParams_p);
void calculateMocnPRBUsageHistory(InternalCellIndex internalCellIndex);
mocnOperatorParams mocnOperatorParams_g;
/* MOCN Changes End */ 
/* SPR 23402 Changes End */
/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* + SPR 17439 */
void printMappingList(void);
void printTokenPeriodicityList(void);
/* - SPR 17439 */
static UInt16 qosProcessTknBktAlgoForUeAddForList(UInt16 ueIndex, 
        LTE_LIST tokenPeriodicityListMap, 
        LTE_LIST tokenPeriodicityList,
        TknUpdateUENode ** ueNode_pp);
static void qosProcessTknBktAlgoForUeDelForList(UInt16 tknPeriodicityArrIndx,
        LTE_LIST * tokenPeriodicityListMap_p, 
        LTE_LIST tokenPeriodicityList, 
        TknUpdateUENode *ueNode_p);
static void insertionSortOfMapingListUeDelete(UInt16 arrayIndx,
        LTE_LIST * tokenPeriodicityListMap_p);
static void insertionSortOfMapingListUeAdd(LTE_LIST * tokenPeriodicityListMap_p);
/* SPR 5599 changes start (ZIP ID 129147) */
STATIC void storeMaxDlBatchesForTokenPeriodicity(UInt16 tokenPeriodicity,
                                                 InternalCellIndex internalCellIndex);
STATIC void storeMaxUlBatchesForTokenPeriodicity(UInt16 tokenPeriodicity,
                                    InternalCellIndex internalCellIndex);
/*CA Changes start */
static UInt16 getUlBatchListIndex(InternalCellIndex internalCellIndex);
/*CA Changes end */
STATIC UInt16 getDlBatchListIndex(InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129147) */
/* QOS_OAM_MODIFY_CHANGES_start */
static void sortArrayMappingList(LTE_LIST *p_tokenPeriodicityListMap);
static void modifyGbrBucketDepthOfUEs( UInt8 dlGbrBktFactor, UInt8 ulGbrBktFactor);
static void modifyAmbrBucketDepthOfUEs(UInt8 dlAmbrBktFactor,UInt8 ulAmbrBktFactor);
STATIC void modifyTokenPeriodicity(InternalCellIndex internalCellIndex);
static void modifyGBRTBDTRAndTBLOfUEs(InternalCellIndex internalCellIndex);
static void modifyAMBRTBDTRAndTBLOfUEs(InternalCellIndex internalCellIndex);
/* Coverity 66974 66975 Start */
static MacRetType validate_qos_strategy_weights_in_modify_layer_req(QosStrategyWeights *qosStrategyWeights);
/* +- SPR 17777 */
extern MacRetType validateModifyQosParamters(MacModifyQosPamams *p_modifyPamams);
/* Coverity 66974 66975 End */
/* QOS_OAM_MODIFY_CHANGES_end */
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
 * Function Name  : dlQosProcessTokenUpdate
 * Inputs         : currentGlobalTick - current global tick,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This funtions fetches the UE's for whom token need to be
 *                  Updated. call the Token update function for DL
 *****************************************************************************/
/* SPR 15909 fix start */
/* +- SPR 17777 */
void dlQosProcessTokenUpdate(InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
{

    /* 1.   Get the token periodicity array index which UE list has to be updated
     * 2.   Get the UE list and number of UEs on token periodicity array index
     * 3.   Pop the UEs one by one from the UE list
     * 4.   call the DL token update functions for updating the tokens
     */

    UInt8 tknUpdtUeCnt = 0;
    UInt16 ueIndex = 0;
    UInt16 listIndex = 0;
    DLUEContext *ueDLContext_p = PNULL;
    DLUEContextInfo *ueDLContextInfo_p = PNULL;
    TknUpdateUENode *tknUpdateNode_p = PNULL;
    UInt16 count=0;
    /*CA Changes start  */
    TokenPeriodicityLisElem *list_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListDl_gp[internalCellIndex]->node.next);
    /*CA Changes end  */
    LTE_LIST *ueList_p = PNULL;


    /* get the index of token periodicity array */
    listIndex = getDlBatchListIndex(internalCellIndex);//(currentGlobalTick % dlSchdConfig_g.dlTokenPeriodicity);

    /* Get the List of UEs on this index */
    /* SPR# 3228 Changes Starts */
    for (count = 0; ((count <listIndex) && (list_p)); count++)
        /* SPR# 3228 Changes Ends */
    {
        list_p = (TokenPeriodicityLisElem *)(list_p->elemAnchor.next);
    }    
    /* SPR# 3228 Changes Starts */
    /* Coverity 5.0.1 ID:24914 fix start */
    if((count == listIndex) && (list_p))
    {    
        /* Coverity 5.0.1 ID:24914 fix End */
        /* SPR# 3228 Changes Ends */
    ueList_p = &(list_p->tokenList);
    
    tknUpdtUeCnt = ueList_p->count;
    for(count=0; count < tknUpdtUeCnt; count++)
    {
        /* Pop UE one by one from the list and process it */
        tknUpdateNode_p = (TknUpdateUENode*)getListNode(ueList_p, count);
        if(PNULL != tknUpdateNode_p)
        {
            ueIndex = tknUpdateNode_p->ueIndex;
        
            ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
            if((!ueDLContextInfo_p->pendingDeleteFlag) && (PNULL != ueDLContext_p))
            {
                /* + CA Changes */
                dlQosUpdateTokens(ueDLContext_p, internalCellIndex);
                /* - CA Changes */
            }
        }
	    }
    }
}/* End of Fucntion dlQosProcessTokenUpdate */

/*****************************************************************************
 * Function Name  : ulQosProcessTokenUpdate
 * Inputs         : currentGlobalTick - current global tick,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This funtions fetches the UE's for whom token need to be
 *                  Updated. call the Token update function for UL
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
/* +- SPR 17777 */
void ulQosProcessTokenUpdate(InternalCellIndex internalCellIndex,ExecutionLegFunctionArg* execArgs)
    /* +- SPR 17777 */
/* SPR 15909 fix end */
/*CA Changes end  */
{
    /* 1.   Get the token periodicity array index of which UE list has to be updated
     * 2.   Get the UE list and number of UEs on token periodicity array index
     * 3.   Pop the UEs one by one from the UE list
     * 4.   call the UL token update functions for updating the tokens
     */

    /* Get number of UE 's to be update for current tick */
    UInt8 tknUpdtUeCnt = 0;
    UInt16 ueIndex = 0;
    UInt16 listIndex = 0;
    ULUEContext *ueULContext_p = PNULL;
    ULUEContextInfo *ueULContextInfo_p = PNULL;
    TknUpdateUENode *tknUpdateNode_p = PNULL;
    UInt16 count=0;
    TokenPeriodicityLisElem *list_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListUl_gp[internalCellIndex]->node.next);
    LTE_LIST *ueList_p = PNULL; 

    /* get the start UE index */
    listIndex = getUlBatchListIndex(internalCellIndex);//(currentGlobalTick % dlSchdConfig_g.dlTokenPeriodicity); 
    
    /* Get the List of UEs on this index */
    /* SPR# 3228 Changes Starts */
    for (count = 0; ((count < listIndex) && (list_p)); count++)
        /* SPR# 3228 Changes Ends */
    {
        list_p = (TokenPeriodicityLisElem *)(list_p->elemAnchor.next);
    }    
    /* SPR# 3228 Changes Starts */
    /* Coverity 5.0.1 ID:24915 fix start */
    if((count == listIndex) && (list_p))
    {   
    /* Coverity 5.0.1 ID:24915 fix end*/
        ueList_p = &(list_p->tokenList);
        tknUpdtUeCnt = ueList_p->count;

        for(count=0; count < tknUpdtUeCnt; count++)
        {
            /* Pop UE one by one from the list and process it */
            tknUpdateNode_p = (TknUpdateUENode*)getListNode(ueList_p, count);

            if(PNULL != tknUpdateNode_p)
            {
                ueIndex = tknUpdateNode_p->ueIndex;

                ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
                ueULContext_p = ueULContextInfo_p->ulUEContext_p;

                if((!ueULContextInfo_p->pendingDeleteFlag)&&(PNULL != ueULContext_p))
                {
                    /*CA Changes start  */
                    ulQosUpdateTokens(ueULContext_p, internalCellIndex,execArgs);
                    /*CA Changes end  */
                }
            }   
        }
    }
    /* SPR# 3228 Changes Ends */
}

/*****************************************************************************
 * Function Name  : qosProcessTknBktAlgoForUeAdd
 * Inputs         : ueIndex, 
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This funtions Add the node in the UE list maitained for
 *                  token Update. Increments the number of UE's for which token
 *                  Updated to be done. Also sorts the token periodicity array
 *                  mapping list.
 *****************************************************************************/
/*CA Changes start  */
void qosProcessTknBktAlgoForUeAdd(UInt16 ueIndex, InternalCellIndex internalCellIndex)
{
    TknUpdateUENode *ueNode_p = PNULL;
    UInt16 tknPeriodicityArrIndx = qosToken_g[internalCellIndex].dlMaxBatches + qosToken_g[internalCellIndex].ulMaxBatches; //Initialize with invalid value
    
    tknPeriodicityArrIndx = qosProcessTknBktAlgoForUeAddForList(ueIndex, 
            *tokenPeriodicityListMapDl_gp[internalCellIndex], *tokenPeriodicityListDl_gp[internalCellIndex], &ueNode_p);
    
    /* Save the address of the UE node and token periodicity array index in update UE array */
    tknUpdateUEArray_g[ueIndex].ueNodeAddrDl_p = ueNode_p;
    tknUpdateUEArray_g[ueIndex].tknPeriodicityArrIndxDl = tknPeriodicityArrIndx;

    ueNode_p = PNULL;
    tknPeriodicityArrIndx = qosProcessTknBktAlgoForUeAddForList(ueIndex, 
            *tokenPeriodicityListMapUl_gp[internalCellIndex], *tokenPeriodicityListUl_gp[internalCellIndex], &ueNode_p);
    
    /* Save the address of the UE node and token periodicity array index in update UE array */
    tknUpdateUEArray_g[ueIndex].ueNodeAddrUl_p = ueNode_p;
    tknUpdateUEArray_g[ueIndex].tknPeriodicityArrIndxUl = tknPeriodicityArrIndx;
    
    insertionSortOfMapingListUeAdd(tokenPeriodicityListMapDl_gp[internalCellIndex]);
    insertionSortOfMapingListUeAdd(tokenPeriodicityListMapUl_gp[internalCellIndex]);
    
    (qosToken_g[internalCellIndex].tknUpdateUeCount)++;
}
/*CA Changes end  */

/*****************************************************************************
 * Function Name  : qosProcessTknBktAlgoForUeAddForList
 * Inputs         : ueIndex,
 *					tokenPeriodicityListMap,
 *					tokenPeriodicityList,
 * Outputs        : ueNode_pp - pointer to token update node 
 * Returns        : tknPeriodicityArrIndx 
 * Description    : This funtions Add the node in the UE list maitained for
 *                  token Update. Increments the number of UE's for which token
 *                  Updated to be done. Also sorts the token periodicity array
 *                  mapping list.
 *****************************************************************************/
static UInt16  qosProcessTknBktAlgoForUeAddForList(UInt16 ueIndex, 
        LTE_LIST tokenPeriodicityListMap, 
        LTE_LIST tokenPeriodicityList,
        TknUpdateUENode ** ueNode_pp)
{
    /*
     * Add the node In the List
     * Increment Number of UE in the List
     * Calculate the number of UE.s for which token update  
     * to be done in a tick (numTknUpdtUe_g)
     */

    /* Find the tail of mapping list */
    TokenPeriodicityMap *tail_p = (TokenPeriodicityMap *)tokenPeriodicityListMap.node.previous;
    /* Get the token periodicity array index */
    UInt16 tknPeriodicityArrIndx = tail_p->tknPeriodicityArrIndx;
    LTE_LIST *ueList_p = PNULL;
    UInt16 count=0;
    /* Get the List of UEs on this index */
    TokenPeriodicityLisElem *list_p = (TokenPeriodicityLisElem *)(tokenPeriodicityList.node.next);
    
    for (count = 0; count <tknPeriodicityArrIndx; count++)
    {
        list_p = (TokenPeriodicityLisElem *)(list_p->elemAnchor.next);
    }    
    ueList_p = &(list_p->tokenList);


    /* Create the UE node that has to be added int the UE list */

    GET_MEM_FROM_POOL(TknUpdateUENode,(*ueNode_pp),sizeof(TknUpdateUENode),PNULL);
    
    /* + Coverity 24546 */    
    if (PNULL != *ueNode_pp)
    {
        (*ueNode_pp)->ueIndex = ueIndex;     

        listInsertNode(ueList_p , &((*ueNode_pp)->tknUpdateAnchor));
    }
    /* - Coverity 24546 */    

    return tknPeriodicityArrIndx;        
}

/*****************************************************************************
 * Function Name  : qosProcessTknBktAlgoForUeDelete
 * Inputs         : ueIndex,
 *					internalCellIndex
 * Outputs        : updates the token UE updation count tknUpdateUeCount_g
 * Returns        : None
 * Description    : This funtions Delete the node in the UE list maitained for
 *                  token Update. Decrements the number of UE's for which token
 *                  Updated to be done. Also sorts the token periodicity mapping
 *                  list.
 *****************************************************************************/
/*CA Changes start  */
void qosProcessTknBktAlgoForUeDelete(UInt16 ueIndex, InternalCellIndex internalCellIndex)
{
    TknUpdateUENode * ueNode_p = tknUpdateUEArray_g[ueIndex].ueNodeAddrDl_p;
    UInt16 tknPeriodicityArrIndx = tknUpdateUEArray_g[ueIndex].tknPeriodicityArrIndxDl;
    
    qosProcessTknBktAlgoForUeDelForList(tknPeriodicityArrIndx,
            tokenPeriodicityListMapDl_gp[internalCellIndex], *tokenPeriodicityListDl_gp[internalCellIndex], ueNode_p);
    tknUpdateUEArray_g[ueIndex].ueNodeAddrDl_p = PNULL;
    
    ueNode_p = tknUpdateUEArray_g[ueIndex].ueNodeAddrUl_p;
    tknPeriodicityArrIndx = tknUpdateUEArray_g[ueIndex].tknPeriodicityArrIndxUl;
    
    qosProcessTknBktAlgoForUeDelForList(tknPeriodicityArrIndx, 
            tokenPeriodicityListMapUl_gp[internalCellIndex], *tokenPeriodicityListUl_gp[internalCellIndex], ueNode_p);
    tknUpdateUEArray_g[ueIndex].ueNodeAddrUl_p = PNULL;

    (qosToken_g[internalCellIndex].tknUpdateUeCount)--;
}
/*CA Changes end  */


/*****************************************************************************
 * Function Name  : qosProcessTknBktAlgoForUeDelForList 
 * Inputs         : tknPeriodicityArrIndx,
 *                  tokenPeriodicityListMap,
 *                  tokenPeriodicityList
 * Outputs        : ueNode_p - pointer to token update node 
 * Returns        : None 
 * Description    : This funtions Delete the node in the UE list maitained for
 *                  token Update. Decrements the number of UE's for which token
 *                  Updated to be done. Also sorts the token periodicity mapping
 *                  list.
 *****************************************************************************/
void qosProcessTknBktAlgoForUeDelForList(UInt16 tknPeriodicityArrIndx,
        LTE_LIST * tokenPeriodicityListMap_p, 
        LTE_LIST tokenPeriodicityList, 
        TknUpdateUENode *ueNode_p)
{
    /*
     * Get the token periodicity array index on which this UE has been added
     * using UEId mapping array
     * Delete the node from the List
     * decrement Number of UE in the List
     * Sort the token periodicity array mapping list
     */

    LTE_LIST *ueList_p = PNULL;

    /* Get the token periodicity array index on which this UE has been added */
    
    TokenPeriodicityLisElem *list_p = (TokenPeriodicityLisElem *)(tokenPeriodicityList.node.next);
    UInt16 count=0;

    /* Get the UE node address from the UE mapping array */

    /* Get the UE list from which this UE has to be deleted */
    for (count = 0; count <tknPeriodicityArrIndx; count++)
    {
        list_p = (TokenPeriodicityLisElem *)(list_p->elemAnchor.next);
    }    
    ueList_p = &(list_p->tokenList);
    
    if(PNULL != ueNode_p)
    {

        /* Delete the UE node from the list */
        listDeleteNode(ueList_p, &(ueNode_p->tknUpdateAnchor));

        /* Free the memory of the UE node */
        freeMemPool(ueNode_p);

        /* Sort the token periodicity array mapping list */
        insertionSortOfMapingListUeDelete(tknPeriodicityArrIndx, tokenPeriodicityListMap_p);

        /* Update the UEId mapping list */
    }
}

/****************************************************************************
 * Function Name  : initTokenPeriodicityArray
 * Inputs         : internalCellIndex
 * Outputs        : None 
 * Returns        : None
 * Description    : This API inits the peroidicity array and its mapping list 
 *                  confirmation if required. This is called at cell config
 ****************************************************************************/
/* CA Changes start  */
void initTokenPeriodicityArray(InternalCellIndex internalCellIndex)
{
    /* Local variable for malloc operation */
    TokenPeriodicityMap *tokenMap_p = PNULL;
    TokenPeriodicityLisElem  *elem_p = PNULL;
    UInt16 count = 0;

    storeMaxDlBatchesForTokenPeriodicity(cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->initParams_p->dlSchdConfig.dlTokenPeriodicity,
            internalCellIndex);
    storeMaxUlBatchesForTokenPeriodicity(cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->initParams_p->dlSchdConfig.dlTokenPeriodicity, 
            internalCellIndex);

    /* Start 128 UE: Memory Changes */
    /* For DL and UL */
    /* The mem pool will be created only first time when cell was configured */ 
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        createMemPool(sizeof (TokenPeriodicityLisElem), qosToken_g[internalCellIndex].dlMaxBatches);
        createMemPool(sizeof (TokenPeriodicityLisElem), qosToken_g[internalCellIndex].ulMaxBatches);
        createMemPool(sizeof (TokenPeriodicityMap), qosToken_g[internalCellIndex].dlMaxBatches);
        createMemPool(sizeof (TokenPeriodicityMap), qosToken_g[internalCellIndex].ulMaxBatches);
    }
    /* End 128 UE: Memory Changes */

    /* Initialize List in each Periodicity array */
    /* Rel 5.3.1: Memory related changes start*/
    listInit(tokenPeriodicityListDl_gp[internalCellIndex]);
    listInit(tokenPeriodicityListUl_gp[internalCellIndex]);
    /* Rel 5.3.1: Memory related changes end*/
    for(count=0; count < qosToken_g[internalCellIndex].dlMaxBatches; count++)
    {
        /* allocate contiguous memory for token pariodicity array */     
        GET_MEM_FROM_POOL(TokenPeriodicityLisElem, elem_p, sizeof(TokenPeriodicityLisElem), PNULL);

        /* + coverity 24545 */
        if (PNULL != elem_p)
        {
            listInit(&(elem_p->tokenList));
            listPushNode(tokenPeriodicityListDl_gp[internalCellIndex], &(elem_p->elemAnchor));
        }
        /* - coverity 24545 */
    }    

    for(count=0; count < qosToken_g[internalCellIndex].ulMaxBatches; count++)
    {
        /* allocate contiguous memory for token pariodicity array */     
        GET_MEM_FROM_POOL(TokenPeriodicityLisElem, elem_p, sizeof(TokenPeriodicityLisElem), PNULL);

        /* + coverity 24545 */
        if (PNULL != elem_p)
        {
            listInit(&(elem_p->tokenList));
            listPushNode(tokenPeriodicityListUl_gp[internalCellIndex], &(elem_p->elemAnchor));
        }
        /* - coverity 24545 */
    }


    /* Initialize the mapping list */
    listInit(tokenPeriodicityListMapDl_gp[internalCellIndex]);
    listInit(tokenPeriodicityListMapUl_gp[internalCellIndex]);

    /* Create the nodes to map the periodicity array elements and insert in mapping list */
    for(count=0; count < qosToken_g[internalCellIndex].dlMaxBatches; count++)
    {
        GET_MEM_FROM_POOL(TokenPeriodicityMap, tokenMap_p, sizeof(TokenPeriodicityMap), PNULL);

        /* + coverity 24545 */
        if (PNULL != tokenMap_p)
        {
            tokenMap_p->noOfUes=0;
            tokenMap_p->tknPeriodicityArrIndx=count;
            /* Insert the nodes in the list */
            listPushNode(tokenPeriodicityListMapDl_gp[internalCellIndex], &(tokenMap_p->node));
        }
        /* - coverity 24545 */
    }  

    for(count=0; count < qosToken_g[internalCellIndex].ulMaxBatches; count++)
    {
        GET_MEM_FROM_POOL(TokenPeriodicityMap, tokenMap_p, sizeof(TokenPeriodicityMap), PNULL);

        /* + coverity 24545 */
        if (PNULL != tokenMap_p)
        {
            tokenMap_p->noOfUes=0;
            tokenMap_p->tknPeriodicityArrIndx=count;
            /* Insert the nodes in the list */
            listPushNode(tokenPeriodicityListMapUl_gp[internalCellIndex], &(tokenMap_p->node));
        }
        /* - coverity 24545 */
    }  


    /* SPR7784 Fix Start */
    /* update the previousTokenPeriodicityValDl/UL_g variable to store current token
     * periodicity value and it will be used token periodicity value will be changed */
    qosToken_g[internalCellIndex].previousTokenPeriodicityValDl = qosToken_g[internalCellIndex].dlMaxBatches;
    qosToken_g[internalCellIndex].previousTokenPeriodicityValUl = qosToken_g[internalCellIndex].ulMaxBatches;
    /* SPR7784 Fix End */
}
/*CA Changes end  */

/****************************************************************************
 * Function Name  : insertionSortOfMapingListUeAdd
 * Inputs         : tokenPeriodicityListMap_p - pointer to token perodicity 
 *                  list node to insert 
 * Outputs        : None 
 * Returns        : Void
 * Description    : This API adds the ue in the mapping node and sorts this 
 *                  list using insertion sort.
 ****************************************************************************/
void insertionSortOfMapingListUeAdd(LTE_LIST * tokenPeriodicityListMap_p)
{
    TokenPeriodicityMap *tokenMap_p = (TokenPeriodicityMap *)tokenPeriodicityListMap_p->node.previous;
    TokenPeriodicityMap *tailPrev_p = PNULL; 

    /* Increment the number of UEs in last node */
    UInt16 noOfUEsInCurrNode = ++tokenMap_p->noOfUes;
    /* Delete node */
    listDeleteNode(tokenPeriodicityListMap_p , &(tokenMap_p->node));

    /* Find the node before which current node has to be inserted */
     TokenPeriodicityMap *tail_p = (TokenPeriodicityMap *)tokenPeriodicityListMap_p->node.previous;
     
     while (PNULL!=tail_p)
     {
        if (noOfUEsInCurrNode <= tail_p->noOfUes)
        {
            break;
        } 
        tailPrev_p = tail_p;
        tail_p = (TokenPeriodicityMap *)tail_p->node.previous;
     }
     /* Insert the node */
    if (PNULL == tail_p)
    {
        ylInsertBefore( tokenPeriodicityListMap_p, &(tailPrev_p->node), &(tokenMap_p->node));
    }    
    else
    {
        ylInsertAfter(tokenPeriodicityListMap_p, &(tail_p->node), &(tokenMap_p->node));
    }
}

/****************************************************************************
 * Function Name  : insertionSortOfMapingListUeDelete
 * Inputs         : arrayIndx
 *                  tokenPeriodicityListMap_p - pointer to token perodicity 
 *                  list node to insert 
 * Outputs        : None 
 * Returns        : None
 * Description    : This API deletes the ue in the mapping node and sorts this 
 *                  list using insertion sort.
 ****************************************************************************/
void insertionSortOfMapingListUeDelete(UInt16 arrayIndx,
        LTE_LIST * tokenPeriodicityListMap_p)
{
    TokenPeriodicityMap *tokenMap_p = (TokenPeriodicityMap *)tokenPeriodicityListMap_p->node.next;
    TokenPeriodicityMap *prevStartNode_p = PNULL;
    TokenPeriodicityMap *startNode_p = PNULL;

    /* Search for particular node */
    while (PNULL != tokenMap_p) 
    {
        if (arrayIndx == tokenMap_p->tknPeriodicityArrIndx)
        {
            break;
        }
        tokenMap_p = (TokenPeriodicityMap *)tokenMap_p->node.next;
    }
    /* SPR  3444 changes start */
    if(tokenMap_p == PNULL)
    {
        return;
    }
    /* SPR  3444 changes end */

    /* decrement the number of UEs */
    UInt16 noOfUEsInCurrNode = --tokenMap_p->noOfUes;
    
    startNode_p = (TokenPeriodicityMap *)tokenMap_p->node.next;
    /* Delete node */
    listDeleteNode(tokenPeriodicityListMap_p , &(tokenMap_p->node));

   
    while(PNULL != startNode_p)
    {
        if (noOfUEsInCurrNode >= startNode_p->noOfUes)
        {
            break;
        }
        prevStartNode_p = startNode_p;
        startNode_p = (TokenPeriodicityMap *)startNode_p->node.next;

    }

    if (PNULL == startNode_p)
    {    
        ylInsertAfter(tokenPeriodicityListMap_p, &(prevStartNode_p->node), &(tokenMap_p->node));
    }
    else
    {
        ylInsertBefore(tokenPeriodicityListMap_p, &(startNode_p->node), &(tokenMap_p->node));
    }    
}
/* QOS_OAM_MODIFY_CHANGES_start */
/****************************************************************************
 * Function Name  : expansionOfTokenPeriodicityArrayUl
 * Inputs         : internalCellIndex
 * Outputs        : None 
 * Returns        : None
 * Description    : This API expands the token periodicity UL array list and 
 *                  mapping list and sorts the mapping list.
 ****************************************************************************/
void expansionOfTokenPeriodicityArrayUl(InternalCellIndex internalCellIndex)
{
    /* SPR7784 Fix Start */
    /*CA Changes start  */
    UInt16 oldPeriodicityVal = qosToken_g[internalCellIndex].previousTokenPeriodicityValUl;
    UInt16 newPeriodicityVal = qosToken_g[internalCellIndex].ulMaxBatches;
    /*CA Changes end  */
    /* SPR7784 Fix End */
    UInt16 newMaxNoOfUesInOneList = MAX_UE_SUPPORTED/newPeriodicityVal;
    /* SPR9371 Fix Start*/
    if (0 == newMaxNoOfUesInOneList)
    {
        /* The Periodicity value should ideally be so configured that the
         * newMaxNoOfUesInOneList should not be 0*/  
        newMaxNoOfUesInOneList = 1;
    }
    /* SPR9371 Fix End*/
    UInt16 oldMaxNoOfUesInOneList = MAX_UE_SUPPORTED/oldPeriodicityVal;
    UInt16 noOfUesToBeShifted = 0;
    UInt16 tknPeriodicityArrIndx = 0;
    /* Local variable for malloc operation */
    TokenPeriodicityMap *tokenMap_p = PNULL;
    TokenPeriodicityLisElem  *elem_p = PNULL;
    UInt16 count = 0;
    UInt16 count1 = 0;
    UInt16 count2 = 0;
    TknUpdateUENode *ueNode_p = PNULL;
    /*CA Changes start  */
    TokenPeriodicityLisElem *list1_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListUl_gp[internalCellIndex]->node.next);
    TokenPeriodicityLisElem *list2_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListUl_gp[internalCellIndex]->node.previous);
    TokenPeriodicityMap *listMap1_p =(TokenPeriodicityMap*)(tokenPeriodicityListMapUl_gp[internalCellIndex]->node.next);
    TokenPeriodicityMap *listMap2_p = (TokenPeriodicityMap*)(tokenPeriodicityListMapUl_gp[internalCellIndex]->node.previous);
    /*CA Changes end  */
    LTE_LIST_NODE *node_p = PNULL;

    /* expand List in each Periodicity array */
    for(count=0; count < (newPeriodicityVal-oldPeriodicityVal); count++)
    {
        /* allocate memory for each token pariodicity array node */     
        GET_MEM_FROM_POOL(TokenPeriodicityLisElem, elem_p, sizeof(TokenPeriodicityLisElem), PNULL);

        /* + coverity 24544 */
        if (PNULL != elem_p)
        {
            listInit(&(elem_p->tokenList));
            /*CA Changes start  */
            listPushNode(tokenPeriodicityListUl_gp[internalCellIndex], &(elem_p->elemAnchor));
            /*CA Changes end  */
        }
        /* + coverity 24544 */
    }    

    /* expand the token periodicity array mapping list */
    for(count=0; count < (newPeriodicityVal-oldPeriodicityVal); count++)
    {
        GET_MEM_FROM_POOL(TokenPeriodicityMap, tokenMap_p, sizeof(TokenPeriodicityMap), PNULL);
        
        /* + coverity 24544 */
        if (PNULL != tokenMap_p)
        {
            tokenMap_p->noOfUes=0;
            /* SPR# 3228 Changes Starts */
            tokenMap_p->tknPeriodicityArrIndx= oldPeriodicityVal + count;
            /* SPR# 3228 Changes Ends */
            /* Insert the nodes in the list */
            /*CA Changes start  */
            listPushNode(tokenPeriodicityListMapUl_gp[internalCellIndex], &(tokenMap_p->node));
            /*CA Changes end  */
        }
        /* + coverity 24544 */
    }

   /* Adjust no. of UEs in each token periodicity array index so that UEs should not 
      be more than MAX UEs in allowed in the UE list according to new token periodicity */      

   /* move list2_p pointer to beggining of new added nodes */
   list2_p = (TokenPeriodicityLisElem *)(list2_p->elemAnchor.next);
   listMap2_p = (TokenPeriodicityMap *)(listMap2_p->node.next);
  
   tknPeriodicityArrIndx = oldPeriodicityVal;

   /* Start with 1st node of token periodicity array to shift the extra UEs to newly added nodes */
   for (count1 = 0; count1 < oldPeriodicityVal; count1++)
   {
       /* If older nodes have extra UEs then shift it */
       if (list1_p->tokenList.count > newMaxNoOfUesInOneList)
       {
           /* No. of UEs to be shifted */
           noOfUesToBeShifted = oldMaxNoOfUesInOneList - newMaxNoOfUesInOneList; 
           for (count2 = 0; count2 < noOfUesToBeShifted; count2++)
           {
               node_p = listPopNode(&(list1_p->tokenList));
               /* find the node where space is available to accomodate popped UE */
               /* SPR9371 Fix Start*/
               while (list2_p && list2_p->tokenList.count >= newMaxNoOfUesInOneList)
               {    
               /* SPR9371 Fix End*/
                   list2_p = (TokenPeriodicityLisElem *)(list2_p->elemAnchor.next);
                   listMap2_p = (TokenPeriodicityMap *)(listMap2_p->node.next);
                   tknPeriodicityArrIndx++;
               }    
               listPushNode(&(list2_p->tokenList), node_p);
               listMap2_p->noOfUes++;
               /* update the UE mapping list */

               /* +COVERITY 10692 */
               if (PNULL != node_p)
               {
                   ueNode_p = (TknUpdateUENode *)node_p;
                   tknUpdateUEArray_g[ueNode_p->ueIndex].tknPeriodicityArrIndxUl = tknPeriodicityArrIndx ;
               }
               /* -COVERITY 10692 */
           }
       }
       list1_p = (TokenPeriodicityLisElem *)(list1_p->elemAnchor.next);
   }  

   /* Modify the no of UEs in older nodes of token periodicity array mapping list */
   /*CA Changes start  */
   list1_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListUl_gp[internalCellIndex]->node.next);
   /*CA Changes end  */
   for (count1 = 0; count1 < oldPeriodicityVal; count1++)
   {
       /*CA Changes start  */
       listMap1_p = (TokenPeriodicityMap *)(tokenPeriodicityListMapUl_gp[internalCellIndex]->node.next);
       /*CA Changes end  */
       for (count2 = 0; count2 < oldPeriodicityVal; count2++)
       {
           if (count1 == listMap1_p->tknPeriodicityArrIndx)
           {
               listMap1_p->noOfUes = list1_p->tokenList.count;
               break;
           } 
           listMap1_p = (TokenPeriodicityMap *)(listMap1_p->node.next);
       } 
       list1_p = (TokenPeriodicityLisElem *)(list1_p->elemAnchor.next);
   }

   /*CA Changes start  */
   sortArrayMappingList(tokenPeriodicityListMapUl_gp[internalCellIndex]);
   /*CA Changes end  */
}    
/****************************************************************************
 * Function Name  : expansionOfTokenPeriodicityArrayDl
 * Inputs         : internalCellIndex
 * Outputs        : None 
 * Returns        : None
 * Description    : This API expands the token periodicity DL array list and 
 *                  mapping list and sorts the mapping list.
 ****************************************************************************/
void expansionOfTokenPeriodicityArrayDl(InternalCellIndex internalCellIndex)
{
    /* SPR7784 Fix Start */
    /*CA Changes start  */
    UInt16 oldPeriodicityVal = qosToken_g[internalCellIndex].previousTokenPeriodicityValDl;
    UInt16 newPeriodicityVal = qosToken_g[internalCellIndex].dlMaxBatches;
    /*CA Changes end  */
    /* SPR7784 Fix End */

    UInt16 newMaxNoOfUesInOneList = MAX_UE_SUPPORTED/newPeriodicityVal;
    /* SPR9371 Fix Start*/
    if (0 == newMaxNoOfUesInOneList)
    {
        /* The Periodicity value should ideally be so configured that the
         * newMaxNoOfUesInOneList should not be 0*/  
        newMaxNoOfUesInOneList = 1;
    }
    /* SPR9371 Fix End*/
    UInt16 oldMaxNoOfUesInOneList = MAX_UE_SUPPORTED/oldPeriodicityVal;
    UInt16 noOfUesToBeShifted = 0;
    UInt16 tknPeriodicityArrIndx = 0;
    /* Local variable for malloc operation */
    TokenPeriodicityMap *tokenMap_p = PNULL;
    TokenPeriodicityLisElem  *elem_p = PNULL;
    UInt16 count = 0;
    UInt16 count1 = 0;
    UInt16 count2 = 0;
    TknUpdateUENode *ueNode_p = PNULL;
    /*CA Changes start  */
    TokenPeriodicityLisElem *list1_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListDl_gp[internalCellIndex]->node.next);
    TokenPeriodicityLisElem *list2_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListDl_gp[internalCellIndex]->node.previous);
    TokenPeriodicityMap *listMap1_p =(TokenPeriodicityMap*)(tokenPeriodicityListMapDl_gp[internalCellIndex]->node.next);
    TokenPeriodicityMap *listMap2_p = (TokenPeriodicityMap*)(tokenPeriodicityListMapDl_gp[internalCellIndex]->node.previous);
    /*CA Changes end  */
    LTE_LIST_NODE *node_p = PNULL;

    /* expand List in each Periodicity array */
    for(count=0; count < (newPeriodicityVal-oldPeriodicityVal); count++)
    {
        /* allocate memory for each token pariodicity array node */     
        GET_MEM_FROM_POOL(TokenPeriodicityLisElem, elem_p, sizeof(TokenPeriodicityLisElem), PNULL);

        /* + coverity 24543 */
        if (PNULL != elem_p)
        {
            listInit(&(elem_p->tokenList));
            /*CA Changes start  */
            listPushNode(tokenPeriodicityListDl_gp[internalCellIndex], &(elem_p->elemAnchor));
            /*CA Changes end  */
        }
        /* + coverity 24543 */
    }    

    /* expand the token periodicity array mapping list */
    for(count=0; count < (newPeriodicityVal-oldPeriodicityVal); count++)
    {
        GET_MEM_FROM_POOL(TokenPeriodicityMap, tokenMap_p, sizeof(TokenPeriodicityMap), PNULL);
        
        /* + coverity 24543 */
        if (PNULL != tokenMap_p)
        {
            tokenMap_p->noOfUes=0;
            /* SPR# 3228 Changes Starts */
            tokenMap_p->tknPeriodicityArrIndx= oldPeriodicityVal + count;
            /* SPR# 3228 Changes Ends */
            /* Insert the nodes in the list */
            /*CA Changes start  */
            listPushNode(tokenPeriodicityListMapDl_gp[internalCellIndex], &(tokenMap_p->node));
            /*CA Changes end  */
        }
        /* + coverity 24543 */
    }

   /* Adjust no. of UEs in each token periodicity array index so that UEs should not 
      be more than MAX UEs in allowed in the UE list according to new token periodicity */      

   /* move list2_p pointer to beggining of new added nodes */
   list2_p = (TokenPeriodicityLisElem *)(list2_p->elemAnchor.next);
   listMap2_p = (TokenPeriodicityMap *)(listMap2_p->node.next);
    tknPeriodicityArrIndx = oldPeriodicityVal;

   /* Start with 1st node of token periodicity array to shift the extra UEs to newly added nodes */
   for (count1 = 0; count1 < oldPeriodicityVal; count1++)
   {
       /* If older nodes have extra UEs then shift it */
       if (list1_p->tokenList.count > newMaxNoOfUesInOneList)
       {
           /* No. of UEs to be shifted */
           noOfUesToBeShifted = oldMaxNoOfUesInOneList - newMaxNoOfUesInOneList; 
           for (count2 = 0; count2 < noOfUesToBeShifted; count2++)
           {
               node_p = listPopNode(&(list1_p->tokenList));
               /* find the node where space is available to accomodate popped UE */
               /* SPR9371 Fix Start*/
               while (list2_p && list2_p->tokenList.count >= newMaxNoOfUesInOneList)
               {    
               /* SPR9371 Fix End*/
                   list2_p = (TokenPeriodicityLisElem *)(list2_p->elemAnchor.next);
                   listMap2_p = (TokenPeriodicityMap *)(listMap2_p->node.next);
                   tknPeriodicityArrIndx++;
               }    
               listPushNode(&(list2_p->tokenList), node_p);
               listMap2_p->noOfUes++;
               /* update the UE mapping list */
               /* Coverity ID 10691 and 10692 start */
               if (PNULL != node_p)
               {
                   ueNode_p = (TknUpdateUENode *)node_p;
                   tknUpdateUEArray_g[ueNode_p->ueIndex].tknPeriodicityArrIndxDl = tknPeriodicityArrIndx ;
               }
               /* Coverity ID 10691 and 10692 end */
           }
       }
       list1_p = (TokenPeriodicityLisElem *)(list1_p->elemAnchor.next);
   }  

   /* Modify the no of UEs in older nodes of token periodicity array mapping list */
   /*CA Changes start  */
   list1_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListDl_gp[internalCellIndex]->node.next);
   /*CA Changes end  */
   for (count1 = 0; count1 < oldPeriodicityVal; count1++)
   {
       /*CA Changes start  */
       listMap1_p = (TokenPeriodicityMap *)(tokenPeriodicityListMapDl_gp[internalCellIndex]->node.next);
       /*CA Changes end  */
       for (count2 = 0; count2 < oldPeriodicityVal; count2++)
       {
           if (count1 == listMap1_p->tknPeriodicityArrIndx)
           {
               listMap1_p->noOfUes = list1_p->tokenList.count;
               break;
           } 
           listMap1_p = (TokenPeriodicityMap *)(listMap1_p->node.next);
       } 
       list1_p = (TokenPeriodicityLisElem *)(list1_p->elemAnchor.next);
   }

   /*CA Changes start  */
   sortArrayMappingList(tokenPeriodicityListMapDl_gp[internalCellIndex]);
   /*CA Changes end  */
}    
/****************************************************************************
 * Function Name  : contractionOfTokenPeriodicityArrayUl
 * Inputs         : internalCellIndex
 * Outputs        : None 
 * Returns        : None
 * Description    : This API expands the token periodicity array list and 
 *                  mapping list and sorts the mapping list.
 ****************************************************************************/
void contractionOfTokenPeriodicityArrayUl(InternalCellIndex internalCellIndex)
{
    /* SPR7784 Fix Start */
  /*SPR 9429 Fix Start*/  
    UInt16 oldPeriodicityVal = qosToken_g[internalCellIndex].previousTokenPeriodicityValUl;
  /*SPR 9429 Fix End*/  
    UInt16 newPeriodicityVal = qosToken_g[internalCellIndex].ulMaxBatches;
    /* SPR7784 Fix End */
    UInt16 newMaxNoOfUesInOneList = MAX_UE_SUPPORTED/newPeriodicityVal;
    /* SPR9371 Fix Start*/
    if (0 == newMaxNoOfUesInOneList)
    {
        /* The Periodicity value should ideally be so configured that the
         * newMaxNoOfUesInOneList should not be 0*/  
        newMaxNoOfUesInOneList = 1;
    }
    /* SPR9371 Fix End*/
    UInt16 noOfUesToBeShifted = 0;
    /* SPR 9429 Changes Start */
    UInt16 tknPeriodicityArrIndx = newPeriodicityVal-1;
    /* SPR 9429 Changes end */
    /* Local variable for malloc operation */
    UInt16 count1 = 0;
    UInt16 count2 = 0;
    /*CA Changes start  */
    TokenPeriodicityLisElem *list1_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListUl_gp[internalCellIndex]->node.next);
    TokenPeriodicityLisElem *list2_p = PNULL;
    TokenPeriodicityMap *listMap1_p = (TokenPeriodicityMap *)(tokenPeriodicityListMapUl_gp[internalCellIndex]->node.next);
    /*CA Changes end  */
    TokenPeriodicityLisElem *tempElem_p = PNULL;
    LTE_LIST_NODE *node_p = PNULL;
    TknUpdateUENode *ueNode_p = PNULL;
    TokenPeriodicityMap *tmp_p = PNULL;

   /* Adjust no. of UEs in each token periodicity array index so that UEs should not 
      be more than MAX UEs in allowed in the UE list according to new token periodicity */      



   /* Start with last node of token periodicity array from which to shift the UEs to remaining nodes*/ 
/*SPR9429 Fix Start*/
   for (count1 = 0; count1 < (oldPeriodicityVal - newPeriodicityVal); count1++)
/*SPRi9429 Fix End*/
   {
       /* If older nodes have extra UEs then shift it */
       /*CA Changes start  */
       list2_p = (TokenPeriodicityLisElem *)ylPopTail(tokenPeriodicityListUl_gp[internalCellIndex]);
       /*CA Changes end  */
       /* + Coverity 24590 */
       if ( PNULL == list2_p )
       {
           break;
       }
       /* - Coverity 24590 */
       tempElem_p = list2_p;
       if (list2_p->tokenList.count > 0)
       {
           /* No. of UEs to be shifted */
           noOfUesToBeShifted = list2_p->tokenList.count;           
               
           for (count2 = 0; count2 < noOfUesToBeShifted; count2++)
           {
               node_p = listPopNode(&(list2_p->tokenList));
               /* +COVERITY 10690 */
               if(PNULL == node_p)
               {
                   break;
               }
               /* -COVERITY 10690 */
               /* find the node where space is available to accomodate popped UE */
               /* SPR9371 Fix Start*/
               while (list1_p && list1_p->tokenList.count >= newMaxNoOfUesInOneList)
               {   
               /* SPR9371 Fix End*/    
                   tknPeriodicityArrIndx++;
                   list1_p = (TokenPeriodicityLisElem *)(list1_p->elemAnchor.next);
               }    
               listPushNode(&(list1_p->tokenList), node_p);
               /* update the UE mapping list */
               ueNode_p = (TknUpdateUENode *)node_p;
               tknUpdateUEArray_g[ueNode_p->ueIndex].tknPeriodicityArrIndxUl = tknPeriodicityArrIndx ;
           }
       }

       /* free the token periodicity array element as UEs have been shifted to remaining UE list */
       freeMemPool(tempElem_p);
       /* + SPR 5584 (COVERITY CID 25534) FIX */
       tempElem_p = PNULL;
       /* - SPR 5584 (COVERITY CID 25534) FIX */
   }  
       
   /* Delete those nodes of token periodicity array mapping list which corresponding 
    * token periodicity array element has been deleted in abaove step */ 
 /*SPR9429 Fix Start*/
   for (count2 = 0; count2 < oldPeriodicityVal; count2++)
 /*SPR9429 Fix End*/
   {
       /* if mapping list node found which stored the index of periodic array list, delete it */
 /*SPR9429 Fix Start*/
       if ((newPeriodicityVal-1) < listMap1_p->tknPeriodicityArrIndx)    
 /*SPR9429 Fix End*/
       {
           tmp_p = listMap1_p;
           listMap1_p = (TokenPeriodicityMap *)(listMap1_p->node.next);
           /*CA Changes start  */
           listDeleteNode(tokenPeriodicityListMapUl_gp[internalCellIndex], (LTE_LIST_NODE *)&(tmp_p->node));
           /*CA Changes end  */
           freeMemPool(tmp_p);
       }
       else
       {    
           listMap1_p = (TokenPeriodicityMap *)(listMap1_p->node.next);
       }    
   } 

   /* Modify the no of UEs in older nodes of token periodicity array mapping list */
   list1_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListUl_gp[internalCellIndex]->node.next);
 /*SPR9429 Fix Start*/
   for (count1 = 0; count1 < newPeriodicityVal; count1++)
   {
       listMap1_p =(TokenPeriodicityMap *)(tokenPeriodicityListMapUl_gp[internalCellIndex]->node.next);
       for (count2 = 0; count2 < newPeriodicityVal; count2++)
       {
	       /* SPR# 3228 Changes Starts */
 /*SPR9429 Fix End*/          
	       if(PNULL != listMap1_p)
	       {
		       /* SPR# 3228 Changes Ends */
           if (count1 == listMap1_p->tknPeriodicityArrIndx)
           {
               listMap1_p->noOfUes = list1_p->tokenList.count;
               break;
           } 
           listMap1_p = (TokenPeriodicityMap *)(listMap1_p->node.next);
	       }
       } 
       list1_p = (TokenPeriodicityLisElem *)(list1_p->elemAnchor.next);
   }

   /* sort the mapping list */
   /*CA Changes start  */
   sortArrayMappingList(tokenPeriodicityListMapUl_gp[internalCellIndex]);
   /*CA Changes end  */
}    
/****************************************************************************
 * Function Name  : contractionOfTokenPeriodicityArrayDl
 * Inputs         : internalCellIndex
 * Outputs        : None 
 * Returns        : None
 * Description    : This API expands the token periodicity array list and 
 *                  mapping list and sorts the mapping list.
 ****************************************************************************/
void contractionOfTokenPeriodicityArrayDl(InternalCellIndex internalCellIndex)
{
    /* SPR7784 Fix Start */
    UInt16 oldPeriodicityVal = qosToken_g[internalCellIndex].previousTokenPeriodicityValDl;
    UInt16 newPeriodicityVal =  qosToken_g[internalCellIndex].dlMaxBatches;


    /* SPR7784 Fix End */


    UInt16 newMaxNoOfUesInOneList = MAX_UE_SUPPORTED/newPeriodicityVal;
    UInt16 noOfUesToBeShifted = 0;
    /* Local variable for malloc operation */
    UInt16 count1 = 0;
    UInt16 count2 = 0;
    /*CA Changes start  */
    TokenPeriodicityLisElem *list1_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListDl_gp[internalCellIndex]->node.next);
    /*CA Changes end  */
    TokenPeriodicityLisElem *list2_p = PNULL;
    /*CA Changes start  */
    TokenPeriodicityMap *listMap1_p = (TokenPeriodicityMap *)(tokenPeriodicityListMapDl_gp[internalCellIndex]->node.next);
    /*CA Changes end  */
    TokenPeriodicityLisElem *tempElem_p = PNULL;
    LTE_LIST_NODE *node_p = PNULL;
    TknUpdateUENode *ueNode_p = PNULL;
    TokenPeriodicityMap *tmp_p = PNULL;

    /* SPR 9429 Changes Start */
    UInt16 tknPeriodicityArrIndx = newPeriodicityVal-1;
    /* SPR 9429 Changes end */
        /* SPR9371 Fix Start*/
    if (0 == newMaxNoOfUesInOneList)
    {
        /* The Periodicity value should ideally be so configured that the
         * newMaxNoOfUesInOneList should not be 0*/  
        newMaxNoOfUesInOneList = 1;
    }
    /* SPR9371 Fix End*/
   /* Adjust no. of UEs in each token periodicity array index so that UEs should not 
      be more than MAX UEs in allowed in the UE list according to new token periodicity */      



    /* Start with last node of token periodicity array from which to shift the UEs to remaining nodes */
/*SPR9429 Fix Start*/
    for (count1 = 0; count1 < (oldPeriodicityVal - newPeriodicityVal); count1++)
    {
/*SPR9429 Fix End*/        
        /* If older nodes have extra UEs then shift it */
        /*CA Changes start  */
        list2_p = (TokenPeriodicityLisElem *)ylPopTail(tokenPeriodicityListDl_gp[internalCellIndex]);
        /*CA Changes end  */
        /* SPR# 3228 Changes Starts */
        if(PNULL!= list2_p)
        {
            /* SPR# 3228 Changes Ends */
            tempElem_p = list2_p;
            if (list2_p->tokenList.count > 0)
            {
                /* No. of UEs to be shifted */
                noOfUesToBeShifted = list2_p->tokenList.count;           

                for (count2 = 0; count2 < noOfUesToBeShifted; count2++)
                {
                    node_p = listPopNode(&(list2_p->tokenList));
                    /* +COVERITY 10689 */
                    if(PNULL == node_p)
                    {
                        break;
                    }
                    /* -COVERITY 10689 */
                    /* find the node where space is available to accomodate popped UE */
                    /* SPR9371 Fix Start*/
                    while (list1_p && list1_p->tokenList.count >= newMaxNoOfUesInOneList)
                    {   
                    /* SPR9371 Fix End*/
                        tknPeriodicityArrIndx++;
                        list1_p = (TokenPeriodicityLisElem *)(list1_p->elemAnchor.next);
                    }    
                    listPushNode(&(list1_p->tokenList), node_p);
                    /* update the UE mapping list */
                    ueNode_p = (TknUpdateUENode *)node_p;
                    tknUpdateUEArray_g[ueNode_p->ueIndex].tknPeriodicityArrIndxDl = tknPeriodicityArrIndx ;
                }
            }
            /* +COVERITY 10904 */    
            /* + SPR 5584 (COVERITY CID 25534) FIX */
            /* move to the next UE list from which UEs have to be moved */
            list2_p = (TokenPeriodicityLisElem *)(list2_p->elemAnchor.next);

            /* free the token periodicity array element as UEs have been shifted to remaining UE list */
            freeMemPool(tempElem_p);
            tempElem_p = PNULL;
            /* - SPR 5584 (COVERITY CID 25534) FIX */
            /* -COVERITY 10904 */    
        }
   }  
       
   /* Delete those nodes of token periodicity array mapping list which corresponding 
    * token periodicity array element has been deleted in abaove step */ 
   for (count2 = 0; count2 < oldPeriodicityVal; count2++)
   {
       /* SPR7784 Fix Start */ 
       if(PNULL == listMap1_p)
       {
           break;
       }
       /* SPR7784 Fix End */ 
   
    /*SPR9429 Fix Start*/
       /* if mapping list node found which stored the index of periodic array list, delete it */
       if ((newPeriodicityVal-1) < listMap1_p->tknPeriodicityArrIndx)    
       {
    /*SPR9429 Fix End*/	 
           tmp_p = listMap1_p;
           listMap1_p = (TokenPeriodicityMap *)(listMap1_p->node.next);
           /*CA Changes start  */
           listDeleteNode(tokenPeriodicityListMapDl_gp[internalCellIndex], (LTE_LIST_NODE *)&(tmp_p->node));
           /*CA Changes end  */
           freeMemPool(tmp_p);
       }
       else
       {    
           listMap1_p = (TokenPeriodicityMap *)(listMap1_p->node.next);
       }    
   } 

   /* Modify the no of UEs in older nodes of token periodicity array mapping list */
   list1_p = (TokenPeriodicityLisElem *)(tokenPeriodicityListDl_gp[internalCellIndex]->node.next);
  /*SPR9429 Fix Start*/
   for (count1 = 0; count1 < newPeriodicityVal; count1++)
  /*SPR9429 Fix End*/     
   {
       listMap1_p =(TokenPeriodicityMap *)(tokenPeriodicityListMapDl_gp[internalCellIndex]->node.next);
      /*SPR9429 Fix Start*/  
       for (count2 = 0; count2 < newPeriodicityVal; count2++)
       /*SPR9429 Fix End*/     
       {
           /* SPR# 3228 Changes Starts */
           if(PNULL != listMap1_p)
           {
               /* SPR# 3228 Changes Ends */
               if (count1 == listMap1_p->tknPeriodicityArrIndx)
               {
                   listMap1_p->noOfUes = list1_p->tokenList.count;
                   break;
               } 
               listMap1_p = (TokenPeriodicityMap *)(listMap1_p->node.next);
           } 
       } 
       list1_p = (TokenPeriodicityLisElem *)(list1_p->elemAnchor.next);
   }

   /* sort the mapping list */
   /*CA Changes start  */
   sortArrayMappingList(tokenPeriodicityListMapDl_gp[internalCellIndex]);
   /*CA Changes end  */
}    
/****************************************************************************
 * Function Name  : sortArrayMappingList
 * Inputs         : p_tokenPeriodicityListMap
 * Outputs        : None 
 * Returns        : None
 * Description    : This sorts the token periodicity array mapping list.
 ****************************************************************************/
void sortArrayMappingList(LTE_LIST *p_tokenPeriodicityListMap)
{
    TokenPeriodicityMap *tokenMap_p = PNULL;
    TokenPeriodicityMap *tailPrev_p = PNULL;
    TokenPeriodicityMap *tail_p = PNULL;
    LTE_LIST temp_list;

    /* Init the temp list */
    listInit(&temp_list);

    /* Pop the node one by one from the mapping list and insert it into the temp list in  descending order */
    while (PNULL != (tokenMap_p = (TokenPeriodicityMap *)listPopNode(p_tokenPeriodicityListMap)))
    {   
        /* Start the insertion in temp list from the tail */
        /* Find the node before which current node has to be inserted */
        tail_p = (TokenPeriodicityMap *)temp_list.node.previous;
     
         while (PNULL!=tail_p)
         {
            if (tokenMap_p->noOfUes <= tail_p->noOfUes)
            {
                break;
            } 
            tailPrev_p = tail_p;
            tail_p = (TokenPeriodicityMap *)tail_p->node.previous;
         }
         /* Insert the node */
        if (PNULL == tail_p)
        {
            ylInsertBefore( &temp_list, &(tailPrev_p->node), &(tokenMap_p->node));
        }    
        else
        {
            ylInsertAfter(&temp_list, &(tail_p->node), &(tokenMap_p->node));
        }
    }
    p_tokenPeriodicityListMap->node.next = temp_list.node.next;
    p_tokenPeriodicityListMap->node.previous = temp_list.node.previous;
    p_tokenPeriodicityListMap->count = temp_list.count;
}    
/* QOS_OAM_MODIFY_CHANGES_end */


/****************************************************************************
 * Function Name  : storeMaxDlBatchesForTokenPeriodicity 
 * Inputs         : tokenPeriodicity - token periodicity to store in batch,
 *					internalCellIndex 
 * Outputs        : None
 * Returns        : None
 * Description    : This function stores max UL batches of token periodicity 
 ****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129147) */
/*CA Changes start  */
void storeMaxDlBatchesForTokenPeriodicity(UInt16 tokenPeriodicity,
                             InternalCellIndex internalCellIndex)
/* SPR 5599 changes end (ZIP ID 129147) */
{
#ifdef FDD_CONFIG
    qosToken_g[internalCellIndex].dlMaxBatches = tokenPeriodicity;
#elif TDD_CONFIG
    /** CA-TDD Changes Start:06 **/
    qosToken_g[internalCellIndex].dlMaxBatches = (tddConfigInfo_g[internalCellIndex].noOfDlSchedulable)*(tokenPeriodicity/MAX_SUB_FRAME);
    /** CA-TDD Changes End:06 **/
#endif
}
/*CA Changes end  */

/****************************************************************************
 * Function Name  : storeMaxUlBatchesForTokenPeriodicity 
 * Inputs         : tokenPeriodicity - token periodicity to store in batch,
 *					internalCellIndex 
 * Outputs        : None
 * Returns        : None
 * Description    : This function stores max UL batches of token periodicity 
 ****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129147) */
/*CA Changes start  */
void storeMaxUlBatchesForTokenPeriodicity(UInt16 tokenPeriodicity,
                                          InternalCellIndex internalCellIndex)
/* SPR 5599 changes end (ZIP ID 129147) */
{
#ifdef FDD_CONFIG
    qosToken_g[internalCellIndex].ulMaxBatches = tokenPeriodicity;
#elif TDD_CONFIG
    /** CA-TDD Changes Start:06 **/
    qosToken_g[internalCellIndex].ulMaxBatches = (tddConfigInfo_g[internalCellIndex].noOfUlSchedulable)*(tokenPeriodicity/MAX_SUB_FRAME);
    /** CA-TDD Changes End:06 **/
#endif
}
/*CA Changes end  */


/****************************************************************************
 * Function Name  : getUlBatchListIndex 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : ulBatchListIndex_g 
 * Description    : This function returns the UL batch list index in
 *                  token periodicity list content.
 ****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129147) */
/*CA Changes start  */
UInt16 getUlBatchListIndex(InternalCellIndex internalCellIndex)
/* SPR 5599 changes end (ZIP ID 129147) */
{
    UInt16 retVal = qosToken_g[internalCellIndex].ulBatchListIndex;
    qosToken_g[internalCellIndex].ulBatchListIndex = (qosToken_g[internalCellIndex].ulBatchListIndex + 1)%qosToken_g[internalCellIndex].ulMaxBatches;
    return retVal;
}
/*CA Changes end  */


/****************************************************************************
 * Function Name  : getDlBatchListIndex 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : dlBatchListIndex_g 
 * Description    : This function returns the DL batch list index in
 *                  token periodicity list content.
 ****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129147) */
/*CA Changes start  */
UInt16 getDlBatchListIndex(InternalCellIndex internalCellIndex)
/* SPR 5599 changes end (ZIP ID 129147) */
{
    UInt16 retVal = qosToken_g[internalCellIndex].dlBatchListIndex;
    qosToken_g[internalCellIndex].dlBatchListIndex = 
    (qosToken_g[internalCellIndex].dlBatchListIndex + 1)%qosToken_g[internalCellIndex].dlMaxBatches;
    return retVal;
}

/*CA Changes end  */

/** SPR 13173 Fix : Start **/
/* QOS_OAM_MODIFY_CHANGES_start */
/****************************************************************************
 * Function Name  : validate_qos_strategy_weights_in_modify_layer_req 
 * Inputs         : modified QoS parameter's value,
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : This function validates the qos parameters of MAC_MODIFY_LAYER_REQ.
 ****************************************************************************/
/* CA changes start */
/* Coverity 66974 66975 Start */
static MacRetType validate_qos_strategy_weights_in_modify_layer_req(QosStrategyWeights *qosStrategyWeights)
/* Coverity 66974 66974 End */
{
    if (qosStrategyWeights->pdbWeight > MAX_PDB_WEIGHT)
    {
        return MAC_FAILURE;
    }

    if (qosStrategyWeights->qciWeight > MAX_QCI_WEIGHT)
    {
        return MAC_FAILURE;
    }    

    if (qosStrategyWeights->tokenWeight > MAX_TOKEN_WEIGHT)\
    {
        return MAC_FAILURE;
    } 

    if (qosStrategyWeights->qLoadWeight > MAX_QLOAD_WEIGHT)\
    {
        return MAC_FAILURE;
    } 

    if (qosStrategyWeights->mcsWeight > MAX_MCS_WEIGHT)\
    {
        return MAC_FAILURE;
    }  
    return MAC_SUCCESS;
}
/* QOS_OAM_MODIFY_CHANGES_start */
/****************************************************************************
 * Function Name  : validateQosModifyQosParameters
 * Inputs         : modified QoS parameter's value,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : This function validates the qos parameters of MAC_MODIFY_LAYER_REQ.
 ****************************************************************************/
/* CA changes start */
/* Coverity 66974 66975 Start */
/* +- SPR 17777 */
MacRetType validateModifyQosParamters(MacModifyQosPamams *p_modifyPamams)
/* Coverity 66974 66975 End */
{  
     MacRetType resp = MAC_SUCCESS;
     /* SPR 14378 Changes Start */
     /*Parse Qos related params*/
     if (p_modifyPamams->modBitMap & QOS_UL_STRATEGY_WT_BIT)
     {
         resp = validate_qos_strategy_weights_in_modify_layer_req(
                 &(p_modifyPamams->ulQosStrategyWeights));
         if (MAC_FAILURE == resp) 
         {
             return MAC_FAILURE;
         }
     }
     if (p_modifyPamams->modBitMap & QOS_DL_STRATEGY_WT_BIT)
     {
         resp = validate_qos_strategy_weights_in_modify_layer_req(
                 &(p_modifyPamams->dlQosStrategyWeights));
         if (MAC_FAILURE == resp) 
         {
             return MAC_FAILURE;
         }
     }
     if (p_modifyPamams->modBitMap & QOS_OVERALLOC_FACTOR_BIT)
     {
    /*+- SPR 20896 Fix*/
         if (MAX_QOS_STRATEGY_OVER_ALLOC_FACTOR < p_modifyPamams->qosStrategyOverallocFactor)
    /*+- SPR 20896 Fix*/
         {
             return MAC_FAILURE;
         }
     }
     if (p_modifyPamams->modBitMap &  QOS_AMBR_BUCKET_DEPTH_FACTOR_BIT)
     {
         if ((MAX_AMBR_RKT_DEPTH_FACTOR < p_modifyPamams->ambrBktDepthFactor) ||
                 (MIN_AMBR_RKT_DEPTH_FACTOR > p_modifyPamams->ambrBktDepthFactor)) 
         { 
             return MAC_FAILURE;
         }
     }
     if (p_modifyPamams->modBitMap & QOS_GBR_BUCKET_DEPTH_FACTOR_BIT)
     {
         if ((MAX_GBR_RKT_DEPTH_FACTOR < p_modifyPamams->gbrBktDepthFactor) ||
                 (MIN_GBR_RKT_DEPTH_FACTOR > p_modifyPamams->gbrBktDepthFactor))
         {
             return MAC_FAILURE;
         }
     }
     if (p_modifyPamams->modBitMap & QOS_TOKEN_PERIODICITY_BIT)
     {
         if ((MAX_TOKEN_PERIODICITY < p_modifyPamams->tokenPeriodicity) ||
                 (MIN_TOKEN_PERIODICITY > p_modifyPamams->tokenPeriodicity))    
         {
             return MAC_FAILURE;

         }
     }
     if (p_modifyPamams->modBitMap & QOS_MODIFY_RETRAVERSAL_FLAG_BIT)
     {
         /* SPR 22539 Fix + */
         /* SPR 22579 Coverity Changes + */
         if(ENABLE_ALLOW_ALLOC_BITRATE_TOKEN_BASED < p_modifyPamams->enableAllocAfterAllowedBitrate) 
         /* SPR 22579 Coverity Changes - */
         {
             /* SPR 22539 Fix - */
             return MAC_FAILURE;
         }
     }    
     /* SPR 14378 Changes End */
     return MAC_SUCCESS;
}
/** SPR 13173 Fix : End **/
/*CA Changes end  */
/* QOS_OAM_MODIFY_CHANGES_start */
/****************************************************************************
 * Function Name  : qosModifyQosParameters
 * Inputs         : modified QoS parameter's value,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : This function modifies the qos parameters.
 ****************************************************************************/
/* CA changes start */
/* Coverity 66974 66975 Start */
MacRetType qosModifyQosParameters(MacModifyQosPamams *p_modifyPamams,
                              InternalCellIndex internalCellIndex)
/* Coverity 66974 66975 End */
{  
     DLSchedulerStrategyConfig *dlSchdConfig_p =  &cellSpecificParams_g.\
       cellConfigAndInitParams_p[internalCellIndex]->initParams_p->dlSchdConfig;
     ULSchedulerStrategyConfig *ulSchdConfig_p =  &cellSpecificParams_g.\
       cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig;
    
    if (p_modifyPamams->modBitMap & QOS_GBR_BUCKET_DEPTH_FACTOR_BIT)
    {
        dlSchdConfig_p->gbrBktFactor = p_modifyPamams->gbrBktDepthFactor;
        ulSchdConfig_p->gbrBktFactor = p_modifyPamams->gbrBktDepthFactor;

        modifyGbrBucketDepthOfUEs(dlSchdConfig_p->gbrBktFactor,
                             ulSchdConfig_p->gbrBktFactor);
    }
    if (p_modifyPamams->modBitMap &  QOS_AMBR_BUCKET_DEPTH_FACTOR_BIT)
    {
        dlSchdConfig_p->ambrBktFactor = p_modifyPamams->ambrBktDepthFactor;
        ulSchdConfig_p->ambrBktFactor = p_modifyPamams->ambrBktDepthFactor;

        modifyAmbrBucketDepthOfUEs(dlSchdConfig_p->ambrBktFactor,
                                ulSchdConfig_p->ambrBktFactor);
    }
    if (p_modifyPamams->modBitMap & QOS_TOKEN_PERIODICITY_BIT)
    {
        dlSchdConfig_p->dlTokenPeriodicity = p_modifyPamams->tokenPeriodicity;
        ulSchdConfig_p->ulTokenPeriodicity = p_modifyPamams->tokenPeriodicity;    

        modifyTokenPeriodicity(internalCellIndex);
    }
    if (p_modifyPamams->modBitMap & QOS_OVERALLOC_FACTOR_BIT)
    {

        dlSchdConfig_p->strategyBufferRB = (((cellSpecificParams_g.cellConfigAndInitParams_p\
             [internalCellIndex]->cellParams_p->dlAvailableRBs) * 
             p_modifyPamams->qosStrategyOverallocFactor)/100);
        ulSchdConfig_p->strategyBufferRB = (((cellSpecificParams_g.\
             cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs) *
                    p_modifyPamams->qosStrategyOverallocFactor)/100);
    }
    if (p_modifyPamams->modBitMap & QOS_DL_STRATEGY_WT_BIT)
    {
        dlSchdConfig_p->dlQosStrategyWeights.pdbWeight = p_modifyPamams->dlQosStrategyWeights.pdbWeight;
        dlSchdConfig_p->dlQosStrategyWeights.qciWeight = p_modifyPamams->dlQosStrategyWeights.qciWeight;
        dlSchdConfig_p->dlQosStrategyWeights.tokenWeight = p_modifyPamams->dlQosStrategyWeights.tokenWeight;
        dlSchdConfig_p->dlQosStrategyWeights.qLoadWeight = p_modifyPamams->dlQosStrategyWeights.qLoadWeight;
        dlSchdConfig_p->dlQosStrategyWeights.mcsWeight = p_modifyPamams->dlQosStrategyWeights.mcsWeight;
    }
    if (p_modifyPamams->modBitMap & QOS_UL_STRATEGY_WT_BIT)
    {
        ulSchdConfig_p->ulQosStrategyWeights.pdbWeight = p_modifyPamams->ulQosStrategyWeights.pdbWeight;
        ulSchdConfig_p->ulQosStrategyWeights.qciWeight = p_modifyPamams->ulQosStrategyWeights.qciWeight;
        ulSchdConfig_p->ulQosStrategyWeights.tokenWeight = p_modifyPamams->ulQosStrategyWeights.tokenWeight;
        ulSchdConfig_p->ulQosStrategyWeights.qLoadWeight = p_modifyPamams->ulQosStrategyWeights.qLoadWeight;
        ulSchdConfig_p->ulQosStrategyWeights.mcsWeight = p_modifyPamams->ulQosStrategyWeights.mcsWeight;
    }
    if(p_modifyPamams->modBitMap & QOS_MODIFY_RETRAVERSAL_FLAG_BIT)
    {
       cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
         initParams_p->enableAllocAfterAllowedBitrate = p_modifyPamams->enableAllocAfterAllowedBitrate; 
    }
    return MAC_SUCCESS;
}    

/* CA changes end */
/****************************************************************************
 * Function Name  : modifyGbrBucketDepthOfUEs 
 * Inputs         : dlGbrBktFactor,
 *					ulGbrBktFactor
 * Outputs        : None
 * Returns        : None
 * Description    : This function modifies the GBR Bucket Depth qos parameters.
 ****************************************************************************/
void modifyGbrBucketDepthOfUEs( UInt8 dlGbrBktFactor, UInt8 ulGbrBktFactor)
{
    UInt16 index;
    UInt16 lcId = 0;

    for(index = 0; index < MAX_UE_SUPPORTED; index++)
    {
       /* UL, Change tokenDepth for each valid GBR LC */  
        if (NULL != ulUECtxInfoArr_g[index].ulUEContext_p)
        {
            if (FALSE == ulUECtxInfoArr_g[index].pendingDeleteFlag)
            {
                for (lcId = 0; lcId < MAX_LOGICAL_CHANNEL; lcId++)
                {
                    if (LCGID_INVALID_VALUE != ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].lcGId)
                    {
                        if (QOS_GBR == ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].ulQosInfo.channelType)
                        {
                            ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].ulQosInfo.tokenDepth =
                                ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].ulQosInfo.tokenRate *  ulGbrBktFactor;
                        }    
                    }        
                }   
            } 
        }

       /* DL, Change tokenDepth for each valid GBR LC */  
        if (NULL != dlUECtxInfoArr_g[index].dlUEContext_p)
        {
            if (FALSE == dlUECtxInfoArr_g[index].pendingDeleteFlag)
            {
                for (lcId = 0; lcId < MAX_LOGICAL_CHANNEL; lcId++)
                {
                    if (INVALID_LCID != dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].logicalChannelId)
                    {
                        if (QOS_GBR == dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].dlLcQosInfo.channelType)
                        {
                            dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].dlLcQosInfo.tokenDepth = 
                                dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].dlLcQosInfo.tokenRate * dlGbrBktFactor;
                        }    
                    }    
                }    
            }    

        }    
    }  
}   
/****************************************************************************
 * Function Name  : modifyAmbrBucketDepthOfUEs
 * Inputs         : dlAmbrBktFactor, 
 *					ulAmbrBktFactor
 * Outputs        : None
 * Returns        : None
 * Description    : This function modifies the AMBR Bucket Depth qos parameters.
 ****************************************************************************/
void modifyAmbrBucketDepthOfUEs( UInt8 dlAmbrBktFactor,UInt8 ulAmbrBktFactor)
{
    
    UInt16 index;
    
    for(index = 0; index < MAX_UE_SUPPORTED; index++)
    {
       /* UL, Change tokenDepth for AMBR LC */  
        if (NULL != ulUECtxInfoArr_g[index].ulUEContext_p)
        {
            if (FALSE == ulUECtxInfoArr_g[index].pendingDeleteFlag)
            {
                ulUECtxInfoArr_g[index].ulUEContext_p->ulAmbrInfo.ambrTokenDepth = 
                    ulUECtxInfoArr_g[index].ulUEContext_p->ulAmbrInfo.ambrTokenRate * ulAmbrBktFactor;
            } 
        }

       /* DL, Change tokenDepth for AMBR LC */  
        if (NULL != dlUECtxInfoArr_g[index].dlUEContext_p)
        {
            if (FALSE == dlUECtxInfoArr_g[index].pendingDeleteFlag)
            {
                dlUECtxInfoArr_g[index].dlUEContext_p->dlAmbrInfo.ambrTokenDepth = 
                         dlUECtxInfoArr_g[index].dlUEContext_p->dlAmbrInfo.ambrTokenRate * dlAmbrBktFactor;
            }    
        }    
    }  
}   

/****************************************************************************
 * Function Name  : modifyTokenPeriodicity
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function modifies token periodicity qos parameters.
 ****************************************************************************/
void modifyTokenPeriodicity(InternalCellIndex internalCellIndex)
{
   /* SPR7784 Fix Start */ 
   InitParams *initParams_p = cellSpecificParams_g.\
                   cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
   storeMaxDlBatchesForTokenPeriodicity(initParams_p->dlSchdConfig.dlTokenPeriodicity,
                                        internalCellIndex);
   storeMaxUlBatchesForTokenPeriodicity(initParams_p->dlSchdConfig.dlTokenPeriodicity, 
                                         internalCellIndex);
   /* SPR7784 Fix End */ 

   /* SPR7784 Fix Start */ 
   /*CA Changes start  */
   if (qosToken_g[internalCellIndex].previousTokenPeriodicityValDl <
       initParams_p->ulSchdConfig.ulTokenPeriodicity)
   /*CA Changes end  */
   /* SPR7784 Fix End */ 
   {
       /* +COVERITY 10225 */				
       expansionOfTokenPeriodicityArrayDl(internalCellIndex);  
       /* -COVERITY 10225 */				
       /* +COVERITY 10226 */				
       expansionOfTokenPeriodicityArrayUl(internalCellIndex);  
       /* -COVERITY 10226 */				
   }   
   else
   {
      contractionOfTokenPeriodicityArrayDl(internalCellIndex);
      contractionOfTokenPeriodicityArrayUl(internalCellIndex);
   }   
   
    /* update the previousTokenPeriodicityValDl/Ul_g variable to store current token 
    * periodicity value and it will be used token periodicity value will be changed */
   /* SPR7784 Fix Start */ 
    /*CA Changes start  */
    qosToken_g[internalCellIndex].previousTokenPeriodicityValDl = qosToken_g[internalCellIndex].dlMaxBatches;
    qosToken_g[internalCellIndex].previousTokenPeriodicityValUl = qosToken_g[internalCellIndex].ulMaxBatches;
    /*CA Changes end  */
   /* SPR7784 Fix End */ 

    /* Modify GBR token rate, token bucket depth and token bucket limit of all UEs */
    modifyGBRTBDTRAndTBLOfUEs(internalCellIndex);
    /* Modify AMBR token rate, token bucket depth and token bucket limit of all UEs */
    modifyAMBRTBDTRAndTBLOfUEs(internalCellIndex);
}
/****************************************************************************
 * Function Name  : modifyGBRTBDTRAndTBLOfUEs 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function modifies the GBR rate, GBR Bucket Depth 
 *                  GBR token limit on changing token periodicity.
 ****************************************************************************/
/* CA Changes start */
void modifyGBRTBDTRAndTBLOfUEs(InternalCellIndex internalCellIndex)
{
    UInt16 index;
    UInt16 lcId = 0;

   InitParams *initParams_p = cellSpecificParams_g.\
                   cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    for(index = 0; index < MAX_UE_SUPPORTED; index++)
    {
       /* UL, Change tokenDepth for each valid GBR LC */  
        if (NULL != ulUECtxInfoArr_g[index].ulUEContext_p)
        {
            if (FALSE == ulUECtxInfoArr_g[index].pendingDeleteFlag)
            {
                for (lcId = 0; lcId < MAX_LOGICAL_CHANNEL; lcId++)
                {
                    if (LCGID_INVALID_VALUE != ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].lcGId)
                    {
                        if (QOS_GBR == ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].ulQosInfo.channelType)
                        {
                            /* modify token rate */
                             ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].ulQosInfo.tokenRate = 
                                (ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].ulQosInfo.GBR *
                                initParams_p->ulSchdConfig.ulTokenPeriodicity)/NUM_TICKS_PER_SECOND;

                             /* modify token depth */
                            ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].ulQosInfo.tokenDepth =
                                ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].ulQosInfo.tokenRate * 
                                initParams_p->ulSchdConfig.gbrBktFactor;

                            /* modify token limit */
                            ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].ulQosInfo.tokenLimit = 
                                initParams_p->ulSchdConfig.gbrTokenLimitFactor * 
                                ulUECtxInfoArr_g[index].ulUEContext_p->lcInfo[lcId].ulQosInfo.tokenRate;
                        }    
                    }        
                }   
            } 
        }

       /* DL, Change tokenDepth for each valid GBR LC */  
        if (NULL != dlUECtxInfoArr_g[index].dlUEContext_p)
        {
            if (FALSE == dlUECtxInfoArr_g[index].pendingDeleteFlag)
            {
                for (lcId = 0; lcId < MAX_LOGICAL_CHANNEL; lcId++)
                {
                    if (INVALID_LCID != dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].logicalChannelId)
                    {
                        if (QOS_GBR == dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].dlLcQosInfo.channelType)
                        {
                            /* modify token rate */
                            dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].dlLcQosInfo.tokenRate = 
                                (dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].dlLcQosInfo.GBR * 
                                initParams_p->dlSchdConfig.dlTokenPeriodicity)/NUM_TICKS_PER_SECOND;

                            /* modify token depth */
                            dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].dlLcQosInfo.tokenDepth = 
                                dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].dlLcQosInfo.tokenRate * 
                                initParams_p->dlSchdConfig.gbrBktFactor;

                            /* modify token limit */
                            dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].dlLcQosInfo.tokenLimit = 
                                dlUECtxInfoArr_g[index].dlUEContext_p->logicalChannel[lcId].dlLcQosInfo.tokenRate *
                                initParams_p->dlSchdConfig.gbrTokenLimitFactor;
                        }    
                    }    
                }    
            }    

        }    
    }  
}   
/* CA Changes end */
/****************************************************************************
 * Function Name  : modifyAMBRTBDTRAndTBLOfUEs
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function modifies the AMBR rate, AMBR Bucket Depth 
 *                  GBR token limit on changing token periodicity.
 ****************************************************************************/
/* CA Changes start */
void modifyAMBRTBDTRAndTBLOfUEs(InternalCellIndex internalCellIndex)
{
    
    UInt16 index;
    
   InitParams *initParams_p = cellSpecificParams_g.\
                   cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    for(index = 0; index < MAX_UE_SUPPORTED; index++)
    {
       /* UL, Change tokenDepth for AMBR LC */  
        if (NULL != ulUECtxInfoArr_g[index].ulUEContext_p)
        {
            if (FALSE == ulUECtxInfoArr_g[index].pendingDeleteFlag)
            {
                /* modify token rate */
                ulUECtxInfoArr_g[index].ulUEContext_p->ulAmbrInfo.ambrTokenRate = 
                     (ulUECtxInfoArr_g[index].ulUEContext_p->ulAmbrInfo.AMBR *
                     initParams_p->ulSchdConfig.ulTokenPeriodicity)/NUM_TICKS_PER_SECOND;

                /* modify token depth */
                ulUECtxInfoArr_g[index].ulUEContext_p->ulAmbrInfo.ambrTokenDepth = 
                    ulUECtxInfoArr_g[index].ulUEContext_p->ulAmbrInfo.ambrTokenRate * 
                    initParams_p->ulSchdConfig.ambrBktFactor;

                /* modify token limit */
                 ulUECtxInfoArr_g[index].ulUEContext_p->ulAmbrInfo.ambrTokenLimit = 
                     ulUECtxInfoArr_g[index].ulUEContext_p->ulAmbrInfo.ambrTokenRate *
                     initParams_p->ulSchdConfig.ambrTokenLimitFactor;
            } 
        }

       /* DL, Change tokenDepth for AMBR LC */  
        if (NULL != dlUECtxInfoArr_g[index].dlUEContext_p)
        {
            if (FALSE == dlUECtxInfoArr_g[index].pendingDeleteFlag)
            {
                /* modify token rate */
                dlUECtxInfoArr_g[index].dlUEContext_p->dlAmbrInfo.ambrTokenRate = 
                    (dlUECtxInfoArr_g[index].dlUEContext_p->dlAmbrInfo.AMBR* 
                    initParams_p->dlSchdConfig.dlTokenPeriodicity)/NUM_TICKS_PER_SECOND;
                
                /* modify token depth */
                dlUECtxInfoArr_g[index].dlUEContext_p->dlAmbrInfo.ambrTokenDepth = 
                         dlUECtxInfoArr_g[index].dlUEContext_p->dlAmbrInfo.ambrTokenRate*
                         initParams_p->dlSchdConfig.ambrBktFactor;

                /* modify token limit */
                dlUECtxInfoArr_g[index].dlUEContext_p->dlAmbrInfo.ambrTokenLimit =
                    dlUECtxInfoArr_g[index].dlUEContext_p->dlAmbrInfo.ambrTokenRate *
                    initParams_p->dlSchdConfig.ambrTokenLimitFactor;
            }    
        }    
    }  
}   

/* CA Changes end */
/*CA Changes start  */
/********************************************************************************
* FUNCTION NAME : initQosToken
* INPUTS        : number of cells to be configured (numOfCells)
* OUTPUTS       :
* DESCRIPTION   : This function is used to init the desired member of general global structure
*                 qosToken_g. This is called at MAC_INIT_LAYER_REQ
*********************************************************************************/
void initQosToken(UInt8 numOfCells)
{
    UInt8 internalCellIndex = 0;
    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Entry..... \n",__func__);
    createMemPool(sizeof(LTE_LIST) * MAX_QOS_TOKEN_LTE_LIST, 1);
    
    for (internalCellIndex  = 0; internalCellIndex < numOfCells; internalCellIndex++)
    {
        qosToken_g[internalCellIndex].ulMaxBatches = 0;
        qosToken_g[internalCellIndex].dlMaxBatches = 0;
        qosToken_g[internalCellIndex].dlBatchListIndex = 0;
        qosToken_g[internalCellIndex].ulBatchListIndex = 0; 
                            
        GET_MEM_FROM_POOL(LTE_LIST, tokenPeriodicityListDl_gp[internalCellIndex], sizeof(LTE_LIST), PNULL);
        GET_MEM_FROM_POOL(LTE_LIST, tokenPeriodicityListUl_gp[internalCellIndex], sizeof(LTE_LIST), PNULL);
        GET_MEM_FROM_POOL(LTE_LIST, tokenPeriodicityListMapDl_gp[internalCellIndex], sizeof(LTE_LIST), PNULL);
        GET_MEM_FROM_POOL(LTE_LIST, tokenPeriodicityListMapUl_gp[internalCellIndex], sizeof(LTE_LIST), PNULL);
    }        
    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Exit..... \n",__func__);
}

/****************************************************************************
 * Function Name  : deInitQosToken 
 * Inputs         : Internal Cell index
 * Outputs        : None
 * Returns        : None
 * Description    : This function modifies the clean up the memory taken 
 *                  called at the time of MAC_CLEANUP_LAYER_REQ
 ****************************************************************************/
void deInitQosToken(InternalCellIndex internalCellIndex)
{
    if(tokenPeriodicityListDl_gp[internalCellIndex])
    {
        freeMemPool(tokenPeriodicityListDl_gp[internalCellIndex]);
        tokenPeriodicityListDl_gp[internalCellIndex] = PNULL;
    }

    if(tokenPeriodicityListUl_gp[internalCellIndex])
    {
        freeMemPool(tokenPeriodicityListUl_gp[internalCellIndex]);
        tokenPeriodicityListUl_gp[internalCellIndex] = PNULL;
    }

    if(tokenPeriodicityListMapDl_gp[internalCellIndex])
    {
        freeMemPool(tokenPeriodicityListMapDl_gp[internalCellIndex]);
        tokenPeriodicityListMapDl_gp[internalCellIndex] = PNULL;
    }
    if(tokenPeriodicityListMapUl_gp[internalCellIndex])
    {
        freeMemPool(tokenPeriodicityListMapUl_gp[internalCellIndex]);
        tokenPeriodicityListMapUl_gp[internalCellIndex] = PNULL;
    }
    return;
}
/*CA Changes end  */
/* QOS_OAM_MODIFY_CHANGES_end */

/* QOS_OAM_MODIFY_CHANGES_end */
    /* +- SPR 17777 */

/* - SPR_11382 */
/****************************************************************************
 * Function Name  : deInitTokenPeriodicityArray 
 * Inputs         : Internal Cell index
 * Outputs        : None
 * Returns        : None
 * Description    : This function modifies the clean up the memory taken 
 *                  called at the time of cell delete.
 ****************************************************************************/
/* + SPR_11382 */
void deInitTokenPeriodicityArray(InternalCellIndex internalCellIndex)
{
    UInt16 count = 0,loopIndex=0;
    UInt16 count1 = 0,loopIndex1=0;
    TokenPeriodicityLisElem  *elem_p = PNULL;
    TokenPeriodicityMap *tokenMap_p = PNULL;
    LTE_LIST *ueList_p = PNULL;
    TknUpdateUENode *ueNode_p=PNULL;

    count = listCount(tokenPeriodicityListDl_gp[internalCellIndex]);

    for ( loopIndex= 0; loopIndex < count; loopIndex++)
    {
        elem_p = 
            (TokenPeriodicityLisElem *)getFirstListNode(tokenPeriodicityListDl_gp[internalCellIndex]);

        if (elem_p)
        {
            ueList_p = &elem_p->tokenList;

            count1 = listCount(ueList_p);
            for (loopIndex1= 0; loopIndex1 < count1; loopIndex1++)
            {
                ueNode_p = (TknUpdateUENode *)getFirstListNode(ueList_p);
                if (ueNode_p)
                {
                    listDeleteNode(ueList_p, &ueNode_p->tknUpdateAnchor);
                    freeMemPool(ueNode_p);
                    ueNode_p = PNULL;
                }
            }

            listDeleteNode(tokenPeriodicityListDl_gp[internalCellIndex],
                    &elem_p->elemAnchor);
            freeMemPool(elem_p);
            elem_p = PNULL;
        }
    }

    count = listCount(tokenPeriodicityListUl_gp[internalCellIndex]);
    for ( loopIndex= 0; loopIndex < count; loopIndex++)
    {
        elem_p = 
            (TokenPeriodicityLisElem *)getFirstListNode(tokenPeriodicityListUl_gp[internalCellIndex]);

        if (elem_p)
        {
            ueList_p = &elem_p->tokenList;

            count1 = listCount(ueList_p);
            for (loopIndex1= 0; loopIndex1 < count1; loopIndex1++)
            {
                ueNode_p = (TknUpdateUENode *)getFirstListNode(ueList_p);
                if (ueNode_p)
                {
                    listDeleteNode(ueList_p, &ueNode_p->tknUpdateAnchor);
                    freeMemPool(ueNode_p);
                    ueNode_p = PNULL;
                }
            }
            listDeleteNode(tokenPeriodicityListUl_gp[internalCellIndex],
                    &elem_p->elemAnchor);
            freeMemPool(elem_p);
            elem_p = PNULL;
        }
    }

    count = listCount(tokenPeriodicityListMapDl_gp[internalCellIndex]);
    for ( loopIndex= 0; loopIndex < count; loopIndex++)
    {
        tokenMap_p = 
            (TokenPeriodicityMap *)getFirstListNode(tokenPeriodicityListMapDl_gp[internalCellIndex]);

        if (tokenMap_p)
        {
            listDeleteNode(tokenPeriodicityListMapDl_gp[internalCellIndex],
                    &(tokenMap_p->node) );
            freeMemPool(tokenMap_p);
            tokenMap_p = PNULL;
        }
    }

    count = listCount(tokenPeriodicityListMapUl_gp[internalCellIndex]);
    for ( loopIndex= 0; loopIndex < count; loopIndex++)
    {
        tokenMap_p = 
            (TokenPeriodicityMap *)getFirstListNode(tokenPeriodicityListMapUl_gp[internalCellIndex]);

        if (tokenMap_p)
        {
            listDeleteNode(tokenPeriodicityListMapUl_gp[internalCellIndex],
                    &(tokenMap_p->node) );
            freeMemPool(tokenMap_p);
            tokenMap_p = PNULL;
        }
    }
    return;
}

/* - SPR_11382 */
/* Rel 5.3.1: Memory related changes end*/
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
/****************************************************************************
 * Function Name  : populateMocnParams 
 * Inputs         : MacInitReqParams
 * Outputs        : None
 * Returns        : None
 * Description    : This function populates MOCN specific parameters at init 
 ****************************************************************************/
void populateMocnParams(MacInitReqParams *macInitReqParams_p)
{
    UInt8 i=0;
    mocnOperatorParams *mocnOperatorParams_p = &mocnOperatorParams_g;
    memSet(mocnOperatorParams_p, 0, sizeof(mocnOperatorParams));

    if (macInitReqParams_p->mocnParams.numOfOperator > 0)
    {
        mocnOperatorParams_p->isMocnEnabled =  LTE_TRUE;
        mocnOperatorParams_p->numOfOperator = macInitReqParams_p->mocnParams.numOfOperator;
        mocnOperatorParams_p->prbUsageMeasForgettingFactor = macInitReqParams_p->mocnParams.prbUsageMeasForgettingFactor;
        mocnOperatorParams_p->prbUsageMeasResetPeriod = macInitReqParams_p->mocnParams.prbUsageMeasResetPeriod;
    }    
    for (i=0; i < macInitReqParams_p->mocnParams.numOfOperator; i++)
    {
        mocnOperatorParams_p->dedicatedDLPRBsPerOperator[i] = macInitReqParams_p->mocnParams.mocnPerOperatorParams[i].dedicatedDLPRBsPerOperator;
        mocnOperatorParams_p->dedicatedULPRBsPerOperator[i] = macInitReqParams_p->mocnParams.mocnPerOperatorParams[i].dedicatedULPRBsPerOperator;
        mocnOperatorParams_p->reservedDLPRBsPerOperator[i] = macInitReqParams_p->mocnParams.mocnPerOperatorParams[i].reservedDLPRBsPerOperator;
        mocnOperatorParams_p->reservedULPRBsPerOperator[i] = macInitReqParams_p->mocnParams.mocnPerOperatorParams[i].reservedULPRBsPerOperator;

        mocnOperatorParams_p->totalDedicatedDLPRBs +=mocnOperatorParams_p->dedicatedDLPRBsPerOperator[i];
        mocnOperatorParams_p->totalDedicatedULPRBs +=mocnOperatorParams_p->dedicatedULPRBsPerOperator[i];
        mocnOperatorParams_p->totalReservedDLPRBs +=mocnOperatorParams_p->reservedDLPRBsPerOperator[i];
        mocnOperatorParams_p->totalReservedULPRBs +=mocnOperatorParams_p->reservedULPRBsPerOperator[i];
    }    

}
#ifdef KPI_STATS
/****************************************************************************
 * Function Name  : calculateMocnPRBUsageHistory 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function calculates PRB Usage history for MOCN. 
 ****************************************************************************/
void calculateMocnPRBUsageHistory(InternalCellIndex internalCellIndex)
{
	UInt8 i = 0;
	UInt8 j = 0;
	UInt32 activeIndexForMacStats = (gMacStats.pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
	UInt64 totalDLPRBUsage[MAX_NUM_MOCN_OPERATOR] = {0};
	UInt64 totalULPRBUsage[MAX_NUM_MOCN_OPERATOR] = {0};
	UInt32 statsTimeDuration = gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].lastCalculatedTTI;
	mocnOperatorParams *mocnOperatorParams_p = &mocnOperatorParams_g;
	if (statsTimeDuration) 
	{
		for (i=0; i<mocnOperatorParams_p->numOfOperator; i++)
		{
			for(j=0; j<MAX_QCI; j++)
			{     
				totalDLPRBUsage[i] +=gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].totalDLPRBUsagePerOperatorPerQCI[i][j];
				totalULPRBUsage[i] +=gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].totalULPRBUsagePerOperatorPerQCI[i][j];
			}    
			/* calculate  total PRB Usage in percentage */
			totalDLPRBUsage[i] = ((totalDLPRBUsage[i] >> SCALE_UP_MULTIPLYING_FACTOR)*100)/(statsTimeDuration * cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlAvailableRBs);
			totalULPRBUsage[i] = ((totalULPRBUsage[i] >> SCALE_UP_MULTIPLYING_FACTOR)*100)/(statsTimeDuration * cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs);

			/* calculate prbusageHist */
			mocnOperatorParams_p->dlPrbUsageHist[i] =  ((10-mocnOperatorParams_p->prbUsageMeasForgettingFactor) * mocnOperatorParams_p->dlPrbUsageHist[i])/10 +
				(mocnOperatorParams_p->prbUsageMeasForgettingFactor*((UInt8)totalDLPRBUsage[i]))/10;
			mocnOperatorParams_p->ulPrbUsageHist[i] =  ((10-mocnOperatorParams_p->prbUsageMeasForgettingFactor) * mocnOperatorParams_p->ulPrbUsageHist[i])/10 +
				(mocnOperatorParams_p->prbUsageMeasForgettingFactor*((UInt8)totalULPRBUsage[i]))/10;  

			/* Filling priprity factor */
			if (mocnOperatorParams_p->dlPrbUsageHist[i] >= (mocnOperatorParams_p->dedicatedDLPRBsPerOperator[i] + mocnOperatorParams_p->reservedDLPRBsPerOperator[i]) )
			{
				mocnOperatorParams_p->dlPrbUsageDRBPriorityFactor[i] = MOCN_PRIORITY_FACTOR_0;  
			}
			else if(mocnOperatorParams_p->dlPrbUsageHist[i] >= mocnOperatorParams_p->dedicatedDLPRBsPerOperator[i])
			{
				mocnOperatorParams_p->dlPrbUsageDRBPriorityFactor[i] = MOCN_PRIORITY_FACTOR_600; 
			}
			else
			{
				mocnOperatorParams_p->dlPrbUsageDRBPriorityFactor[i] = MOCN_PRIORITY_FACTOR_1200; 
			}

			if (mocnOperatorParams_p->ulPrbUsageHist[i] >= (mocnOperatorParams_p->dedicatedULPRBsPerOperator[i] + mocnOperatorParams_p->reservedULPRBsPerOperator[i] ) )
			{
				mocnOperatorParams_p->ulPrbUsageDRBPriorityFactor[i] = MOCN_PRIORITY_FACTOR_0;  
			}
			else if(mocnOperatorParams_p->ulPrbUsageHist[i] >= mocnOperatorParams_p->dedicatedULPRBsPerOperator[i])
			{
				mocnOperatorParams_p->ulPrbUsageDRBPriorityFactor[i] = MOCN_PRIORITY_FACTOR_600; 
			}
			else
			{
				mocnOperatorParams_p->ulPrbUsageDRBPriorityFactor[i] = MOCN_PRIORITY_FACTOR_1200; 
			}
		}
		/* SRB PRB Usage Priority Factor */    
		mocnOperatorParams_p->prbUsageSRBPriorityFactor = MOCN_SRB_PRIORITY_FACTOR; 
	}

}
#endif
/* MOCN Changes End */
/* SPR 23402 Changes End */
/********************** End of the File *************************************/
