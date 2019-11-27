/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTree.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Wrapper library for Tree implementation.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteTree.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:24:51  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 13:44:39  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/05/15 09:49:56  gur20548
 * remove warning
 *
 * Revision 1.1  2009/05/12 06:06:06  gur19479
 * pdcp merging
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include    "lteTree.h"
#include    "lteLog.h"

/****************************************************************************
  Private Definitions
 ****************************************************************************/



/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Function Name  : treeInit 
 * Inputs         : pTree - Pointer to the LTE_TREE object.
 *                : compare - Function pointer to compare nodes.
 *                : key - Function pointer. 
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Initialises the Red-Black Tree.
 ****************************************************************************/
void    treeInit(LTE_TREE *pTree, SInt32 (*compare)(const void *, const void *), const void *(*key)(const LTE_TREE_NODE *) )
{
    if (pTree != PNULL)
    {
        ytInit(pTree, compare, key);
    }
    return;
}


/****************************************************************************
 * Function Name  : treeCount 
 * Inputs         : pTree - Pointer to the LTE_TREE object.
 * Outputs        : None.
 * Returns        : Number of elements in the RB Tree.
 * Variables      : None.
 * Description    : Counts the number of elements in the RB Tree. 
 ****************************************************************************/
UInt32  treeCount(const LTE_TREE *pTree)
{
    return ytCount(pTree);
}


/****************************************************************************
 * Function Name  : treeDeleteNode 
 * Inputs         : pTree - Pointer to the LTE_TREE object.
 *                  pNode - Pointer to the LTE_TREE_NODE object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Deletes an element in the RB Tree.
 ****************************************************************************/
void    treeDeleteNode(LTE_TREE *pTree, LTE_TREE_NODE *pNode)
{
    if ((pTree != PNULL) && (pNode != PNULL))
    {
        ytDelete(pTree, pNode);
    }
    return;
}


/****************************************************************************
 * Function Name  : treeInsertNode 
 * Inputs         : pTree - Pointer to the LTE_TREE object.
 *                  pNode - Pointer to the LTE_TREE_NODE object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Inserts an element in the RB Tree.
 ****************************************************************************/
void    treeInsertNode(LTE_TREE *pTree, LTE_TREE_NODE *pNode)
{
    if ((pTree != PNULL) && (pNode != PNULL))
    {
        ytInsert(pTree, pNode);
    }
    return;
}


/****************************************************************************
 * Function Name  : getTreeNode 
 * Inputs         : pTree - Pointer to the LTE_TREE object.
 *                  pItem - Pointer to Node to be find in RB Tree. 
 * Outputs        : None.
 * Returns        : Returns pointer to node in the tree.
 * Variables      : None.
 * Description    : Find the node in the tree. Note that the 
 *                  node is not removed from the tree.
 ****************************************************************************/
LTE_TREE_NODE *getTreeNode(const LTE_TREE *pTree, const void *pItem)
{
    if (pTree != PNULL)
    {
        return ytFind(pTree, pItem);
    }
    return PNULL;
}


/****************************************************************************
 * Function Name  : getFirstTreeNode 
 * Inputs         : pTree - Pointer to the LTE_TREE object.
 *                : order - specify the order (preorder, inorder, postorder)
 * Outputs        : None.
 * Returns        : The first node in the tree.
 * Variables      : None.
 * Description    : Returns the first node in the tree. Note that the 
 *                  node is not removed from the tree.
 ****************************************************************************/
LTE_TREE_NODE *getFirstTreeNode(const LTE_TREE *pTree, UInt32 order)
{
    if (pTree != PNULL)
    {
        return ytFirst(pTree, order);
    }
    return PNULL;
}
/****************************************************************************
 * Function Name  : getLastTreeNode 
 * Inputs         : pTree - Pointer to the LTE_TREE object.
 *                : order - specify order (preorder, inorder, postorder)
 * Outputs        : None.
 * Returns        : The last node in the tree.
 * Variables      : None.
 * Description    : Returns the node next in the tree. Note that the 
 *                  node is not removed from the tree.
 ****************************************************************************/
