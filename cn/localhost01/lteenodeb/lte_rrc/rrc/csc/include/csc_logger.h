/******************************************************************************
*
*   FILE NAME:
*       csc_logger.h
*
*   DESCRIPTION:
*       This is the file which contains logging and tracing definitions for CSC
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_LOGGER_H_
#define _CSC_LOGGER_H_

#include "rrc_common_utils.h"

extern const S8 *rrc_csc_log_facility_name;

#define RRC_CSC_TRACE(log_level, format, ...) \
    RRC_LOG(rrc_csc_log_facility_name, log_level, format, ##__VA_ARGS__)

/* SPR 13700 Fix start */
#define RRC_CSC_TRACE_ASN(log_level, format, ...) \
    RRC_LOG_ASN(rrc_csc_log_facility_name, log_level, format, ##__VA_ARGS__)
/* SPR 13700 Fix stop */

#ifdef RRC_DEBUG
    #define RRC_CSC_UT_TRACE_ENTER() \
        RRC_CSC_TRACE(RRC_DETAILED, "Enterning function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)

#define RRC_CSC_UT_TRACE_EXIT() \
        RRC_CSC_TRACE(RRC_DETAILED, "Exiting function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)
#else
    #define RRC_CSC_UT_TRACE_ENTER()
    #define RRC_CSC_UT_TRACE_EXIT()
#endif /* RRC_DEBUG */

#ifdef RRC_DEBUG
    #define RRC_CSC_FSM_INIT_TRACE(fsm_name, state_name) \
        RRC_CSC_TRACE(RRC_BRIEF, "[%s] initialised, initial state [%s]", \
                fsm_name, state_name)

    #define RRC_CSC_FSM_SET_STATE_TRACE(fsm_name, state_name) \
        RRC_CSC_TRACE(RRC_BRIEF, "[%s] state changed to [%s]", \
                fsm_name, state_name)

    #define RRC_CSC_FSM_EVENT_TRACE(fsm_name, event_name, state_name) \
        RRC_CSC_TRACE(RRC_BRIEF, "[%s] event [%s] received in state [%s]", \
                fsm_name, event_name, state_name)
#else
    #define RRC_CSC_FSM_INIT_TRACE(fsm_name, state_name)

    #define RRC_CSC_FSM_SET_STATE_TRACE(fsm_name, state_name)

    #define RRC_CSC_FSM_EVENT_TRACE(fsm_name, event_name, state_name)
#endif /* RRC_DEBUG */

#endif /* _CSC_LOGGER_H_ */

