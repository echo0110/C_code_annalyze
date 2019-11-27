/******************************************************************************
*
*   FILE NAME:
*       ueccmd_timers.h
*
*   DESCRIPTION:
*       This file contains RRC UECC MD timers definitions.
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _UECCMD_TIMERS_H_
#define _UECCMD_TIMERS_H_

#include "rrc_defines.h"

#pragma pack(push, 1)

/* Extandable structure. Basic IEs. */
typedef struct _ueccmd_timer_buf_t
{
    rrc_bool_et         auto_delete;
} ueccmd_timer_buf_t;

#pragma pack(pop)

#endif /* _RRC_UECCMD_TIMERS_H_ */

