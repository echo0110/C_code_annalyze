/***********************************************************************
**  FUNCTION:
**	GTP-U circular linked list implementation
************************************************************************
**
**  FILE NAME:
**	egtpu_list.c
**
**  DESCRIPTION:
**	Contains definitions/routines to operate on
**  circular linked list. These are based on CSPL utility functions.
**
**  DATE	    NAME				REFERENCE	REASON
**  ----	    ----				---------	------
**  Oct 2009    Vipul Aggarwal        			Initial
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/

#include <egtpu_glob.h>
#include <egtpu_list.h>

/********************************************************************************
 * Function Name  : xlInit
 * Inputs         : head - Pointer to head node of XLIST
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion initialise the list 
 ********************************************************************************/
egtpu_void_t xlInit (XLIST *head)
{
    head->next = head->prev = head;
}

/********************************************************************************
 * Function Name  : xlEmpty
 * Inputs         : head - Pointer to head node of XLIST
 * Outputs        : None
 * Returns        : egtpu_boolean_t
 * Description    : This funtion empty the list
 ********************************************************************************/
egtpu_boolean_t xlEmpty (XLIST *head)
{
    return head == head->next;
}

/********************************************************************************
 * Function Name  : xlInsert
 * Inputs         : entry - Pointer to the node to be entered in XLIST,
 *                  prev - Pointer to the previous node of corresponding entry,
 *                  next - Pointer to the next node of corresponding entry
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion inserts a node in the XLIST.
 ********************************************************************************/
egtpu_void_t xlInsert (XLIST *entry, XLIST *prev, XLIST *next)
{
    entry->next = next;
    entry->prev = prev;
    prev->next = entry;
    next->prev = entry;
}

/********************************************************************************
 * Function Name  : xlDelete
 * Inputs         : entry - Pointer to the node to be deleted in XLIST
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion deletes the desired node from XLIST.
 ********************************************************************************/
egtpu_void_t xlDelete (XLIST *entry)
{
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;
}

/********************************************************************************
 * Function Name  : xlPushHead
 * Inputs         : head - Pointer to head node of XLIST,
 *                  entry - Pointer to the node to be entered in XLIST as new head
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion inserts a new node as head in XLIST.
 ********************************************************************************/
egtpu_void_t xlPushHead (XLIST *head, XLIST *entry)
{
    xlInsert (entry, head, head->next);
}

/********************************************************************************
 * Function Name  : xlPushTail
 * Inputs         : head - Pointer to head node of XLIST,
 *                  entry - Pointer to the node to be entered in XLIST as new head
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion inserts a new node as last node in XLIST.
 ********************************************************************************/
egtpu_void_t xlPushTail (XLIST *head, XLIST *entry)
{
    xlInsert (entry, head->prev, head);
}

/********************************************************************************
 * Function Name  : xlPopHead
 * Inputs         : head - Pointer to head node of XLIST
 * Outputs        : None
 * Returns        : node/EGTPU_NULL
 * Description    : This funtion deletes the head node in XLIST and if the XLIST
 *                  is empty then returns NULL.
 ********************************************************************************/
XLIST *xlPopHead (XLIST *head)
{
    XLIST       *node = head->next;

    if (xlEmpty (head))
        return EGTPU_NULL;

    xlDelete (node);
    return node;
}

/********************************************************************************
 * Function Name  : xlPopTail
 * Inputs         : head - Pointer to head node of XLIST
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion removes the last node from XLIST and if the XLIST.
 *                  is empty then returns NULL.
 ********************************************************************************/
XLIST *xlPopTail (XLIST *head)
{
    XLIST       *node = head->prev;

    if (xlEmpty (head))
        return EGTPU_NULL;

    xlDelete (node);
    return node;
}
