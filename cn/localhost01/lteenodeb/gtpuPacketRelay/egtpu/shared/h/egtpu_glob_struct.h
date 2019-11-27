/***********************************************************************
**  FUNCTION:
**      Definitions for GTP-U global structure.
************************************************************************
**
**  FILE NAME:
**      egtpu_glob_struct.h
**
**  DATE           NAME               REFERENCE        REASON
**  ----           ----               ---------        ------
**  Oct, 2009     Vipul Aggarwal                       Original
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef _EGTPU_GLOB_STRUCT_DEF_
#define _EGTPU_GLOB_STRUCT_DEF_
#include "egtpu_glob.h"

struct load_conf {
    int count;
    int ctxt;
    int tot_count;
    int pkt_size;
    int dir;
    int start_load;
};

typedef struct egtpu_global{
    egtpu_pool_t                 mpool_out;
    egtpu_pool_t                 rab_ctxt_pool;
    egtpu_hash_pool_t            tpool;
    egtpu_hash_pool_t            hash_ctxt_pool;
    egtpu_hash_pool_t            hash_pool;
	egtpu_hash_pool_t			table_pool;
    U8bit                       stack_initdb_done;
    egtpu_initdb_info_t          initdb_info;
    egtpu_ext_hdr_map_t          ext_hdr_map;
    egtpu_timer_info_t           tmr_info[1];
#ifdef EGTPU_ENABLE_STATS
    U32bit                      egtpu_stats_arr[EGTPU_MAX_STATS];
#endif
#ifdef EGTPU_ENABLE_TRACE
    U16bit                      egtpu_trace_arr[EGTPU_MAX_TRACE_LEVELS];
#endif 
/*IE's  extracted from SME api related to qos handling*/
    U8bit ip_traffic;
	/*the index in the max_window_size and reorder_timer_drtn is the IP traffic class*/
    egtpu_window_size max_window_size[5];
    egtpu_timer_drtn reorder_timer_drtn[5];
} egtpu_global_t;


#endif
