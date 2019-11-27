/******************************************************************************
*
*   FILE NAME:
*       rrc_cemh_timers.h
*
*   DESCRIPTION:
*       This file contains RRC LLIM timers definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   15 May 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _RRC_CEMH_TIMERS_H_
#define _RRC_CEMH_TIMERS_H_

#include "rrc_llim_timers.h"
#include "rrc_cemh_fsm.h"

#pragma pack(push, 1)

/* Extandable structure. Basic IEs as in rrc_llim_timer_buf_t + CEMH IEs. */
typedef struct _rrc_cemh_timer_buf_t
{
    rrc_bool_t              auto_delete;
    rrc_llim_submodules_et  target_submodule;
    rrc_cell_index_t        cell_index;
    cemh_fsm_event_et       cemh_event;
} rrc_cemh_timer_buf_t;

#pragma pack(pop)

#endif /* _RRC_CEMH_TIMERS_H_ */
