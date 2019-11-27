/******************************************************************************
*
*   FILE NAME:
*       csc_cell_m_timers.h
*
*   DESCRIPTION:
*       This file contains CSC Cell manager timers definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_CELL_M_TIMERS_H_
#define _CSC_CELL_M_TIMERS_H_

#include "csc_timers.h"
#include "csc_cell_m_fsm.h"

#pragma pack(push, 1)

/* Extandable structure. Basic IEs as in csc_timer_buf_t + Cell manager IEs. */
typedef struct _csc_cell_m_timer_buf_t
{
    rrc_bool_et         auto_delete;
    csc_submodules_et   target_submodule;
    rrc_cell_index_t    cell_index;
    cell_m_fsm_event_et cell_m_event;
} csc_cell_m_timer_buf_t;

#pragma pack(pop)

#endif /* _CSC_CELL_M_TIMERS_H_ */

