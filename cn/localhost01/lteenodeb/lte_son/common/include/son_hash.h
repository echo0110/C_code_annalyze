/*! \file son_hash.h
 *  \brief Wrapper for CSPL Hash Table Data Structure
 *  \date 2012/02/27 09:50:42
 *  \author Komal Thareja
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_HASH_
#define _SON_HASH_

/*! \headerfile ylib.h <>
 */
#include    "ylib.h"

/*! \headerfile son_api_defines.h <>
 */
#include    "son_api_defines.h"

/*! \headerfile son_defines.h <>
 */
#include   <son_defines.h>

/*! \  SON_HASH from YHASH */
typedef YHASH SON_HASH;

/*! \  SON_HASH_NODE from YHNODE */
typedef YHNODE SON_HASH_NODE;

/*! \fn son_void_t hash_init(SON_HASH *p_hash, son_u32 n_buckets,son_ul32 (*hash)(const son_void_t*)
 *    ,son_s32 (*compare)(const son_void_t *, const son_void_t *),
 *    const son_void_t *(*keyof)(const SON_HASH_NODE *), SON_HASH_NODE **table)
 *  \brief Intialises the HASH table.
 *  \param p_hash			Pointer to hash variable
 *  \param n_buckets        size of the hash table
 *  \param hash             hash function
 *  \param compare          compare function
 *  \param keyof            keyoff function
 *  \param table            Pointer to the hash table
 */
son_void_t
hash_init
(
    SON_HASH *p_hash,
    son_u32 n_buckets,
    son_ul32 (*hash)
    (
        const son_void_t *
    ),
    son_s32 (*compare)
    (
        const son_void_t *,
        const son_void_t *
    ),
    const son_void_t *(*keyof)
    (
        const SON_HASH_NODE *
    ),
    SON_HASH_NODE * *table
);

/*! \fn son_void_t hash_insert(SON_HASH *p_hash, SON_HASH_NODE *p_node)
 *  \brief Inserts in the hash table.
 *  \param p_hash			Pointer to hash variable
 *  \param p_node           Pointer to the node to be inserted
 */
son_void_t
hash_insert
(
    SON_HASH *p_hash,
    SON_HASH_NODE *p_node
);

/*! \fn son_void_t hash_delete(SON_HASH *p_hash, SON_HASH_NODE *p_node)
 *  \brief Deletes from the hash table
 *  \param p_hash			Pointer to hash variable
 *  \param p_node           Pointer to the node to be deleted
 */
son_void_t
hash_delete
(
    SON_HASH *p_hash,
    SON_HASH_NODE *p_node
);

/*! \fn son_u32  hash_count(const SON_HASH *p_hash)
 *  \brief Counts the number of nodes in the hash table
 *  \param p_hash			Pointer to hash variable
 */
son_u32
hash_count
(
    const SON_HASH *p_hash
);

/*! \fn SON_HASH_NODE *hash_find(const SON_HASH *p_hash, const son_void_t *p_item)
 *  \brief Finds an item in the hash table
 *  \param p_hash			Pointer to hash variable
 *  \param p_item           Pointer to the item that will be compared to
 *                          identify node
 */
SON_HASH_NODE *
hash_find
(
    const SON_HASH *p_hash,
    const son_void_t *p_item
);

/*! \fn SON_HASH_NODE *hash_get_first(const SON_HASH *p_hash)
 *  \brief Returns the first element in the hash table on a sequential search
 *  \param p_hash			Pointer to hash variable
 */
SON_HASH_NODE *
hash_get_first
(
    const SON_HASH *p_hash
);

/*! \fn SON_HASH_NODE *hash_get_next(const SON_HASH *p_hash, SON_HASH_NODE *p_node)
 *  \brief Returns the next element in the hash table
 *  \param p_hash			Pointer to hash variable
 *  \param p_node           Pointer to the current node
 */
SON_HASH_NODE *
hash_get_next
(
    const SON_HASH *p_hash,
    SON_HASH_NODE *p_node
);

#endif /* ifndef _SON_HASH_ */
