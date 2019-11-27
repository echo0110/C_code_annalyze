/******************************************************************************
 * *
 * *   FILE NAME:
 * *       oamh_timers.h
 * *
 * *   DESCRIPTION:
 * *       This file contains RRC OAMH timers definitions.
 * *
 * *   DATE            AUTHOR      REFERENCE       REASON
 * *   01 June 2010    Puneet      ---------       Initial
 * *
 * *   Copyright (c) 2010, Aricent Inc. All Rights Reserved
 * *
 * ******************************************************************************/
#ifndef _OAMH_TIMERS_H_
#define _OAMH_TIMERS_H_

#include "rrc_defines.h"

typedef enum
{
        OAMH_SUBMODULE
} oamh_submodules_et;

typedef enum
{
    OAMH_LICENSE_TIMER,
    OAMH_RRC_CONNECTION_SAMPLING_TIMER,
    OAMH_ERAB_SAMPLING_TIMER,
    OAMH_HANDOVER_SAMPLING_TIMER,
    OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER,
    OAMH_PAGING_SAMPLING_TIMER,
    OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER,
    OAMH_HEALTH_MONITOR_TIMER,
    /*valgrind_fix start*/
    OAMH_INIT_TIMER,
    /*valgrind_fix stop*/
    OAMH_TIMER_LAST
} oamh_timer_id_et;

typedef struct _oamh_timer_data_t
{
    rrc_timer_t    timers_ids[OAMH_TIMER_LAST];

}oamh_timer_data_t;

#pragma pack(push, 1)

/* Extandable structure. Basic IEs. */
typedef struct _oamh_timer_buf_t
{
        rrc_bool_et         auto_delete;
        oamh_submodules_et  target_submodule;
        oamh_timer_id_et    timer_type;
} oamh_timer_buf_t;

#pragma pack(pop)

#endif /* _RRC_OAMH_TIMERS_H_ */
