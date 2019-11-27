
/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id:
 *
 ******************************************************************************
 *
 *  File Description : The file contains wrapper functions over CSPL search
 *                     list functions.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "list_wrap.h"

/******************************************************************************
 Private Definitions
 *****************************************************************************/



/******************************************************************************
 Private Types
 *****************************************************************************/

 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : list_init
 * Inputs         : pList -  Pointer to list being initialised
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to inititalize a list.
 *****************************************************************************/
void list_init(list_t *pList)
{
    ylInit(pList);
}

/*****************************************************************************
 * Function Name  : list_delete_node
 * Inputs         : pList -  Pointer to list being initialised
 *                  pNode -  Node to be deleted
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to delete a node in a list.
 *****************************************************************************/
void list_delete_node(list_t *pList, list_node_t *pNode)
{
    ylDelete(pList, pNode);
}

/*****************************************************************************
 * Function Name  : list_first_node
 * Inputs         : pList -  Pointer to list being initialised
 * Outputs        : None
 * Returns        : First node in the list.
 * Description    : This function is called to first node in the list.
 *****************************************************************************/
list_node_t *list_first_node(const list_t *pList)
{
    return ylFirst((const list_t *)pList);
}

/*****************************************************************************
 * Function Name  : list_next_node
 * Inputs         : pList -  Pointer to list being initialised
 * Outputs        : None
 * Returns        : First node node next to pNode in the list.
 * Description    : This function is called to next node in the list.
 *****************************************************************************/
list_node_t * list_next_node(const list_node_t *pNode)
{
    return ylNext((const list_node_t *)pNode);
}

/*****************************************************************************
 * Function Name  : list_push_head
 * Inputs         : pList -  Pointer to list being initialised
 *                  pNode -  Node to be inserted
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to insert an node at the begining
 *                  of a list.
 *****************************************************************************/
void list_push_head(list_t *pList, list_node_t *pNode)
{
    ylPushHead(pList, pNode);
}


/*****************************************************************************
 * Function Name  : list_count
 * Inputs         : pList -  Pointer to list being initialised
 * Outputs        : None
 * Returns        : Number of nodes in the list pList
 * Description    : This function is called to return number of nodes in a list
 *****************************************************************************/
U32 list_count(list_t *pList)
{
    return ylCount((const list_t *)pList);
}


/*****************************************************************************
 * Function Name  : list_pop_head
 * Inputs         : pList -  Pointer to list being initialised
 * Outputs        : None
 * Returns        : Node popped
 * Description    : This function is called pop a node from list head.
 *****************************************************************************/
list_node_t *list_pop_head(list_t *pList)
{
    return ylPopHead(pList);
}

/*****************************************************************************
 * Function Name  : list_pop_tail
 * Inputs         : pList -  Pointer to list being initialised
 * Outputs        : None
 * Returns        : Node popped
 * Description    : This function is called pop a node from list tail.
 *****************************************************************************/
list_node_t *list_pop_tail(list_t *pList)
{
    return ylPopTail(pList);
}

/*****************************************************************************
 * Function Name  : list_push_tail
 * Inputs         : pList -  Pointer to list being initialised
 *                  pNode -  Node to be inserted
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to insert an node at the end
 *                  of a list.
 *****************************************************************************/
void list_push_tail(list_t *pList, list_node_t *pNode)
{
    ylPushTail(pList, pNode);
}

