/*******************************************************************************
*
*  FILE NAME   : lteRelayM1uTeidSearchTree.c 
*
*  DESCRIPTION : This file contains definition of functions used to perform 
*               initialization, node insertion, deletion and search  related
*               operations on a search tree.
*     
*  
*
*  Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
*******************************************************************************/

#include<lteRelayM1uTeidSearchTree.h>

typedef int (*compare_func)(const void *, const void *);
/*****************************************************************************
 * Function Name  : keyofTeidTree
 * Inputs         : tree_node
 * Outputs        : None
 * Returns        : UInt32      
 * Description    : This function returns key
 *****************************************************************************/

const void *(keyofTeidTree)(const TEIDSearchTreeNode *tree_node)
{
    return (tree_node)?&(((const  TEIDAreaLcTreeInfo *) \
                tree_node)->teid) : PNULL;
}
/*****************************************************************************
 * Function Name  : compareTeidSearchTree
 * Inputs         : elem1
 *                  elem2
 * Outputs        : None
 * Returns        : UInt32      
 * Description    : This function compares elem1 and elem2
 *****************************************************************************/

SInt32 compareTeidSearchTree
(
 /* Pointer to first element being compared */
  const void * elem1 ,

   /* Pointer to second element being compared */
    const void * elem2
     )
{ 

    if ( * ((UInt32 *) elem1) == * ((UInt32 *) elem2))
    {
        return TEID_FIRST_IS_EQUAL_TO_SECOND;
    }
    else if ( * ((UInt32 *) elem1) < * ((UInt32 *) elem2))
    {
        return TEID_FIRST_IS_LESS_THAN_SECOND;
    }
    else
    {
        return TEID_FIRST_IS_GREATER_THAN_SECOND;
    }
} 



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
      /* Pointer to tree being initialised */
      TEIDSearchTree *tree_p)
{
      
     ytInit(tree_p,(compare_func)compareTeidSearchTree,keyofTeidTree);
}

/*****************************************************************************
 * Function Name  : teidGetFirstNode
 * Inputs         : tree_p - pointer to UL Priortised LC Tree
 * Outputs        : Node 
 * Returns        : Pointer to first node of UL Priortised LC Tree according 
 *                  to INORDER Traversal
 * Description    : This function returns the pointer to the first node of 
 *                  UL priortised LC Tree.
 *****************************************************************************/
TEIDSearchTreeNode    *teidGetFirstNode(
    const TEIDSearchTree *tree_p)
{
    return ytFirst(tree_p, T_INORDER);
}



/*****************************************************************************
 * Function Name  : teidTreeNodeCount
 * Inputs         : tree_p -  pointer to Tree
 * Outputs        : Node
 * Returns        : Node count of the tree
 * Description    : The function returns Number of nodes present in a tree.
 *****************************************************************************/
UInt32 teidTreeNodeCount (
    const TEIDSearchTree *tree_p)
{
    return ytCount(tree_p);
}

/*****************************************************************************
 * Function Name  : teidFindTreeNode
 * Inputs         : tree_p -  pointer to Tree
 * Outputs        : Node
 * Returns        : Node count of the tree
 * Description    : The function returns Number of nodes present in a tree.
 *****************************************************************************/

TEIDSearchTreeNode * teidFindTreeNode(
     const TEIDSearchTree *tree_p,
     const void *item)
{
    return ytFind(tree_p, item);
}


/*****************************************************************************
 * Function Name  : teidInsertTreeNode
 * Inputs         : tree_p -  pointer to Tree
 * Outputs        : Node
 * Returns        : Node count of the tree
 * Description    : The function returns Number of nodes present in a tree.
 *****************************************************************************/

void teidInsertTreeNode(
     TEIDSearchTree *tree_p,
     TEIDSearchTreeNode *entry)

{
    ytInsert(tree_p, entry);
}

/*****************************************************************************
 * Function Name  : teidDeleteTreeNode
 * Inputs         : tree_p -  pointer to Tree
 * Outputs        : Node
 * Returns        : Node count of the tree
 * Description    : The function returns Number of nodes present in a tree.
 *****************************************************************************/

void teidDeleteTreeNode(
     TEIDSearchTree *tree_p,
     TEIDSearchTreeNode *entry)
{
    ytDelete(tree_p, entry);
}

