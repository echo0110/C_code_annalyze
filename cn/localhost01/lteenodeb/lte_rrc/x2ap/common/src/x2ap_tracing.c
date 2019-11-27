/*******************************************************************************
*
*  FILE NAME   : x2ap_tracing.c 
*
*  DESCRIPTION : This file contains the definitions of function used for logging
*                and tracing.
*
*  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/

#include "x2ap_tracing.h"

#include "cspl.h"
#include "stacklayer.h"


/*
*   Global variables
*/

/* Used in logger for identifying RRC in general
 *  (just for avoid string duplication) */
const S8* x2ap_log_facility_name = (const S8*)"eNodeB X2AP";


#define FMT_MAX_LEN 255

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
/******************************************************************************
*   FUNCTION NAME: get_x2ap_log_level_from_oam_log_level
*   INPUTS       : U8 oam_log_level
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function returns X2AP log level identifier from OAM log level
*       identifier
*
*   RETURNS:
*       X2AP log level identifier
******************************************************************************/
U8 get_x2ap_log_level_from_oam_log_level
(
    U8 oam_log_level /* OAM log level identifier */
)
{
    U8 x2ap_log_level = X2AP_BRIEF;

    switch (oam_log_level)
    {
        case OAM_LOG_LEVEL_FATAL:
            x2ap_log_level = (U8)X2AP_FATAL;
            break;

        case OAM_LOG_LEVEL_ERROR:
            x2ap_log_level = (U8)X2AP_ERROR;
            break;

        case OAM_LOG_LEVEL_WARNING:
            x2ap_log_level = (U8)X2AP_WARNING;
            break;

        case OAM_LOG_LEVEL_INFO:
            x2ap_log_level = (U8)X2AP_INFO;
            break;

        case OAM_LOG_LEVEL_BRIEF:
            x2ap_log_level = (U8)X2AP_BRIEF;
            break;

        case OAM_LOG_LEVEL_DETAILED:
            x2ap_log_level = (U8)X2AP_DETAILED;
            break;

        case OAM_LOG_LEVEL_DETAILEDALL:
            x2ap_log_level = (U8)X2AP_DETAILEDALL;
            break;

        default:
            RRC_ASSERT(RRC_FALSE);
    }

    return x2ap_log_level;
}


/******************************************************************************
*   FUNCTION NAME: x2ap_change_logging_parameters
*   INPUTS       : x2ap_gb_context_t       *p_x2ap_gb_ctx
*                  U8                      log_level
*                  x2ap_bool_et            log_enabled
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function changes logging parameters
*
*   RETURNS:
*       None
******************************************************************************/
void x2ap_change_logging_parameters
(
    x2ap_gb_context_t       *p_x2ap_gb_ctx,     /* global context */ 
    U8                      log_level,          /* log level to set */
    x2ap_bool_et            log_enabled         /* specifies whether logging
                                                 * should be enabled or not */
)
{
    RRC_ASSERT(PNULL != p_x2ap_gb_ctx);
    /* coverity_97013 start */    
    if (LogSuccess == init_log(LogTarget_Console, NULL, &g_advl_ctxt_x2ap))
		    /* coverity_97013 stop */    
#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    if (log_enabled != p_x2ap_gb_ctx->log_enabled)
    {
               /*SPR 17777 +-*/
        if (RRC_TRUE == (rrc_bool_et)log_enabled)
        {
            enable_log(g_advl_ctxt_x2ap);
        }
        else
        {
            disable_log(g_advl_ctxt_x2ap);
        }
        p_x2ap_gb_ctx->log_enabled = log_enabled;
    }

    set_loglevel_limit(g_advl_ctxt_x2ap, (AdvL_LogLevel)log_level);
    p_x2ap_gb_ctx->log_level = log_level;
#elif (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    if (log_enabled != p_x2ap_gb_ctx->log_enabled)
    {
        if (RRC_FALSE == log_enabled)
        {
            rrc_set_loglevel(RRC_INT_NONE);
        }
        else
        {
            /* No need to enable internal logger, it enables automatically
             * after setting appropriate log level */
            rrc_set_loglevel(p_x2ap_gb_ctx->log_level);
        }
        p_x2ap_gb_ctx->log_enabled = log_enabled;
    }

    if (log_level != p_x2ap_gb_ctx->log_level)
    {
        /* In internal logger rrc_set_loglevel enables logging, so additional
         * check should be provided */
        if (RRC_TRUE == p_x2ap_gb_ctx->log_enabled)
        {
            rrc_set_loglevel(log_level);
        }

        p_x2ap_gb_ctx->log_level = log_level;
    }
#endif /* (LOGGER_TYPE == RRC_ADV_LOGGER) */
}
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */


