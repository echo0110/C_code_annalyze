/******************************************************************************
*
*   FILE NAME:
*       csc_timers.h
*
*   DESCRIPTION:
*       This file contains RRC CSC timers definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_TIMERS_H_
#define _CSC_TIMERS_H_

#include "rrc_defines.h"

typedef enum
{
    CSC_SUBMODULE_CELL_M,
    CSC_SUBMODULE_SFN_M,
    CSC_SUBMODULE_OAMH
} csc_submodules_et;

#pragma pack(push, 1)

/* Extandable structure. Basic IEs. */
typedef struct _csc_timer_buf_t
{
    rrc_bool_et         auto_delete;
    csc_submodules_et   target_submodule;
} csc_timer_buf_t;

#pragma pack(pop)

#endif /* _RRC_LLIM_TIMERS_H_ */

