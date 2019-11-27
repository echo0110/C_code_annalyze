/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteHash.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteHash.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.4  2009/04/10 17:22:38  gur18569
 * merged with optmization changes
 *
 * Revision 1.3  2008/09/09 06:47:45  ssinghal
 * Added comments and removed tabs from the file
 *
 * Revision 1.2  2008/08/25 10:04:35  ssinghal
 * Template applied and minor changes done
 *
 *
 *
 ****************************************************************************/

#ifndef _LTE_HASH_
#define _LTE_HASH_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include    "ylib.h"
#include    "lteTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* This is needed to create a user defined hash table. An object of this 
   needs to be passed to the intialization function of the hash table. */
typedef YHASH LTE_HASH;

/* This is the anchor of the entry to be made in the hash table. This object 
   should be part of every node that is created to be inserted into 
   hash table. */
typedef YHNODE LTE_HASH_NODE;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 *  function(): Description.
 *      Intialises the HASH table.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    hashInit(LTE_HASH *pHash, UInt32 nBuckets, 
        ULong32 (*hash)(const void*),
        SInt32 (*compare)(const void *, const void *),
        const void *(*keyof)(const LTE_HASH_NODE *), LTE_HASH_NODE **table);


/****************************************************************************
 *  function(): Description.
 *      Inserts in the hash table.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void hashInsert(LTE_HASH *pHash, LTE_HASH_NODE *pNode);


/****************************************************************************
 *  function(): Description.
 *      Delets from the hash table.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void hashDelete(LTE_HASH *pHash, LTE_HASH_NODE *pNode);


/****************************************************************************
 *  function(): Description.
 *      Counts the number of nodes in the hash table.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
UInt32  hashCount(const LTE_HASH *pHash);


/****************************************************************************
 *  function(): Description.
 *      Finds an item in the hash table.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_HASH_NODE *hashFind(const LTE_HASH *pHash, const void *item);


/****************************************************************************
 *  function(): Description.
 *      Returns the first element in the hash table on a sequential search.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_HASH_NODE *hashGetFirst(const LTE_HASH *pHash);


/****************************************************************************
 *  function(): Description.
 *      Returns the next element in the hash table.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_HASH_NODE *hashGetNext(const LTE_HASH *pHash, LTE_HASH_NODE *pNode);

#endif
