/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteList.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Wrapper library for list implementation.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteList.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.5.10.1  2009/06/09 11:24:51  gur19140
 * first wave of Integration
 *
 * Revision 1.6  2009/05/06 05:39:24  gur20548
 * pdcp common merged with lte common
 *
 * Revision 1.5  2009/01/12 17:09:02  gur20294
 * added some method
 *
 * Revision 1.4  2008/09/18 05:25:08  gur19836
 * Review comments incorporated
 *
 * Revision 1.3  2008/09/09 11:11:53  ssinghal
 * comments added to functions
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
#include "lteList.h"
#include "lteLog.h"

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
 * Function Name  : listInit
 * Inputs         : pList - Pointer to the LTE_LIST object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Initialises the list.
 ****************************************************************************/
void listInit(LTE_LIST *pList)
{
	if (pList != PNULL)
		ylInit(pList);

	return;
}

/****************************************************************************
 * Function Name  : listCount
 * Inputs         : pList - Pointer to the LTE_LIST object.
 * Outputs        : None.
 * Returns        : Number of elements in the list.
 * Variables      : None.
 * Description    : Counts the number of elements in the list.
 ****************************************************************************/
UInt32 listCount(const LTE_LIST *pList)
{
	return ylCount(pList);
}


/****************************************************************************
 * Function Name  : listDeleteNode
 * Inputs         : pList - Pointer to the LTE_LIST object.
 *                  pNode - Pointer to the LTE_LIST_NODE object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Deletes an element in the list.
 ****************************************************************************/
void listDeleteNode(LTE_LIST *pList, LTE_LIST_NODE *pNode)
{
	if ((pList != PNULL) && (pNode != PNULL))
		ylDelete(pList, pNode);

	return;
}

/****************************************************************************
 * Function Name  : listInsertNode
 * Inputs         : pList - Pointer to the LTE_LIST object.
 *                  pNode - Pointer to the LTE_LIST_NODE object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Inserts an element in the list.
 ****************************************************************************/
void listInsertNode(LTE_LIST *pList, LTE_LIST_NODE *pNode)
{
	if ((pList != PNULL) && (pNode != PNULL))
		ylPushTail(pList, pNode);

	return;
}

/****************************************************************************
 * Function Name  : listPushNode
 * Inputs         : pList - Pointer to the LTE_QUEUE object.
 *                  pNode - Pointer to the LTE_QUEUE_NODE object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function inserts an element in the list.
 ****************************************************************************/
void listPushNode(LTE_LIST *pList, LTE_LIST_NODE *pNode)
{
	listInsertNode(pList, pNode);

	return;
}

/****************************************************************************
 * Function Name  : listPopNode
 * Inputs         : pList - Pointer to the LTE_QUEUE object.
 * Outputs        : None.
 * Returns        : The front popped element.
 * Variables      : None.
 * Description    : This function popps the front element from the list.
 ****************************************************************************/
LTE_LIST_NODE *listPopNode(LTE_LIST *pList)
{
	if (pList != PNULL)
		return ylPopHead(pList);

	return PNULL;
}

/****************************************************************************
 * Function Name  : getListNode
 * Inputs         : pList - Pointer to the LTE_LIST object.
 *                  nodeNum - Node number to be found and returned.
 * Outputs        : None.
 * Returns        : The node in the list at the specified number location.
 * Variables      : None.
 * Description    : Returns the node at the specific location. Note that the
 *                  node is not removed from the list.
 ****************************************************************************/
LTE_LIST_NODE *getListNode(const LTE_LIST *pList, UInt32 nodeNum)
{
	if (pList != PNULL)
		return ylNth(pList, nodeNum);

	return PNULL;
}


/****************************************************************************
 * Function Name  : getFirstListNode
 * Inputs         : pList - Pointer to the LTE_LIST object.
 * Outputs        : None.
 * Returns        : The first node in the list.
 * Variables      : None.
 * Description    : Returns the first node in the list. Note that the
 *                  node is not removed from the list.
 ****************************************************************************/
LTE_LIST_NODE *getFirstListNode(const LTE_LIST *pList)
{
	if (pList != PNULL)
		return ylFirst(pList);

	return PNULL;
}
/****************************************************************************
 * Function Name  : getLastListNode
 * Inputs         : pList - Pointer to the LTE_LIST object.
 * Outputs        : None.
 * Returns        : The next node in the list.
 * Variables      : None.
 * Description    : Returns the node next in the list. Note that the
 *                  node is not removed from the list.
 ****************************************************************************/
