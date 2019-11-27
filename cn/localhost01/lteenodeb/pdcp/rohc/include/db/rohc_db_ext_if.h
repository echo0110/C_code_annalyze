/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_db_ext_if.h,v 1.2.28.9 2010/11/24 03:52:11 gur22059 Exp $
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
 * $Log: rohc_db_ext_if.h,v $
 * Revision 1.2.28.9  2010/11/24 03:52:11  gur22059
 * Declaration of the function left in profile 1 segregation
 *
 * Revision 1.2.28.8  2010/11/09 04:48:40  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.28.7  2010/10/13 04:23:57  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.28.6  2010/10/04 03:25:34  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2.28.5  2010/09/23 06:59:16  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2.28.4  2010/07/22 10:00:34  gur22059
 * Prototype of rohc_db_reinit_dsc is changed
 *
 * Revision 1.2.28.3  2010/07/07 11:52:47  gur21145
 * PROFILE2_comments_and PROFILE0_comments_inc
 *
 * Revision 1.2.28.1  2010/06/25 10:09:14  gur21145
 * PROFILE2_ENHANCEMENT
 *
 * Revision 1.2  2009/05/28 05:02:21  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/



#ifndef _ROHC_DB_EXT_IF_H_
#define _ROHC_DB_EXT_IF_H_

#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "rohc_g_typ.h"
#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_db_typ.h"
#include "rohc_main_typ.h"

/* Initialization */
/* + SPR 17439 */
rohc_return_t   rohc_init_db    ROHC_PROTO
((
    rohc_U32bit_t   entity_pool_size    ,
    rohc_U32bit_t   csc_pool_size       ,
    rohc_U32bit_t   dsc_pool_size
));
/* - SPR 17439 */
/************* Wrappers for List ************/

/* To initialize the list */
rohc_void_t rohc_db_create_list ROHC_PROTO (( rohc_list_t   *p_list));
/* + SPR 17439 */
/* To add a node at the end of the list */
rohc_void_t rohc_db_add_last_node   ROHC_PROTO
((
    rohc_list_t             *p_list ,
    rohc_list_node_t        *p_node
));
/* - SPR 17439 */
/* To get the next node of the list */
rohc_list_node_t *rohc_db_get_next_node     ROHC_PROTO
(( rohc_list_node_t *p_node));

/* To get the first node of the list */
rohc_list_node_t *rohc_db_get_first_node    ROHC_PROTO
((rohc_list_t   *p_list));

/* To get the last node of the list */
rohc_list_node_t *rohc_db_get_last_node ROHC_PROTO
((rohc_list_t   *p_list));

/* To get the total number of elements in the list */
rohc_U32bit_t   rohc_db_count_of_list   ROHC_PROTO ((rohc_list_t    *p_list));
/* + SPR 17439 */
/* To delete a noe in the list */
rohc_void_t     rohc_db_delete_node     ROHC_PROTO
((
    rohc_list_t         *p_list     ,
    rohc_list_node_t    *p_node
));
/* - SPR 17439 */
#ifdef ROHC_UT_FLAG
/* To print the whole list */
rohc_void_t rohc_db_dump_list   ROHC_PROTO ((rohc_list_t    *p_list));
#endif

/*****************Functions for entity ************/
/* + SPR 17439 */
/* To create an entity */
rohc_entity_t   *rohc_db_create_entity  ROHC_NO_PARAMS ();
/* - SPR 17439 */
rohc_return_t   rohc_db_delete_entity   ROHC_PROTO
    ((  rohc_entity_t   *p_entity ));

rohc_return_t   rohc_db_reconfig_entity ROHC_PROTO
    (( rohc_entity_t  *p_entity ));

/****************Wrappers for Compressor Stream Context *******/

/* To initialize a CSC block explicitly */
rohc_void_t rohc_db_init_csc    ROHC_PROTO
((
    rohc_csc_common_t  *p_csc
));

rohc_void_tp    rohc_db_mem_get_ip4_key ROHC_NO_PARAMS ();
rohc_void_tp    rohc_db_mem_get_ip6_key ROHC_NO_PARAMS ();
/* + SPR 17439 */
/* To get a CSC block */
rohc_return_t  rohc_db_get_csc ROHC_PROTO
((
        rohc_cpib_t         * p_cpib    ,
        rohc_entity_t       * p_entity  ,
        rohc_U8bit_t        profile     ,
        rohc_csc_common_t  **p_p_csc   ,
        rohc_error_code_t   *p_ecode
));

