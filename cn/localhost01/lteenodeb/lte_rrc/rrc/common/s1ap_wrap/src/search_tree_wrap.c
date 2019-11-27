
/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: search_tree_wrap.c,v 1.3 2010/03/24 09:50:40 gur18569 Exp $
 *
 ******************************************************************************
 *
 *  File Description : The file contains wrapper functions over CSPL search 
 *                     tree functions.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: search_tree_wrap.c,v $
 * Revision 1.3  2010/03/24 09:50:40  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2.2.2  2010/03/05 10:20:23  gur21006
 * char changed to S8, int changed to S32
 *
 * Revision 1.2.2.1  2010/02/02 13:18:15  ukr15916
 * tabs->spaces expanded
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.3  2009/12/24 10:32:01  gur21006
 * Warnings removed
 *
 * Revision 1.1.2.2  2009/11/05 12:20:18  gur21006
 * Compilation errors removed
 *
 * Revision 1.1.2.1  2009/10/23 16:16:09  gur18569
 * Initial version
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "search_tree_wrap.h"

/******************************************************************************
 Private Definitions
 *****************************************************************************/

/* WARNING REM START */
typedef int (*rrc_compare_func)(const void *, const void *);
/* WARNING REM STOP */



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
 * Function Name  : st_init
 * Inputs         : p_tree -  Pointer to tree being initialised
 *                  compare - Pointer to compare function :
 *                            it returns 0 if A==B, -1 if A<B and 1 if A>B
 *                  keyof -  Pointer to function returning Key
 * Outputs        :
 * Returns        : None
 * Description    : This function is called to inititalize a tree.
 *****************************************************************************/
void st_init (
      /* Pointer to tree being initialised */
      search_tree_t *p_tree,

      /* Pointer to compare function :
       * it returns 0 if A==B, -1 if A<B and 1 if A>B
       */
      S32 (*compare)(const void *, const void *),

      /* Pointer to function returning Key
       */
      const void *(*keyof)(const search_tree_node_t *))
{
     ytInit(p_tree,(rrc_compare_func)compare,keyof);
}


/*****************************************************************************
 * Function Name  : st_get_first_node
 * Inputs         : p_tree - Pointer to Tree
 * Outputs        : none
 * Returns        : Pointer to first node
 * Description    : The function returns first node using DEFAULT_TRAVERSAL.
 *****************************************************************************/
search_tree_node_t    * st_get_first_node (
    const search_tree_t *p_tree )
{
    return ytFirst (p_tree, DEFAULT_TRAVERSAL);
}

/*****************************************************************************
 * Function Name  : st_get_first_node_by_order
 * Inputs         : p -  Pointer to Tree
 *                  order - Order of traversal
 * Outputs        : None
 * Returns        : Pointer to first node.
 *                  NULL if no node present.
 * Description    : The function returns pointer to first node using order
 *                  specified through order argument.
 *****************************************************************************/
search_tree_node_t    * st_get_first_node_by_order (
     /* Pointer to Tree */
    const search_tree_t *p_tree,

    /* Order of traversal */
    S32 order)
{
    return ytFirst (p_tree, order);
}


/*****************************************************************************
 * Function Name  : st_get_next_node
 * Inputs         : p -  Pointer to Tree
 * Outputs        : None
 * Returns        : Pointer to node next to search_tree_node_t * p.
 *                  NULL if no node present.
 * Description    : The function returns node next to p using DEFAULT_TRAVERSAL.
 *****************************************************************************/
search_tree_node_t    * st_get_next_node (
     const search_tree_node_t *p )
{
   return ytNext (p, DEFAULT_TRAVERSAL);
}


/*****************************************************************************
 * Function Name  : st_get_next_node_by_order
 * Inputs         : p -  Pointer to Tree
 *                  order - Order of traversal
 * Outputs        : None
 * Returns        : Pointer to node next to search_tree_node_t *p.
 *                  NULL if no node present.
 * Description    : The function returns node next to p using order specified by
 *                  agrument order.
 *****************************************************************************/
search_tree_node_t    * st_get_next_node_by_order (
     const search_tree_node_t *p,
     S32 order)
{
   return ytNext (p, order);
}


/*****************************************************************************
 * Function Name  : st_get_prev_node
 * Inputs         : p -  Pointer to Tree
 * Outputs        : None
 * Returns        : Pointer to node previous to p.
 *                  NULL if no node present.
 * Description    : The function returns node previous to p ,using
 *                  DEFAULT_TRAVERSAL
 *****************************************************************************/
search_tree_node_t*    st_get_prev_node (
    const search_tree_node_t *p )
{
    return ytPrev (p, DEFAULT_TRAVERSAL);
}


/*****************************************************************************
 * Function Name  : st_get_prev_node_by_order
 * Inputs         : p -  Pointer to Tree
 *                  order - Order of traversal
 * Outputs        : None	
 * Returns        : Pointer to node previous to p.
 *                  NULL if no node present.
 * Description    : The function returns node previous to p ,using
 *                  order specified by argument order.
 *****************************************************************************/
search_tree_node_t*    st_get_prev_node_by_order (
    const search_tree_node_t *p,
    S32 order)
{
    return ytPrev (p, order);
}


/*****************************************************************************
 * Function Name  : st_get_node_count
 * Inputs         : p_tree -  pointer to Tree
 * Outputs        : None
 * Returns        : Node count of the tree.
 * Description    : The function returns Number of nodes present in a tree.
 *****************************************************************************/
U32 st_get_node_count (
    const search_tree_t *p_tree)
{
    return ytCount(p_tree);
}


/*****************************************************************************
 * Function Name  : st_find_tree_node
 * Inputs         : p_tree - Pointer to Tree
 *                  item - Pointer to key element
 * Outputs        : None
 * Returns        : Pointer to node searched.
 *                  NULL : if node not found.
 * Description    : The function finds out the tree node with key element as
 *                  item.
 *****************************************************************************/
search_tree_node_t * st_find_tree_node (
     const search_tree_t *p_tree,
     const void *item)
{
    return ytFind(p_tree, item);
}


/*****************************************************************************
 * Function Name  : st_insert_node
 * Inputs         : p_tree -  Pointer to Tree
 *                  entry  -  Pointer to node being inserted
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to insert a node into tree.
 *****************************************************************************/
void st_insert_node (
     search_tree_t *p_tree,
     search_tree_node_t *entry)
{
    ytInsert(p_tree, entry);
}


/*****************************************************************************
 * Function Name  : st_delete_node
 * Inputs         : p_tree -  Pointer to Tree
 *                  entry  -  Pointer to node being deleted
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to delete a node from tree.
 *****************************************************************************/
void st_delete_node (
     search_tree_t *p_tree,
     search_tree_node_t *entry)
{
    ytDelete(p_tree, entry);
}
/*SPR_16680_START*/
/*****************************************************************************
 * Function Name  : st_insert_duplicate_node
 * Inputs         : p_tree -  Pointer to Tree
 *                  entry  -  Pointer to node being inserted
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to insert node into tree.
 * 		    It allows insertion of the nodes with duplicate keys. 
 *****************************************************************************/
void st_insert_duplicate_node (
     search_tree_t *p_tree,
     search_tree_node_t *entry)
{
    ytInsertWithDuplicate(p_tree, entry);
}
/*SPR_16680_END*/
