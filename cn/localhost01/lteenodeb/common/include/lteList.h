/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteList.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
 * $Log: lteList.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.5.10.1  2009/06/09 11:24:46  gur19140
 * first wave of Integration
 *
 * Revision 1.7  2009/05/12 06:06:00  gur19479
 * pdcp merging
 *
 * Revision 1.6  2009/05/06 05:37:59  gur20548
 * pdcp common merged.
 *
 * Revision 1.1  2009/05/04 14:52:04  gur20548
 * pdcp files added
 *
 * Revision 1.5  2009/01/12 18:02:26  gur20294
 * added new APIs
 *
 * Revision 1.4  2008/09/18 05:25:46  gur19836
 * Review comments incorporated
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
#ifndef _LTE_LIST_
#define _LTE_LIST_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "ylib.h"
#include "lteTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* This is needed to create a user defined list. An object of this needs to
   be passed to the intialization function of the list. */
typedef YLIST LTE_LIST;

/* This is the anchor of the entry to be made in the list. This object should
   be part of every node that is created to be inserted into a list. */
typedef YLNODE LTE_LIST_NODE;

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
 *      Initialises the list.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void listInit(LTE_LIST *pList);

/****************************************************************************
 *  function(): Description.
 *      Counts the number of nodes in the list.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
UInt32 listCount(const LTE_LIST *pList);

/****************************************************************************
 *  function(): Description.
 *      Deletes from the list.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void listDeleteNode(LTE_LIST *pList, LTE_LIST_NODE *pNode);

/****************************************************************************
 *  function(): Description.
 *      Inserts in the list.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void listInsertNode(LTE_LIST *pList, LTE_LIST_NODE *pNode);

/****************************************************************************
 *  function(): Description.
 *      Pushes in the list.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void listPushNode(LTE_LIST *pList, LTE_LIST_NODE *pNode);

/****************************************************************************
 *  function(): Description.
 *      Popps in the list.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_LIST_NODE *listPopNode(LTE_LIST *pList);

/****************************************************************************
 *  function(): Description.
 *      Retunrs the list node at the specified node number.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_LIST_NODE *getListNode(const LTE_LIST *pList, UInt32 nodeNum);

/****************************************************************************
 *  function(): Description.
 *      Retunrs the list node next to the node specified.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_LIST_NODE *getNextListNode(const LTE_LIST_NODE *pNode);
/****************************************************************************
 *  function(): Description.
 *      Retunrs the first node of the list.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_LIST_NODE *getFirstListNode(const LTE_LIST *pList);
/****************************************************************************
 *  function(): Description.
 *      Retunrs the last node of the list.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_LIST_NODE *getLastListNode(const LTE_LIST *pList);
/****************************************************************************
 *  function(): Description.
 *      Retunrs the list node previous to the node specified.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

LTE_LIST_NODE *getPrevListNode(const LTE_LIST_NODE *pNode);

/****************************************************************************
 * Function Name  : listWalk
 * Inputs         : pList - Pointer to the LTE_QUEUE object.
 *                  pNode - Pointer to the LTE_QUEUE_NODE object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function inserts an element in the list.
 ****************************************************************************/
void listWalk(const LTE_LIST *pList, void (*walker) (LTE_LIST_NODE *, void *), void *pValue);

/****************************************************************************
* Function Name  : printList
* Inputs         : queue_p - Pointer to the List
* Outputs        : Prints the details of all the nodes present in the list
* Returns        : None
* Variables      :
* Description    : This function prints all the nodes present in the list
		   sequentially.
****************************************************************************/
UInt32 printList(const LTE_LIST *list_p);

/****************************************************************************
 * Function Name  : listInsertNodeAtHead
 * Inputs         : pList - Pointer to the LTE_LIST object.
 *                  pNode - Pointer to the LTE_LIST_NODE object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Inserts an element in the head of the list.
 ****************************************************************************/
void listInsertNodeAtHead(LTE_LIST *pList, LTE_LIST_NODE *pNode);

/****************************************************************************
 * Function Name  : listInsertNodeBefore
 * Inputs         : pList - Pointer to the LTE_LIST object.
 *                  pSrcNode - Pointer to the LTE_LIST_NODE object before which
 *                  the new node needs to be added.
 *                  pNode - Pointer to the LTE_LIST_NODE to be added in the list
 * Outputs        : None.
 * Returns        : None.
 * Description    : Inserts an element before a particular node in the list.
 ****************************************************************************/
void listInsertNodeBefore(LTE_LIST *pList, LTE_LIST_NODE *pSrcNode,
    LTE_LIST_NODE *pNode);

/****************************************************************************
 * Function Name  : listInsertNodeAfter
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to insert Node after Node in linked List.
 ********************************************************************************/
void listInsertNodeAfter(LTE_LIST *pList, LTE_LIST_NODE *pSrcNode,
			 LTE_LIST_NODE *pNode);
#endif
