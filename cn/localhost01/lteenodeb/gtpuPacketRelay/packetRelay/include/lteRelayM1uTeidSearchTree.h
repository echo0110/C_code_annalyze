/******************************************************************************
*
*   FILE NAME   : lteRelayM1uTeidSearchTree.h
*   
*
*   DESCRIPTION : This file contains the structure and function declaration
*                required for x2ap search tree.
*
*   
*  
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _TEID_SEARCH_TREE_H_
#define _TEID_SEARCH_TREE_H_

#include "ylib.h"
#include "lteList.h"
#include "lteRelay.h"

#define TEID_FIRST_IS_EQUAL_TO_SECOND 0
#define TEID_FIRST_IS_LESS_THAN_SECOND 1
#define TEID_FIRST_IS_GREATER_THAN_SECOND -1
/* Tree data type */
typedef YTREE TEIDSearchTree;

/* Tree node data type */
typedef YTNODE TEIDSearchTreeNode ;

typedef struct AreaLcInfoList_t
{
UInt8 areaId;
UInt8 lcId;
}AreaLcInfoList;


typedef struct TEIDAreaLcInfo_t
{
  TEIDSearchTreeNode teidAnchor;
  UInt32 teid;
  LTE_LIST teidInfoList;
}TEIDAreaLcTreeInfo;


typedef struct TEIDInfo_t
{
  LTE_LIST_NODE teidAnchor;
  UInt8 sIp[RELAY_ADDR_LEN_IPV6];
  UInt8 mIp[RELAY_ADDR_LEN_IPV6];
  UInt8 listCount;
  /* SPR 18387 fix start */
  AreaLcInfoList areaLcList[MAX_AREA_LIST_COUNT];
  /* SPR 18387 fix end */
}TEIDInfo;
/* Function Prototypes */
/*****************************************************************************
 * Function Name  : teidTreeInit
 * Inputs         : tree_p - pointer to UL Priortised LC Tree,
 *                  compare_p - pointer to compare function for Priortised LC
 *                              Tree,
 *                  keyOf_p   - pointer to key function
 * Outputs        : None 
 * Returns        : None
 * Description    : This function initialises the teid LC Tree.
 *****************************************************************************/
void teidTreeInit(
      TEIDSearchTree *tree_p);

/*****************************************************************************
 * Function Name  : teidGetFirstNode
 * Inputs         : tree_p - pointer to UL Priortised LC Tree
 * Outputs        : Node 
 * Returns        : Pointer to first node of UL Priortised LC Tree according 
 *                  to INORDER Traversal
 * Description    : This function returns the pointer to the first node of 
 *                  UL priortised LC Tree.
 *****************************************************************************/
TEIDSearchTreeNode    * teidGetFirstNode(
    const TEIDSearchTree *tree_p);


/*****************************************************************************
 * Function Name  : teidTreeNodeCount
 * Inputs         : tree_p -  pointer to Tree
 * Outputs        : Node
 * Returns        : Node count of the tree
 * Description    : The function returns Number of nodes present in a tree.
 *****************************************************************************/
UInt32 teidTreeNodeCount (
    const TEIDSearchTree *tree_p);

/*****************************************************************************
 * Function Name  : teidFindTreeNode
 * Inputs         : tree_p -  pointer to Tree
 * Outputs        : Node
 * Returns        : Node count of the tree
 * Description    : The function returns Number of nodes present in a tree.
 *****************************************************************************/

TEIDSearchTreeNode * teidFindTreeNode(
     const TEIDSearchTree *tree_p,
     const void *item);

/*****************************************************************************
 * Function Name  : teidInsertTreeNode
 * Inputs         : tree_p -  pointer to Tree
 * Outputs        : Node
 * Returns        : Node count of the tree
 * Description    : The function returns Number of nodes present in a tree.
 *****************************************************************************/
void teidInsertTreeNode(
     TEIDSearchTree *tree_p,
     TEIDSearchTreeNode *entry);

/*****************************************************************************
 * Function Name  : teidDeleteTreeNode
 * Inputs         : tree_p -  pointer to Tree
 * Outputs        : Node
 * Returns        : Node count of the tree
 * Description    : The function returns Number of nodes present in a tree.
 *****************************************************************************/
void teidDeleteTreeNode(
     TEIDSearchTree *tree_p,
     TEIDSearchTreeNode *entry);



#endif
