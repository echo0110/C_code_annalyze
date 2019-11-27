/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_db_list.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the wrapper functions for CSPL List
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_db_list.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/27 14:50:19  gur19836
 * File Header and Function Headers updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_com_def.h"
#include "rohc_trace_mcr.h"
#include "rohc_g_typ.h"
#include "rohc_db_ext_if.h"
#include "lteFramework.h"

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
 * Function Name  : rohc_db_create_list
 * Inputs         : The pointer to list taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to create list
 *****************************************************************************/
rohc_void_t	rohc_db_create_list	ROHC_PARAMS((p_list), rohc_list_t	*p_list)
{
	listInit(p_list);
}

/*****************************************************************************
 * Function Name  : rohc_db_add_last_node
 * Inputs         : The pointer to list and the pointer to the node
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to insert a node in the list after the
 *                  last node
 *****************************************************************************/
rohc_void_t	rohc_db_add_last_node	ROHC_PARAMS
(
	(p_list, p_node),
	rohc_list_t				*p_list	_AND_
	rohc_list_node_t		*p_node
)
{
	listInsertNode(p_list, p_node);
}

/*****************************************************************************
 * Function Name  : rohc_db_get_next_node
 * Inputs         : The pointer to the node is taken as input
 * Outputs        : 
 * Returns        : pointer to the next node.
 * Description    : This function is used to access the next node in the list
 *****************************************************************************/
rohc_list_node_t *rohc_db_get_next_node	ROHC_PARAMS
(
	(p_node), rohc_list_node_t	*p_node)
{
	return getNextListNode(p_node);
}

/*****************************************************************************
 * Function Name  : rohc_db_get_first_node
 * Inputs         : The pointer to the list is taken as input
 * Outputs        : 
 * Returns        : pointer to the list node.
 * Description    : This function is used to access the first node in the list
 *****************************************************************************/
rohc_list_node_t *rohc_db_get_first_node	ROHC_PARAMS
( (p_list), rohc_list_t	*p_list)
{
	return	getFirstListNode(p_list);
}

/*****************************************************************************
 * Function Name  : rohc_db_get_last_node
 * Inputs         : The pointer to the list is taken as input
 * Outputs        : 
 * Returns        : pointer to the list node.
 * Description    : This function is used to access the last node in the list
 *****************************************************************************/
rohc_list_node_t *rohc_db_get_last_node	ROHC_PARAMS
( (p_list), rohc_list_t	*p_list)
{
	return	getLastListNode(p_list);
}

/*****************************************************************************
 * Function Name  : rohc_db_count_of_list
 * Inputs         : The pointer to the list is taken as input
 * Outputs        : 
 * Returns        : count
 * Description    : This function is used to get the number of nodes in the list
 *****************************************************************************/
rohc_U32bit_t	rohc_db_count_of_list 	ROHC_PARAMS
( (p_list), rohc_list_t	*p_list)
{
	return listCount(p_list);
}

#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_db_count_of_list
 * Inputs         : The pointer to the list is taken as input
 * Outputs        : 
 * Returns        : count
 * Description    : This function is used to get the number of nodes in the list
 *****************************************************************************/
rohc_void_t		rohc_db_delete_node		ROHC_PARAMS
(	(p_list, p_node),
	rohc_list_t			*p_list		_AND_
	rohc_list_node_t	*p_node
)
{
    UInt32 numTimerNodes = 0;
    numTimerNodes = listCount(p_list);
    if ( 0 < numTimerNodes )
    {
        listDeleteNode(p_list, p_node);
    }
}
#endif

/*
rohc_void_t	rohc_db_dump_list	ROHC_PARAMS
( (p_list), rohc_list_t	*p_list)
{
	listWalk(p_list, rohc_protocol_node_walk, ROHC_NULL);
}*/
