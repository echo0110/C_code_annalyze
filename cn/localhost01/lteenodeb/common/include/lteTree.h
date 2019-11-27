/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTree.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 *
 *
 ****************************************************************************/


#ifndef _LTE_TREE_
#define _LTE_TREE_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include    "ylib.h"
#include    "lteTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* This is needed to create a user defined tree. An object of this needs to 
   be passed to the intialization function of the tree. */
typedef YTREE LTE_TREE;

/* This is the anchor of the entry to be made in the tree. This object should
   be part of every node that is created to be inserted into a tree. */
typedef YTNODE LTE_TREE_NODE;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 *  function(): Description.
 *      Initialises the tree.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

void    treeInit(LTE_TREE *pTree, SInt32 (*compare)(const void *, const void *), const void *(*key)(const LTE_TREE_NODE *) );

/****************************************************************************
 *  function(): Description.
 *      Counts the number of nodes in the tree.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
UInt32  treeCount(const LTE_TREE *pTree);


/****************************************************************************
 *  function(): Description.
 *      Deletes from the RB tree.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    treeDeleteNode(LTE_TREE *pTree, LTE_TREE_NODE *pNode);


/****************************************************************************
 *  function(): Description.
 *      Inserts in the RB tree.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    treeInsertNode(LTE_TREE *pTree, LTE_TREE_NODE *pNode);

/****************************************************************************
 *  function(): Description.
 *      Retunrs the tree node at the specified node number.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_TREE_NODE *getTreeNode(const LTE_TREE *pTree, const void* pItem);


/****************************************************************************
 *  function(): Description.
 *      Retunrs the tree node next to the node specified.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

LTE_TREE_NODE *getNextTreeNode(const LTE_TREE_NODE *pNode, UInt32 order);
/****************************************************************************
 *  function(): Description.
 *      Retunrs the first node of the tree.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

LTE_TREE_NODE *getFirstTreeNode(const LTE_TREE *pTree, UInt32 order);
/****************************************************************************
 *  function(): Description.
 *      Retunrs the last node of the tree.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

LTE_TREE_NODE *getLastTreeNode(const LTE_TREE *pTree, UInt32 order);
/****************************************************************************
 *  function(): Description.
 *      Retunrs the tree node previous to the node specified.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

LTE_TREE_NODE *getPrevTreeNode(const LTE_TREE_NODE *pNode, UInt32 order);


/****************************************************************************
 * Function Name  : treeWalk
 * Inputs         : pTree - Pointer to the LTE_TREE object.
 *                : walker - Function pointer to compare nodes.
 *                : pValue - pointer to value.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Initialises the Red-Black Tree.
 ****************************************************************************/
void    treeWalk(LTE_TREE *pTree, void (*walker)(LTE_TREE_NODE *, void *), void * pValue);
void    treeDelete(LTE_TREE *pTree, void (*walker)(LTE_TREE_NODE *, void *), void * pValue);

/****************************************************************************
* Function Name  : printTree
* Inputs         : tree_p - Pointer to the Tree
* Outputs        : Prints the details of all the nodes present in the tree
* Returns        : None
* Variables      : 
* Description    : This function prints all the nodes present in the tree. 
****************************************************************************/
UInt32 printTree(const LTE_TREE* tree_p);

#endif
