/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_db_entity.c,v 1.1.1.1.16.1 2010/11/03 06:54:27 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functions for Database access for
 *                     entities.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_db_entity.c,v $
 * Revision 1.1.1.1.16.1  2010/11/03 06:54:27  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.9.32.3  2010/10/13 03:45:19  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.9.32.2  2010/09/28 06:55:32  gur11835
 * Corrected usage of ROHC context block pointer stored at PDCP layer
 *
 * Revision 1.9.32.1  2010/09/13 04:09:16  gur22059
 * Packet context retrieval done at PDCP.
 *
 * Revision 1.9  2009/05/28 23:53:37  gur19479
 * updated for memory issues
 *
 * Revision 1.8  2009/05/28 23:25:02  gur19479
 * modified for memory issues
 *
 * Revision 1.7  2009/05/28 13:49:55  gur19140
 * *** empty log message ***
 *
 * Revision 1.6  2009/05/27 14:50:19  gur19836
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
#include  "lteTree.h"
#include "rohc_def.h"
#include "rohc_g_typ.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "rohc_trace_mcr.h"
#include "rohc_db_def.h"
#include "rohc_db_typ.h"
#include "rohc_db_ext_if.h"
#include "rohc_db_proto.h"
#include "lteFramework.h"
#include "rohcTimer.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* + SPR 17439 */
rohc_void_t     rohc_db_free_entity (rohc_entity_t   *p_entity);
/* - SPR 17439 */

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern  rohc_global_t   rohc_global;

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*****************************************************************************
 * Function Name  : rohc_init_db
 * Inputs         : The pool sizes are taken as input
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE
 * Description    : This function is used to create the memory pools
 *****************************************************************************/
