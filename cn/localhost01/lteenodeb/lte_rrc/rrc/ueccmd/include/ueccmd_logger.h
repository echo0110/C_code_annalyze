/******************************************************************************
*
*   FILE NAME:
*       ueccmd_logger.h
*
*   DESCRIPTION:
*       This is the file which contains logging and tracing definitions for 
*       UECC MD.
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _UECCMD_LOGGER_H_
#define _UECCMD_LOGGER_H_

#include "rrc_common_utils.h"

extern const S8 *rrc_ueccmd_log_facility_name;

#define RRC_UECCMD_TRACE(log_level, format, ...) \
    RRC_LOG(rrc_ueccmd_log_facility_name, log_level, format, ##__VA_ARGS__)

#ifdef RRC_DEBUG
    #define RRC_UECCMD_UT_TRACE_ENTER() \
        RRC_UECCMD_TRACE(RRC_DETAILED, "Enterning function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)

    #define RRC_UECCMD_UT_TRACE_EXIT() \
        RRC_UECCMD_TRACE(RRC_DETAILED, "Exiting function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)
#else
    #define RRC_UECCMD_UT_TRACE_ENTER()
    #define RRC_UECCMD_UT_TRACE_EXIT()
#endif /* RRC_DEBUG */

#endif /* _UECCMD_LOGGER_H_ */
