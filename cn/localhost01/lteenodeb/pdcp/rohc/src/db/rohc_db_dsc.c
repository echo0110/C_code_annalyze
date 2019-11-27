/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_db_dsc.c,v 1.4.30.9 2010/11/26 05:36:59 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functions for Database access for
 *                     de-compressor stream context.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_db_dsc.c,v $
 * Revision 1.4.30.9  2010/11/26 05:36:59  gur22059
 * Use of Register variables for Profile 1
 *
 * Revision 1.4.30.8  2010/11/24 03:38:29  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.4.30.7  2010/11/09 05:11:30  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.4.30.6  2010/10/13 03:44:51  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.4.30.5  2010/10/01 14:08:57  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.4.30.4  2010/07/22 10:41:48  gur22059
 * SPR 562:Introduced a specific check for Decompressor State
 *
 * Revision 1.4  2009/05/28 23:25:02  gur19479
 * modified for memory issues
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
#include "rohc_g_typ.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "rohc_trace_mcr.h"
#include "rohc_db_ext_if.h"
#include "rohc_db_def.h"
#include "rohc_db_typ.h"
#include "rohc_db_proto.h"
#include "lteTree.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 STATIC rohc_void_t rohc_p2_free_dsc  ROHC_PROTO
 ((
     rohc_dsc_common_t  *p_dsc));

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
 *                  (There is an array for storing DSC in case context id is
 *                  configured as Small CID and a tree in case of Large CID).
 *****************************************************************************/
rohc_void_t rohc_db_init_decomp_stream  ROHC_PARAMS
((p_entity), rohc_entity_t  *p_entity)
{
    rohc_U32bit_t   count = 0;
    ROHC_ENTER_FN("rohc_db_init_decomp_stream");
    if (ROHC_TRUE == p_entity->large_cids)
    {
        /* Construct RB Tree */
        treeInit(&(p_entity->un1.dsc_tree), (p_compare)dsc_compare,
                (p_keyof)dsc_keyof);
    }
    else
    {
        /* Array approach to access a DSC */
		/* Fixing SPR 1635: "<" changed to "<=" as total number of small CID
			can be upto 16 and not 15 */
        for(count = 0; count<=ROHC_MAX_CID; count++)
            p_entity->un1.p_dsc[count] = ROHC_NULL; 
    }
    ROHC_EXIT_FN("rohc_db_init_decomp_stream");
}

/*****************************************************************************
 * Function Name  : rohc_db_reinit_dsc
 * Inputs         : The de-compressor stream context is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to re-initlialize the de-Compressor
 *                  Stream Context.
 *****************************************************************************/
rohc_void_t rohc_db_reinit_dsc    ROHC_PARAMS
((p_dsc), rohc_dsc_common_t  *p_dsc)
{
    ROHC_ENTER_FN("rohc_db_reinit_dsc");

    /* SPR 6607 Fix Start */
    ROHC_MEMSET((rohc_void_tp)p_dsc, 0, sizeof(rohc_un_dsc_t));
    /* SPR 6607 Fix End */

    p_dsc->d_state = ROHC_D_ST_NC;

    p_dsc->d_trans_state = ROHC_D_TST_DONE;
    p_dsc->d_mode = ROHC_MODE_U;
    p_dsc->ack_recvd = ROHC_FALSE;
    p_dsc->udp_cksum_present = ROHC_FALSE;
    ROHC_EXIT_FN("rohc_db_reinit_dsc");
}

/*****************************************************************************
 * Function Name  : rohc_db_init_dsc
 * Inputs         : The de-compressor stream context and DPIB are taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to initlialize the de-Compressor
 *                  Stream Context.
 *****************************************************************************/
rohc_void_t     rohc_db_init_dsc    ROHC_PARAMS
(   (p_dsc, context_id),
    rohc_dsc_common_t  *p_dsc      _AND_   
    rohc_U32bit_t      context_id)
{
    rohc_db_reinit_dsc(p_dsc);

    /* Initialize the list */
    rohc_db_create_list(&(p_dsc->inneripextlist));
    rohc_db_create_list(&(p_dsc->outeripextlist));

    p_dsc->profile = ROHC_INVALID_PROFILE;
    p_dsc->cid = context_id;
    p_dsc->rnd= p_dsc->rnd2=2;
}

/*****************************************************************************
 * Function Name  : rohc_db_p1_create_dsc_sub_blocks
 * Inputs         : The de-compressor stream context is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to initlialize the de-Compressor
 *                  Stream Context Sub blocks depending on the profile
 *****************************************************************************/
rohc_void_t rohc_db_p1_create_dsc_sub_blocks    ROHC_PARAMS
     /* + SPR 17439 */
