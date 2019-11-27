/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_db_csc.c,v 1.6.30.7 2010/11/24 03:37:35 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functions for Database access for
 *                     compressor stream context.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_db_csc.c,v $
 * Revision 1.6.30.7  2010/11/24 03:37:35  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.6.30.6  2010/11/09 05:10:59  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.6.30.5  2010/10/13 03:44:02  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.6.30.4  2010/09/23 05:08:26  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.6.30.3  2010/08/19 08:46:36  gur22059
 * SPR 568 FIX : Context creation for profile-1 (RTP) packets is updated
 *
 * Revision 1.6.30.1  2010/07/08 09:52:00  gur21145
 * code review comments of prof 0 , 2 enhancement activity
 *
 * Revision 1.6  2009/05/28 23:24:06  gur19479
 * modified for memory issues
 *
 * Revision 1.5  2009/05/28 13:49:34  gur19140
 * *** empty log message ***
 *
 * Revision 1.4  2009/05/27 14:50:19  gur19836
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
#include "rohc_def.h"
#include "rohc_typ.h"
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "rohc_protocol_header.h"
#include "rohc_trace_mcr.h"
#include "rohc_gcom_mcr.h"
#include "rohc_db_typ.h"
#include "rohc_db_proto.h"
#include "rohc_db_def.h"
#include "rohc_db_ext_if.h"
#include "rohc_main_typ.h"
#include "lteTree.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/
/* + SPR 17439 */
static rohc_S32bit_t   rohc_compare_profile_1_ip4  ROHC_PROTO
((
    rohc_profile1_t     *p_key1   ,
    rohc_profile1_t     *p_key2  ));

static rohc_S32bit_t   rohc_compare_profile_1_ip6  ROHC_PROTO
((
    rohc_profile1_t     *p_key1    ,
    rohc_profile1_t     *p_key2  ));
/* + SPR 17439 */
rohc_void_tp    rohc_db_create_key
(   
    rohc_cpib_t     *p_cpib     ,
    rohc_U8bit_t    profile
);
/* - SPR 17439 */
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
 * Function Name  : rohc_db_create_tree
 * Inputs         : The rohc entity context are taken as input
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to create a RB-Tree for Compressor
 *                  Stream Context. (There are 3 RB-Trees based on the profile.)
 *****************************************************************************/
rohc_void_t rohc_db_create_tree     ROHC_PARAMS
((p_entity), rohc_entity_t  *p_entity)
{
    ROHC_ENTER_FN("rohc_db_create_tree");
    /* Initializing Tree for Profile 1 */
    p_entity->profile_tree[ROHC_IP_UDP_RTP].compare = 
                                        (p_compare)rohc_compare_profile_1;
    p_entity->profile_tree[ROHC_IP_UDP_RTP].keyof =
                                        (p_keyof)rohc_keyof_profile; 

    treeInit(&(p_entity->profile_tree[ROHC_IP_UDP_RTP].profile_seed), 
                (p_compare)p_entity->profile_tree[ROHC_IP_UDP_RTP].compare, 
                p_entity->profile_tree[ROHC_IP_UDP_RTP].keyof);

    /* Initializing Tree for Profile 2 */
    p_entity->profile_tree[ROHC_IP_UDP].compare = 
                                        (p_compare)rohc_compare_profile_2;
    p_entity->profile_tree[ROHC_IP_UDP].keyof = 
                                        (p_keyof)rohc_keyof_profile; 

    treeInit(&(p_entity->profile_tree[ROHC_IP_UDP].profile_seed), 
                p_entity->profile_tree[ROHC_IP_UDP].compare, 
                p_entity->profile_tree[ROHC_IP_UDP].keyof);

    /* Initializing Tree for Profile 3 */
    p_entity->profile_tree[ROHC_IP_ESP - 1].compare = 
                                        (p_compare)rohc_compare_profile_3;
    p_entity->profile_tree[ROHC_IP_ESP - 1].keyof = 
                                        (p_keyof)rohc_keyof_profile; 
                                        
    treeInit(&(p_entity->profile_tree[ROHC_IP_ESP - 1].profile_seed), 
                p_entity->profile_tree[ROHC_IP_ESP - 1].compare, 
                p_entity->profile_tree[ROHC_IP_ESP - 1].keyof);

    /* Initializing Tree for Profile 6 */
    p_entity->profile_tree[ROHC_IP_TCP].compare = 
                                        (p_compare)rohc_compare_profile_6;
    p_entity->profile_tree[ROHC_IP_TCP].keyof =
                                        (p_keyof)rohc_keyof_profile; 

    treeInit(&(p_entity->profile_tree[ROHC_IP_TCP].profile_seed), 
                (p_compare)p_entity->profile_tree[ROHC_IP_TCP].compare, 
                p_entity->profile_tree[ROHC_IP_TCP].keyof);

    /* Fixing SPR 1635 : commenting it  as it has to be called later 
    rohc_db_fb_init(p_entity);
	*/
    ROHC_EXIT_FN("rohc_db_create_tree");
}

/*****************************************************************************
 * Function Name  : rohc_db_init_csc
 * Inputs         : The compressor stream context and CPIB are taken as input
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to initlialize Compressor Stream 
 *                  Context.
 *****************************************************************************/
rohc_void_t rohc_db_init_csc    ROHC_PARAMS
((p_csc),  rohc_csc_common_t   *p_csc)
{
    ROHC_ENTER_FN("rohc_db_init_csc");

    /* SPR 6607 Fix Start */
    ROHC_MEMSET((rohc_void_tp)p_csc, 0, sizeof(rohc_un_csc_t));
    /* SPR 6607 Fix End */

    /* Initialize other fields */
    p_csc->udp_cksum_present = ROHC_FALSE;
    p_csc->ack_recvd = ROHC_FALSE;
    p_csc->static_info_update_counter = 0;
    p_csc->dynamic_info_update_counter = 0;
    p_csc->c_state = ROHC_C_ST_IR;
    p_csc->c_trans_state = ROHC_C_TST_DONE;
    p_csc->c_mode = ROHC_MODE_U;
    p_csc->new_inner_rnd = ROHC_RND_PRESENT;
    p_csc->old_inner_rnd = ROHC_RND_PRESENT;
    p_csc->new_inner_nbo = 1;
    p_csc->old_inner_nbo = 1;
    p_csc->new_outer_rnd = ROHC_RND_PRESENT;
    p_csc->old_outer_rnd = ROHC_RND_PRESENT;
    p_csc->new_outer_nbo = 1;
    p_csc->old_outer_nbo = 1;
    p_csc->profile = ROHC_INVALID_PROFILE;
    p_csc->rohc_gsw.start_index = 0;
    p_csc->rohc_gsw.end_index = ROHC_MAX_LENGTH_GSW-1;
    ROHC_EXIT_FN("rohc_db_init_csc");
}

/*****************************************************************************
 * Function Name  : rohc_db_mem_get_ip4_key
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to key
 * Description    : This function gets the IPv4 key
 *****************************************************************************/
rohc_void_tp    rohc_db_mem_get_ip4_key ROHC_NO_PARAMS ()
{
    rohc_void_tp    *p_key;
    ROHC_ENTER_FN("rohc_db_mem_get_ip4_key");

    p_key = allocateMemFromPool(rohc_global.ip4_key_pool);
    if(p_key == ROHC_NULL)
    {
       extendPool(rohc_global.ip4_key_pool,
                        ROHC_POOL_EXTEND_STEP);
       p_key = allocateMemFromPool(rohc_global.ip4_key_pool);
        if (ROHC_NULL == p_key)
             ltePanic("%s:ipv4 Extend Pool fails.",__FUNCTION__);
       PoolTunningNeeded("ip4_key_pool");
    }
    ROHC_EXIT_FN("rohc_db_mem_get_ip4_key");
    return p_key;
}