/* To get all CSC's this function needs to be called */
rohc_void_t     rohc_db_get_all_csc     ROHC_PROTO
((
    rohc_entity_t   *p_entity       ,
    rohc_db_list_t  **p_list
));

/* To get the number of CSC in a profile */
rohc_U32bit_t   rohc_db_get_no_of_nodes ROHC_PROTO
((
    rohc_entity_t   *p_entity   ,
    rohc_U8bit_t    profile
));

rohc_csc_t *  rohc_db_create_csc_for_ctx_update  ROHC_PROTO
((
    rohc_entity_t       *p_entity       ,
    stream_pkt_info_t   *p_stream_pkt_info  ,
    rohc_error_code_t   *p_ecode
));

rohc_csc_common_t  *   rohc_db_create_csc_new_cid  ROHC_PROTO
((
    rohc_entity_t       *p_entity       ,
    rohc_cpib_t         *p_cpib         ,
    rohc_error_code_t   *p_ecode
));

rohc_void_t *   rohc_db_get_uncomp_csc  ROHC_PROTO
((
    rohc_entity_t       *p_entity       ,
    rohc_error_code_t   *p_ecode
));

rohc_csc_t  *   rohc_db_create_csc_with_cid ROHC_PROTO
((
    rohc_entity_t       *p_entity       ,
    rohc_cpib_t         *p_cpib         ,
    rohc_U32bit_t       context_id
));

rohc_void_t     rohc_db_delete_csc  ROHC_PROTO
((
    rohc_entity_t       *p_entity   ,
    rohc_csc_t          *p_csc
));
/* - SPR 17439 */

/****************Wrappers for De-Compressor Stream Context *******/

rohc_void_t rohc_db_init_decomp_stream  ROHC_PROTO
((rohc_entity_t  *p_entity));
/* + SPR 17439 */
/* To get a DSC block */
rohc_void_t * rohc_db_get_dsc ROHC_PROTO
    ((
        rohc_dpib_t     * p_dpib    ,
        rohc_entity_t   * p_entity
    ));

rohc_void_t * rohc_db_create_new_dsc ROHC_PROTO
((
    rohc_U32bit_t   context_id     ,
    rohc_entity_t   *p_entity
));

rohc_void_t* rohc_db_create_dsc_from_pool    ROHC_PROTO
((
    rohc_entity_t   *p_entity   ,
    rohc_U8bit_t    context_id
));

rohc_void_t  *rohc_db_create_dsc ROHC_PROTO
((
    rohc_entity_t   *p_entity   ,
    rohc_U32bit_t   context_id
));

/* To get all DSC's this function needs to be called until NULL is returned */
rohc_void_t  rohc_db_get_all_dsc   ROHC_PROTO
    ((
        rohc_entity_t   *p_entity   ,
        rohc_db_list_t  **p_list
    ));

rohc_protocol_node_t    *rohc_mem_get_node  ROHC_NO_PARAMS();

rohc_return_t   rohc_init_dsc_from_context_block    ROHC_PROTO
((
    rohc_dsc_t              *p_dsc  ,
    stream_pkt_info_t    *p_stream_pkt_info
));
/* - SPR 17439 */
rohc_void_t rohc_db_p1_create_dsc_sub_blocks    ROHC_PROTO
(( rohc_dsc_common_t  *p_dsc));

rohc_void_t rohc_db_p2_create_dsc_sub_blocks    ROHC_PROTO
(( rohc_dsc_common_t  *p_dsc));

rohc_void_t rohc_db_p6_create_dsc_sub_blocks    ROHC_PROTO
(( rohc_dsc_common_t  *p_dsc));

rohc_void_t rohc_db_reinit_dsc    ROHC_PROTO
((
  rohc_dsc_common_t      *p_dsc
));
/* + SPR 17439 */
/* for fb handling */
rohc_void_t * rohc_db_fb_get_csc_small_cid ROHC_PROTO
((
    rohc_entity_t *p_entity ,
    rohc_U16bit_t context_id
));

rohc_void_t * rohc_db_fb_get_csc_large_cid  ROHC_PROTO
((
    rohc_entity_t *p_entity ,
    rohc_U16bit_t context_id
));
/* - SPR 17439 */
#endif /* _ROHC_DB_EXT_IF_H_ */
