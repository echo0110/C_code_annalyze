/*******************************************************************************
*
*  FILE NAME   : s1ap_logger.c
*
*  DESCRIPTION : This file contains the definitions of function used for logging
*                and tracing.
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/

#include "s1ap_logger.h"

#include "cspl.h"
#include "stacklayer.h"


/*
*   Global variables
*/

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
/******************************************************************************
*   FUNCTION NAME: get_s1ap_log_level_from_oam_log_level
*   INPUTS       : U8 oam_log_level
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function returns s1ap log level identifier from OAM log level
*       identifier
*
*   RETURNS:
*       s1ap log level identifier
******************************************************************************/
U8 get_s1ap_log_level_from_oam_log_level
(
    U8 oam_log_level /* OAM log level identifier */
)
{
    U8 s1ap_log_level = S1AP_BRIEF;

    switch (oam_log_level)
    {
        case OAM_LOG_LEVEL_FATAL:
            s1ap_log_level = (U8)S1AP_FATAL;
            break;

        case OAM_LOG_LEVEL_ERROR:
            s1ap_log_level = (U8)S1AP_ERROR;
            break;

        case OAM_LOG_LEVEL_WARNING:
            s1ap_log_level = (U8)S1AP_WARNING;
            break;

        case OAM_LOG_LEVEL_INFO:
            s1ap_log_level = (U8)S1AP_INFO;
            break;

        case OAM_LOG_LEVEL_BRIEF:
            s1ap_log_level = (U8)S1AP_BRIEF;
            break;

        case OAM_LOG_LEVEL_DETAILED:
            s1ap_log_level = (U8)S1AP_DETAILED;
            break;

        case OAM_LOG_LEVEL_DETAILEDALL:
            s1ap_log_level = (U8)S1AP_DETAILEDALL;
            break;

        default:
            RRC_ASSERT(RRC_FALSE);
    }

    return s1ap_log_level;
}


/******************************************************************************
*   FUNCTION NAME: s1ap_change_logging_parameters
*   INPUTS       : s1ap_gb_context_t       *p_s1ap_gb_ctx
*                  U8                      log_level
*                  rrc_bool_et             log_enabled
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function changes logging parameters
*
*   RETURNS:
*       None
******************************************************************************/
void s1ap_change_logging_parameters
(
    s1ap_gb_context_t       *p_s1ap_gb_ctx,     /* global context */
    U8                      log_level,          /* log level to set */
    rrc_bool_et             log_enabled         /* specifies whether logging
                                                 * should be enabled or not */
)
{
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    /* coverity_97014 start */    
    if (LogSuccess == init_log(LogTarget_Console, NULL, &g_advl_ctxt_s1ap))
		    /* coverity_97014 stop */    
		    if (log_enabled != p_s1ap_gb_ctx->log_enabled)
		    {
		    if (RRC_TRUE == log_enabled)
		    {
		    enable_log(g_advl_ctxt_s1ap);
		    }
		    else
		    {
		    disable_log(g_advl_ctxt_s1ap);
		    }
		    p_s1ap_gb_ctx->log_enabled = log_enabled;
		    }

    set_loglevel_limit(g_advl_ctxt_s1ap, (AdvL_LogLevel)log_level);
    p_s1ap_gb_ctx->log_level = log_level;
#elif (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    if (log_enabled != p_s1ap_gb_ctx->log_enabled)
    {
        if (RRC_FALSE == log_enabled)
        {
            rrc_set_loglevel(RRC_INT_NONE);
        }
        else
        {
            /* No need to enable internal logger, it enables automatically
             * after setting appropriate log level */
            rrc_set_loglevel(p_s1ap_gb_ctx->log_level);
        }
        p_s1ap_gb_ctx->log_enabled = log_enabled;
    }

    if (log_level != p_s1ap_gb_ctx->log_level)
    {
        /* In internal logger rrc_set_loglevel enables logging, so additional
         * check should be provided */
        if (RRC_TRUE == p_s1ap_gb_ctx->log_enabled)
        {
            rrc_set_loglevel(log_level);
        }

        p_s1ap_gb_ctx->log_level = log_level;
    }
#endif /* (LOGGER_TYPE == RRC_ADV_LOGGER) */
}
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */

