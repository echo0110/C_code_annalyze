/****************************************************************************
*
* ARICENT -
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
* SPR-612 Coverity Fix Starts
* $Id: son_hash.c $
* SPR-612 Coverity Fix Ends
*
****************************************************************************
*
* File Description: Wrapper for CSPL Hash Table Data Structure
*
****************************************************************************
* Revision Details
* ----------------
*
* SPR-612 Coverity Fix Starts
* $Log: son_hash.c $
* SPR-612 Coverity Fix Ends
* Revision 0.1  2012/02/27 09:50:42  Komal Thareja
* Initial code
*
****************************************************************************/

/****************************************************************************
* Standard Library Includes
****************************************************************************/

/****************************************************************************
* Project Includes
****************************************************************************/
#include    "son_hash.h"
#include    <son_utils.h>

/****************************************************************************
*  Private Definitions
****************************************************************************/
/* Used in logger for identifying SON UTILS */
const son_8 *son_hash_facility_name = "SON HASH";

/****************************************************************************
*  Private Types
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
* Function Name  : hash_init
* Inputs         : p_hash - Pointer to the SON_HASH object.
*                  nbuckets - Maximum number of indexes in the hash.
*                  hash - callback function for hash functionality.
*                  compare - callback function for comparison of two keys.
*                  keyof - keyof function to return the type of node.
*                  table - memory fot the hash.
* Outputs        : None.
* Returns        : None.
* Variables      : None.
* Description    : Initialises the hash table.
****************************************************************************/
son_void_t
hash_init
(
    SON_HASH            *p_hash,
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
    const son_void_t    *(*keyof)
    (
        const SON_HASH_NODE *
    ),
    SON_HASH_NODE       * *table
)
{
    if (SON_PNULL == p_hash || SON_PNULL == hash || SON_PNULL == compare
        || SON_PNULL == keyof ||  SON_PNULL == table)
    {
        SON_LOG(SON_OAM_LOG_ON, son_hash_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
    else
    {
        yhInit(p_hash, n_buckets, hash, compare, keyof, table);
    }
}

/****************************************************************************
* Function Name  : hash_insert
* Inputs         : p_hash - Pointer to the SON_HASH object.
*                  p_node - Pointer to the SON_HASH_NODE object.
* Outputs        : None.
* Returns        : None.
* Variables      : None.
* Description    : Inserts a node in the hash table.
****************************************************************************/
son_void_t
hash_insert
(
    SON_HASH        *p_hash,
    SON_HASH_NODE   *p_node
)
{
    if (SON_PNULL == p_hash || SON_PNULL == p_node)
    {
        SON_LOG(SON_OAM_LOG_ON, son_hash_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
    else
    {
        yhInsert(p_hash, p_node);
    }
}

/****************************************************************************
* Function Name  : hash_delete
* Inputs         : p_hash - Pointer to the SON_HASH object.
*                  p_node - Pointer to the SON_HASH_NODE object.
* Outputs        : None.
* Returns        : None.
* Variables      : None.
* Description    : Deletes a node from the hash table.
****************************************************************************/
son_void_t
hash_delete
(
    SON_HASH        *p_hash,
    SON_HASH_NODE   *p_node
)
{
    if (SON_PNULL == p_hash || SON_PNULL == p_node)
    {
        SON_LOG(SON_OAM_LOG_ON, son_hash_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
    else
    {
        yhDelete(p_hash, p_node);
    }
}

/****************************************************************************
* Function Name  : hash_count
* Inputs         : p_hash - Pointer to the SON_HASH object.
* Outputs        : None.
* Returns        : Number of nodes.
* Variables      : None.
* Description    : Returns the number of nodes in the hash table.
****************************************************************************/
son_u32
hash_count
(
    const SON_HASH *p_hash
)
{
    son_u32 count = 0;

    if (SON_PNULL == p_hash)
    {
        SON_LOG(SON_OAM_LOG_ON, son_hash_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
    else
    {
        count = yhCount(p_hash);
    }

    return count;
}

/****************************************************************************
* Function Name  : hash_find
* Inputs         : p_hash - Pointer to the SON_HASH object.
*                  p_item - Pointer to the key node to be found.
* Outputs        : None.
* Returns        : Node, if found.
* Variables      : None.
* Description    : Finds a key node in the hash table.
****************************************************************************/
SON_HASH_NODE *
hash_find
(
    const SON_HASH      *p_hash,
    const son_void_t    *p_item
)
{
    SON_HASH_NODE *p_node = SON_PNULL;

    if (SON_PNULL == p_hash || SON_PNULL == p_item)
    {
        SON_LOG(SON_OAM_LOG_ON, son_hash_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
    else
    {
        p_node = yhFind(p_hash, p_item);
    }

    return p_node;
}

/****************************************************************************
* Function Name  : hash_get_first
* Inputs         : p_hash - Pointer to the SON_HASH object.
* Outputs        : None.
* Returns        : First node in the hash table.
* Variables      : None.
* Description    : Returns the first node in the hash table.
****************************************************************************/
SON_HASH_NODE *
hash_get_first
(
    const SON_HASH *p_hash
)
{
    SON_HASH_NODE *p_node = SON_PNULL;

    if (SON_PNULL == p_hash)
    {
        SON_LOG(SON_OAM_LOG_ON, son_hash_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
    else
    {
        p_node = yhFirst(p_hash);
    }

    return p_node;
}

/****************************************************************************
* Function Name  : hash_get_next
* Inputs         : p_hash - Pointer to the SON_HASH object.
*                  p_node - Pointer to the SON_HASH_NODE object.
* Outputs        : None.
* Returns        : Next node to the current node.
* Variables      : None.
* Description    : Returns the next node in the hash table.
****************************************************************************/
SON_HASH_NODE *
hash_get_next
(
    const SON_HASH  *p_hash,
    SON_HASH_NODE   *p_node
)
{
    SON_HASH_NODE *p_next_node = SON_PNULL;

    if (SON_PNULL == p_hash || SON_PNULL == p_node)
    {
        SON_LOG(SON_OAM_LOG_ON, son_hash_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
    else
    {
        p_next_node = yhNext(p_hash, p_node);
    }

    return p_next_node;
}