(   (p_dsc), rohc_dsc_common_t  *p_dsc)
     /* - SPR 17439 */
{
    rohc_dpib_t     *p_dpib = p_dsc->dpib;
    rohc_U8bit_t    length = 0;
    ROHC_ENTER_FN("rohc_db_p1_create_dsc_sub_blocks");

    /* Allocating all the pointers */
    if(p_dpib->inner_ip4_static != ROHC_NULL)
    {   /* Inner IP4 is present */
        if(p_dsc->inner_ip4 == ROHC_NULL)
        {
            p_dsc->inner_ip4 = (rohc_ip4_t *)(ROHC_MALLOC(sizeof(rohc_ip4_t)));
            if (p_dsc->inner_ip4 == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }
            ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip4, 0, sizeof(rohc_ip4_t));
        }   
    }
    else    
    {   /* Inner IP6 is present */
        if(p_dsc->inner_ip6 == ROHC_NULL)
        {   
            p_dsc->inner_ip6 = (rohc_ip6_t *)(ROHC_MALLOC(sizeof(rohc_ip6_t)));
            if (p_dsc->inner_ip6 == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }
            ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip6, 0, sizeof(rohc_ip6_t));
        }   
    }

    if(p_dpib->outer_ip4_static != ROHC_NULL)
    {   /* Outer IP4 is present */
        if(p_dsc->outer_ip4 == ROHC_NULL)
        {   
            p_dsc->outer_ip4 = (rohc_ip4_t *)(ROHC_MALLOC(sizeof(rohc_ip4_t)));
            if (p_dsc->outer_ip4 == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }

            ROHC_MEMSET((rohc_void_tp) p_dsc->outer_ip4, 0, sizeof(rohc_ip4_t));
        }   
    }
    else if(p_dpib->outer_ip6_static != ROHC_NULL)
    {   /* Outer IP6 is present */
        if(p_dsc->outer_ip6 == ROHC_NULL)
        {   
            p_dsc->outer_ip6 = (rohc_ip6_t *)(ROHC_MALLOC(sizeof(rohc_ip6_t)));
            if (p_dsc->outer_ip6 == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }

            ROHC_MEMSET((rohc_void_tp) p_dsc->outer_ip6, 0, sizeof(rohc_ip6_t));
        }
    }

    if(p_dsc->udp == ROHC_NULL)
    {   
        length = sizeof(rohc_udp_t) + sizeof(rohc_U16bit_t);
        p_dsc->udp = (rohc_udp_t *)(ROHC_MALLOC(length));
        if (p_dsc->udp == ROHC_NULL)
        {
            ltePanic("Memory Allocation Failure");
            return;
        }

        ROHC_MEMSET((rohc_void_tp) p_dsc->udp, 0, length);
    }   
    if(p_dsc->rtp == ROHC_NULL)
    {
        length=sizeof(rohc_rtp_t)+((p_dpib->rtp_head_dyn->ver_p_rx_cc &
                ROHC_DYN_CC_MASK) * 4);
        p_dsc->rtp = (rohc_rtp_t *)(ROHC_MALLOC(length));
        if (p_dsc->rtp == ROHC_NULL)
        {
            ltePanic("Memory Allocation Failure");
            return;
        }

        ROHC_MEMSET((rohc_void_tp) p_dsc->rtp, 0, length);
    }
    ROHC_EXIT_FN("rohc_db_p1_create_dsc_sub_blocks");
}

/*****************************************************************************
 * Function Name  : rohc_db_p2_create_dsc_sub_blocks
 * Inputs         : The de-compressor stream context is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to initlialize the de-Compressor
 *                  Stream Context Sub blocks depending on the profile
 *****************************************************************************/
rohc_void_t rohc_db_p2_create_dsc_sub_blocks    ROHC_PARAMS
     /* + SPR 17439 */
