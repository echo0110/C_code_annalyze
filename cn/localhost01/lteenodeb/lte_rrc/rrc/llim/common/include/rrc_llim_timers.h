/******************************************************************************
*
*   FILE NAME:
*       rrc_llim_timers.h
*
*   DESCRIPTION:
*       This file contains RRC LLIM timers definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 Apr 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _RRC_LLIM_TIMERS_H_
#define _RRC_LLIM_TIMERS_H_

#include "rrc_defines.h"

typedef enum
{
    LLIM_SUBMODULE_CEMH,
    LLIM_SUBMODULE_UEMH,
    LLIM_SUBMODULE_OAMH
} rrc_llim_submodules_et;

#pragma pack(push, 1)

/* Extandable structure. Basic IEs. */
typedef struct _rrc_llim_timer_buf_t
{
    rrc_bool_t              auto_delete;
    rrc_llim_submodules_et  target_submodule;
} rrc_llim_timer_buf_t;

#pragma pack(pop)

#endif /* _RRC_LLIM_TIMERS_H_ */
