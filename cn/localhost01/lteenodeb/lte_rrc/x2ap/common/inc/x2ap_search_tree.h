/******************************************************************************
*
*   FILE NAME   : x2ap_search_tree.h
*
*   DESCRIPTION : This file contains the structure and function declaration
*                required for x2ap search tree.
*
*   DATE            AUTHOR         REFERENCE       REASON
*   28-JUNE-2010    Vaibhav Singh  ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _X2AP_SEARCH_TREE_H_
#define _X2AP_SEARCH_TREE_H_

#include "commoninc.h"

/* Traversal macros */
#define X2AP_INORDER_TRAVERSAL     T_INORDER
#define X2AP_PREORDER_TRAVERSAL    T_PREORDER
#define X2AP_POSTORDER_TRAVERSAL   T_POSTORDER
#define X2AP_DEFAULT_TRAVERSAL     T_INORDER

/* Tree data type */
typedef YTREE x2ap_search_tree_t;

/* Tree node data type */
typedef YTNODE x2ap_search_tree_node_t ;


/* Function Prototypes */
void x2ap_search_tree_init(
      x2ap_search_tree_t *p_tree,
      S32 (*compare)(const void *, const void *),
      const void *(*keyof)(const x2ap_search_tree_node_t *));

x2ap_search_tree_node_t    * x2ap_st_get_first_node(
    const x2ap_search_tree_t *p_tree);


x2ap_search_tree_node_t    * x2ap_st_get_first_node_by_order(
    const x2ap_search_tree_t *p_tree,
    S32 order);

x2ap_search_tree_node_t    * x2ap_st_get_next_node(
     const x2ap_search_tree_node_t *p);

x2ap_search_tree_node_t    * x2ap_st_get_next_node_by_order(
     const x2ap_search_tree_node_t *p,
     S32 order);

x2ap_search_tree_node_t*    x2ap_st_get_prev_node(
    const x2ap_search_tree_node_t *p);

x2ap_search_tree_node_t*    x2ap_st_get_prev_node_by_order(
    const x2ap_search_tree_node_t *p,
    S32 order);

U32 x2ap_st_get_node_count(
    const x2ap_search_tree_t *p_tree);

x2ap_search_tree_node_t * x2ap_st_find_tree_node(
     const x2ap_search_tree_t *p_tree,
     const void *item);

void x2ap_st_insert_node(
     x2ap_search_tree_t *p_tree,
     x2ap_search_tree_node_t *entry);

void x2ap_st_delete_node(
     x2ap_search_tree_t *p_tree,
     x2ap_search_tree_node_t *entry);

#endif