(   (p_dsc), rohc_dsc_common_t  *p_dsc)
     /* - SPR 17439 */
{
    rohc_dpib_t     *p_dpib = p_dsc->dpib;
    rohc_U8bit_t    length = 0;
    ROHC_ENTER_FN("rohc_db_p2_create_dsc_sub_blocks");

    /* Allocating all the pointers */
    if(p_dpib->inner_ip4_static != ROHC_NULL)
    {   /* Inner IP4 is present */
        if(p_dsc->inner_ip4 == ROHC_NULL)
        {
            p_dsc->inner_ip4 = (rohc_ip4_t *)(ROHC_MALLOC(sizeof(rohc_ip4_t)));
            if (p_dsc->inner_ip4 == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }
            ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip4, 0, sizeof(rohc_ip4_t));
        }   
    }
    else    
    {   /* Inner IP6 is present */
        if(p_dsc->inner_ip6 == ROHC_NULL)
        {   
            p_dsc->inner_ip6 = (rohc_ip6_t *)(ROHC_MALLOC(sizeof(rohc_ip6_t)));
            if (p_dsc->inner_ip6 == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }
            ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip6, 0, sizeof(rohc_ip6_t));
        }   
    }

    if(p_dpib->outer_ip4_static != ROHC_NULL)
    {   /* Outer IP4 is present */
        if(p_dsc->outer_ip4 == ROHC_NULL)
        {   
            p_dsc->outer_ip4 = (rohc_ip4_t *)(ROHC_MALLOC(sizeof(rohc_ip4_t)));
            if (p_dsc->outer_ip4 == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }

            ROHC_MEMSET((rohc_void_tp) p_dsc->outer_ip4, 0, sizeof(rohc_ip4_t));
        }   
    }
    else if(p_dpib->outer_ip6_static != ROHC_NULL)
    {   /* Outer IP6 is present */
        if(p_dsc->outer_ip6 == ROHC_NULL)
        {   
            p_dsc->outer_ip6 = (rohc_ip6_t *)(ROHC_MALLOC(sizeof(rohc_ip6_t)));
            if (p_dsc->outer_ip6 == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }

            ROHC_MEMSET((rohc_void_tp) p_dsc->outer_ip6, 0, sizeof(rohc_ip6_t));
        }
    }
    if(p_dsc->udp == ROHC_NULL)
    {   
        length = sizeof(rohc_udp_t) + sizeof(rohc_U16bit_t);
        p_dsc->udp = (rohc_udp_t *)(ROHC_MALLOC(length));
        if (p_dsc->udp == ROHC_NULL)
        {
            ltePanic("Memory Allocation Failure");
            return;
        }
        ROHC_MEMSET((rohc_void_tp) p_dsc->udp, 0, length);
    }   

    ROHC_EXIT_FN("rohc_db_p2_create_dsc_sub_blocks");
}

/*****************************************************************************
 * Function Name  : rohc_db_p6_create_dsc_sub_blocks
 * Inputs         : The de-compressor stream context is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to initlialize the de-Compressor
 *                  Stream Context Sub blocks depending on the profile
 *****************************************************************************/
rohc_void_t rohc_db_p6_create_dsc_sub_blocks ROHC_PARAMS
     /* + SPR 17439 */
(   (p_dsc), rohc_dsc_common_t  *p_dsc )
     /* - SPR 17439 */
{
    rohc_dpib_t     *p_dpib = p_dsc->dpib;
    rohc_U8bit_t    length = 0;
    ROHC_ENTER_FN("rohc_db_p6_create_dsc_sub_blocks");

    /* Allocating all the pointers */
    if(ROHC_NULL != p_dpib->inner_ip4_static)
    {   /* Inner IP4 is present */
        if(ROHC_NULL == p_dsc->inner_ip4)
        {
            p_dsc->inner_ip4 = (rohc_ip4_t *)(ROHC_MALLOC(sizeof(rohc_ip4_t)));
            if (ROHC_NULL == p_dsc->inner_ip4)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }
            ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip4, 0, sizeof(rohc_ip4_t));
        }   
    }
    else    
    {   /* Inner IP6 is present */
        if(ROHC_NULL == p_dsc->inner_ip6)
        {   
            p_dsc->inner_ip6 = (rohc_ip6_t *)(ROHC_MALLOC(sizeof(rohc_ip6_t)));
            if (ROHC_NULL == p_dsc->inner_ip6)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }
            ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip6, 0, sizeof(rohc_ip6_t));
        }   
    }

    if(ROHC_NULL != p_dpib->outer_ip4_static)
    {   /* Outer IP4 is present */
        if(ROHC_NULL == p_dsc->outer_ip4)
        {   
            p_dsc->outer_ip4 = (rohc_ip4_t *)(ROHC_MALLOC(sizeof(rohc_ip4_t)));
            if (ROHC_NULL == p_dsc->outer_ip4)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }

            ROHC_MEMSET((rohc_void_tp) p_dsc->outer_ip4, 0, sizeof(rohc_ip4_t));
        }   
    }
    else if(ROHC_NULL != p_dpib->outer_ip6_static)
    {   /* Outer IP6 is present */
        if(ROHC_NULL == p_dsc->outer_ip6)
        {   
            p_dsc->outer_ip6 = (rohc_ip6_t *)(ROHC_MALLOC(sizeof(rohc_ip6_t)));
            if (ROHC_NULL == p_dsc->outer_ip6)
            {
                ltePanic("Memory Allocation Failure");
                return;
            }

            ROHC_MEMSET((rohc_void_tp) p_dsc->outer_ip6, 0, sizeof(rohc_ip6_t));
        }
    }
    if(ROHC_NULL == p_dsc->tcp)
    {  
        /*total bytes to allocate for decompressor context of tcp is fixed size
        of TCP header + 40 bytes reserved for TCP Options*/
        length = sizeof(rohc_tcp_t) + 60 * sizeof(rohc_U8bit_t); 
        p_dsc->tcp = (rohc_tcp_t *)(ROHC_MALLOC(length));
        if (ROHC_NULL == p_dsc->tcp)
        {
            ltePanic("Memory Allocation Failure");
            return;
        }
        ROHC_MEMSET((rohc_void_tp) p_dsc->tcp, 0, length);
    }   

    ROHC_EXIT_FN("rohc_db_p6_create_dsc_sub_blocks");
}

