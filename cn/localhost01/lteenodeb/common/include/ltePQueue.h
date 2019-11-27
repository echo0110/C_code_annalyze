/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: ltePQueue.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
 * $Log: ltePQueue.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3  2008/09/09 06:47:46  ssinghal
 * Added comments and removed tabs from the file
 *
 * Revision 1.2  2008/08/25 10:04:35  ssinghal
 * Template applied and minor changes done
 *
 *
 *
 ****************************************************************************/


#ifndef _LTE_PQUEUE_
#define _LTE_PQUEUE_

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

/* This is needed to create a user defined priority queue. An object of this 
   needs to be passed to the intialization function of the priority queue. */
typedef YSQUEUE LTE_PQUEUE;

/* This is the anchor of the entry to be made in the priority queue. 
   This object should be part of every node that is created to be inserted 
   into priority queue. */
typedef YSNODE LTE_PQUEUE_NODE;

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
 *      This API initialises the priority queue.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

void    pqInit(LTE_PQUEUE *, SInt32 (*)(const LTE_PQUEUE_NODE *) );


/****************************************************************************
 *  function(): Description.
 *      This API counts the number of nodes in the priority queue.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
UInt32  pqCount(const LTE_PQUEUE *);


/****************************************************************************
 *  function(): Description.
 *      This API pops one element from the head of the priority queue.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_PQUEUE_NODE *pqPopHead( LTE_PQUEUE * );


/****************************************************************************
 *  function(): Description.
 *      This API pushes one element at the tail of the priority queue.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    pqPushTail( LTE_PQUEUE *, LTE_PQUEUE_NODE * );

#endif
