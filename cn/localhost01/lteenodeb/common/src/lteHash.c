/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteHash.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Wrapper library for Hash table implementation. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteHash.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.5  2009/04/10 17:22:38  gur18569
 * merged with optmization changes
 *
 * Revision 1.4  2008/09/09 11:11:53  ssinghal
 * comments added to functions
 *
 * Revision 1.3  2008/08/29 06:16:45  ssinghal
 * hashCount function corrected
 *
 * Revision 1.2  2008/08/25 10:05:02  ssinghal
 * Template applied and minor changes done
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include    "lteHash.h"

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
 * Function Name  : hashInit 
 * Inputs         : pHash - Pointer to the LTE_HASH object.
 *                  nbuckets - Maximum number of indexes in the hash.
 *                  hash - callback function for hash functionality.
 *                  compare - callback function for comaprison of two keys.
 *                  keyof - keyof function to return the type of node.
 *                  table - memory fot the hash.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Initialises the hash table.
 ****************************************************************************/
void    hashInit(LTE_HASH *pHash, UInt32 nBuckets, ULong32 (*hash)(const void *), 
        SInt32 (*compare)(const void *, const void *), 
        const void *(*keyof)(const LTE_HASH_NODE *), LTE_HASH_NODE **table) 
{
    yhInit(pHash, nBuckets, hash, compare, keyof, table);
    return;
}


/****************************************************************************
 * Function Name  : hashInsert 
 * Inputs         : pHash - Pointer to the LTE_HASH object.
 *                  pNode - Pointer to the LTE_HASH_NODE object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Inserts a node in the hash table.
 ****************************************************************************/
void hashInsert(LTE_HASH *pHash, LTE_HASH_NODE *pNode)
{
    yhInsert(pHash, pNode);
}


/****************************************************************************
 * Function Name  : hashDelete 
 * Inputs         : pHash - Pointer to the LTE_HASH object.
 *                  pNode - Pointer to the LTE_HASH_NODE object.
 * Outputs        : None. 
 * Returns        : None.
 * Variables      : None.
 * Description    : Deletes a node from the hash table.
 ****************************************************************************/
void hashDelete(LTE_HASH *pHash, LTE_HASH_NODE *pNode)
{
    yhDelete(pHash, pNode);
}


/****************************************************************************
 * Function Name  : hashCount 
 * Inputs         : pHash - Pointer to the LTE_HASH object.
 * Outputs        : None. 
 * Returns        : Number of nodes.
 * Variables      : None.
 * Description    : Returns the number of nodes in the hash table.
 ****************************************************************************/
UInt32  hashCount(const LTE_HASH *pHash)
{
    return yhCount(pHash);
}


/****************************************************************************
 * Function Name  : hashFind 
 * Inputs         : pHash - Pointer to the LTE_HASH object.
 *                  item - Pointer to the key node to be found.
 * Outputs        : None.
 * Returns        : Node, if found.
 * Variables      : None.
 * Description    : Finds a key node in the hash table.
 ****************************************************************************/
LTE_HASH_NODE *hashFind(const LTE_HASH *pHash, const void *item)
{
    return yhFind(pHash, item);
}


/****************************************************************************
 * Function Name  : hashGetFirst 
 * Inputs         : pHash - Pointer to the LTE_HASH object.
 * Outputs        : None.
 * Returns        : First node in the hash table.
 * Variables      : None.
 * Description    : Returns the first node in the hash table.
 ****************************************************************************/
LTE_HASH_NODE *hashGetFirst(const LTE_HASH *pHash)
{
    return yhFirst(pHash);
}


/****************************************************************************
 * Function Name  : hashGetNext
 * Inputs         : pHash - Pointer to the LTE_HASH object.
 *                  pNode - Pointer to the LTE_HASH_NODE object.
 * Outputs        : None.
 * Returns        : Next node to the current node.
 * Variables      : None.
 * Description    : Returns the next node in the hash table.
 ****************************************************************************/
LTE_HASH_NODE *hashGetNext(const LTE_HASH *pHash, LTE_HASH_NODE *pNode)
{
    return yhNext(pHash, pNode);
}