/*****************************************************************************
 * Function Name  : rohc_db_add_dsc
 * Inputs         : The rohc entity context, KEY value and DPIB block 
 * Outputs        : 
 * Returns        : pointer to rohc_dsc_t
 * Description    : This function is used to add a node in the RB-Tree for
 *                  De-Compressor Stream Context.
 *****************************************************************************/
rohc_void_t rohc_db_add_dsc ROHC_PARAMS
(   (p_entity, p_tnode),
    rohc_entity_t       *p_entity   _AND_
    rohc_tree_node_t    *p_tnode
)
{
    ROHC_ENTER_FN("rohc_db_add_dsc");
    treeInsertNode(&(p_entity->un1.dsc_tree), p_tnode);
    ROHC_EXIT_FN("rohc_db_add_dsc");
}

/*****************************************************************************
 * Function Name  : rohc_db_create_dsc_from_pool
 * Inputs         : The rohc entity context and context id are taken as input
 * Outputs        : 
 * Returns        : pointer to rohc_dsc_t
 * Description    : This function is used to create a node in the Array for
 *                  Compressor Stream Context. This function is called in case
 *                  Small CID is configured.
 *****************************************************************************/
rohc_void_t* rohc_db_create_dsc_from_pool    ROHC_PARAMS
(   (p_entity, context_id),
    rohc_entity_t   *p_entity   _AND_
    rohc_U8bit_t    context_id
)
{   
    ROHC_ENTER_FN("rohc_db_create_dsc_from_pool");
    p_entity->un1.p_dsc[context_id] = allocateMemFromPool(rohc_global.dsc_pool);
    if(ROHC_NULL == p_entity->un1.p_dsc[context_id])
    {
        extendPool(rohc_global.dsc_pool, ROHC_POOL_EXTEND_STEP);
        p_entity->un1.p_dsc[context_id] = allocateMemFromPool(rohc_global.dsc_pool);
        if (ROHC_NULL == p_entity->un1.p_dsc[context_id])
             ltePanic("%s:dsc pool Extend Pool fails.",__FUNCTION__);
        PoolTunningNeeded("dsc_pool");
    }
    ROHC_EXIT_FN("rohc_db_create_dsc_from_pool");
    return (p_entity->un1.p_dsc[context_id]);
}

/*****************************************************************************
 * Function Name  : rohc_db_create_dsc
 * Inputs         : The rohc entity context and CPIB block are taken as input
 * Outputs        : 
 * Returns        : pointer to rohc_dsc_t
 * Description    : This function is used to create a node in the RB-Tree for
 *                  Compressor Stream Context.
 *****************************************************************************/
rohc_void_t  *rohc_db_create_dsc ROHC_PARAMS
(   (p_entity, context_id),
    rohc_entity_t   *p_entity   _AND_
    rohc_U32bit_t   context_id
)
{
    /* This function is called only in Tree implementation */
    rohc_dsc_node_t     *p_tnode;

    ROHC_ENTER_FN("rohc_db_create_dsc");

    p_tnode = (rohc_dsc_node_t *)allocateMemFromPool(rohc_global.dsc_node_pool);
    if(ROHC_NULL == p_tnode)
    {
        extendPool(rohc_global.dsc_node_pool, ROHC_POOL_EXTEND_STEP);
        p_tnode = (rohc_dsc_node_t *)allocateMemFromPool(rohc_global.dsc_node_pool);
        if (ROHC_NULL == p_tnode)
        {
            ltePanic("%s:dsc node Extend Pool fails.",__FUNCTION__);
            /*Klockworks Warning Fix Start*/
            return ROHC_NULL;
            /*Klockworks Warning Fix End*/
        }
        PoolTunningNeeded("dsc_node_pool");
    }

    p_tnode->p_dsc = allocateMemFromPool(rohc_global.dsc_pool);
    if(ROHC_NULL == (p_tnode->p_dsc))
    {
        extendPool(rohc_global.dsc_pool, ROHC_POOL_EXTEND_STEP);
        p_tnode->p_dsc = allocateMemFromPool(rohc_global.dsc_pool);
        if (ROHC_NULL == p_tnode->p_dsc)
        {
            freeMemPool(p_tnode);
            ltePanic("%s:dsc pool Extend Pool fails.",__FUNCTION__);
            return PNULL;
        }
        PoolTunningNeeded("dsc_pool");
    }
    
    p_tnode->context_id = context_id;
    rohc_db_add_dsc(p_entity, (rohc_tree_node_t *)p_tnode);

    ROHC_EXIT_FN("rohc_db_create_dsc");
    return (p_tnode->p_dsc);
}


