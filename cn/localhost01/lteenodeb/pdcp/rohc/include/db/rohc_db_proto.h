/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_db_proto.h,v 1.3.32.4 2010/11/09 04:48:56 gur22059 Exp $
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
 * $Log: rohc_db_proto.h,v $
 * Revision 1.3.32.4  2010/11/09 04:48:56  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.32.3  2010/10/13 04:24:35  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.32.2  2010/09/23 06:59:40  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.3.32.1  2010/07/22 10:18:09  gur22059
 * Prototype of rohc_db_init_dsc is changed
 *
 * Revision 1.3  2009/05/28 05:02:21  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/


#ifndef _ROHC_DB_PROTO_H_
#define _ROHC_DB_PROTO_H_

#include "rohc_db_typ.h"
#include "rohc_main_typ.h"

rohc_void_t rohc_db_init_entity ROHC_PROTO (( rohc_entity_t *p_entity));

rohc_void_t rohc_db_create_tree     ROHC_PROTO ((rohc_entity_t *p_entity));

rohc_void_t rohc_db_free_csc    ROHC_PROTO ((rohc_csc_common_t *p_csc));

rohc_void_t rohc_db_free_dsc    ROHC_PROTO ((rohc_dsc_common_t *p_dsc));

/* + SPR 17439 */
/* To initialize a DSC block explicitly */
rohc_void_t rohc_db_init_dsc    ROHC_PROTO
((
    rohc_dsc_common_t *p_dsc    ,
    rohc_U32bit_t     context_id
));

rohc_S32bit_t   rohc_compare_profile_1  ROHC_PROTO
((
    rohc_void_tp    key1    ,
    rohc_void_tp    key2
));

rohc_S32bit_t   rohc_compare_profile_2  ROHC_PROTO
((
    rohc_void_tp    key1    ,
    rohc_void_tp    key2
));

rohc_S32bit_t   rohc_compare_profile_3  ROHC_PROTO
((
    rohc_void_tp    key1    ,
    rohc_void_tp    key2
));


rohc_S32bit_t   rohc_compare_profile_6  ROHC_PROTO
((
    rohc_void_tp    key1    ,
    rohc_void_tp    key2
));

rohc_void_tp    rohc_keyof_profile  ROHC_PROTO
(( rohc_tree_node_t *p_node ));

rohc_S32bit_t   rohc_db_compare_ip4 ROHC_PROTO
((
    rohc_static_def_ip4_t   key1    ,
    rohc_static_def_ip4_t   key2
));

rohc_S32bit_t   rohc_db_compare_ip6 ROHC_PROTO
((
    rohc_static_def_ip6_t   key1    ,
    rohc_static_def_ip6_t   key2
));

rohc_void_t csc_walker  ROHC_PROTO
((
    rohc_tree_node_t    *p_tnode    ,
    rohc_void_t         *p_walker_param
));

rohc_U32bit_t   rohc_db_generate_context_id ROHC_PROTO
((  rohc_entity_t   *p_entity ));

rohc_void_t rohc_db_add_dsc ROHC_PROTO
((  rohc_entity_t       *p_entity   ,
    rohc_tree_node_t    *p_tnode
));

rohc_void_t     rohc_db_delete_all_csc  ROHC_PROTO
((  rohc_entity_t   *p_entity ));

rohc_void_t     release_csc ROHC_PROTO
((
    rohc_tree_node_t    *p_tnode    ,
    rohc_void_t         *p_walker_param
));

//rohc_void_t rohc_db_free_csc    ROHC_PROTO
//((  rohc_csc_t  *p_csc  ));

rohc_void_t dsc_walker  ROHC_PROTO
((
    rohc_tree_node_t    *p_tnode    ,
    rohc_void_t         *p_walker_param
));

rohc_S32bit_t   dsc_compare     ROHC_PROTO
((
    rohc_U32bit_t   *p_key1    ,
    rohc_U32bit_t   *p_key2
));

rohc_void_tp    dsc_keyof   ROHC_PROTO
(( rohc_tree_node_t  *p_node));

rohc_void_t     rohc_db_delete_all_dsc  ROHC_PROTO
((  rohc_entity_t   *p_entity));

rohc_void_t release_dsc ROHC_PROTO
((
    rohc_tree_node_t    *p_tnode    ,
    rohc_void_t         *p_walker_param
));

//rohc_void_t     rohc_db_free_dsc    ROHC_PROTO
//((  rohc_dsc_t  *p_dsc ));

rohc_csc_common_t * rohc_db_create_csc ROHC_PROTO
((
    rohc_void_tp    p_key   ,
    rohc_entity_t   *p_entity   ,
    rohc_U8bit_t    profile
));

rohc_return_t   rohc_create_key_for_context_update  ROHC_PROTO
((
    stream_pkt_info_t    *p_stream_pkt_info  ,
    rohc_void_t             **p_p_key
));

/* new for feedback handling */

rohc_void_t rohc_db_fb_init  ROHC_PROTO
((
  rohc_entity_t  *p_entity
));

rohc_void_t rohc_db_fb_add_csc ROHC_PROTO
((
    rohc_entity_t       *p_entity   ,
    rohc_tree_node_t    *p_tnode
));

rohc_void_t  rohc_db_fb_add_csc_node ROHC_PROTO
((
    rohc_entity_t     *p_entity   ,
    rohc_U32bit_t     context_id ,
    rohc_void_t       *p_csc
));

rohc_void_t fb_release_csc_node  ROHC_PROTO
(( 
    rohc_tree_node_t    *p_tnode    ,
    rohc_void_t         *p_walker_param
));

rohc_void_t fb_csc_walker  ROHC_PROTO
(( 
    rohc_tree_node_t    *p_tnode    ,
    rohc_void_t         *p_walker_param
));

rohc_void_t rohc_db_fb_delete_all_csc_node  ROHC_PROTO
((
    rohc_entity_t   *p_entity
)); 

rohc_S32bit_t   fb_csc_compare     ROHC_PROTO
(( 
    rohc_U32bit_t   *p_key1 ,
    rohc_U32bit_t   *p_key2
));

rohc_void_tp    fb_csc_keyof   ROHC_PROTO
(( rohc_tree_node_t  *p_node));

rohc_void_t     rohc_db_fb_delete_csc_node  ROHC_PROTO
 ((rohc_entity_t   *p_entity ,
 rohc_csc_t     *p_csc
 ));

/* - SPR 17439 */
#endif /* _ROHC_DB_PROTO_H_ */