LTE_TREE_NODE *getLastTreeNode(const LTE_TREE *pTree, UInt32 order)
{
    if (pTree != PNULL)
    {
        return ytLast(pTree, order);
    }
    return PNULL;
}
/****************************************************************************
 * Function Name  : getNextTreeNode 
 * Inputs         : pNode - Pointer to the LTE_TREE_NODE object.
 *                : order - specify order (preorder, inorder, postorder)
 * Outputs        : None.
 * Returns        : The next node in the Tree.
 * Variables      : None.
 * Description    : Returns the node next in the tree. Note that the 
 *                  node is not removed from the tree.
 ****************************************************************************/
LTE_TREE_NODE *getNextTreeNode(const LTE_TREE_NODE *pNode, UInt32 order)
{
    if (pNode != PNULL)
    {
        return ytNext(pNode, order);
    }
    return PNULL;
}

/****************************************************************************
 * Function Name  : getPrevTreeNode 
 * Inputs         : pNode - Pointer to the LTE_TREE_NODE object.
 *                : order - specify order (preorder, inorder, postorder).
 * Outputs        : None.
 * Returns        : The prev node in the Tree.
 * Variables      : None.
 * Description    : Returns the node next in the tree. Note that the 
 *                  node is not removed from the tree.
 ****************************************************************************/
LTE_TREE_NODE *getPrevTreeNode(const LTE_TREE_NODE *pNode, UInt32 order)
{
    if (pNode != PNULL)
    {
        return ytPrev(pNode, order);
    }
    return PNULL;
}

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
void    treeWalk(LTE_TREE *pTree, void (*walker)(LTE_TREE_NODE *, void *), void * pValue)
{
    if (pTree != PNULL)
    {
        ytWalk(pTree, walker, pValue);
    }
    return;
}
/****************************************************************************
 * Function Name  : treeDelete 
 * Inputs         : pTree - Pointer to the LTE_TREE object.
 *                : walker - Function pointer to compare nodes.
 *                : pValue - pointer to value.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : delete the Red-Black Tree.
 ****************************************************************************/
void  treeDelete(LTE_TREE *pTree, void (*walker)(LTE_TREE_NODE *, void *), void * pValue)
{
    LTE_TREE_NODE	*p = PNULL;
    LTE_TREE_NODE	*next_p = PNULL;
    
    if (pTree != PNULL){
        for(p = getFirstTreeNode(pTree,T_INORDER);p;){
            next_p = getNextTreeNode(p,T_INORDER);        
            walker(p, pValue);
            p = next_p;
        }
    }
}


/****************************************************************************
* Function Name  : printTree
* Inputs         : queue_p - Pointer to the Tree.
* Outputs        : Prints the details of all the nodes present in the tree
* Returns        : None
* Variables      : 
* Description    : This function prints all the nodes present in the tree. 
****************************************************************************/
/* +- SPR 17777 */
/*
UInt32 printTree(const LTE_TREE* tree_p)
{
    UInt32 x = 1;
    LTE_TREE_NODE *node_p = PNULL;

    if (tree_p != PNULL)
    {
        node_p = getFirstTreeNode(tree_p);
         
        lteLog(LOG_INFO,"No of nodes in Tree = %d\n", treeCount(tree_p)); 
        
        while(node_p)
        {
            lteLog(LOG_INFO,"Node No = %d\t", x); 
            lteLog(LOG_INFO,"Address of Node = %p\n", node_p); 
            node_p = node_p->next;
            x++;
        }
        return treeCount(tree_p);
    }
    else
    {
        lteLog(LOG_MAJOR,"In Fn %s, LTE_TREE* passed is NULL\n",__FUNCTION__);
        return 0;
    }
        return 0;
}
*/        
/* +- SPR 17777 */