/*****************************************************************************
 * Function Name  : rohc_db_mem_get_ip6_key
 * Inputs         : None
 * Outputs        : None
 * Returns        : Pointer to IPv6 key
 * Description    : This function gets the IPv6 key
 *****************************************************************************/
rohc_void_tp    rohc_db_mem_get_ip6_key ROHC_NO_PARAMS ()
{
    rohc_void_tp    *p_key;
    ROHC_ENTER_FN("rohc_db_mem_get_ip6_key");

    p_key = allocateMemFromPool(rohc_global.ip6_key_pool);
    if(p_key == ROHC_NULL)
    {
        extendPool(rohc_global.ip6_key_pool,
                        ROHC_POOL_EXTEND_STEP);
        p_key = allocateMemFromPool(rohc_global.ip6_key_pool);
        if (ROHC_NULL == p_key)
             ltePanic("%s:ipv6 Extend Pool fails.",__FUNCTION__);
        PoolTunningNeeded("ip6_key_pool");
    }
    ROHC_EXIT_FN("rohc_db_mem_get_ip6_key");
    return p_key;
}

/*****************************************************************************
 * Function Name  : rohc_db_create_key
 * Inputs         : The CPIB block and profile are taken as input
 * Outputs        : None
 * Returns        : pointer to the key
 * Description    : This function is used to create a key for inserting a node
 *                  in the tree.
 *****************************************************************************/
rohc_void_tp    rohc_db_create_key  ROHC_PARAMS
(   (p_cpib, profile),
    rohc_cpib_t     *p_cpib     _AND_
    rohc_U8bit_t    profile
)
{
    rohc_profile1_t         *p_prof1_key = ROHC_NULL;
    rohc_profile2_t         *p_prof2_key = ROHC_NULL;
    rohc_profile3_t         *p_prof3_key = ROHC_NULL;
    rohc_profile6_t         *p_prof6_key = ROHC_NULL;
    rohc_static_def_ip_t    *p_ip = ROHC_NULL;
    rohc_void_tp            p_key = ROHC_NULL;

    if(p_cpib->inner_ip4 != ROHC_NULL)
    {
        /* IPv4 is the IP version */
        p_key = rohc_db_mem_get_ip4_key();
        if(p_key == ROHC_NULL)
            return ROHC_NULL;
        p_ip = (rohc_static_def_ip_t *)p_key;
        p_ip->ip_version = ROHC_VERSION_IP4;
        p_ip->un1.ipv4.src_addr =
                ((rohc_ip4_t *)(p_cpib->inner_ip4))->src;
        p_ip->un1.ipv4.dst_addr =
                ((rohc_ip4_t *)(p_cpib->inner_ip4))->dst;
    }
    else if(p_cpib->inner_ip6 != ROHC_NULL)
    {
        /* IPv6 is the IP version */
        p_key = rohc_db_mem_get_ip6_key();
        if(p_key == ROHC_NULL)  return ROHC_NULL;
        p_ip = (rohc_static_def_ip_t *)p_key;
        p_ip->ip_version = ROHC_VERSION_IP6;
        ROHC_MEMCPY((rohc_void_t *)(p_ip->un1.ipv6.src_addr),
            (rohc_void_t *)(((rohc_ip6_t *)(p_cpib->inner_ip6))->src), 16);
        ROHC_MEMCPY((rohc_void_t *)(p_ip->un1.ipv6.dst_addr),
            (rohc_void_t *)(((rohc_ip6_t *)(p_cpib->inner_ip6))->dst), 16);
        p_ip->un1.ipv6.flow_label =
            ((rohc_ip6_t *)(p_cpib->inner_ip6))->ip6_ctlun.ip6_un1.un1_flow &
                        ROHC_IP6_FLOWLABEL_MASK;
    }
    /* Coverity 5.0.1 ID:24940,24941,24942 fix start*/
    else
    {
        return ROHC_NULL;
    }
    /* Coverity 5.0.1 ID:24940,24941,24942 fix end*/

    switch(profile)
    {
        case ROHC_IP_UDP_RTP:
        {
            p_prof1_key = (rohc_profile1_t *)p_key;
            p_prof1_key->udp.src_port = ((rohc_udp_t *)(p_cpib->udp))->sport;
            p_prof1_key->udp.dst_port = ((rohc_udp_t *)(p_cpib->udp))->dport;
            p_prof1_key->rtp.ssrc = ((rohc_rtp_t *)(p_cpib->rtp))->ssrc;
        }
        break;
        case ROHC_IP_UDP:
        {
            p_prof2_key = (rohc_profile2_t *)p_key;
            p_prof2_key->udp.src_port = ((rohc_udp_t *)(p_cpib->udp))->sport;
            p_prof2_key->udp.dst_port = ((rohc_udp_t *)(p_cpib->udp))->dport;
        }
        break;
        case ROHC_IP_ESP:
        {
            p_prof3_key = (rohc_profile3_t *)p_key;
            p_prof3_key->esp.spi = ((rohc_esp_t *)(p_cpib->esp))->spi;
        }
        break;
        case ROHC_IP_TCP:
        {
            p_prof6_key = (rohc_profile6_t *)p_key;
            p_prof6_key->tcp.src_port = ((rohc_tcp_t *)(p_cpib->tcp))->sport;
            p_prof6_key->tcp.dst_port = ((rohc_tcp_t *)(p_cpib->tcp))->dport;
        }
        break;
    } /* end of switch */

    return p_key;
}

/*****************************************************************************
 * Function Name  : rohc_db_create_csc
 * Inputs         : The rohc entity context, KEY value and CPIB block are 
 *                  taken as input
 * Outputs        : None
 * Returns        : pointer to rohc_csc_t
 * Description    : This function is used to create a node in the RB-Tree for
 *                  Compressor Stream Context.
 *****************************************************************************/
