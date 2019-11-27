/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_db_csc_fb.c,v 1.4.28.7 2010/11/24 03:37:59 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functions for Database access
 *                     for compressor stream context.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_db_csc_fb.c,v $
 * Revision 1.4.28.7  2010/11/24 03:37:59  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.4.28.6  2010/11/09 05:11:15  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.4.28.5  2010/10/13 03:44:32  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.4.28.4  2010/09/23 05:09:34  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.4.28.3  2010/07/07 11:43:42  gur21145
 * PROFILE2_comments_and PROFILE0_comments_incrohc_db_csc_fb.c
 *
 * Revision 1.4.28.1  2010/06/25 10:07:10  gur21145
 * PROFILE2_ENHANCEMENTS
 *
 * Revision 1.4  2009/05/28 23:24:36  gur19479
 * modified for memory isses
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
#include "lteTree.h"
#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "rohc_trace_mcr.h"
#include "rohc_db_ext_if.h"
#include "rohc_db_def.h"
#include "rohc_db_typ.h"
#include "rohc_db_proto.h"

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
extern rohc_global_t    rohc_global;

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*****************************************************************************
 * Function Name  : rohc_db_init_decomp_stream
 * Inputs         : The rohc entity context are taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to initialize the data structure for
 *                  de-Compressor Stream Context.
 *                  (There is an array for storing csc in case context id is 
 *                  configured as Small CID and a tree in case of Large CID).
 *****************************************************************************/
rohc_void_t rohc_db_fb_init  ROHC_PARAMS
((p_entity), rohc_entity_t  *p_entity)
{
    rohc_U32bit_t   count = 0;
    ROHC_ENTER_FN("rohc_db_fb_init");
    
    if (ROHC_TRUE == p_entity->large_cids)
    {
        /* Construct RB Tree */
        treeInit(&(p_entity->un_fb.csc_tree), (p_compare)fb_csc_compare,
                (p_keyof)fb_csc_keyof);
    }
    else
    {
        /* Array approach to access a csc */
		/* Fixing SPR 1635: "<" changed to "<=" as total number of small CID
			can be upto 16 and not 15 */
        for(count = 0; count<=ROHC_MAX_CID; count++)
            p_entity->un_fb.p_csc[count] = ROHC_NULL; 
    }
    
    ROHC_EXIT_FN("rohc_db_fb_init");
}

/*****************************************************************************
 * Function Name  : rohc_db_fb_add_csc
 * Inputs         : The rohc entity context, KEY value and CPIB block are 
 *                  taken as input
 * Outputs        : 
 * Returns        : pointer to rohc_csc_t
 * Description    : This function is used to add a node in the RB-Tree for 
 *                  Compressor Stream Context.
 *****************************************************************************/
rohc_void_t rohc_db_fb_add_csc ROHC_PARAMS
(   (p_entity, p_tnode),
    rohc_entity_t       *p_entity   _AND_
    rohc_tree_node_t    *p_tnode
)
{
    ROHC_ENTER_FN("rohc_db_fb_add_csc");
    treeInsertNode(&(p_entity->un_fb.csc_tree), p_tnode);
    ROHC_EXIT_FN("rohc_db_fb_add_csc");
}

/*****************************************************************************
 * Function Name  : rohc_db_fb_add_csc_node
 * Inputs         : The rohc entity context and CPIB block are taken as input
 * Outputs        : 
 * Returns        : pointer to rohc_csc_t
 * Description    : This function is used to create a node in the RB-Tree for
 *                  Compressor Stream Context.
 *****************************************************************************/
