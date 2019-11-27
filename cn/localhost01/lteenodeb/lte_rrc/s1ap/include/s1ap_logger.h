/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 *  File Details
 *  ------------
 *  $Id: s1ap_logger.h,v 1.3 2010/11/16 13:18:55 gur21239 Exp $
 ****************************************************************************
 *
 *  File Description : 
 *  This is the file which contains logging and tracing definitions 
 *  for S1AP
 *
 ****************************************************************************
 *
 *  Revision Details
 *  ----------------
 *
 *  $Log: s1ap_logger.h,v $
 *  Revision 1.3  2010/11/16 13:18:55  gur21239
 *  spr 667 related changes done
 *
 *  Revision 1.2  2010/01/04 16:10:05  ukr15916
 *  no message
 *
 *  Revision 1.1.2.2  2009/12/28 05:09:42  gur18569
 *  fixed indentation
 *
 *  Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 *  Shifted to level of rrc dir
 *
 *  Revision 1.1.2.1  2009/10/23 16:11:39  gur18569
 *  Initial version
 *
 *
 *
 ****************************************************************************/
#ifndef _S1AP_LOGGER_H_
#define _S1AP_LOGGER_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "rrc_common_utils.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "s1ap_global_ctx.h"


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
extern const S8 *rrc_s1ap_log_facility_name;

extern U8 get_s1ap_log_level_from_oam_log_level
(
    U8 oam_log_level
);

extern void s1ap_change_logging_parameters
(
    s1ap_gb_context_t    *p_s1ap_gb_ctx,
    U8                    log_level,
    rrc_bool_et           log_on_off
);


#if (LOGGER_TYPE == RRC_ADV_LOGGER)

#define S1AP_FATAL        LogFatal
#define S1AP_ERROR        LogError
#define S1AP_WARNING      LogWarning
#define S1AP_INFO         LogInfo
#define S1AP_BRIEF        LogBrief
#define S1AP_DETAILED     LogDetailed
#define S1AP_ASN          S1AP_DETAILED
#define S1AP_DETAILEDALL  LogTrace
#elif (LOGGER_TYPE == RRC_INTERNAL_LOGGER)

#define S1AP_FATAL       QVLOG_CRITICAL
#define S1AP_ERROR       QVLOG_MAJOR
#define S1AP_WARNING     QVLOG_MINOR
#define S1AP_INFO        QVLOG_INFO
#define S1AP_BRIEF       QVLOG_DETAIL
#define S1AP_DETAILED    QVLOG_DUMP
#define S1AP_INT_NONE    QVLOG_NONE
#define S1AP_ASN         S1AP_DETAILED
#define S1AP_DETAILEDALL QVLOG_DUMP


#endif

#define RRC_S1AP_TRACE(log_level, format, ...) \
    S1AP_LOG(rrc_s1ap_log_facility_name, log_level, format, ##__VA_ARGS__)

#ifdef RRC_DEBUG
#define RRC_S1AP_UT_TRACE_ENTER()    RRC_S1AP_TRACE(RRC_DETAILED,\
        "Entering function: %s, in file: %s \n",__FUNCTION__,__FILE__)

#define RRC_S1AP_UT_TRACE_EXIT()     RRC_S1AP_TRACE(RRC_DETAILED,\
        "Exiting function: %s, in file: %s \n",__FUNCTION__,__FILE__)
#else
#define RRC_S1AP_UT_TRACE_ENTER()
#define RRC_S1AP_UT_TRACE_EXIT()

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
#endif /* _S1AP_LOGGER_H */