/*****************************************************************************
 * Function Name  : rohc_db_get_dsc
 * Inputs         : The rohc entity context and CPIB block are taken as input
 * Outputs        : 
 * Returns        : pointer to rohc_dsc_t
 * Description    : This function is used to access a node in the RB-Tree for
 *                  Compressor Stream Context.
 *****************************************************************************/
rohc_void_t * rohc_db_get_dsc ROHC_PARAMS
( (p_dpib, p_entity),
        rohc_dpib_t   *p_dpib     _AND_
        rohc_entity_t        *p_entity
)
{
    rohc_dsc_node_t     *p_tnode;
    rohc_U32bit_t       context_id = p_dpib->field_val[ROHC_CID_FID-
                                ROHC_VAR_FID_BASE];
    rohc_void_t          *p_dsc = ROHC_NULL;

    ROHC_ENTER_FN("rohc_db_get_dsc");
    if (ROHC_TRUE == p_entity->large_cids)
    {
        p_tnode = (rohc_dsc_node_t *)getTreeNode(&(p_entity->un1.dsc_tree), 
                                            (rohc_void_tp)&context_id);
        if(ROHC_NULL == p_tnode)
        {
            p_dsc = ROHC_NULL;
        }
        else
        {
            p_dsc = p_tnode->p_dsc;
        }
    }
    else
    {
        p_dsc = (p_entity->un1.p_dsc[context_id]);
    }

    ROHC_EXIT_FN("rohc_db_get_dsc");
    return p_dsc;
}

/*****************************************************************************
 * Function Name  : rohc_db_create_new_dsc
 * Inputs         : The rohc entity context and CPIB block are taken as input
 * Outputs        : 
 * Returns        : pointer to rohc_dsc_t
 * Description    : This function is used to create a new Decompressor stream
 *                  context.
 *****************************************************************************/
rohc_void_t * rohc_db_create_new_dsc ROHC_PARAMS
( (context_id, p_entity),
        rohc_U32bit_t   context_id      _AND_
        rohc_entity_t   *p_entity
)
{
    rohc_void_t   *p_dsc = ROHC_NULL;

    ROHC_ENTER_FN("rohc_db_create_new_dsc");
    if (ROHC_TRUE == p_entity->large_cids)
    {
        p_dsc = rohc_db_create_dsc(p_entity, (rohc_U8bit_t)context_id);
    }
    else
    {
        p_dsc = rohc_db_create_dsc_from_pool(p_entity,(rohc_U8bit_t)context_id);
    }
    if(PNULL != p_dsc)
    rohc_db_init_dsc(p_dsc, context_id);

    ROHC_EXIT_FN("rohc_db_create_new_dsc");
    return p_dsc;
}

/*****************************************************************************
 * Function Name  : rohc_db_free_dsc
 * Inputs         : The pointer to DSC is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is called to free a DSC node by explicitly
 *                  freeing the inside pointers.
 *****************************************************************************/
