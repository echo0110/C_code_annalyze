/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_oam_platform_intf.h $
 *
 *******************************************************************************
 *
 * File Description: RRM OAM Platform interface header file RRM OAM platform
 *                   APIs function declaration
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _RRM_PLATFORM_OAM_INTF_H
#define _RRM_PLATFORM_OAM_INTF_H

#include "rrm_api_types.h"
#include "rrm_api_defines.h"

#define RRM_MAX_SOURCE 3
#define RRM_PERIODIC    0
#define RRM_EVENT       1

typedef enum
{
    SOURCE_TNL,
    SOURCE_MEMORY,
    SOURCE_CPU
}rrm_oam_load_src_et;

typedef struct _rrm_oam_load_status_t
{
    rrm_bitmask_t bitmask; /*^ BITMASK ^*/
    U32 load_src;  /*^ M, 0, H, 0, 2 ^*/
    U8 load_level;  /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_load_status_t;

typedef struct _rrm_plat_cell_load_t
{
    rrm_oam_eutran_global_cell_id_t global_cell_id;/*^ M, 0, N, 0, 0 ^*/
    U8 ind_type; /*^ M, 0, H, 0, 1 ^*/
/*SPR_17664_start*/
    U8  count; /*^ M, 0, B, 1, RRM_OAM_MAX_RESOURCE ^*/
/*SPR_17664_end*/
    rrm_oam_load_status_t load_source[RRM_OAM_MAX_RESOURCE];/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_plat_cell_load_t;

typedef struct _platform_rrm_load_ind_t
{
    rrm_bitmask_t bitmask; /*^ BITMASK ^*/
/*SPR_17664_start*/
    U8 num_of_cell; /*^ M, 0, B, 1, RRM_MAX_NUM_CELLS ^*/
/*SPR_17664_end*/
    rrm_plat_cell_load_t cell_load[RRM_MAX_NUM_CELLS]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}platform_rrm_load_ind_t;/*^ API, RRM_PLATFORM_LOAD_IND ^*/

#endif

