/******************************************************************************
*
*   FILE NAME:
*       rrc_llim_logger.h
*
*   DESCRIPTION:
*       This is the file which contains logging and tracing definitions
*
*   DATE            AUTHOR      REFERENCE       REASON
*   23 Apr 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _RRC_LLIM_LOGGER_H_
#define _RRC_LLIM_LOGGER_H_

#include "rrc_common_utils.h"

extern const S8 *rrc_llim_log_facility_name;

#define RRC_LLIM_TRACE(log_level, format, ...) \
    RRC_LOG(rrc_llim_log_facility_name, log_level, format, ##__VA_ARGS__)

#ifdef RRC_DEBUG
    #define RRC_LLIM_UT_TRACE_ENTER() \
        RRC_LLIM_TRACE(RRC_DETAILED, "Enterning function: %s, in file: %s \n",\
        __FUNCTION__, __FILE__)

#define RRC_LLIM_UT_TRACE_EXIT() \
        RRC_LLIM_TRACE(RRC_DETAILED, "Exiting function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)
#else
    #define RRC_LLIM_UT_TRACE_ENTER()
    #define RRC_LLIM_UT_TRACE_EXIT()
#endif /* RRC_DEBUG */

#ifdef RRC_DEBUG
    #define RRC_LLIM_FSM_INIT_TRACE(fsm_name, state_name) \
        RRC_LLIM_TRACE(RRC_BRIEF, "[%s] initialised, initial state [%s]", \
                fsm_name, state_name)

    #define RRC_LLIM_UE_FSM_INIT_TRACE(ue_index, fsm_name, state_name) \
        RRC_LLIM_TRACE(RRC_BRIEF, \
            "[UE:%u] [%s] initialised, initial state [%s]", \
            ue_index, fsm_name, state_name)

    #define RRC_LLIM_FSM_SET_STATE_TRACE(fsm_name, state_name) \
        RRC_LLIM_TRACE(RRC_BRIEF, "[%s] state changed to [%s]", \
                fsm_name, state_name)

    #define RRC_LLIM_UE_FSM_SET_STATE_TRACE(ue_index, fsm_name, state_name) \
        RRC_LLIM_TRACE(RRC_BRIEF, "[UE:%u] [%s] state changed to [%s]", \
                ue_index, fsm_name, state_name)

    #define RRC_LLIM_FSM_EVENT_TRACE(fsm_name, event_name, state_name) \
        RRC_LLIM_TRACE(RRC_BRIEF, "[%s] event [%s] received in state [%s]", \
                fsm_name, event_name, state_name)

    #define RRC_LLIM_UE_FSM_EVENT_TRACE(ue_index, fsm_name, \
        event_name, state_name) \
            RRC_LLIM_TRACE(RRC_BRIEF, \
                "[UE:%u] [%s] event [%s] received in state [%s]", \
                ue_index, fsm_name, event_name, state_name)
#else
    #define RRC_LLIM_FSM_INIT_TRACE(fsm_name, state_name)

    #define RRC_LLIM_UE_FSM_INIT_TRACE(ue_index, fsm_name, state_name)

    #define RRC_LLIM_FSM_SET_STATE_TRACE(fsm_name, state_name)

    #define RRC_LLIM_UE_FSM_SET_STATE_TRACE(ue_index, fsm_name, state_name)

    #define RRC_LLIM_FSM_EVENT_TRACE(fsm_name, event_name, state_name)

    #define RRC_LLIM_UE_FSM_EVENT_TRACE(ue_index, fsm_name, event_name, \
        state_name)
#endif /* RRC_DEBUG */

#endif /* _RRC_LLIM_LOGGER_H_ */

