/****************************************************************************
*
* ARICENT -
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
* SPR-612 Coverity Fix Starts
* $Id: son_list.c $
* SPR-612 Coverity Fix Ends
*
****************************************************************************
*
* File Description: Wrapper for CSPL List Data Structure
*
****************************************************************************
* Revision Details
* ----------------
*
* SPR-612 Coverity Fix Starts
* $Log: son_list.c $
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
#include    "son_list.h"
#include    <son_utils.h>

/****************************************************************************
*  Private Definitions
****************************************************************************/
/* Used in logger for identifying SON UTILS */
const son_8 *son_list_facility_name = "SON LIST";

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
* Function Name  : listInit
* Inputs         : p_list - Pointer to the SON_LIST object.
* Outputs        : None.
* Returns        : None.
* Variables      : None.
* Description    : Initializes the list.
****************************************************************************/
son_void_t
list_init
(
    SON_LIST *p_list
)
{
    if (SON_PNULL != p_list)
    {
        ylInit(p_list);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
}

/****************************************************************************
* Function Name  : listCount
* Inputs         : p_list - Pointer to the SON_LIST object.
* Outputs        : None.
* Returns        : Number of elements in the list.
* Variables      : None.
* Description    : Counts the number of elements in the list.
****************************************************************************/
son_u32
list_count
(
    const SON_LIST *p_list
)
{
    son_u32 count = 0;

    if (SON_PNULL == p_list)
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
    else
    {
        count = ylCount(p_list);
    }

    return count;
}

/****************************************************************************
* Function Name  : list_delete_node
* Inputs         : p_list - Pointer to the SON_LIST object.
*                  p_node - Pointer to the SON_LIST_NODE object.
* Outputs        : None.
* Returns        : None.
* Variables      : None.
* Description    : Deletes an element in the list.
****************************************************************************/
son_void_t
list_delete_node
(
    SON_LIST        *p_list,
    SON_LIST_NODE   *p_node
)
{
    if ((p_list != SON_PNULL) && (p_node != SON_PNULL))
    {
        ylDelete(p_list, p_node);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
}

/****************************************************************************
* Function Name  : list_insert_node
* Inputs         : p_list - Pointer to the SON_LIST object.
*                  p_node - Pointer to the SON_LIST_NODE object.
* Outputs        : None.
* Returns        : None.
* Variables      : None.
* Description    : Inserts an element in the list.
****************************************************************************/
son_void_t
list_insert_node
(
    SON_LIST        *p_list,
    SON_LIST_NODE   *p_node
)
{
    if ((SON_PNULL != p_list) && (SON_PNULL != p_node))
    {
        ylPushTail(p_list, p_node);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
}

/****************************************************************************
* Function Name  : list_pop_node
* Inputs         : p_list - Pointer to the SON_QUEUE object.
* Outputs        : None.
* Returns        : The front popped element.
* Variables      : None.
* Description    : This function pops the front element from the list.
****************************************************************************/
SON_LIST_NODE *
list_pop_node
(
    SON_LIST *p_list
)
{
    SON_LIST_NODE *p_node = SON_PNULL;

    if (SON_PNULL != p_list)
    {
        p_node = ylPopHead(p_list);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }

    return p_node;
}

/****************************************************************************
* Function Name  : get_list_node
* Inputs         : p_list - Pointer to the SON_LIST object.
*                  nodeNum - Node number to be found and returned.
* Outputs        : None.
* Returns        : The node in the list at the specified number location.
* Variables      : None.
* Description    : Returns the node at the specific location. Note that the
*                  node is not removed from the list.
****************************************************************************/
SON_LIST_NODE *
get_list_node
(
    const SON_LIST  *p_list,
    son_u32 nodeNum
)
{
    SON_LIST_NODE *p_node = SON_PNULL;

    if (p_list != SON_PNULL)
    {
        p_node = ylNth(p_list, nodeNum);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }

    return p_node;
}

/****************************************************************************
* Function Name  : get_first_list_node
* Inputs         : p_list - Pointer to the SON_LIST object.
* Outputs        : None.
* Returns        : The first node in the list.
* Variables      : None.
* Description    : Returns the first node in the list. Note that the
*                  node is not removed from the list.
****************************************************************************/
SON_LIST_NODE *
get_first_list_node
(
    const SON_LIST *p_list
)
{
    SON_LIST_NODE *p_node = SON_PNULL;

    if (p_list != SON_PNULL)
    {
        p_node = ylFirst(p_list);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }

    return p_node;
}

/****************************************************************************
* Function Name  : get_last_list_node
* Inputs         : p_list - Pointer to the SON_LIST object.
* Outputs        : None.
* Returns        : The next node in the list.
* Variables      : None.
* Description    : Returns the node next in the list. Note that the
*                  node is not removed from the list.
****************************************************************************/
SON_LIST_NODE *
get_last_list_node
(
    const SON_LIST *p_list
)
{
    SON_LIST_NODE *p_node = SON_PNULL;

    if (p_list != SON_PNULL)
    {
        p_node = ylLast(p_list);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }

    return p_node;
}

/****************************************************************************
* Function Name  : get_next_list_node
* Inputs         : p_node - Pointer to the SON_LIST_NODE object.
* Outputs        : None.
* Returns        : The next node in the list.
* Variables      : None.
* Description    : Returns the node next in the list. Note that the
*                  node is not removed from the list.
****************************************************************************/
SON_LIST_NODE *
get_next_list_node
(
    const SON_LIST_NODE *p_node
)
{
    SON_LIST_NODE *p_next_node = SON_PNULL;

    if (SON_PNULL != p_node)
    {
        p_next_node = ylNext(p_node);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }

    return p_next_node;
}

/****************************************************************************
* Function Name  : get_prev_list_node
* Inputs         : p_node - Pointer to the SON_LIST_NODE object.
* Outputs        : None.
* Returns        : The next node in the list.
* Variables      : None.
* Description    : Returns the node next in the list. Note that the
*                  node is not removed from the list.
****************************************************************************/
SON_LIST_NODE *
get_prev_list_node
(
    const SON_LIST_NODE *p_node
)
{
    SON_LIST_NODE *p_prev_node = SON_PNULL;

    if (SON_PNULL != p_node)
    {
        p_prev_node = ylPrev(p_node);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }

    return p_prev_node;
}

/****************************************************************************
* Function Name  : list_find_node
* Inputs         : p_list - Pointer to the SON_LIST object.
*                  p_key - Pointer to the key
*                  keyof - keyof function to return the type of node.
*                  compare - callback function for comparison of two keys.
* Outputs        : None.
* Returns        : Pointer to the node found
* Description    : It is used to find a node with given value from the list.
****************************************************************************/
SON_LIST_NODE *
list_find_node
(
    const SON_LIST      *p_list,
    const son_void_t    *p_key,
    const son_void_t    *(*keyof)
    (
        const SON_LIST_NODE *
    ),
    int (*compare)
    (
        const son_void_t *,
        const son_void_t *
    )
)
{
    SON_LIST_NODE *p_node = SON_PNULL;

    if (SON_PNULL == p_list || SON_PNULL == p_key ||
        SON_PNULL == keyof || SON_PNULL == compare)
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
            "Input pointer received is NULL");
    }
    else
    {
        p_node = ylFind(p_list, p_key, keyof, compare);
    }

    return p_node;
}

/****************************************************************************
* Function Name  : list_sort
* Inputs         : p_list - Pointer to the SON_LIST object.
*                  keyof - keyof function to return the type of node.
*                  compare - callback function for comparison of two keys.
* Outputs        : None
* Returns        : son_void_t
* Description    : It is used to sort a given linked  list.
****************************************************************************/
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
)
{
    if (SON_PNULL == p_list || SON_PNULL == keyof || 
            SON_PNULL == compare)
    {
        SON_LOG(SON_OAM_LOG_ON, son_list_facility_name, SON_ERROR,
                "Input pointer received is NULL");
    }
    else
    {
        ylSort(p_list, keyof, compare);
    }
    return p_list;
}