LTE_LIST_NODE *getLastListNode(const LTE_LIST *pList)
{
	if (pList != PNULL)
		return ylLast(pList);

	return PNULL;
}
/****************************************************************************
 * Function Name  : getNextListNode
 * Inputs         : pNode - Pointer to the LTE_LIST_NODE object.
 * Outputs        : None.
 * Returns        : The next node in the list.
 * Variables      : None.
 * Description    : Returns the node next in the list. Note that the
 *                  node is not removed from the list.
 ****************************************************************************/
LTE_LIST_NODE *getNextListNode(const LTE_LIST_NODE *pNode)
{
	if (pNode != PNULL)
		return ylNext(pNode);

	return PNULL;
}

/****************************************************************************
 * Function Name  : getPrevListNode
 * Inputs         : pNode - Pointer to the LTE_LIST_NODE object.
 * Outputs        : None.
 * Returns        : The next node in the list.
 * Variables      : None.
 * Description    : Returns the node next in the list. Note that the
 *                  node is not removed from the list.
 ****************************************************************************/
LTE_LIST_NODE *getPrevListNode(const LTE_LIST_NODE *pNode)
{
	if (pNode != PNULL)
		return ylPrev(pNode);

	return PNULL;
}

/****************************************************************************
* Function Name  : printList
* Inputs         : queue_p - Pointer to the List
* Outputs        : Prints the details of all the nodes present in the list
* Returns        : None
* Variables      :
* Description    : This function prints all the nodes present in the list
		   sequentially.
****************************************************************************/
UInt32 printList(const LTE_LIST *list_p)
{
	UInt32 x = 1;
	LTE_LIST_NODE *node_p = PNULL;

	if (list_p != PNULL) {
		node_p = getFirstListNode(list_p);

		LTE_LOG(LOG_INFO, PNULL, "Num of nodes in List = %d\n", listCount(list_p));
		while (node_p) {
			LTE_LOG(LOG_INFO, PNULL, "Node Num = %d\t", x);
			LTE_LOG(LOG_INFO, PNULL, "Address of Node = %p\n", node_p);
			node_p = node_p->next;
			x++;
		}
		return listCount(list_p);
	} else {
		LTE_LOG(LOG_MAJOR, PNULL, "In Fn %s, LTE_LIST* passed is NULL\n", __func__);
		return 0;
	}
}

/* DRX_CHG */
/****************************************************************************
 * Function Name  : listInsertNodeAtHead
 * Inputs         : pList - Pointer to the LTE_LIST object.
 *                  pNode - Pointer to the LTE_LIST_NODE object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Inserts an element in the head of the list.
 ****************************************************************************/
void listInsertNodeAtHead(LTE_LIST *pList, LTE_LIST_NODE *pNode)
{
	if ((pList != PNULL) && (pNode != PNULL))
		ylPushHead(pList, pNode);

	return;
}

/****************************************************************************
 * Function Name  : listInsertNodeBefore
 * Inputs         : pList - Pointer to the LTE_LIST object.
 *                  pSrcNode - Pointer to the LTE_LIST_NODE object before which
 *                  the new node needs to be added.
 *                  pNode - Pointer to the LTE_LIST_NODE to be added in the list
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Inserts an element before a particulat node in the list.
 ****************************************************************************/
void listInsertNodeBefore(LTE_LIST *pList, LTE_LIST_NODE *pSrcNode, LTE_LIST_NODE *pNode)
{
	if ((pList != PNULL) && (pNode != PNULL)) {
		if (pSrcNode == PNULL)
		    ylPushHead(pList, pNode);
		else
		    ylInsertBefore(pList, pSrcNode, pNode);
	}

	return;
}
/* DRX_CHG */

/*****************************************************************************
 * Function Name  : listInsertNodeAfter
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to insert Node after Node in linked List.
 *******************************************************************************/
void listInsertNodeAfter(LTE_LIST *pList, LTE_LIST_NODE *pSrcNode,
			 LTE_LIST_NODE *pNode)
{
	if ((pList != PNULL) && (pNode != PNULL))
		ylInsertAfter(pList, pSrcNode, pNode);

	return;
}