rohc_void_t     rohc_db_free_dsc    ROHC_PARAMS
((p_dsc),   rohc_dsc_common_t  *p_dsc)
{
    switch(p_dsc->profile)
    {
        case ROHC_UNCOMP :
        {
            freeMemPool(p_dsc);
            break;
        }

        case ROHC_IP_UDP_RTP :
            {
            if(((rohc_dsc_rtp_t *)p_dsc)->rtp != ROHC_NULL)
            freeMemPool(((rohc_dsc_rtp_t *)p_dsc)->rtp);
                break; /* Coverity 10127 Fix */
            }

        case ROHC_IP_UDP :
        {
            rohc_p2_free_dsc(p_dsc);
            break;
        }
      
        /* Case for ROHC Profile 6*/
        case ROHC_IP_TCP:
        {   
            if( ROHC_NULL != p_dsc->inner_ip4 )
                freeMemPool(p_dsc->inner_ip4);
            if( ROHC_NULL != p_dsc->outer_ip4 )
                freeMemPool(p_dsc->outer_ip4);
            if( ROHC_NULL != p_dsc->inner_ip6 ) 
                freeMemPool(p_dsc->inner_ip6);
            if( ROHC_NULL != p_dsc->outer_ip6 )
                freeMemPool(p_dsc->outer_ip6);
            if( ROHC_NULL != p_dsc->tcp )
                freeMemPool(p_dsc->tcp);
            break;    
        }
         
        default :
            ROHC_LOG(LOG_INFO,"Freeing of DSC failed for Profile [%d]",p_dsc->profile);
    }
}
/*****************************************************************************
 * Function Name  : release_dsc
 * Inputs         : The tree node is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is executed on all the nodes of the Tree to 
 *                  delete the DSC.
 *****************************************************************************/
rohc_void_t release_dsc  ROHC_PARAMS
(   (p_tnode, p_walker_param),
    rohc_tree_node_t    *p_tnode    _AND_
    rohc_void_t         *p_walker_param
)
{
    if(((rohc_dsc_node_t *)p_tnode)->p_dsc != ROHC_NULL)
    {
        /* + SPR 17439 */
        rohc_db_free_dsc((rohc_dsc_common_t*)(((rohc_dsc_node_t *)p_tnode)->p_dsc));
        /* - SPR 17439 */
    }
    treeDeleteNode((rohc_rb_tree_t *)(p_walker_param), p_tnode);
    freeMemPool(p_tnode);
}

/*****************************************************************************
 * Function Name  : dsc_walker
 * Inputs         : The tree node is taken as input
 * Outputs        : 
 * Returns        : 
 * Description    : This function is executed on all the nodes of the Tree
 *****************************************************************************/
rohc_void_t dsc_walker  ROHC_PARAMS
(   (p_tnode, p_walker_param),
    rohc_tree_node_t    *p_tnode    _AND_
    rohc_void_t         *p_walker_param
)
{
    rohc_dsc_node_t     *pl_tnode;

    ROHC_ENTER_FN("dsc_walker");
    pl_tnode = (rohc_dsc_node_t *)p_tnode;

    ((rohc_db_list_t *)p_walker_param)->p_data[((rohc_db_list_t *)p_walker_param)->counter] = pl_tnode->p_dsc;
    (((rohc_db_list_t *)p_walker_param)->counter)++;
    ROHC_EXIT_FN("dsc_walker");
}

/*****************************************************************************
 * Function Name  : rohc_db_get_all_dsc
 * Inputs         : The rohc entity is taken as input
 * Outputs        : 
 * Returns        : 
 * Description    : This function is used to access all the nodes of the DSC
 *                  array or RB Tree.
 *****************************************************************************/
rohc_void_t     rohc_db_delete_all_dsc  ROHC_PARAMS
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
            if(p_entity->un1.p_dsc[count] != ROHC_NULL)
            {
                rohc_db_free_dsc((p_entity->un1.p_dsc[count]));
                p_entity->un1.p_dsc[count] = ROHC_NULL;
            }
        } /* end of for */
    }
    else
    {
        /* Traverse and delete all nodes in the tree */
        treeDelete(&(p_entity->un1.dsc_tree),
                (p_walker)release_dsc, 
                (rohc_void_t *)(&(p_entity->un1.dsc_tree)));
    }
}

/*****************************************************************************
 * Function Name  : rohc_db_get_all_dsc
 * Inputs         : The rohc entity is taken as input
 * Outputs        : 
 * Returns        : 
 * Description    : This function is used to access all the nodes of the DSC
 *                  array or RB Tree.
 *****************************************************************************/