/* Allocate memory and then call treeInsertNode */
rohc_csc_common_t  *rohc_db_create_csc ROHC_PARAMS
(
    (p_key, p_entity, profile),
    rohc_void_tp    p_key   _AND_
    rohc_entity_t   *p_entity   _AND_
    rohc_U8bit_t    profile
)
{
    rohc_profile_node_t     *p_rbt_node = ROHC_NULL;
    rohc_csc_common_t       *p_csc = ROHC_NULL;
    
    ROHC_ENTER_FN("rohc_db_create_csc");

    p_rbt_node = allocateMemFromPool(rohc_global.node_pool);
    if(p_rbt_node == ROHC_NULL)
    {
        extendPool(rohc_global.node_pool, ROHC_POOL_EXTEND_STEP);
        p_rbt_node = allocateMemFromPool(rohc_global.node_pool);
        if (ROHC_NULL == p_rbt_node)
        {
             ltePanic("%s:node csc Extend Pool fails.",__FUNCTION__);
             /*Klockworks Warning Fix Start*/
             return ROHC_NULL;
             /*Klockworks Warning Fix End*/
        }
        PoolTunningNeeded("node_pool");
    }
    p_rbt_node->p_key = p_key;

    p_rbt_node->p_csc = allocateMemFromPool(rohc_global.csc_pool);
    if(p_rbt_node->p_csc == ROHC_NULL)
    {
        extendPool(rohc_global.csc_pool, ROHC_POOL_EXTEND_STEP);
        p_rbt_node->p_csc = allocateMemFromPool(rohc_global.csc_pool);
        if (ROHC_NULL == p_rbt_node->p_csc)
        {
            freeMemPool(p_rbt_node);
            /*Klockworks Warning Fix Start*/
            ltePanic("%s:csc Extend Pool fails.",__FUNCTION__);
            return ROHC_NULL;
            /*Klockworks Warning Fix End*/
        }
        PoolTunningNeeded("csc_pool");
    }

    p_csc = (rohc_csc_common_t *)(p_rbt_node->p_csc);
    rohc_db_init_csc(p_csc);
    p_csc->p_rb_tnode = p_rbt_node;
    p_csc->profile = profile;
    p_csc->ir_ret_time = p_entity->max_ir_return_time;
    p_csc->fo_ret_time = p_entity->max_fo_return_time;
    p_csc->ir_ret_timer.tdata.p_csc_ptr = p_csc;
    p_csc->ir_ret_timer.tdata.p_entity_ptr = p_entity;
    p_csc->ir_ret_timer.tdata.timer_type = ROHC_IR_RET_TIMER;
    p_csc->fo_ret_timer.tdata.p_entity_ptr = p_entity;
    p_csc->fo_ret_timer.tdata.p_csc_ptr = p_csc;
    p_csc->fo_ret_timer.tdata.timer_type = ROHC_FO_RET_TIMER;

    if(profile == ROHC_UNCOMP)
    {
        p_csc->c_state = ROHC_CUN_ST_IR;
    }
    treeInsertNode(&p_entity->profile_tree[profile].profile_seed, 
                (rohc_tree_node_t *)p_rbt_node);
    ROHC_EXIT_FN("rohc_db_create_csc");
    return ((rohc_csc_common_t *)(p_rbt_node->p_csc));
}

/*****************************************************************************
 * Function Name  : rohc_db_get_csc
 * Inputs         : The rohc entity context are taken as input
 * Outputs        : None
 * Returns        : pointer to rohc_csc_t
 * Description    : This function is used to access a Compressor Stream Context
 *****************************************************************************/