rohc_void_t  rohc_db_fb_add_csc_node ROHC_PARAMS
(   (p_entity, context_id, p_csc),
    rohc_entity_t     *p_entity   _AND_
    rohc_U32bit_t     context_id  _AND_
    rohc_void_t       *p_csc
)
{
    /* This function is called only in Tree implementation */
    rohc_csc_node_t     *p_tnode;

    ROHC_ENTER_FN("rohc_db_fb_add_csc_node");

    p_tnode = (rohc_csc_node_t *)allocateMemFromPool(rohc_global.csc_node_pool);
    if(ROHC_NULL == p_tnode)
    {
        extendPool(rohc_global.csc_node_pool, ROHC_POOL_EXTEND_STEP);
        p_tnode = (rohc_csc_node_t *)allocateMemFromPool(rohc_global.csc_node_pool);
        if (ROHC_NULL == p_tnode)
        {
            ltePanic("%s:node Extend Pool fails.",__FUNCTION__);
            /*Klockworks Warning Fix Start*/
            return;
            /*Klockworks Warning Fix End*/
        }
             
        PoolTunningNeeded("csc_node_pool");
    }

    /* coverity_55386 Fix Start */
    if(PNULL != p_tnode)
    {
    /* coverity_55386 Fix End */

        p_tnode->context_id = context_id;
        p_tnode->p_csc = p_csc;

        if (ROHC_TRUE == p_entity->large_cids)
        {
            rohc_db_fb_add_csc(p_entity,(rohc_tree_node_t *)p_tnode);
            ((rohc_csc_common_t *)p_csc)->p_csc_tnode = p_tnode;
        }
        else
        {   
            p_entity->un_fb.p_csc[context_id] = p_csc;

            /* coverity_55386 Fix Start */
            freeMemPool(p_tnode);
            /* coverity_55386 Fix End */
        }
    }
    
    ROHC_EXIT_FN("rohc_db_fb_add_csc_node");
}


/*****************************************************************************
 * Function Name  : rohc_db_fb_get_csc_small_cid
 * Inputs         : The rohc entity context and CPIB block are taken as input
 * Outputs        : 
 * Returns        : pointer to rohc_csc_t
 * Description    : This function is used to access a node in the RB-Tree for
 *                  Compressor Stream Context.
 *****************************************************************************/
rohc_void_t * rohc_db_fb_get_csc_small_cid ROHC_PARAMS
( (p_entity,context_id),
        rohc_entity_t *p_entity _AND_
        rohc_U16bit_t context_id
)
{
    rohc_void_t     *p_csc = ROHC_NULL;
    ROHC_ENTER_FN("rohc_db_fb_get_csc_small_cid");
        
    p_csc = (p_entity->un_fb.p_csc[context_id]);

    ROHC_EXIT_FN("rohc_db_fb_get_csc_small_cid");
    return p_csc;
}

/*****************************************************************************
 * Function Name  : rohc_db_fb_get_csc_large_cid
 * Inputs         : The rohc entity context and CPIB block are taken as input
 * Outputs        : 
 * Returns        : pointer to rohc_csc_t
 * Description    : This function is used to access a node in the RB-Tree for
 *                  Compressor Stream Context.
 *****************************************************************************/
rohc_void_t * rohc_db_fb_get_csc_large_cid ROHC_PARAMS
( (p_entity,context_id),
        rohc_entity_t *p_entity _AND_
        rohc_U16bit_t context_id
)
{
    rohc_csc_node_t     *p_tnode;
    rohc_void_t   *p_csc = ROHC_NULL;
    ROHC_ENTER_FN("rohc_db_fb_get_csc_large_cid");
        
    p_tnode = (rohc_csc_node_t *)getTreeNode(&(p_entity->un_fb.csc_tree), 
                                            (rohc_void_tp)&context_id);
        if(ROHC_NULL == p_tnode)
        {
            p_csc = ROHC_NULL;
        }
        else
        {
            p_csc = p_tnode->p_csc;
        }

    ROHC_EXIT_FN("rohc_db_fb_get_csc_large_cid");
    return p_csc;
}

/*****************************************************************************
 * Function Name  : fb_release_csc
 * Inputs         : The tree node is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is executed on all the nodes of the Tree to
 *                  delete the csc.
 *****************************************************************************/
rohc_void_t fb_release_csc_node  ROHC_PARAMS
(   (p_tnode, p_walker_param),
    rohc_tree_node_t    *p_tnode    _AND_
    rohc_void_t         *p_walker_param
)
{
    treeDeleteNode((rohc_rb_tree_t *)(p_walker_param), p_tnode);
    freeMemPool(p_tnode);
}

