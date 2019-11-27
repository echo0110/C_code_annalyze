/******************************************************************************
*
*   FILE NAME:
*       csc_sfn_m_timers.h
*
*   DESCRIPTION:
*       This file contains CSC SFN manager timers definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   05 June 2009    VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_SFN_M_TIMERS_H_
#define _CSC_SFN_M_TIMERS_H_

#include "csc_timers.h"

#pragma pack(push, 1)

/* Extandable structure. Basic IEs as in csc_timer_buf_t + SFN manager IEs. */
typedef struct _csc_sfn_m_timer_buf_t
{
    rrc_bool_et         auto_delete;
    csc_submodules_et   target_submodule;
    rrc_cell_index_t    cell_index;
} csc_sfn_m_timer_buf_t;

#pragma pack(pop)

#endif /* _CSC_SFN_M_TIMERS_H_ */