rohc_return_t   rohc_db_get_csc ROHC_PARAMS
(
    (p_cpib, p_entity, profile, p_p_csc, p_ecode),
    rohc_cpib_t         *p_cpib     _AND_
    rohc_entity_t       *p_entity   _AND_
    rohc_U8bit_t        profile     _AND_
    /* + SPR 17439 */
    rohc_csc_common_t   **p_p_csc   _AND_
    /* - SPR 17439 */
    rohc_error_code_t   *p_ecode
)
{
    rohc_profile_node_t     *p_rb_node = ROHC_NULL;
    rohc_void_tp            p_key = ROHC_NULL;

    ROHC_ENTER_FN("rohc_db_get_csc");
    
    if(profile == ROHC_UNCOMP)
    {
	*p_p_csc = rohc_db_get_uncomp_csc(p_entity,p_ecode);
        /*Klockworks Warning Fix Start*/
        if (ROHC_NULL ==  *p_p_csc)
        {
            *p_ecode = ROHC_MEMORY_FAILURE;
            return ROHC_FAILURE;
        }
        /*Klockworks Warning Fix End*/
	return ROHC_SUCCESS;
    }
    /* First construct the key */
    p_key = rohc_db_create_key(p_cpib, profile);
    if(p_key == ROHC_NULL)
    {
        *p_ecode = ROHC_MEMORY_FAILURE;
        return ROHC_FAILURE;
    }

    p_rb_node = (rohc_profile_node_t *)
        getTreeNode(&(p_entity->profile_tree[profile].profile_seed), p_key);

    if(p_rb_node != ROHC_NULL)
    {
        *p_p_csc = p_rb_node->p_csc;
        /* Set the CPIB pointer */
        ((rohc_csc_common_t *)(*p_p_csc))->p_cpib = p_cpib;
    }
    else
    {
        *p_p_csc = ROHC_NULL;
        *p_ecode = ROHC_NO_ERR;
    }   
    /* Free the key */
    freeMemPool(p_key);
    
    ROHC_EXIT_FN("rohc_db_get_csc");
    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohc_db_free_csc
 * Inputs         : The pointer to CSC is taken as input
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to free the CSC node. All the pointers
 *                  in CSC are freed explicitly.
 *****************************************************************************/
rohc_void_t rohc_db_free_csc    ROHC_PARAMS
(   (p_csc), rohc_csc_common_t     *p_csc)
{
    switch(p_csc->profile)
    {
        case ROHC_UNCOMP:
        {
            freeMemPool(p_csc);
            break;
        }
        
        case ROHC_IP_UDP_RTP :
        case ROHC_IP_UDP :
        {
            /* The Translation Table of Extension header contains pointers which
             * needs to be freed explicitly */
            rohc_U8bit_t    count = 0;
            for(count=0; count<p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt_next_free_index;
                count++)
            {
                if(p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[count].r_ext_hdr_tt.p_item
                    != ROHC_NULL)
                {
                    freeMemPool(p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[count].
                        r_ext_hdr_tt.p_item);
                }
            }   /* end of for */
            for(count = 0; count<p_csc->rohc_c_uo_ext_hdr_tt.
                    uo_ext_hdr_tt_next_free_index; count++)
            {
                if(p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[count].p_item != ROHC_NULL)
                {
                    freeMemPool(p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[count].p_item);
                }
            } /* end of for */
            freeMemPool(p_csc);
            break;
        }
        case ROHC_IP_TCP :
            freeMemPool(p_csc);
            break;
        default:
            ROHC_LOG(LOG_INFO,"Freeing of CSC failed for Profile [%d]",p_csc->profile);
            break; 
    }
}

/*****************************************************************************
 * Function Name  : csc_walker
 * Inputs         : The RB Tree node is taken as input
 * Outputs        : None
 * Returns        : None
 * Description    : his function is used to traverse the CSC tree
 *****************************************************************************/
rohc_void_t     csc_walker  ROHC_PARAMS
(   (p_tnode, p_walker_param),
    rohc_tree_node_t    *p_tnode    _AND_
    rohc_void_t         *p_walker_param
)
{
    rohc_profile_node_t     *pl_tnode;
    rohc_db_list_t          *p_list;

    ROHC_ENTER_FN("csc_walker");
    pl_tnode = (rohc_profile_node_t *)p_tnode;

    p_list = (rohc_db_list_t *)(p_walker_param);

    p_list->p_data[p_list->counter] = pl_tnode->p_csc;
    p_list->counter++;


    ROHC_EXIT_FN("csc_walker"); 
}

/*****************************************************************************
 * Function Name  : release_csc
 * Inputs         : The RB Tree node is taken as input
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to delete a node in the tree
 *****************************************************************************/
rohc_void_t     release_csc  ROHC_PARAMS
(   (p_tnode, p_walker_param),
    rohc_tree_node_t    *p_tnode    _AND_
    rohc_void_t         *p_walker_param
)
{
    if(p_tnode){
        if(((rohc_profile_node_t *)p_tnode)->p_key != ROHC_NULL) {
            freeMemPool(((rohc_profile_node_t *)p_tnode)->p_key);
        }
        if(((rohc_profile_node_t *)p_tnode)->p_csc != ROHC_NULL) {
            if(ROHC_IP_TCP == ((rohc_csc_common_t*)(((rohc_profile_node_t *)
                                p_tnode)->p_csc))->profile)
            {
                rohc_U8bit_t        index = 0;
                rohc_csc_tcp_t  *p_csc = (rohc_csc_tcp_t*)(((
                                rohc_profile_node_t *) p_tnode)->p_csc);
                /* check if there are any existing generic options*/
                if(p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.next_free_index)
                {
                    /* free the generic  options in the
                     * context*/
                    for(index = 0; index < p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.
                            next_free_index; index++)
                    {
                        ROHC_FREE(p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.
                                rohc_generic_opt_elem[index].opt_val);
                        p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.rohc_generic_opt_elem
                            [index].opt_val = ROHC_NULL;
                    }
                }

            }
            rohc_db_free_csc(((rohc_profile_node_t *)p_tnode)->p_csc);
        }

        treeDeleteNode((rohc_rb_tree_t *)(p_walker_param), p_tnode);
        freeMemPool(p_tnode);
    }
}

/*****************************************************************************
 * Function Name  : rohc_db_get_all_csc
 * Inputs         : The rohc entity context is taken as input
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to access all the nodes of the RB-Tree
 *****************************************************************************/
#ifdef ROHC_FUNC_ENHANCEMENT
rohc_void_t     rohc_db_get_all_csc     ROHC_PARAMS
(   (p_entity, p_p_list), 
    rohc_entity_t   *p_entity       _AND_
    rohc_db_list_t  **p_p_list
)
{
    rohc_U8bit_t    profile = 0;

    ROHC_ENTER_FN("rohc_db_get_all_csc");

    *p_p_list = (rohc_db_list_t *)ROHC_MALLOC(sizeof(rohc_db_list_t));
    if(*p_p_list == ROHC_NULL)  
    {
        ltePanic("Memory Allocation Failure");
        return;
    }
    (*p_p_list)->counter = 0;

    for(profile = 1; profile<ROHC_ALL_PROFILE; profile++)
    {
        treeWalk(&(p_entity->profile_tree[profile].profile_seed), 
                (p_walker)csc_walker, (rohc_void_t *)(*p_p_list));
    }
    ROHC_EXIT_FN("rohc_db_get_all_csc");
}
#endif

/*****************************************************************************
 * Function Name  : rohc_db_delete_all_csc
 * Inputs         : The rohc entity context is taken as input
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to delete all CSC nodes in RB tree
 *****************************************************************************/
rohc_void_t     rohc_db_delete_all_csc  ROHC_PARAMS
(   (p_entity), rohc_entity_t   *p_entity)
{
    rohc_U8bit_t    profile = 0;
    ROHC_ENTER_FN("rohc_db_delete_all_csc");
    /* deletes the tree node used for Feedback*/
    rohc_db_fb_delete_all_csc_node(p_entity);

    for(profile = 1; profile<ROHC_ALL_PROFILE; profile++)
    {
        treeDelete(&(p_entity->profile_tree[profile].profile_seed),
                (p_walker)release_csc,
                (void *)&(p_entity->profile_tree[profile].profile_seed));
    }
    ROHC_EXIT_FN("rohc_db_delete_all_csc");
}

#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_db_get_no_of_nodes
 * Inputs         : The rohc entity context and profile are taken as input
 * Outputs        : None
 * Returns        : Number of nodes in tree
 * Description    : This function is used to get the number of nodes in the RB-Tree
 *****************************************************************************/
rohc_U32bit_t   rohc_db_get_no_of_nodes ROHC_PARAMS
(   (p_entity, profile),
    rohc_entity_t   *p_entity   _AND_
    rohc_U8bit_t    profile
)
{
    return treeCount(&(p_entity->profile_tree[profile].profile_seed));
}
#endif

/*****************************************************************************
 * Function Name  : rohc_db_compare_ip4
 * Inputs         : The 2 keys are taken as input
 * Outputs        : None
 * Returns        : -1, 0, 1
 * Description    : This function is used to compare IPv4 fields of 2 nodes in
 *                  a RB-Tree
 *****************************************************************************/
rohc_S32bit_t   rohc_db_compare_ip4     ROHC_PARAMS
(   (key1, key2),
    rohc_static_def_ip4_t   key1    _AND_
    rohc_static_def_ip4_t   key2
)
{
    ROHC_ENTER_FN("rohc_db_comnpare_ip4");
    if(key1.dst_addr < key2.dst_addr)
        return(-1);
    else if(key1.dst_addr > key2.dst_addr)
        return(1);
    else
    {

        if(key1.src_addr < key2.src_addr)
            return(-1);
        else if(key1.src_addr > key2.src_addr)
            return(1);
        else
            return(0);
    }
    ROHC_EXIT_FN("rohc_db_comnpare_ip4");
}

/*****************************************************************************
 * Function Name  : rohc_db_compare_ip6
 * Inputs         : The 2 keys are taken as input
 * Outputs        : 
 * Returns        : -1, 0, 1
 * Description    : This function is used to compare IPv6 fields of 2 nodes
 *****************************************************************************/
rohc_S32bit_t   rohc_db_compare_ip6     ROHC_PARAMS
(   (key1, key2),
    rohc_static_def_ip6_t   key1    _AND_
    rohc_static_def_ip6_t   key2
)
{
    ROHC_ENTER_FN("rohc_db_comnpare_ip6");
    if(ROHC_MEMCMP(key1.dst_addr, key2.dst_addr, 16) < 0)
        return(-1);
    else if(ROHC_MEMCMP(key1.dst_addr, key2.dst_addr, 16) > 0)
        return(1);
    else
    {
        /* Compare IPv6 src address */
        if(ROHC_MEMCMP(key1.src_addr, key2.src_addr, 16) < 0)
            return(-1);
        else if(ROHC_MEMCMP(key1.src_addr, key2.src_addr, 16) > 0)
            return(1);
        else
        {
            /* Compare Flow label */
            if(key1.flow_label < key2.flow_label)
                return(-1);
            else if(key1.flow_label > key2.flow_label)
                return(1);
            else
                return(0);
        }
    }
    ROHC_EXIT_FN("rohc_db_comnpare_ip6");
}
    
/*****************************************************************************
 * Function Name  : rohc_compare_profile_1
 * Inputs         : The 2 keys are taken as input
 * Outputs        : 
 * Returns        : -1, 0, 1
 * Description    : This function is used to compare 2 nodes of Profile 1 RB-Tree.
 *                  (The order of comparison is IP Version, IP Dst address, IP src
 *                  address, UDP src port, UDP dest port, RTP SSRC. This function
 *                  has a good scope of improvement and can increase performance
 *                  by having a good comparison strategy. A more efficient
 *                  strategy would be to consider all the fields for comparsion
 *                  rather then having an order which may not be very appropriate).
 *****************************************************************************/
rohc_S32bit_t   rohc_compare_profile_1  ROHC_PARAMS
(
    (key1, key2),
    rohc_void_tp    key1    _AND_
    rohc_void_tp    key2
)
{
    rohc_profile1_t     *p_key1 = (rohc_profile1_t *)key1;
    rohc_profile1_t     *p_key2 = (rohc_profile1_t *)key2;
    /*Start SPR 6159 Fix*/ 
    rohc_S32bit_t       ret = 0;
    /*End SPR 6159 Fix*/

    ROHC_ENTER_FN("rohc_comnpare_profile_1");
    if(p_key1->ip.ip_version == p_key2->ip.ip_version)
    {
        if(p_key1->ip.ip_version == ROHC_VERSION_IP4)
        {
            ret = rohc_compare_profile_1_ip4(p_key1,p_key2);
            return ret;
        } /* IPv4 */
        else if(p_key1->ip.ip_version == ROHC_VERSION_IP6)
        {
            ret = rohc_compare_profile_1_ip6(p_key1,p_key2);
            return ret;
        } /* IPv6 */
    } /* end of IP version comp */
    else
    {
        if(p_key1->ip.ip_version < p_key2->ip.ip_version)
            return(-1);
        else if(p_key1->ip.ip_version > p_key2->ip.ip_version)
            return(1);
    }
    ROHC_EXIT_FN("rohc_compare_profile_1");
    return 0;
}

/*****************************************************************************
 * Function Name  : rohc_compare_profile_2
 * Inputs         : The 2 Keys are taken as input
 * Outputs        : 
 * Returns        : -1, 0, 1
 * Description    : This function is used to compare 2 nodes of Profile 2 RB-Tree
 *                  (The order of comparison is IP Version, IP Dst address, IP src
 *                  address, UDP dest port, UDP src port.
 *                  This function has a good scope of improvement and can increase
 *                  performance by having a good comparison strategy.
 *                  A more efficient strategy would be to consider all the fields
 *                  for comparsion rather then having an order which may not be
 *                  very appropriate).
 *****************************************************************************/
rohc_S32bit_t   rohc_compare_profile_2  ROHC_PARAMS
(
    (key1, key2),
    rohc_void_tp    key1    _AND_
    rohc_void_tp    key2
)
{
    rohc_profile2_t     *p_key1 = (rohc_profile2_t *)key1;
    rohc_profile2_t     *p_key2 = (rohc_profile2_t *)key2;
    /*Start SPR 6159 Fix*/
    rohc_S32bit_t       ret = 0;
    /*End SPR 6159 Fix*/

    ROHC_ENTER_FN("rohc_comnpare_profile_2");
    if(p_key1->ip.ip_version == p_key2->ip.ip_version)
    {
        if(p_key1->ip.ip_version == ROHC_VERSION_IP4)
        {
            ret = rohc_db_compare_ip4(p_key1->ip.un1.ipv4, p_key2->ip.un1.ipv4);
            if(ret == 0)
            {
                if(p_key1->udp.dst_port < p_key2->udp.dst_port)
                    return(-1);
                else if(p_key1->udp.dst_port > p_key2->udp.dst_port)
                    return(1);
                else
                {
                    if(p_key1->udp.src_port < p_key2->udp.src_port)
                        return(-1);
                    else if(p_key1->udp.src_port > p_key2->udp.src_port)
                        return(1);
                    else
                        return 0;
                }
            }
            else
                return ret;
        }
        else if(p_key1->ip.ip_version == ROHC_VERSION_IP6)
        {
            ret = rohc_db_compare_ip6(p_key1->ip.un1.ipv6, p_key2->ip.un1.ipv6);
            if(ret == 0)
            {
                if(p_key1->udp.dst_port < p_key2->udp.dst_port)
                    return(-1);
                else if(p_key1->udp.dst_port > p_key2->udp.dst_port)
                    return(1);
                else
                {
                    if(p_key1->udp.src_port < p_key2->udp.src_port)
                        return(-1);
                    else if(p_key1->udp.src_port > p_key2->udp.src_port)
                        return(1);
                    else
                        return 0;
                }
            }
            else
                return ret;
        }
    }
    else
    {
        if(p_key1->ip.ip_version < p_key2->ip.ip_version)
            return(-1);
        else if(p_key1->ip.ip_version > p_key2->ip.ip_version)
            return(1);
    }
    ROHC_ENTER_FN("rohc_comnpare_profile_2");
    return 0;
}

/*****************************************************************************
 * Function Name  : rohc_compare_profile_3
 * Inputs         : The 2 Keys are taken as input
 * Outputs        : 
 * Returns        : SUCCES/FAILURE
 * Description    : This function is used to compare 2 nodes of Profile 3 RB-Tree
 *                  (The order of comparison is IP Version, IP Dst address, IP src
 *                  address, ESP SPI.
 *                  This function has a good scope of improvement and can increase
 *                  performance by having a good comparison strategy.
 *                  A more efficient strategy would be to consider all the fields
 *                  for comparsion rather then having an order which may not be
 *                  very appropriate).
 *****************************************************************************/
rohc_S32bit_t   rohc_compare_profile_3  ROHC_PARAMS
(
    (key1, key2),
    rohc_void_tp    key1    _AND_
    rohc_void_tp    key2
)
{
    rohc_profile3_t     *p_key1 = (rohc_profile3_t *)key1;
    rohc_profile3_t     *p_key2 = (rohc_profile3_t *)key2;
    rohc_U32bit_t       ret = ROHC_SUCCESS;

    ROHC_ENTER_FN("rohc_comnpare_profile_3");

    if(p_key1->ip.ip_version == p_key2->ip.ip_version)
    {
        if(p_key1->ip.ip_version == ROHC_VERSION_IP4)
        {
            ret = rohc_db_compare_ip4(p_key1->ip.un1.ipv4, p_key2->ip.un1.ipv4);
            if(ret == 0)
            {
                if(p_key1->esp.spi < p_key2->esp.spi)
                    return(-1);
                else if(p_key1->esp.spi > p_key2->esp.spi)
                    return(1);
                else
                    return 0;
            }
            else
                return ret;
        }
        else if(p_key1->ip.ip_version == ROHC_VERSION_IP6)
        {
            ret = rohc_db_compare_ip6(p_key1->ip.un1.ipv6, p_key2->ip.un1.ipv6);
            if(ret == 0)
            {
                if(p_key1->esp.spi < p_key2->esp.spi)
                    return(-1);
                else if(p_key1->esp.spi > p_key2->esp.spi)
                    return(1);
                else
                    return 0;
            }
            else
                return ret;
        }
    }
    else
    {
        if(p_key1->ip.ip_version < p_key2->ip.ip_version)
            return(-1);
        else if(p_key1->ip.ip_version > p_key2->ip.ip_version)
            return(1);
    }
    ROHC_EXIT_FN("rohc_comnpare_profile_3");
    return 0;
}

/*****************************************************************************
 * Function Name  : rohc_compare_profile_6
 * Inputs         : The 2 Keys are taken as input
 * Outputs        : 
 * Returns        : -1, 0, 1
 * Description    : This function is used to compare 2 nodes of Profile 2 RB-Tree
 *                  (The order of comparison is IP Version, IP Dst address, IP src
 *                  address, UDP dest port, UDP src port.
 *                  This function has a good scope of improvement and can increase
 *                  performance by having a good comparison strategy.
 *                  A more efficient strategy would be to consider all the fields
 *                  for comparsion rather then having an order which may not be
 *                  very appropriate).
 *****************************************************************************/
rohc_S32bit_t   rohc_compare_profile_6 ROHC_PARAMS
(
    (key1, key2),
    rohc_void_tp    key1    _AND_
    rohc_void_tp    key2
)
{
    rohc_profile6_t     *p_key1 = (rohc_profile6_t *)key1;
    rohc_profile6_t     *p_key2 = (rohc_profile6_t *)key2;
    rohc_S32bit_t       ret = 0;

    ROHC_ENTER_FN("rohc_comnpare_profile_6");
    if(p_key1->ip.ip_version == p_key2->ip.ip_version)
    {
        if(p_key1->ip.ip_version == ROHC_VERSION_IP4)
        {
            ret = rohc_db_compare_ip4(p_key1->ip.un1.ipv4, p_key2->ip.un1.ipv4);
            if(ret == 0)
            {
                if(p_key1->tcp.dst_port < p_key2->tcp.dst_port)
                    return(-1);
                else if(p_key1->tcp.dst_port > p_key2->tcp.dst_port)
                    return(1);
                else
                {
                    if(p_key1->tcp.src_port < p_key2->tcp.src_port)
                        return(-1);
                    else if(p_key1->tcp.src_port > p_key2->tcp.src_port)
                        return(1);
                    else
                        return 0;
                }
            }
            else
                return ret;
        }
        else if(p_key1->ip.ip_version == ROHC_VERSION_IP6)
        {
            ret = rohc_db_compare_ip6(p_key1->ip.un1.ipv6, p_key2->ip.un1.ipv6);
            if(ret == 0)
            {
                if(p_key1->tcp.dst_port < p_key2->tcp.dst_port)
                    return(-1);
                else if(p_key1->tcp.dst_port > p_key2->tcp.dst_port)
                    return(1);
                else
                {
                    if(p_key1->tcp.src_port < p_key2->tcp.src_port)
                        return(-1);
                    else if(p_key1->tcp.src_port > p_key2->tcp.src_port)
                        return(1);
                    else
                        return 0;
                }
            }
            else
                return ret;
        }
    }
    else
    {
        if(p_key1->ip.ip_version < p_key2->ip.ip_version)
            return(-1);
        else if(p_key1->ip.ip_version > p_key2->ip.ip_version)
            return(1);
    }
    ROHC_EXIT_FN("rohc_comnpare_profile_6");
    return 0;
}

/*****************************************************************************
 * Function Name  : rohc_keyof_profile
 * Inputs         : The tree node is taken as input
 * Outputs        : 
 * Returns        : rohc_void_t
 * Description    : This function is used by CSPL to access the key of a node.
 *****************************************************************************/
rohc_void_tp    rohc_keyof_profile    ROHC_PARAMS
((p_node), rohc_tree_node_t *p_node )
{
    return  (((rohc_profile_node_t *)p_node)->p_key);
}

/*****************************************************************************
 * Function Name  : rohc_db_generate_context_id
 * Inputs         : The rohc entity context are taken as input
 * Outputs        : 
 * Returns        : context_id
 * Description    : This function is used to generate context id
 *****************************************************************************/
rohc_U32bit_t   rohc_db_generate_context_id ROHC_PARAMS
    ((p_entity), rohc_entity_t  *p_entity)
{
    /*Coverity Fix 32371 start*/
    rohc_U32bit_t   cid = ROHC_INVALID_CONTEXT_ID;
    /*Coverity Fix 32371 end*/
/* SPR #1635 Now context Id is stored in entity block rather than global,
 ** so extract from that block */

    cid =  ++(p_entity->context_id);
    
    ROHC_ENTER_FN("rohc_db_generate_context_id");
    /* In case of large CID MAX CID is 15 else 2^14 -1 */
    if(cid > p_entity->max_cid)
    {
        /* Call Error Handler */
        /*Coverity Fix 32371 start*/
        cid = ROHC_INVALID_CONTEXT_ID;
        /*Coverity Fix 32371 end*/
    }
    ROHC_EXIT_FN("rohc_db_generate_context_id");
    return cid;
}

#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_create_key_for_context_update
 * Inputs         : The rohc entity context are taken as input
 * Outputs        : 
 * Returns        : context_id
 * Description    : This function is used to generate a key for inserting a context
 *                  reloc block in the CSC tree at target RNC side.
 *****************************************************************************/
rohc_return_t   rohc_create_key_for_context_update  ROHC_PARAMS
(   (p_stream_pkt_info, p_p_key),
    stream_pkt_info_t       *p_stream_pkt_info  _AND_
    rohc_void_t             **p_p_key
)
{
    rohc_profile1_t     *p_prof1_key = ROHC_NULL;
    rohc_profile2_t     *p_prof2_key = ROHC_NULL;
    rohc_profile3_t     *p_prof3_key = ROHC_NULL;

    ROHC_ENTER_FN("rohc_create_key_for_context_update");

    switch(p_stream_pkt_info->profile)
    {
        case ROHC_IP_UDP_RTP:
        {
            if(p_stream_pkt_info->profile_union.profile_1.inner_ip.ip_version ==
                    ROHC_VERSION_IP4)
            {
                *p_p_key = rohc_db_mem_get_ip4_key();
                if(*p_p_key == ROHC_NULL)
                    return ROHC_FAILURE;
                p_prof1_key = (rohc_profile1_t *)(*p_p_key);
                p_prof1_key->ip.un1.ipv4.src_addr = 
                p_stream_pkt_info->profile_union.profile_1.inner_ip.ip_union.ip4.src;
                p_prof1_key->ip.un1.ipv4.dst_addr =
                p_stream_pkt_info->profile_union.profile_1.inner_ip.ip_union.ip4.dst;
            }
            else
            {
                /* IPv6 present */
                *p_p_key = rohc_db_mem_get_ip6_key();
                if(*p_p_key == ROHC_NULL)
                    return ROHC_FAILURE;
                p_prof1_key = (rohc_profile1_t *)(*p_p_key);
                ROHC_MEMCPY(p_prof1_key->ip.un1.ipv6.src_addr, 
                p_stream_pkt_info->profile_union.profile_1.inner_ip.ip_union.ip6.src, 
                    (4*sizeof(rohc_U32bit_t)));
                ROHC_MEMCPY(p_prof1_key->ip.un1.ipv6.dst_addr,
                p_stream_pkt_info->profile_union.profile_1.inner_ip.ip_union.ip6.dst, 
                    (4*sizeof(rohc_U32bit_t)));
                p_prof1_key->ip.un1.ipv6.flow_label = 
                    p_stream_pkt_info->profile_union.profile_1.inner_ip.ip_union.ip6.
                    v_flowlabel_nh & ROHC_IPv6_STATIC_FLOWLABEL_MASK;
            }
            p_prof1_key->ip.ip_version = 
                    p_stream_pkt_info->profile_union.profile_1.inner_ip.ip_version;
            p_prof1_key->rtp.ssrc = 
                p_stream_pkt_info->profile_union.profile_1.rtp.ssrc;
            p_prof1_key->udp.src_port =
                p_stream_pkt_info->profile_union.profile_1.udp.src;
            p_prof1_key->udp.dst_port = 
                p_stream_pkt_info->profile_union.profile_1.udp.dst;
        }
        break;
        case ROHC_IP_UDP:
        {
            if(p_stream_pkt_info->profile_union.profile_2.inner_ip.ip_version ==
                    ROHC_VERSION_IP4)
            {
                *p_p_key = rohc_db_mem_get_ip4_key();
                if(*p_p_key == ROHC_NULL)
                    return ROHC_FAILURE;
                p_prof2_key = (rohc_profile2_t *)(*p_p_key);
                p_prof2_key->ip.un1.ipv4.src_addr =
                p_stream_pkt_info->profile_union.profile_2.inner_ip.ip_union.ip4.src;
                p_prof2_key->ip.un1.ipv4.dst_addr = 
                p_stream_pkt_info->profile_union.profile_2.inner_ip.ip_union.ip4.dst;
            }
            else
            {
                /* IPv6 present */
                *p_p_key = rohc_db_mem_get_ip6_key();
                if(*p_p_key == ROHC_NULL)
                    return ROHC_FAILURE;
                p_prof2_key = (rohc_profile2_t *)(*p_p_key);
                ROHC_MEMCPY(p_prof2_key->ip.un1.ipv6.src_addr, 
                p_stream_pkt_info->profile_union.profile_2.inner_ip.ip_union.ip6.src, 
                    (4*sizeof(rohc_U32bit_t)));
                ROHC_MEMCPY(p_prof2_key->ip.un1.ipv6.dst_addr,
                p_stream_pkt_info->profile_union.profile_2.inner_ip.ip_union.ip6.dst, 
                    (4*sizeof(rohc_U32bit_t)));
                p_prof2_key->ip.un1.ipv6.flow_label = 
                    p_stream_pkt_info->profile_union.profile_2.inner_ip.ip_union.ip6.
                    v_flowlabel_nh & ROHC_IPv6_STATIC_FLOWLABEL_MASK;
            }
            p_prof2_key->ip.ip_version =
                    p_stream_pkt_info->profile_union.profile_2.inner_ip.ip_version;
            p_prof2_key->udp.src_port =
                   p_stream_pkt_info->profile_union.profile_2.udp.src;
            p_prof2_key->udp.dst_port =
                    p_stream_pkt_info->profile_union.profile_2.udp.dst;
        }
        break;
        case ROHC_IP_ESP:
        {
            if(p_stream_pkt_info->profile_union.profile_3.inner_ip.ip_version ==
                    ROHC_VERSION_IP4)
            {
                *p_p_key = rohc_db_mem_get_ip4_key();
                if(*p_p_key == ROHC_NULL)
                    return ROHC_FAILURE;
                p_prof3_key = (rohc_profile3_t *)(*p_p_key);
                p_prof3_key->ip.un1.ipv4.src_addr =
                p_stream_pkt_info->profile_union.profile_3.inner_ip.ip_union.ip4.src;
                p_prof3_key->ip.un1.ipv4.dst_addr =
                p_stream_pkt_info->profile_union.profile_3.inner_ip.ip_union.ip4.dst;
            }
            else
            {
                /* IPv6 present */
                *p_p_key = rohc_db_mem_get_ip6_key();
                if(*p_p_key == ROHC_NULL)
                    return ROHC_FAILURE;
                p_prof3_key = (rohc_profile3_t *)(*p_p_key);
                ROHC_MEMCPY(p_prof3_key->ip.un1.ipv6.src_addr, 
                p_stream_pkt_info->profile_union.profile_3.inner_ip.ip_union.ip6.src, 
                    (4*sizeof(rohc_U32bit_t)));
                ROHC_MEMCPY(p_prof3_key->ip.un1.ipv6.dst_addr,
                p_stream_pkt_info->profile_union.profile_3.inner_ip.ip_union.ip6.dst, 
                    (4*sizeof(rohc_U32bit_t)));
                p_prof3_key->ip.un1.ipv6.flow_label = 
                    p_stream_pkt_info->profile_union.profile_3.inner_ip.ip_union.ip6.
                    v_flowlabel_nh & ROHC_IPv6_STATIC_FLOWLABEL_MASK;
            }
            p_prof3_key->ip.ip_version =
                    p_stream_pkt_info->profile_union.profile_3.inner_ip.ip_version;
            p_prof3_key->esp.spi = 
                    p_stream_pkt_info->profile_union.profile_3.esp.spi;
        }
        break;
        default:
        {
            return ROHC_FAILURE;
        }
        break;
    } /* end of switch */
    ROHC_EXIT_FN("rohc_create_key_for_context_update");
    return ROHC_SUCCESS;
}

#endif
#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_mem_get_node
 * Inputs         : None
 * Outputs        : None
 * Returns        : pointer to node
 * Description    : This function is used to allocate memory for a node in the list
 *****************************************************************************/
rohc_protocol_node_t    *rohc_mem_get_node  ROHC_NO_PARAMS()
{
    rohc_protocol_node_t    *p_node;
    ROHC_ENTER_FN("rohc_mem_get_node");

    p_node = (rohc_protocol_node_t *)allocateMemFromPool(rohc_global.list_node_pool);
    if(ROHC_NULL == p_node)
    {
        extendPool(rohc_global.list_node_pool, ROHC_POOL_EXTEND_STEP);
        p_node = (rohc_protocol_node_t *)
                        allocateMemFromPool(rohc_global.list_node_pool);
        if (ROHC_NULL == p_node)
             ltePanic("%s:node Extend Pool fails.",__FUNCTION__);

        PoolTunningNeeded("list_node_pool");
    }
    ROHC_EXIT_FN("rohc_mem_get_node");
    return p_node;
}

#endif
#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_db_delete_csc
 * Inputs         : pointer to entity and the CSC to be deleted.
 * Outputs        : 
 * Returns        : None
 * Description    : This function is used to delete a CSC from the tree
 *****************************************************************************/
rohc_void_t     rohc_db_delete_csc  ROHC_PARAMS
(   (p_entity, p_csc),
    rohc_entity_t       *p_entity   _AND_
    rohc_csc_t          *p_csc
)
{
    rohc_profile_node_t     *p_rb_node = p_csc->p_rb_tnode;
    rohc_U8bit_t            profile = p_csc->profile;

    ROHC_ENTER_FN("rohc_db_delete_csc");

    if(p_rb_node == ROHC_NULL)
    {
        return;
    }
/*Start SPR #: 1391 */  
    rohc_db_fb_delete_csc_node(p_entity,p_csc);
/*End SPR #: 1391 */    

    if(p_rb_node->p_key != ROHC_NULL)
    {
        freeMemPool(((rohc_profile_node_t *)p_rb_node)->p_key);
    }
    if(p_rb_node->p_csc != ROHC_NULL)
    {
        rohc_db_free_csc((rohc_csc_t *)(p_rb_node->p_csc));
    }
    treeDeleteNode(&(p_entity->profile_tree[profile].profile_seed), 
        (rohc_tree_node_t *)p_rb_node);

    p_rb_node = ROHC_NULL;

    ROHC_EXIT_FN("rohc_db_delete_csc");
}

#endif

/*****************************************************************************
 * Function Name  : rohc_db_get_uncomp_csc
 * Inputs         : pointer to entity and CPIB and error code
 * Outputs        : 
 * Returns        : pointer to node
 * Description    : This function is used to create a CSC with a new CID.
 *                  (If the function is returning NULL, then the caller should
 *                  check the error code for appropriate action).
 *****************************************************************************/
rohc_void_t *  rohc_db_get_uncomp_csc  ROHC_PARAMS
(   (p_entity, p_ecode),
    rohc_entity_t       *p_entity       _AND_
    rohc_error_code_t   *p_ecode
)
{
    rohc_csc_uncomp_t       *p_csc = ROHC_NULL;
    rohc_U32bit_t           context_id;
    ROHC_ENTER_FN("rohc_db_get_uncomp_csc");
    /* Generate the context id */
    /*
    context_id = rohc_db_generate_context_id(p_entity);
    if(context_id == ROHC_INVALID_CID)
    {
        *p_ecode = ROHC_INVALID_CID;
        return ROHC_NULL;
    }
    */
    context_id = 0; /* As per the current implementation cid 0 is used for
                     uncompress profile*/
    if(p_entity->uncomp_csc != ROHC_NULL)
    {   
/* PQT SPR 1686*/
     return p_entity->uncomp_csc;
    }
    else
    {   
        p_csc = allocateMemFromPool(rohc_global.csc_pool);
        if(p_csc == ROHC_NULL)
        {   
            extendPool(rohc_global.csc_pool, ROHC_POOL_EXTEND_STEP);
            p_csc = allocateMemFromPool(rohc_global.csc_pool);
            if (ROHC_NULL == p_csc)
            {
                *p_ecode = ROHC_MEMORY_FAILURE;
                ltePanic("%s:csc Extend Pool fails.",__FUNCTION__);
                /*Klockworks Warning Fix Start*/
                return ROHC_NULL;
                /*Klockworks Warning Fix End*/
            }

            PoolTunningNeeded("csc_pool");
        }   
    }
    p_csc->profile=ROHC_UNCOMP;
   
    p_csc->c_state = ROHC_CUN_ST_IR;
    p_csc->field_value[ROHC_CID_FID - ROHC_VAR_FID_BASE] = context_id;
    /* Set the CPIB pointer */
    p_entity->uncomp_csc = p_csc;
    ROHC_EXIT_FN("rohc_db_get_uncomp_csc");
    return (rohc_void_t *)p_csc;
}

/*****************************************************************************
 * Function Name  : rohc_db_create_csc_new_cid
 * Inputs         : pointer to entity and CPIB and error code
 * Outputs        : 
 * Returns        : pointer to node
 * Description    : This function is used to create a CSC with a new CID
 *                  (If the function is returning NULL, then the caller should
 *                  check the error code for appropriate action).
 *****************************************************************************/
rohc_csc_common_t  *   rohc_db_create_csc_new_cid  ROHC_PARAMS
(   (p_entity, p_cpib, p_ecode),
    rohc_entity_t       *p_entity       _AND_
    rohc_cpib_t         *p_cpib         _AND_
    rohc_error_code_t   *p_ecode
)
{
    rohc_void_tp            p_key = ROHC_NULL;
    rohc_csc_common_t       *p_csc = ROHC_NULL;
    rohc_U32bit_t           context_id;

    ROHC_ENTER_FN("rohc_db_create_csc_new_cid");
	
	if(p_cpib->determined_profile == ROHC_UNCOMP)
	{
		 return rohc_db_get_uncomp_csc(p_entity,p_ecode);
	}

   /* Generate the context id */
   context_id = rohc_db_generate_context_id(p_entity);
   /* Coverity fix 32371 start*/
   if(context_id ==  ROHC_INVALID_CONTEXT_ID)
   {
        *p_ecode = ROHC_INVALID_CID;
        return ROHC_NULL;
   }

   /* Coverity fix 32371 end */
   /* First construct a key for creating CSC */
   /* 
   ** START_SPR_568_FIX: As p_cpib->determined_profile is never set in case of
   ** profile-1 (it is only set for profile-2 type of packets) hence create key
   ** fails for RTP packets.
   */
    p_key = rohc_db_create_key(p_cpib, p_cpib->determined_profile);
   /* END_SPR_568_FIX */
    
    if(p_key == ROHC_NULL)
    {
        *p_ecode = ROHC_MEMORY_FAILURE;
        return ROHC_NULL;
    }

   /* 
   ** START_SPR_568_FIX: As p_cpib->determined_profile is never set in case of
   ** profile-1 (it is only set for profile-2 type of packets) hence create 
   ** node in RB tree for CSC fails for RTP packets.
   */
    p_csc = rohc_db_create_csc(p_key, p_entity, p_cpib->determined_profile);
   /* END_SPR_568_FIX */
    
    if(p_csc == ROHC_NULL)
    {
        freeMemPool(p_key);
        *p_ecode = ROHC_MEMORY_FAILURE;
        return ROHC_NULL;
    }
    p_csc->field_value[ROHC_CID_FID - ROHC_VAR_FID_BASE] = context_id;
    /* Set the CPIB pointer */
    p_csc->p_cpib = p_cpib;
    if(p_csc->profile == ROHC_IP_UDP ||
       p_csc->profile == ROHC_IP_UDP_RTP)
    {
        if(((rohc_udp_t *)p_csc->p_cpib->udp)->cksum != 0)
        {
            p_csc->static_delta |= ROHC_UDP_CHECK_SUM_PRESENT;  
        }   
    }
    if(p_csc->profile == ROHC_IP_TCP)
    {
        if(((rohc_tcp_t *)p_csc->p_cpib->tcp)->checksum != 0)
        {
            p_csc->static_delta |= ROHC_TCP_CHECK_SUM_PRESENT;  
        }   
    }
    if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
    {
        p_csc->static_delta |= ROHC_INNER_IP4_PRESENT;
    } 
    if(p_csc->p_cpib->inner_ip6 != ROHC_NULL)
    {
        p_csc->static_delta |= ROHC_INNER_IP6_PRESENT;
    }
    if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
    {
        p_csc->static_delta |= ROHC_OUTER_IP4_PRESENT;
    } 
    if(p_csc->p_cpib->outer_ip6 != ROHC_NULL)
    {
        p_csc->static_delta |= ROHC_OUTER_IP6_PRESENT;
    }

    /* for feedback */
    rohc_db_fb_add_csc_node(p_entity,context_id,p_csc);
    
    ROHC_EXIT_FN("rohc_db_create_csc_new_cid");
    return (p_csc);
}

/*****************************************************************************
 * Function Name  : rohc_compare_profile_1_ip4
 * Inputs         : The 2 keys are taken as input
 * Outputs        : 
 * Returns        : -1, 0, 1
 * Description    : This function is used to compare 2 nodes of Profile 1 RB-Tree.
 *                  (The order of comparison is IP Version, IP Dst address, IP src
 *                  address, UDP src port, UDP dest port, RTP SSRC.
 *****************************************************************************/
static rohc_S32bit_t   rohc_compare_profile_1_ip4  ROHC_PARAMS
(
    (p_key1, p_key2),
    rohc_profile1_t     *p_key1    _AND_
    rohc_profile1_t     *p_key2    
    
)
{
    rohc_S32bit_t       ret = 0;
    ret = rohc_db_compare_ip4(p_key1->ip.un1.ipv4, p_key2->ip.un1.ipv4);
    if(ret == 0)
    {
        if(p_key1->udp.src_port < p_key2->udp.src_port)
            return(-1);
        else if(p_key1->udp.src_port > p_key2->udp.src_port)
            return(1);
        else
        {
            if(p_key1->udp.dst_port < p_key2->udp.dst_port)
                return(-1);
            else if(p_key1->udp.dst_port > p_key2->udp.dst_port)
                return(1);
            else
            {
                if(p_key1->rtp.ssrc < p_key2->rtp.ssrc)
                    return(-1);
                else if(p_key1->rtp.ssrc > p_key2->rtp.ssrc)
                    return(1);
                else
                    return(0);
            }
        }
    }
    else
        return ret;
}

/*****************************************************************************
 * Function Name  : rohc_compare_profile_1_ip6
 * Inputs         : The 2 keys are taken as input
 * Outputs        : 
 * Returns        : -1, 0, 1
 * Description    : This function is used to compare 2 nodes of Profile 1 RB-Tree.
 *                  (The order of comparison is IP Version, IP Dst address, IP src
 *                  address, UDP src port, UDP dest port, RTP SSRC.
 *****************************************************************************/
static rohc_S32bit_t   rohc_compare_profile_1_ip6  ROHC_PARAMS
(
    (p_key1, p_key2),
    rohc_profile1_t     *p_key1    _AND_
    rohc_profile1_t     *p_key2    
    
)
{
    rohc_S32bit_t       ret = 0;
    ret = rohc_db_compare_ip6(p_key1->ip.un1.ipv6, p_key2->ip.un1.ipv6);
    if(ret == 0)
    {
        if(p_key1->udp.src_port < p_key2->udp.src_port)
            return(-1);
        else if(p_key1->udp.src_port > p_key2->udp.src_port)
            return(1);
        else
        {
            if(p_key1->udp.dst_port < p_key2->udp.dst_port)
                return(-1);
            else if(p_key1->udp.dst_port > p_key2->udp.dst_port)
                return(1);
            else
            {
                if(p_key1->rtp.ssrc < p_key2->rtp.ssrc)
                    return(-1);
                else if(p_key1->rtp.ssrc > p_key2->rtp.ssrc)
                    return(1);
                else
                    return(0);
            }
        }
    }
    else
        return ret;
}
