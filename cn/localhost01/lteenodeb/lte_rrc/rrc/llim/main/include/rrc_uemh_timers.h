/******************************************************************************
*
*   FILE NAME:
*       rrc_uemh_timers.h
*
*   DESCRIPTION:
*       This file contains UEMH LLIM timers definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   14 May 2009     SergiyK     ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _RRC_UEMH_TIMERS_H_
#define _RRC_UEMH_TIMERS_H_

#include "rrc_llim_timers.h"
#include "rrc_uemh_fsm.h"

#pragma pack(push, 1)

/* Extandable structure. Basic IEs as in rrc_llim_timer_buf_t + UEMH IEs. */
typedef struct _rrc_uemh_timer_buf_t
{
    rrc_bool_t              auto_delete;
    rrc_llim_submodules_et  target_submodule;
    rrc_ue_index_t          ue_index;
    uemh_fsm_event_et       uemh_event;
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
} rrc_uemh_timer_buf_t;

#define SUSPEND_UE_ENTITY_TIMER  30
#pragma pack(pop)

#endif /* _RRC_UEMH_TIMERS_H_ */
