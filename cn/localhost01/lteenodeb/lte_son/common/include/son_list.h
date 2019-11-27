/*! \file son_list.h
 *  \brief Wrapper for CSPL List Data Structure
 *  \date 2012/02/27 09:50:42
 *  \author Komal Thareja
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_LIST_
#define _SON_LIST_

/*! \headerfile ylib.h <>
 */
#include    "ylib.h"

/*! \headerfile son_api_defines.h <>
 */
#include        "son_api_defines.h"

/*! \headerfile son_defines.h <>
 */
#include   <son_defines.h>

/*! \  SON_LIST from YLIST */
typedef YLIST SON_LIST;

/*! \  SON_LIST_NODE from YLNODE */
typedef YLNODE SON_LIST_NODE;

/*! \fn son_void_t  list_init(SON_LIST *p_list)
 *  \brief Initializes the list
 *  \param p_list			Pointer to YLIST structure of the Doubly link list
 */
son_void_t
list_init
(
    SON_LIST *p_list
);

/*! \fn son_u32  list_count(const SON_LIST *p_list)
 *  \brief Counts the number of nodes in the list
 *  \param p_list			Pointer to YLIST structure of the Doubly link list
 */
son_u32
list_count
(
    const SON_LIST *p_list
);

/*! \fn son_void_t  list_delete_node(SON_LIST *p_list, SON_LIST_NODE *p_node)
 *  \brief Deletes from the list
 *  \param p_list			Pointer to YLIST structure of the Doubly link list
 *  \param p_node           pointer to the node to be deleted
 */
son_void_t
list_delete_node
(
    SON_LIST *p_list,
    SON_LIST_NODE *p_node
);

/*! \fn son_void_t  list_insert_node(SON_LIST *p_list, SON_LIST_NODE *p_node)
 *  \brief Insert a node to the list
 *  \param p_list			Pointer to YLIST structure of the Doubly link list
 *  \param p_node           pointer to the node to be inserted
 */
son_void_t
list_insert_node
(
    SON_LIST *p_list,
    SON_LIST_NODE *p_node
);

/*! \fn SON_LIST_NODE* list_pop_node(SON_LIST *p_list)
 *  \brief Pops in the list
 *  \param p_list			Pointer to YLIST structure of the Doubly link list
 */
SON_LIST_NODE *
list_pop_node
(
    SON_LIST *p_list
);

/*! \fn SON_LIST_NODE* get_list_node(const SON_LIST *p_list, son_u32 nodeNum)
 *  \brief Returns the list node at the specified node number
 *  \param p_list			Pointer to YLIST structure of the Doubly link list
 *  \param nodeNum          number of the node to be fetched
 */
SON_LIST_NODE *
get_list_node
(
    const SON_LIST *p_list,
    son_u32 nodeNum
);

/*! \fn SON_LIST_NODE *get_next_list_node(const SON_LIST_NODE *p_node)
 *  \brief Returns the list node next to the node specified
 *  \param p_node			Pointer to current node
 */
SON_LIST_NODE *
get_next_list_node
(
    const SON_LIST_NODE *p_node
);

/*! \fn SON_LIST_NODE *get_first_list_node(const SON_LIST *p_list)
 *  \brief Returns the first node of the list
 *  \param p_list			Pointer to YLIST structure of the Doubly link list
 */
SON_LIST_NODE *
get_first_list_node
(
    const SON_LIST *p_list
);

/*! \fn SON_LIST_NODE *get_last_list_node(const SON_LIST *p_list)
 *  \brief Returns the last node of the list
 *  \param p_list			Pointer to YLIST structure of the Doubly link list
 */
SON_LIST_NODE *
get_last_list_node
(
    const SON_LIST *p_list
);

/*! \fn SON_LIST_NODE *get_prev_list_node(const SON_LIST_NODE *p_node)
 *  \brief Returns the list node previous to the node specified
 *  \param p_node		Pointer to the current node
 */
SON_LIST_NODE *
get_prev_list_node
(
    const SON_LIST_NODE *p_node
);

/*! \fn SON_LIST_NODE* list_find_node(	const SON_LIST *p_list,
 *      const son_void_t *p_key, const son_void_t *(*keyof)
 *      (const SON_LIST_NODE *), int (*compare)(const son_void_t *,
 *      const son_void_t *))
 *  \brief Finds node in the list
 *  \param p_list      Pointer to YLIST structure of the Doubly link list
 *  \param p_key       Pointer to key
 *  \param keyof       keyof function
 *  \param compare     compare function
 */
SON_LIST_NODE *
list_find_node
(
    const SON_LIST *p_list,
    const son_void_t *p_key,
    const son_void_t *(*keyof)
    (
        const SON_LIST_NODE *
    ),
    int (*compare)
    (
        const son_void_t *,
        const son_void_t *
    )
);

/*! \fn son_void_t* list_sort(SON_LIST *p_list,
 *      const son_void_t *(*keyof)(const SON_LIST_NODE *), 
 *      int (*compare)(const son_void_t *, const son_void_t *))
 *  \brief Sorts a given linked list
 *  \param p_list      Pointer to YLIST structure of the Doubly link list
 *  \param keyof       keyof function
 *  \param compare     compare function
 */
son_void_t *
list_sort
(
    SON_LIST            *p_list,
    const son_void_t    *(*keyof)
    (
        const SON_LIST_NODE *
    ),
    int (*compare)
    (
        const son_void_t *,
        const son_void_t *
    )
);

#endif /* ifndef _SON_LIST_ */
