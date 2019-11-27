/******************************************************************************
*
*   FILE NAME:
*       rrc_shared_stats.h
*
*   DESCRIPTION:
*       This file contains shared memory related definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   4 Sep 2012                  ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_SHARED_STATS_H_
#define _RRC_SHARED_STATS_H_

#include "rrc_defines.h"
#include "rrc_oam_intf.h"
#include "rrc_rrm_intf.h"

/* Macros for mapping stats shared memory */
#define RRC_STATS_SHM_KEY   35543
#define RRC_STATS_SHM_CREATE_FLAGS (IPC_CREAT | 0666)
#define RRC_STATS_SHM_LOCATE_FLAGS (0666)

/* Structures for storing Stat shared memory info */
typedef struct _rrc_uecc_cell_stats_t
{
    rrc_bool_et                                  is_cell_configured;

    rrc_oam_connection_related_stats_t           rrc_connection_related_stats;

    rrc_oam_erab_related_stats_t                 rrc_erab_related_stats;
    
    rrc_oam_handover_related_stats_t             rrc_handover_related_stats;
    
    rrc_uecc_oam_stat_ue_assoc_s1_connection_t   rrc_ue_assoc_s1_conn_related_stats;
    
    U32                                          rrc_IratIncMobility_LAI;

}rrc_uecc_cell_stats_t;

typedef struct _rrc_uecc_instance_stats_t
{
    rrc_uecc_cell_stats_t      rrc_uecc_cell_stats[MAX_NUM_CELLS];

}rrc_uecc_instance_stats_t;

typedef struct _rrc_uecc_stats_t
{
    U8  count;

    rrc_uecc_instance_stats_t  rrc_uecc_instance_stats[MAX_UECC_INSTANCES];

}rrc_uecc_stats_t;

typedef struct _rrc_csc_cell_stats_t
{
    rrc_bool_et                 is_cell_configured;

    rrc_csc_oam_stat_paging_t   rrc_paging_related_stats;

}rrc_csc_cell_stats_t;

typedef struct _rrc_csc_stats_t
{
    rrc_csc_cell_stats_t   rrc_csc_cell_stats[MAX_NUM_CELLS];

}rrc_csc_stats_t;

typedef struct _rrc_stats_t
{
    rrc_uecc_stats_t   rrc_uecc_stats;

    rrc_csc_stats_t    rrc_csc_stats;

}rrc_stats_t;

typedef struct _rrc_stats_info_t
{
    S32             rrc_shm_id;
    rrc_stats_t     *p_rrc_stats;

} rrc_stats_info_t;

/* RRC stats handle */
extern rrc_stats_info_t g_rrc_stats;

/* GDB fix - duplicate symbols at link time */
/* rrc_stats_info_t g_rrc_stats; */

rrc_return_et init_shared_mem_stats( void );

#endif /* _RRC_SHARED_STATS_H_ */