#ifdef ROHC_FUNC_ENHANCEMENT
rohc_void_t  rohc_db_get_all_dsc   ROHC_PARAMS
( (p_entity, p_p_list),
        rohc_entity_t   *p_entity   _AND_
        rohc_db_list_t  **p_p_list
)
{
    rohc_U32bit_t   count = 0;
    ROHC_ENTER_FN("rohc_db_get_all_dsc");

    *p_p_list = (rohc_db_list_t *)ROHC_MALLOC(sizeof(rohc_db_list_t));
    if(*p_p_list == ROHC_NULL)
    {
         ltePanic("Memory Allocation Failure");
         return;
    }
    (*p_p_list)->counter=0;

    if(p_entity->large_cids == ROHC_FALSE)
    {
        /* Send all the elements of the array */
		/* Fixing SPR 1635: "<" changed to "<=" as total number of small CID
			can be upto 16 and not 15 */
        for(count=0; count<=ROHC_MAX_CID; count++)
        {
            if(p_entity->un1.p_dsc[count] != ROHC_NULL)
            {
                (*p_p_list)->p_data[(*p_p_list)->counter] = 
                        p_entity->un1.p_dsc[count];
                (*p_p_list)->counter++;
            }
        }
    }
    else
    {
        /* Traverse the Tree and get all nodes */
        treeWalk(&(p_entity->un1.dsc_tree),
                (p_walker)dsc_walker, (rohc_void_t *)(*p_p_list));
    }
    ROHC_EXIT_FN("rohc_db_get_all_dsc");
}
#endif

/*****************************************************************************
 * Function Name  : dsc_compare
 * Inputs         : The keys of both the nodes are taken as input
 * Outputs        : 
 * Returns        : -1, 0, 1
 * Description    : This function is used to compare 2 nodes of the RB Tree
 *****************************************************************************/
rohc_S32bit_t   dsc_compare     ROHC_PARAMS
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
 * Function Name  : dsc_keyof
 * Inputs         : The tree node is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to access the key of a node of RB Tree
 *****************************************************************************/
rohc_void_tp    dsc_keyof   ROHC_PARAMS
(   (p_node), rohc_tree_node_t  *p_node)
{
    return (rohc_void_tp)(&(((rohc_dsc_node_t *)p_node)->context_id));
}

#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_init_dsc_from_context_block
 * Inputs         : The tree node is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used to initialize a DSC from the context
 *                  updation block received at the target RNC side.
 *****************************************************************************/
rohc_return_t   rohc_init_dsc_from_context_block    ROHC_PARAMS
(   (p_dsc, p_stream_pkt_info),
    rohc_dsc_t              *p_dsc  _AND_
    stream_pkt_info_t   *p_stream_pkt_info
)
{
    ROHC_ENTER_FN("rohc_init_dsc_from_context_block");
    /* Initialize the list */
    rohc_db_create_list(&(p_dsc->inneripextlist));
    rohc_db_create_list(&(p_dsc->outeripextlist));

    /* Malloc necessary pointers  */
    switch(p_stream_pkt_info->profile)
    {
        case ROHC_IP_UDP_RTP:
        {
            if(p_stream_pkt_info->profile_union.profile_1.inner_ip.ip_version ==
                ROHC_VERSION_IP4)
            {
                p_dsc->inner_ip4 = (rohc_ip4_t *)(ROHC_MALLOC(sizeof(rohc_ip4_t)));
                if (p_dsc->inner_ip4 == ROHC_NULL)
                {
                    ltePanic("Memory Allocation Failure");
                    return ROHC_FAILURE;
                }
                ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip4, 0, sizeof(rohc_ip4_t));
            }
            else
            {
                p_dsc->inner_ip6 = (rohc_ip6_t *)(ROHC_MALLOC(sizeof(rohc_ip6_t)));
                if (p_dsc->inner_ip6 == ROHC_NULL)
                {
                    ltePanic("Memory Allocation Failure");
                    return ROHC_FAILURE;
                }

                ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip6, 0, sizeof(rohc_ip6_t));
            }
            p_dsc->udp = (rohc_udp_t *)(ROHC_MALLOC(sizeof(rohc_udp_t) + 
                                                    sizeof(rohc_U16bit_t)));
            if (p_dsc->udp == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return ROHC_FAILURE;
            }

            /*ROHC_MEMSET((rohc_void_tp) p_dsc->udp, 0, (sizeof(rohc_udp_t) + 
                                                    sizeof(rohc_U16bit_t)));*/
                        /* Contains 16 bit SN */
            p_dsc->rtp = (rohc_rtp_t *)(ROHC_MALLOC(sizeof(rohc_rtp_t) +
                            (ROHC_MAX_ENTRIES_IN_CSRC_TT *
                            sizeof(rohc_U32bit_t))));
            if (p_dsc->rtp == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return ROHC_FAILURE;
            }

            /*ROHC_MEMSET((rohc_void_tp) p_dsc->rtp, 0,(sizeof(rohc_rtp_t) +
                            (ROHC_MAX_ENTRIES_IN_CSRC_TT *
                            sizeof(rohc_U32bit_t))));*/
        }
        break;
        case ROHC_IP_UDP:
        {
            if(p_stream_pkt_info->profile_union.profile_2.inner_ip.ip_version ==
                ROHC_VERSION_IP4)
            {
                p_dsc->inner_ip4 = (rohc_ip4_t *)(ROHC_MALLOC(sizeof(rohc_ip4_t)));
                if (p_dsc->inner_ip4 == ROHC_NULL)
                {
                    ltePanic("Memory Allocation Failure");
                    return ROHC_FAILURE;
                }
                ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip4, 0, sizeof(rohc_ip4_t));
            }
            else
            {
                p_dsc->inner_ip6 = (rohc_ip6_t *)(ROHC_MALLOC(sizeof(rohc_ip6_t)));
                if (p_dsc->inner_ip6 == ROHC_NULL)
                {
                    ltePanic("Memory Allocation Failure");
                    return ROHC_FAILURE;
                }
                ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip6, 0, sizeof(rohc_ip6_t));
            }
            p_dsc->udp = (rohc_udp_t *)(ROHC_MALLOC(sizeof(rohc_udp_t) +
                            sizeof(rohc_U16bit_t)));
            if (p_dsc->udp == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return ROHC_FAILURE;
            }
            /*ROHC_MEMSET((rohc_void_tp) p_dsc->udp, 0, (sizeof(rohc_udp_t) + 
                                                    sizeof(rohc_U16bit_t)));*/
        }
        break;
        case ROHC_IP_ESP:
        {
            if(p_stream_pkt_info->profile_union.profile_3.inner_ip.ip_version ==
                ROHC_VERSION_IP4)
            {
                p_dsc->inner_ip4 = (rohc_ip4_t *)(ROHC_MALLOC(sizeof(rohc_ip4_t)));
                if (p_dsc->inner_ip4 == ROHC_NULL)
                {
                    ltePanic("Memory Allocation Failure");
                    return ROHC_FAILURE;
                }
                ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip4, 0, sizeof(rohc_ip4_t));
            }
            else
            {
                p_dsc->inner_ip6 = (rohc_ip6_t *)(ROHC_MALLOC(sizeof(rohc_ip6_t)));
                if (p_dsc->inner_ip6 == ROHC_NULL)
                {
                    ltePanic("Memory Allocation Failure");
                    return ROHC_FAILURE;
                }
                ROHC_MEMSET((rohc_void_tp) p_dsc->inner_ip6, 0, sizeof(rohc_ip6_t));
            }
            p_dsc->esp = (rohc_esp_t *)(ROHC_MALLOC(sizeof(rohc_esp_t)));
            if (p_dsc->esp == ROHC_NULL)
            {
                ltePanic("Memory Allocation Failure");
                return ROHC_FAILURE;
            }
            ROHC_MEMSET((rohc_void_tp) p_dsc->esp, 0, (sizeof(rohc_esp_t)) );
        }
        break;
    } /* end of switch */
    ROHC_EXIT_FN("rohc_init_dsc_from_context_block");
    return ROHC_SUCCESS;
}
#endif