rohc_return_t   rohc_init_db    ROHC_PARAMS
(
    (entity_pool_size, csc_pool_size, dsc_pool_size),
    rohc_U32bit_t   entity_pool_size    _AND_
    rohc_U32bit_t   csc_pool_size       _AND_
    rohc_U32bit_t   dsc_pool_size
)
{
    ROHC_ENTER_FN("rohc_init_db");


    rohc_global.entity_pool = createAndReturnMemPool(sizeof(rohc_entity_t), 
                                entity_pool_size);
    if (ROHC_NULL == rohc_global.entity_pool)
        ltePanic("%s:ROHC Entity Pool Create fails.",__FUNCTION__);
    rohc_global.csc_pool = createAndReturnMemPool(sizeof(rohc_un_csc_t), csc_pool_size);
    if (ROHC_NULL == rohc_global.csc_pool)
        ltePanic("%s:CSC Pool Create fails.",__FUNCTION__);
    rohc_global.dsc_pool = createAndReturnMemPool(sizeof(rohc_un_dsc_t), dsc_pool_size);
    if (ROHC_NULL == rohc_global.dsc_pool)
        ltePanic("%s:DSC Pool Create fails.",__FUNCTION__);
    rohc_global.ip4_key_pool = createAndReturnMemPool(IP4_KEY_POOL_SIZE, 
                                csc_pool_size);
    if (ROHC_NULL == rohc_global.ip4_key_pool)
        ltePanic("%s:ipv4 Pool Create fails.",__FUNCTION__);
    rohc_global.ip6_key_pool = createAndReturnMemPool(IP6_KEY_POOL_SIZE, 
                                csc_pool_size);
/* Fix for Memory Optimisation  Start */
    /* Removed CPIB and DPIB Pool Allocation */
/* Fix for Memory Optimisation  End */
    if (ROHC_NULL == rohc_global.ip6_key_pool)
        ltePanic("%s:ipv6 Pool Create fails.",__FUNCTION__);
    rohc_global.node_pool = createAndReturnMemPool(sizeof(rohc_profile_node_t), 
                                csc_pool_size);
    if (ROHC_NULL == rohc_global.node_pool)
        ltePanic("%s:node Pool Create fails.",__FUNCTION__);
    rohc_global.dsc_node_pool = createAndReturnMemPool(sizeof(rohc_dsc_node_t), 
                                dsc_pool_size);
    if (ROHC_NULL == rohc_global.dsc_node_pool)
        ltePanic("%s:dsc node Pool Create fails.",__FUNCTION__);
    rohc_global.csc_node_pool = createAndReturnMemPool(sizeof(rohc_csc_node_t), 
                                csc_pool_size);
    if (ROHC_NULL == rohc_global.csc_node_pool)
        ltePanic("%s:csc node Pool Create fails.",__FUNCTION__);
    rohc_global.list_node_pool = createAndReturnMemPool(sizeof(rohc_protocol_node_t), 
                                ROHC_MAX_PROTOCOL_NODE_POOL);
    if (ROHC_NULL == rohc_global.list_node_pool)
        ltePanic("%s:list node Pool Create fails.",__FUNCTION__);
    rohc_global.rohc_state = ROHC_INIT;

    ROHC_MEMSET(&(rohc_global.cpib),0,sizeof(rohc_cpib_t));
    ROHC_MEMSET(&(rohc_global.dpib),0,sizeof(rohc_dpib_t));
    
    ROHC_EXIT_FN("rohc_init_db");
    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohc_db_free_entity
 * Inputs         : The rohc entity is taken as input
 * Outputs        : 
 * Returns        : void
 * Description    : This function is used to free an entity
 *****************************************************************************/
rohc_void_t     rohc_db_free_entity ROHC_PARAMS
((p_entity),    rohc_entity_t   *p_entity)
{
    freeMemPool((rohc_void_t *)p_entity);
}

/*****************************************************************************
 * Function Name  : rohc_db_delete_entity
 * Inputs         : The rohc entity is taken as input
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE
 * Description    : This function is used to delete an entity
 *****************************************************************************/
rohc_return_t   rohc_db_delete_entity   ROHC_PARAMS
(   
    (p_entity),
    rohc_entity_t   *p_entity
)
{
    ROHC_ENTER_FN("rohc_db_delete_entity");
    /* Get All CSC and DSC and free the inside pointers explicitly */
    if(p_entity){
        if (ROHC_DIR_TX == p_entity->direction)
            rohc_db_delete_all_csc(p_entity);
        else
            rohc_db_delete_all_dsc(p_entity);
        rohc_db_free_entity(p_entity);
    }
    //rohcFreeFromPool();
    ROHC_EXIT_FN("rohc_db_delete_entity");
    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohc_db_init_entity
 * Inputs         : The rohc entity is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to initliaze an entity
 *****************************************************************************/
rohc_void_t rohc_db_init_entity     ROHC_PARAMS
(
    (p_entity),
    rohc_entity_t   *p_entity
)
{   
    ROHC_ENTER_FN("rohc_db_init_entity");
    ROHC_MEMSET(p_entity, 0, sizeof(rohc_entity_t));
    /* Initialize the data structures */
    rohc_db_create_list(&(p_entity->feedback_recv));
    rohc_db_create_list(&(p_entity->feedback_to_be_sent));

    /* To initialize compressor stream context */
    rohc_db_create_tree(p_entity);

    /* Initialize various field */

    p_entity->k_1 = ROHC_MAX_K_1_VAL;
    p_entity->k_2 = ROHC_MAX_K_2_VAL;
    p_entity->n_1 = ROHC_MAX_N_1_VAL;
    p_entity->n_2 = ROHC_MAX_N_2_VAL;
    p_entity->m_1 = ROHC_MAX_M_1_VAL;
    p_entity->m_2 = ROHC_MAX_M_2_VAL;
    p_entity->n = ROHC_MAX_N_VAL;
    p_entity->profiles = ROHC_ALL_PROFILE;
    p_entity->max_static_info_update_counter = ROHC_MAX_STATIC_INFO_COUNTER;
    p_entity->max_dynamic_info_update_counter = ROHC_MAX_DYN_INFO_COUNTER;
    p_entity->max_ir_return_time = ROHC_MAX_IR_RETURN_TIME;
    p_entity->max_fo_return_time = ROHC_MAX_FO_RETURN_TIME;
    p_entity->max_p0_ir_counter= ROHC_MAX_P0_IR_COUNTER;
    p_entity->large_cids = ROHC_FALSE;
    p_entity->modeTransitionUtoO = ROHC_TRIGGER_U_TO_O_MODE;
    p_entity->modeTransitionUtoR = ROHC_TRIGGER_U_TO_R_MODE;
    p_entity->modeTransitionOtoU = ROHC_TRIGGER_O_TO_U_MODE;
    p_entity->modeTransitionOtoR = ROHC_TRIGGER_O_TO_R_MODE;
    p_entity->modeTransitionRtoU = ROHC_TRIGGER_R_TO_U_MODE;
    p_entity->modeTransitionRtoO = ROHC_TRIGGER_R_TO_O_MODE;

    p_entity->p_csc =  NULL;

    ROHC_EXIT_FN("rohc_db_init_entity");
}

/*****************************************************************************
 * Function Name  : rohc_db_create_entity
 * Inputs         : None
 * Outputs        : 
 * Returns        : Pointer to rohc entity
 * Description    : This function is used to create an entity
 *****************************************************************************/
rohc_entity_t   *rohc_db_create_entity  ROHC_NO_PARAMS()
{
    rohc_entity_t   *p_buf = ROHC_NULL;
    ROHC_ENTER_FN("rohc_db_create_entity");

    p_buf = (rohc_entity_t *)allocateMemFromPool(rohc_global.entity_pool);
    if(p_buf == NULL)
    {
        extendPool(rohc_global.entity_pool, ROHC_POOL_EXTEND_STEP);
        p_buf = (rohc_entity_t *)allocateMemFromPool(rohc_global.entity_pool);
        if (ROHC_NULL == p_buf)
        {
             ltePanic("%s:ipv6 Extend Pool fails.",__FUNCTION__);
            /*Klockworks Warning Fix Start*/
            return ROHC_NULL;
            /*Klockworks Warning Fix End*/
        }
        PoolTunningNeeded("entity_pool");
    }

    rohc_db_init_entity(p_buf);
    ROHC_EXIT_FN("rohc_db_create_entity");
    return p_buf;
}

/*****************************************************************************
 * Function Name  : rohc_db_reconfig_entity
 * Inputs         : The rohc entity is taken as input
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE
 * Description    : This function is used to reconfigure an entity.
 *                  (This function is called only when the entity is re-configured
 *                  to change from Small CID to Large CID).
 *****************************************************************************/
#ifdef ROHC_FUNC_ENHANCEMENT
rohc_return_t   rohc_db_reconfig_entity ROHC_PARAMS
    ((p_entity), rohc_entity_t  *p_entity)
{
    /* Nothing to be done for compressor side */
    /* For the decompressor side - Initlialize the tree
     * and move the elements of the array in the tree */

    rohc_U8bit_t    count = 0;
    rohc_dsc_node_t *p_tnode = ROHC_NULL;
    rohc_db_list_t  *p_list;

    ROHC_ENTER_FN("rohc_db_reconfig_entity");
    /* First fetch all the nodes */
    rohc_db_get_all_dsc(p_entity, &p_list);

    /* Construct RB Tree */
    treeInit(&(p_entity->un1.dsc_tree), (p_compare)dsc_compare, 
                (p_keyof)dsc_keyof);

    for (count = 0; count<p_list->counter; count++)
    {
        if(p_list->p_data[count] != ROHC_NULL)
        {
            p_tnode = (rohc_dsc_node_t *)allocateMemFromPool(rohc_global.dsc_node_pool);
            if(ROHC_NULL == p_tnode)
            {
                extendPool(rohc_global.dsc_node_pool, ROHC_POOL_EXTEND_STEP);
                p_tnode = (rohc_dsc_node_t *)
                            allocateMemFromPool(rohc_global.dsc_node_pool);
                if (ROHC_NULL == p_tnode)
                     ltePanic("%s:dsc node Extend Pool fails.",__FUNCTION__);
                PoolTunningNeeded("dsc_node_pool");
            }
            p_tnode->p_dsc = p_list->p_data[count];
            p_tnode->context_id = 
                    ((rohc_dsc_t *)(p_list->p_data[count]))->cid;
            rohc_db_add_dsc(p_entity, (rohc_tree_node_t *)p_tnode);
        }
    } /* end of for */
    /* Delete the list */
    ROHC_FREE(p_list);
    p_list = ROHC_NULL;
    ROHC_EXIT_FN("rohc_db_reconfig_entity");
    return ROHC_SUCCESS;
}
#endif


