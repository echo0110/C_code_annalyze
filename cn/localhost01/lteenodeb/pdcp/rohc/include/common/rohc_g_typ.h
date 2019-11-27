/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_g_typ.h,v 1.1.1.1.16.1 2010/11/03 06:13:01 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains definition for rohc entity
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_g_typ.h,v $
 * Revision 1.1.1.1.16.1  2010/11/03 06:13:01  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.4.32.3  2010/10/13 06:48:10  gur22059
 * Removed HIGH_PERFORMANCE Flag from entity structure
 *
 * Revision 1.4.32.2  2010/09/28 06:56:35  gur11835
 * Corrected usage of ROHC context block pointer stored at PDCP layer
 *
 * Revision 1.4.32.1  2010/09/13 04:08:31  gur22059
 * Packet context retrieval done at PDCP.
 *
 * Revision 1.4  2009/05/28 04:41:36  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_G_TYPES_H_
#define _ROHC_G_TYPES_H_

#include "lteFramework.h"
#include "ltePdcpStatistic.h"
#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_com_def.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "lteLayer2CommanTypes.h"

typedef rohc_S32bit_t (*p_compare)(const rohc_void_t *, const rohc_void_t *);
typedef const rohc_void_t *(*p_keyof)(const rohc_tree_node_t *);

typedef struct rohc_protocol_node_t
{
    rohc_list_node_t  node;
    rohc_U8bit_t  protocol;
    rohc_U8bit_t  length;
    rohc_void_tp   ptr;
}rohc_protocol_node_t;

typedef struct rohc_statsistics
{
    rohc_U32bit_t   rohc_stats[ROHC_MAX_ENTITY_STATS];
    rohc_U32bit_t   rohc_profile_stats[ROHC_ALL_PROFILE][ROHC_MAX_PROFILE_STATS];
}rohc_statistics_t;



struct  rohc_profile_node_t
{
    rohc_tree_node_t    tnode;
    rohc_void_tp        p_key;
    rohc_void_tp        p_csc;
};

typedef struct rohc_profile_t
{
    rohc_rb_tree_t  profile_seed;
    rohc_S32bit_t (*compare)(const rohc_void_t *, const rohc_void_t *);
    const rohc_void_t *(*keyof)(const rohc_tree_node_t *);
}rohc_profile_t;

typedef struct rohc_dsc_node_t
{
    rohc_tree_node_t    tnode;
    rohc_U32bit_t       context_id;
    rohc_void_tp        *p_dsc;
}rohc_dsc_node_t;

struct rohc_csc_node_t
{
    rohc_tree_node_t    tnode;
    rohc_U32bit_t       context_id;
    rohc_void_tp        *p_csc;
};

typedef struct rohc_fb_info_t
{
    rohc_U16bit_t len;
    rohc_U8bit_t *p_buff;
}rohc_fb_info_t;    

typedef struct rohc_entity_id_t
{
    rohc_U16bit_t ueIndex;
    rohc_U16bit_t lcId;
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    rohc_U16bit_t  qci;
#endif
}rohc_entity_id_t;

