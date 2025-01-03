/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: oamh_logger.h,v 1.3 2010/03/24 09:50:42 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description :
 *      This file contains OAMH logger definitions
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: oamh_logger.h,v $
 * Revision 1.3  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2.2.1  2010/03/05 10:33:52  gur21006
 * int changed to S32 and char changed to S8
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.4.2  2009/12/09 12:15:41  ukr15971
 * warnings fixed
 *
 * Revision 1.1.4.1  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.10  2009/11/19 15:00:38  ukr16018
 * W80.
 *
 * Revision 1.1.2.9  2009/11/19 09:52:22  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.8  2009/11/18 14:31:04  ukr18877
 * Tabs in LLIM, CSC and OAMH fixed
 *
 * Revision 1.1.2.7  2009/11/18 09:40:54  ukr16018
 * Apply code review comments.
 *
 * Revision 1.1.2.6  2009/11/16 11:35:31  ukr15971
 * OAMH: inspection log fixes
 *
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_OAMH_LOGGER_H
#define INCLUDED_OAMH_LOGGER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "rrc_common_utils.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
extern const S8 *rrc_oamh_log_facility_name;

#define RRC_OAMH_TRACE(log_level, format, ...) \
    RRC_LOG(rrc_oamh_log_facility_name, log_level, format, ##__VA_ARGS__)

#ifdef RRC_DEBUG
    #define RRC_OAMH_UT_TRACE_ENTER() \
        RRC_OAMH_TRACE(RRC_DETAILED, "Enterning function: %s, in file: %s \n",\
        __FUNCTION__, __FILE__)

#define RRC_OAMH_UT_TRACE_EXIT() \
        RRC_OAMH_TRACE(RRC_DETAILED, "Exiting function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)
#else
    #define RRC_OAMH_UT_TRACE_ENTER()
    #define RRC_OAMH_UT_TRACE_EXIT()
#endif /* RRC_DEBUG */

#ifdef RRC_DEBUG
    #define RRC_OAMH_FSM_INIT_TRACE(fsm_name, state_name) \
        RRC_OAMH_TRACE(RRC_BRIEF, "[%s] initialised, initial state [%s]", \
                fsm_name, state_name)

    #define RRC_OAMH_FSM_SET_STATE_TRACE(fsm_name, state_name) \
        RRC_OAMH_TRACE(RRC_BRIEF, "[%s] state changed to [%s]", \
                fsm_name, state_name)

    #define RRC_OAMH_FSM_EVENT_TRACE(fsm_name, event_name, state_name) \
        RRC_OAMH_TRACE(RRC_BRIEF, "[%s] event [%s] received in state [%s]", \
                fsm_name, event_name, state_name)
#else
    #define RRC_OAMH_FSM_INIT_TRACE(fsm_name, state_name)

    #define RRC_OAMH_FSM_SET_STATE_TRACE(fsm_name, state_name)

    #define RRC_OAMH_FSM_EVENT_TRACE(fsm_name, event_name, state_name)
#endif /* RRC_DEBUG */

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
U8 oamh_get_rrc_log_level_from_oam_log_level
(
    U8 oam_log_level
);

void oamh_change_logging_parameters
(
    rrc_oamh_gb_context_t   *p_oamh_gb_context,
    U8                      log_level,
    rrc_bool_et             log_on_off
);
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */

#endif /* INCLUDED_OAMH_LOGGER_H */

