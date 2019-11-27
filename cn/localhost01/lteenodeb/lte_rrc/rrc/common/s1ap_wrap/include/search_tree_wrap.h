
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: search_tree_wrap.h,v 1.3 2010/03/24 09:50:40 gur18569 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for search tree 
 *                     wrapper functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: search_tree_wrap.h,v $
 * Revision 1.3  2010/03/24 09:50:40  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2.2.2  2010/03/05 08:57:39  gur21006
 * char changed to S8, int changed to S32
 *
 * Revision 1.2.2.1  2010/02/02 13:18:15  ukr15916
 * tabs->spaces expanded
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.2  2009/11/05 12:21:31  gur21006
 * Compilation errors removed
 *
 * Revision 1.1.2.1  2009/10/23 16:16:38  gur18569
 * Initial version
 *
 *
 *
 ****************************************************************************/

#ifndef _SEARCH_TREE_WRAP_H_
#define _SEARCH_TREE_WRAP_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "cspl.h"
#include "rrc_defines.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* Traversal macros */
#define INORDER_TRAVERSAL T_INORDER
#define PREORDER_TRAVERSAL T_PREORDER
#define POSTORDER_TRAVERSAL T_POSTORDER
#define DEFAULT_TRAVERSAL T_INORDER



/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* Tree data type */
typedef YTREE search_tree_t;

/* Tree node data type */
typedef YTNODE search_tree_node_t ;


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

void st_init (
      /* Pointer to tree being initialised */
      search_tree_t *p_tree,

      /* Pointer to compare function :
       * it returns 0 if A==B, -1 if A<B and 1 if A>B
       */
      S32 (*compare)(const void *, const void *),

      /* Pointer to function returning Key
       */
      const void *(*keyof)(const search_tree_node_t *));


search_tree_node_t    * st_get_first_node (
     /* Pointer to Tree */
    const search_tree_t *p_tree );

search_tree_node_t    * st_get_first_node_by_order (
     /* Pointer to Tree */
    const search_tree_t *p_tree,

    /* Order of traversal */
    S32 order);


search_tree_node_t    * st_get_next_node (
     /* Pointer to Tree */
     const search_tree_node_t *p );


search_tree_node_t    * st_get_next_node_by_order (
     /* Pointer to Tree */
     const search_tree_node_t *p,

    /* Order of traversal */
     S32 order);


search_tree_node_t*    st_get_prev_node (
     /* Pointer to Tree */
    const search_tree_node_t *p );


search_tree_node_t*    st_get_prev_node_by_order (
     /* pointer to Tree */
    const search_tree_node_t *p,

    /* Order of traversal */
    S32 order);


U32 st_get_node_count (
     /* pointer to Tree */
    const search_tree_t *p_tree);


search_tree_node_t * st_find_tree_node (
     /* Pointer to Tree */
     const search_tree_t *p_tree,

     /* Pointer to key element */
     const void *item);


void st_insert_node (
     /* Pointer to Tree */
     search_tree_t *p_tree,

     /* Pointer to node being inserted */
     search_tree_node_t *entry);


void st_delete_node(
     /* Pointer to Tree */
     search_tree_t *p_tree,

     /* Pointer to node being deleted */
     search_tree_node_t *entry);

/*SPR_16680_START*/
void st_insert_duplicate_node (
     /* Pointer to Tree */
     search_tree_t *p_tree,
     
     /* Pointer to node being inserted */
     search_tree_node_t *entry);
/*SPR_16680_END*/

#endif  /* _SEARCH_TREE_WRAP_H_ */
