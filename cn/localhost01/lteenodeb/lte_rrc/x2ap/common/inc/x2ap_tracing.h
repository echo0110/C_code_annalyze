/*******************************************************************************
*
*  FILE NAME   : x2ap_tracing.h
*
*  DESCRIPTION : This file contains the structure and function declaration
*                required for x2ap Logging and tracing.
*
*  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _X2AP_TRACE_H_
#define _X2AP_TRACE_H_

#include "commoninc.h"
#include "rrc_common_utils.h"
#include "x2ap_db.h"
#include "x2ap_types.h"

/*
*   logging
*/

void x2ap_set_loglevel(U8 new_log_level);
void x2ap_set_module_loglevel(U8 new_log_level);

extern const S8 *x2ap_log_facility_name;

U8 get_x2ap_log_level_from_oam_log_level
(
    U8 oam_log_level
);

void x2ap_change_logging_parameters
(
    x2ap_gb_context_t       *p_x2ap_gb_ctx,
    U8                      log_level,
    x2ap_bool_et            log_on_off
);


#if (LOGGER_TYPE == RRC_ADV_LOGGER)

#define X2AP_FATAL        LogFatal
#define X2AP_ERROR        LogError
#define X2AP_WARNING      LogWarning
#define X2AP_INFO         LogInfo
#define X2AP_BRIEF        LogBrief
#define X2AP_DETAILED     LogDetailed
#define X2AP_ASN          X2AP_DETAILED
/* SPR 21251 Start */
#define X2AP_DETAILEDALL  LogDebug
/* SPR 21251 End */

#elif (LOGGER_TYPE == RRC_INTERNAL_LOGGER)

#define X2AP_FATAL       QVLOG_CRITICAL
#define X2AP_ERROR       QVLOG_MAJOR
#define X2AP_WARNING     QVLOG_MINOR
#define X2AP_INFO        QVLOG_INFO
#define X2AP_BRIEF       QVLOG_DETAIL
#define X2AP_DETAILED    QVLOG_DUMP
#define X2AP_INT_NONE    QVLOG_NONE
#define X2AP_ASN         X2AP_DETAILED
#define X2AP_DETAILEDALL QVLOG_DUMP


#endif

#define X2AP_TRACE(log_level, format, ...) \
    X2AP_LOG(x2ap_log_facility_name, log_level, format, ##__VA_ARGS__)

/*
*   UT Trace
*/
#ifdef X2AP_DEBUG
    #define X2AP_UT_TRACE_ENTER() \
        X2AP_TRACE(X2AP_DETAILED, "Entering function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)

    #define X2AP_UT_TRACE_EXIT() \
        X2AP_TRACE(X2AP_DETAILED, "Exiting function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)
#else
//#ifdef RRC_UNIT_TEST_FRAMEWORK
#if 0
    #define X2AP_UT_TRACE_ENTER() \
        X2AP_TRACE(X2AP_DETAILED, "Entering function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)

    #define X2AP_UT_TRACE_EXIT() \
        X2AP_TRACE(X2AP_DETAILED, "Exiting function: %s, in file: %s \n", \
        __FUNCTION__, __FILE__)
#endif        
//#else
    #define X2AP_UT_TRACE_ENTER()
    #define X2AP_UT_TRACE_EXIT()
//#endif
#endif

#endif 