/*****************************************************************************
 * Function Name  : rohc_p2_free_dsc
 * Inputs         : The pointer to DSC is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is called to free a DSC node by explicitly
 *                  freeing the inside pointers.
 *****************************************************************************/
STATIC rohc_void_t     rohc_p2_free_dsc    ROHC_PARAMS
((p_dsc),   rohc_dsc_common_t  *p_dsc)
{
    rohc_U8bit_t    count = 0;
    /* Free the item pointers in the extension header TT */
    for(count=0; count<p_dsc->rohc_d_r_ext_hdr_tt.r_ext_hdr_tt_next_free_index;
            count++)
    {
        if(p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[count].p_item != ROHC_NULL)
        {
            freeMemPool(p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[count].p_item);
        }
    } /* end of for */
    for(count=0;count<p_dsc->rohc_d_uo_ext_hdr_tt.uo_ext_hdr_tt_next_free_index;
            count++)
    {
        if(p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[count].p_item != 
                ROHC_NULL)
        {
            freeMemPool(p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[count].p_item);
        }
    } /* end of for */

    if(p_dsc->inner_ip4 != ROHC_NULL)
        freeMemPool(p_dsc->inner_ip4);
    if(p_dsc->outer_ip4 != ROHC_NULL)
        freeMemPool(p_dsc->outer_ip4);
    if(p_dsc->inner_ip6 != ROHC_NULL)
        freeMemPool(p_dsc->inner_ip6);
    if(p_dsc->outer_ip6 != ROHC_NULL)
        freeMemPool(p_dsc->outer_ip6);
    if(p_dsc->udp != ROHC_NULL)
        freeMemPool(p_dsc->udp);
    if(p_dsc->payload != ROHC_NULL)
        freeMemPool(p_dsc->payload);

    freeMemPool(p_dsc);
}