#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : csc_walker
 * Inputs         : The tree node is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is executed on all the nodes of the Tree
 *****************************************************************************/
rohc_void_t fb_csc_walker  ROHC_PARAMS
(   (p_tnode, p_walker_param),
    rohc_tree_node_t    *p_tnode    _AND_
    rohc_void_t         *p_walker_param
)
{
    rohc_csc_node_t     *pl_tnode;

    ROHC_ENTER_FN("fb_csc_walker");
    pl_tnode = (rohc_csc_node_t *)p_tnode;

    ((rohc_db_list_t *)p_walker_param)->p_data[((rohc_db_list_t *)p_walker_param)->counter] = pl_tnode->p_csc;
    (((rohc_db_list_t *)p_walker_param)->counter)++;
    ROHC_EXIT_FN("fb_csc_walker");
}
#endif

/*****************************************************************************
 * Function Name  : rohc_db_fb_delete_all_csc
 * Inputs         : The rohc entity is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to access all the nodes of the csc
 *                  array or RB Tree.
 *****************************************************************************/
rohc_void_t     rohc_db_fb_delete_all_csc_node  ROHC_PARAMS
((p_entity), rohc_entity_t   *p_entity)
{
    rohc_U8bit_t    count = 0;
    if(p_entity->large_cids == ROHC_FALSE)
    {
        /* The CSC are in the array format */
		/* Fixing SPR 1635: "<" changed to "<=" as total number of small CID
			can be upto 16 and not 15 */
        for(count=0; count<=ROHC_MAX_CID; count++)
        {
            if(p_entity->un_fb.p_csc[count] != ROHC_NULL)
            {
                p_entity->un_fb.p_csc[count] = ROHC_NULL;
            }
        } /* end of for */
    }
    else
    {
        /* Traverse and delete all nodes in the tree */
        /* + Coverity 36241 */
        treeDelete(&(p_entity->un_fb.csc_tree),
                (p_walker)fb_release_csc_node, 
                (rohc_void_t *)(&(p_entity->un_fb.csc_tree)));
        /* - Coverity 36241 */
    }
}
#ifdef ROHC_FUNC_ENHANCEMENT
/*Start SPR #: 1391 */
rohc_void_t     rohc_db_fb_delete_csc_node  ROHC_PARAMS
((p_entity,p_csc), 
 rohc_entity_t   *p_entity _AND_
 rohc_csc_t     *p_csc
 )
{
    rohc_csc_node_t *p_csc_node = p_csc->p_csc_tnode;

    /*if(p_entity->large_cids == ROHC_FALSE)
    {
        p_entity->un_fb.p_csc[p_csc->field_value[ROHC_CID_FID -
            ROHC_VAR_FID_BASE]] = ROHC_NULL;
    }*/
    
    if(p_entity->large_cids == ROHC_TRUE)
    {
        if (p_csc_node == ROHC_NULL)
        {
            return;
        }   
        treeDeleteNode((rohc_rb_tree_t *) &(p_entity->un_fb.csc_tree),
                    (rohc_tree_node_t *)p_csc_node);
    }
}
#endif
/*End SPR #: 1391 */		


/*****************************************************************************
 * Function Name  : csc_compare
 * Inputs         : The keys of both the nodes are taken as input
 * Outputs        : 
 * Returns        : -1, 0, 1
 * Description    : This function is used to compare 2 nodes of the RB Tree
 *****************************************************************************/
rohc_S32bit_t   fb_csc_compare     ROHC_PARAMS
(   (p_key1, p_key2),
    rohc_U32bit_t   *p_key1 _AND_
    rohc_U32bit_t   *p_key2
)
{
    if(*p_key1 < *p_key2)   
        return(-1);
    else if(*p_key1 > *p_key2)
        return(1);
    else
        return 0;
}

/*****************************************************************************
 * Function Name  : csc_keyof
 * Inputs         : The tree node is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to access the key of a node of RB Tree
 *****************************************************************************/
rohc_void_tp    fb_csc_keyof   ROHC_PARAMS
(   (p_node), rohc_tree_node_t  *p_node)
{
    return (rohc_void_tp)(&(((rohc_csc_node_t *)p_node)->context_id));
}

