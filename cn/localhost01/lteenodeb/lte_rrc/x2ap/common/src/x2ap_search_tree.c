/*******************************************************************************
*
*  FILE NAME   : x2ap_search_tree.c
*
*  DESCRIPTION : This file contains definition of functions used to perform 
*               initialization, node insertion, deletion and search  related
*               operations on a search tree.
*      DATE             NAME               REFERENCE                REASON
*  28-JUNE-2010         Vaibhav Singh      36.423_880(3GPP Spec)    Initial
*
*  Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
*******************************************************************************/

#include<x2ap_search_tree.h>

typedef int (*compare_func)(const void *, const void *);
/*****************************************************************************
 * Function Name  : x2ap_search_tree_init
 * Inputs         : p_tree -  Pointer to tree being initialised
 *                  compare - Pointer to compare function :
 *                            it returns 0 if A==B, -1 if A<B and 1 if A>B
 *                  keyof -  Pointer to function returning Key
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to inititalize a tree.
 *****************************************************************************/
void x2ap_search_tree_init(
      /* Pointer to tree being initialised */
      x2ap_search_tree_t *p_tree,

      /* Pointer to compare function :
       * it returns 0 if A==B, -1 if A<B and 1 if A>B
       */
      S32 (*compare)(const void *, const void *),

      /* Pointer to function returning Key
       */
      const void *(*keyof)(const x2ap_search_tree_node_t *))
{
     ytInit(p_tree,(compare_func)compare,keyof);
}

/*****************************************************************************
 * Function Name  : x2ap_st_get_first_node
 * Inputs         : p_tree - Pointer to Tree
 * Outputs        : None
 * Returns        : Pointer to first node
 * Description    : The function returns first node using DEFAULT_TRAVERSAL.
 *****************************************************************************/
x2ap_search_tree_node_t    * x2ap_st_get_first_node(
    const x2ap_search_tree_t *p_tree)
{
    return ytFirst(p_tree, X2AP_DEFAULT_TRAVERSAL);
}

/*****************************************************************************
 * Function Name  : x2ap_st_get_first_node_by_order
 * Inputs         : p -  Pointer to Tree
 *                  order - Order of traversal
 * Outputs        : None
 * Returns        : Pointer to first node.
 *                  NULL if no node present.
 * Description    : The function returns pointer to first node using order
 *                  specified through order argument.
 *****************************************************************************/
x2ap_search_tree_node_t    * x2ap_st_get_first_node_by_order(
     /* Pointer to Tree */
    const x2ap_search_tree_t *p_tree,

    /* Order of traversal */
    S32 order)
{
    return ytFirst(p_tree, order);
}

/*****************************************************************************
 * Function Name  : x2ap_st_get_next_node
 * Inputs         : p -  Pointer to Tree
 * Outputs        : None
 * Returns        : Pointer to node next to x2ap_search_tree_node_t * p.
 *                  NULL if no node present.
 * Description    : The function returns node next to p using DEFAULT_TRAVERSAL.
 *****************************************************************************/
x2ap_search_tree_node_t    * x2ap_st_get_next_node(
     const x2ap_search_tree_node_t *p)
{
   return ytNext(p, X2AP_DEFAULT_TRAVERSAL);
}


/*****************************************************************************
 * Function Name  : x2ap_st_get_next_node_by_order
 * Inputs         : p -  Pointer to Tree
 *                  order - Order of traversal
 * Outputs        : None
 * Returns        : Pointer to node next to x2ap_search_tree_node_t *p.
 *                  NULL if no node present.
 * Description    : The function returns node next to p using order specified by
 *                  agrument order.
 *****************************************************************************/
x2ap_search_tree_node_t    * x2ap_st_get_next_node_by_order(
     const x2ap_search_tree_node_t *p,
     S32 order)
{
   return ytNext(p, order);
}

/*****************************************************************************
 * Function Name  : x2ap_st_get_prev_node
 * Inputs         : p -  Pointer to Tree
 * Outputs        : None
 * Returns        : Pointer to node previous to p.
 *                  NULL if no node present.
 * Description    : The function returns node previous to p ,using
 *                  DEFAULT_TRAVERSAL
 *****************************************************************************/
x2ap_search_tree_node_t*    x2ap_st_get_prev_node(
    const x2ap_search_tree_node_t *p)
{
    return ytPrev(p, X2AP_DEFAULT_TRAVERSAL);
}

/*****************************************************************************
 * Function Name  : x2ap_st_get_prev_node_by_order
 * Inputs         : p -  Pointer to Tree
 *                  order - Order of traversal
 * Outputs        : None
 * Returns        : Pointer to node previous to p.
 *                  NULL if no node present.
 * Description    : The function returns node previous to p ,using
 *                  order specified by argument order.
 *****************************************************************************/
x2ap_search_tree_node_t*    x2ap_st_get_prev_node_by_order(
    const x2ap_search_tree_node_t *p,
    S32 order)
{
    return ytPrev(p, order);
}

/*****************************************************************************
 * Function Name  : x2ap_st_get_node_count
 * Inputs         : p_tree -  pointer to Tree
 * Outputs        : None
 * Returns        : Node count of the tree.
 * Description    : The function returns Number of nodes present in a tree.
 *****************************************************************************/
U32 x2ap_st_get_node_count(
    const x2ap_search_tree_t *p_tree)
{
    return ytCount(p_tree);
}


/*****************************************************************************
 * Function Name  : x2ap_st_find_tree_node
 * Inputs         : p_tree - Pointer to Tree
 *                  item - Pointer to key element
 * Outputs        : None
 * Returns        : Pointer to node searched.
 *                  NULL : if node not found.
 * Description    : The function finds out the tree node with key element as
 *                  item.
 *****************************************************************************/
x2ap_search_tree_node_t * x2ap_st_find_tree_node(
     const x2ap_search_tree_t *p_tree,
     const void *item)
{
    return ytFind(p_tree, item);
}

/*****************************************************************************
 * Function Name  : x2ap_st_insert_node
 * Inputs         : p_tree -  Pointer to Tree
 *                  entry  -  Pointer to node being inserted
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to insert a node into tree.
 *****************************************************************************/
void x2ap_st_insert_node(
     x2ap_search_tree_t *p_tree,
     x2ap_search_tree_node_t *entry)
{
    ytInsert(p_tree, entry);
}


/*****************************************************************************
 * Function Name  : x2ap_st_delete_node
 * Inputs         : p_tree -  Pointer to Tree
 *                  entry  -  Pointer to node being deleted
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to delete a node from tree.
 *****************************************************************************/
void x2ap_st_delete_node(
     x2ap_search_tree_t *p_tree,
     x2ap_search_tree_node_t *entry)
{
    ytDelete(p_tree, entry);
}