typedef struct rohc_entity_t
{
    rohc_statistics_t ro_stats;
    
    /* Feedback to be read by comp */
    rohc_list_t   feedback_recv;
    
    /* feedback packets to be sent to be given to PDCP */
    rohc_list_t   feedback_to_be_sent;

    /* Structures for behavior det */
    rohc_list_t   beh_det;

    /* RB Tree implementation is per profile */
    rohc_profile_t  profile_tree[ROHC_ALL_PROFILE];

    /* DSC data structre is array for small CID and
     * RB tree for large CID */
    union {
        rohc_void_tp    p_dsc[ROHC_MAX_CID+1];
        rohc_rb_tree_t  dsc_tree;
    }un1;

    /* CSC data structre is array for small CID and
     * RB tree for large CID */
    union {
        rohc_void_tp    p_csc[ROHC_MAX_CID+1];
        rohc_rb_tree_t  csc_tree;
    }un_fb;

    rohc_fb_info_t fb_info;
    
    rohc_void_t *uncomp_dsc;
    rohc_void_t *uncomp_csc;
    
    rohc_U32bit_t   k_1;
    rohc_U32bit_t   k_2;
    rohc_U32bit_t   n_1;
    rohc_U32bit_t   n_2;
    rohc_U32bit_t   m_1;
    rohc_U32bit_t   m_2;
    rohc_U32bit_t   n;
  
    /* mode - R-mode, U-mode or O-mode */
    rohc_U8bit_t    mode;
    
    /* system clock resolution in units of milliseconds */
    rohc_U8bit_t    clock_resolution;

    rohc_U8bit_t    jitter;

    rohc_U32bit_t   max_ir_return_time;

    /* Default value of FO return time of FO-Return timer for U-mode */
    /*compressor operations (in ms)*/
    rohc_U32bit_t   max_fo_return_time;
    /* Max number of IR Packets need to be sent before transisting to higher
      state in U mode (optimistic approach)*/

    /* ROHC ENTITY ID */
    rohc_entity_id_t   rohc_entity_id;
    
    /* maximum CID value that can be used. */
    rohc_U16bit_t    max_cid;

    rohc_U8bit_t    max_static_info_update_counter;
    
    /* Max number of IR-DYN, UOR-2*, UO-1* Packets need to be sent before */
    /* transisting to the higher state in U mode (optimistic approach)*/
    rohc_U8bit_t    max_dynamic_info_update_counter;
    
    /* Max number of p0_IR packet sent to update the state*/
    rohc_U8bit_t    max_p0_ir_counter;

    rohc_U8bit_t    p0_ir_counter;

    /* inferred field - If max_cid > 15 then
     *  large_cids = TRUE else FALSE. */
    rohc_U8bit_t    large_cids;

    /* profile supported, can from one of the following
    * Profile A 0x0001 (IP/UDP/RTP)
    * Profile B 0x0002 (IP/UDP)
    * Profile C 0x0003 (IP/ESP)
    * Profile D 0x0000 (Uncompressed) */
    rohc_U8bit_t    profiles;
    rohc_bool_t     dont_create_csc_flag;
    rohc_bool_t     fb_in_process;
    /*dsc for uncomp (P0) profile */
    rohc_U32bit_t   context_id;
    rohc_U16bit_t   direction;
    rohc_U16bit_t   profilesSupported;
   /* mode transition parameters */
    rohc_U32bit_t    modeTransitionUtoO;
    rohc_U32bit_t    modeTransitionUtoR;
    rohc_U32bit_t    modeTransitionRtoO;
    rohc_U32bit_t    modeTransitionRtoU;
    rohc_U32bit_t    modeTransitionOtoU;
    rohc_U32bit_t    modeTransitionOtoR;
    rohc_void_t      *p_csc;

} rohc_entity_t;


typedef struct rohc_global_t
{
    /* Contains ptr to the buffer pool - for allocating
     * the stream context pools */  
    QMPOOL      entity_pool;
    QMPOOL      csc_pool;
    QMPOOL      dsc_pool;
    QMPOOL      node_pool;
    QMPOOL      dsc_node_pool;
    QMPOOL      csc_node_pool;
    QMPOOL      ip4_key_pool;
    QMPOOL      ip6_key_pool;
    QMPOOL      list_node_pool;
    
    rohc_U32bit_t   gen_id;
    rohc_U8bit_t rohc_state;
    /* SPR 9662 changes start */
#ifdef STRUCT_ALIGNMENT
    rohc_U8bit_t padding[3];
#endif
    /* SPR 9662 changes end */
    rohc_cpib_t  cpib;
    rohc_dpib_t  dpib;
#define ROHC_INIT 1
}rohc_global_t;

extern rohc_global_t rohc_global;

/* Structure to be passed from Decompressor CPM/PRM to PBM
to from feedback packet*/
typedef struct rohc_feedback_t
{
    /* encoded CID */
    rohc_U16bit_t   cid;
    rohc_U16bit_t   cid_len;
    rohc_U8bit_t    feedback_type;
#define ROHC_FEEDBACK_TYPE1 (1U)    
#define ROHC_FEEDBACK_TYPE2 (2U)    
    rohc_U32bit_t   sn;
    rohc_U16bit_t   msn;
#define ROHC_MAX_OPT    (10U)   
#define ROHC_INVALID_OPT_TYPE   (0xffU)
    rohc_U8bit_t    opt_type[ROHC_MAX_OPT];
    rohc_U8bit_t    opt_val[ROHC_MAX_OPT];
    rohc_U8bit_t    ack_type;
    rohc_U8bit_t    mode;
}rohc_feedback_t;

typedef struct rohc_encoded_value{
    rohc_U32bit_t   value;
    rohc_U8bit_t    bits;
} rohc_encoded_value_t;


#endif 
