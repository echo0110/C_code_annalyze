/******************************************************************************
**
**  FILE NAME     :  oamh_ed.c
**
**  DESCRIPTION   :  This file is the main entry point handler for OAMH Module
**
**
**  DATE        AUTHOR      REF             REASON
**  --------    -------     ---             ----------
**  13-May-09   Shashank    DDv0.2          Initial
**  21-May-09   Shashank    DDv0.2          Review Comments incorporated.
**  27-May-09   Shashank    DDv0.2      Formal review comments incorporated.
**  14-Oct-09   OleksandrM  DDv1.41     Global redesign
**
**     Copyright (C) 2006 Aricent Inc . All Rights Reserved
******************************************************************************/


/****************************************************************************
* Includes
****************************************************************************/
#include "oamh_fsm.h"
#include "rrc_oam_il_parser.h"
#include "rrc_oam_il_composer.h"
#include "oamh_logger.h"
#include "rrc_handlers.h"
#include "rrc_common_md.h"
#include "rrc_defines.h"
#include "oamh_timers.h"
#include "oamh_ed.h"
#include "rrc_shared_stats.h"

/****************************************************************************
* Global variables
****************************************************************************/
/*Bug 9305 start*/
extern U32 rrc_health_monitor_val[RRC_MAX_THREADS];
/*Bug 9305 end*/
/*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
extern QLOCK rrc_lock;
#endif
/*Lockless Implementation stop*/

extern oamh_health_monitor_desriptors_t oamh_health_monitor_desriptors[RRC_MAX_THREADS];

/* Used in logger for identifying OAMH (just for avoid string duplication) */
const S8* rrc_oamh_log_facility_name = (const S8*)"eNodeB RRC OAMH";

/****************************************************************************
 Utils
****************************************************************************/
#define RRC_OAM_RESET_CELL_STATS_W_FOR_UECC_STATUS 0x01
#define RRC_OAM_RESET_CELL_STATS_W_FOR_CSC_STATUS  0x02

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
/******************************************************************************
*   FUNCTION NAME: oamh_get_rrc_log_level_from_oam_log_level
*   INPUTS       : U8 oam_log_level
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function returns RRC log level identifier from OAM log level
*       identifier
*
*   RETURNS:
*       RRC log level identifier
******************************************************************************/
U8 oamh_get_rrc_log_level_from_oam_log_level
(
    U8 oam_log_level /* OAM log level identifier */
)
{
    U8 rrc_log_level = RRC_BRIEF;

    switch (oam_log_level)
    {
        case OAM_LOG_LEVEL_FATAL:
            rrc_log_level = (U8)RRC_FATAL;
            break;

        case OAM_LOG_LEVEL_ERROR:
            rrc_log_level = (U8)RRC_ERROR;
            break;

        case OAM_LOG_LEVEL_WARNING:
            rrc_log_level = (U8)RRC_WARNING;
            break;

        case OAM_LOG_LEVEL_INFO:
            rrc_log_level = (U8)RRC_INFO;
            break;

        case OAM_LOG_LEVEL_BRIEF:
            rrc_log_level = (U8)RRC_BRIEF;
            break;

        case OAM_LOG_LEVEL_DETAILED:
            rrc_log_level = (U8)RRC_DETAILED;
            break;
            /*SPR 20457 fix INTEL_COMMON flag removed*/
        case OAM_LOG_LEVEL_DETAILEDALL:
            rrc_log_level = (U8)RRC_COMP_PARSE;
            break;
        default:
            RRC_ASSERT(RRC_FALSE);
    }

    return rrc_log_level;
}
/* BUG_13115 start */
/******************************************************************************
*   FUNCTION NAME: oamh_get_oam_log_level_from_rrc_log_level
*   INPUTS       : U8 rrc_log_level
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function returns OAM log level identifier from RRC log level
*       identifier
*
*   RETURNS:
*       OAM log level identifier
******************************************************************************/
U8 oamh_get_oam_log_level_from_rrc_log_level
(
    U8 rrc_log_level /* RRC log level identifier */
    )
{
    U8 oam_log_level = RRC_NULL;

    switch (rrc_log_level)
    {
        case RRC_FATAL:
            oam_log_level = (U8)OAM_LOG_LEVEL_FATAL;
            break;

        case RRC_ERROR:
            oam_log_level = (U8)OAM_LOG_LEVEL_ERROR;
            break;

        case RRC_WARNING:
            oam_log_level = (U8)OAM_LOG_LEVEL_WARNING;
            break;

        case RRC_INFO:
            oam_log_level = (U8)OAM_LOG_LEVEL_INFO;
            break;

        case RRC_BRIEF:
            oam_log_level = (U8)OAM_LOG_LEVEL_BRIEF;
            break;

        case RRC_DETAILED:
            oam_log_level = (U8)OAM_LOG_LEVEL_DETAILED;
            break;
            /*SPR 20457 fix INTEL_COMMON flag removed*/
        case RRC_COMP_PARSE:
            oam_log_level = (U8)OAM_LOG_LEVEL_DETAILEDALL;
            break;
        default:
            RRC_ASSERT(RRC_FALSE);
    }

    return oam_log_level;
}
/* BUG_13115 stop */

/*spr 22474 start*/
/****************************************************************************
 *  * Function Name  : oamh_prepare_memPool_stats
 *  * Inputs         : pointer to buffer
 *  * Outputs        : prepares the mempool stats for OAM
 *  * Returns        : None
 *  * Description    : This function updates the stats of mempool
 *  ****************************************************************************/
void oamh_prepare_memPool_stats
(
    rrc_MemPoolStats *csplMemPoolStats_p
)
{
    UInt32 allocBufMemPool = 0, nbufMemPool = 0, peakBufMemPool = 0;
    UInt32 poolIndex = 0, bufSize = 0, validPool = 0;
    
    /* Get the mempool stats */
    for( poolIndex = 0,bufSize = MINBUF; poolIndex < NVARMEMPOOL; poolIndex++, bufSize *= 2)
    {
        allocBufMemPool = nbufMemPool = peakBufMemPool = 0;
        nbufMemPool = getMemPoolStatsInShm(poolIndex, &allocBufMemPool, &peakBufMemPool );
        /* Send only pools which has non zero total buffers */
        if (nbufMemPool)
        {
            csplMemPoolStats_p->stats[validPool].bufSize = bufSize;
            csplMemPoolStats_p->stats[validPool].numOfBuf = nbufMemPool;
            csplMemPoolStats_p->stats[validPool].numOfAllocBuf = allocBufMemPool;
            csplMemPoolStats_p->stats[validPool].numOfPeakBuf = peakBufMemPool;
            validPool++;
        }
    }
    /* Set the end marker as bufSize to 0 so that OAM is aware where the status ends in array */
    if (validPool < NVARMEMPOOL)
    {
        csplMemPoolStats_p->stats[validPool].bufSize = 0;
    }
    return;
}

/*spr 22474 end*/

/******************************************************************************
*   FUNCTION NAME: oamh_change_logging_parameters
*   INPUTS       : rrc_oamh_gb_context_t   *p_oamh_gb_context
*                  U8                      log_level
*                  rrc_bool_et             log_enabled
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function changes logging parameters
*
*   RETURNS:
*       None
******************************************************************************/
void oamh_change_logging_parameters
(
    rrc_oamh_gb_context_t   *p_oamh_gb_context, /* OAMH global context */
    U8                      log_level,          /* log leve to set */
    rrc_bool_et             log_enabled         /* specifies whether logging
                                                 * should be enabled or not */
)
{
    RRC_ASSERT(PNULL != p_oamh_gb_context);

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    if (log_enabled != p_oamh_gb_context->log_enabled)
    {
        if (RRC_TRUE == log_enabled)
        {
            enable_log(g_advl_ctxt_rrc);
        }
        else
        {
            disable_log(g_advl_ctxt_rrc);
        }
        p_oamh_gb_context->log_enabled = log_enabled;
    }

    set_loglevel_limit(g_advl_ctxt_rrc, (AdvL_LogLevel)log_level);
    p_oamh_gb_context->log_level = log_level;

#elif (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    if (log_enabled != p_oamh_gb_context->log_enabled)
    {
        if (RRC_FALSE == log_enabled)
        {
            rrc_set_loglevel(RRC_INT_NONE);
        }
        else
        {
            /* No need to enable internal logger, it enables automatically
             * after setting appropriate log level */
            rrc_set_loglevel(p_oamh_gb_context->log_level);
        }
        p_oamh_gb_context->log_enabled = log_enabled;
    }

    if (log_level != p_oamh_gb_context->log_level)
    {
        /* In internal logger rrc_set_loglevel enables logging, so additional
         * check should be provided */
        if (RRC_TRUE == p_oamh_gb_context->log_enabled)
        {
            rrc_set_loglevel(log_level);
        }

        p_oamh_gb_context->log_level = log_level;
    }
#endif /* (LOGGER_TYPE == RRC_ADV_LOGGER) */
}
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */

/****************************************************************************
 Static functions
****************************************************************************/

static void oamh_dispatch_message(void *, rrc_oamh_gb_context_t *);
static void oamh_oam_dispatch_message(void *, U16, rrc_oamh_gb_context_t *);
static void oamh_csc_dispatch_message(void *, U16, rrc_oamh_gb_context_t *);
static void oamh_uecc_dispatch_message(void *, U16, rrc_oamh_gb_context_t *);
static void oamh_ueccmd_dispatch_message(void *, U16, rrc_oamh_gb_context_t *);
static void oamh_llim_dispatch_message(void *, U16, rrc_oamh_gb_context_t *);

static void oamh_csc_process_cell_start_ind
(
    void                  *p_msg,
    rrc_oamh_gb_context_t *p_oamh_gb_context
);
static void oamh_csc_process_cell_stop_ind
(
    void                  *p_msg,
    rrc_oamh_gb_context_t *p_oamh_gb_context
);
/*##################### MSG HANDLERS #######################################*/

/*spr 22474 start*/
/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_get_debug_info_req 
*   INPUTS       : void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      :
*   DESCRIPTION:
*       This function handles RRC_OAM_GET_DEBUG_INFO_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_get_debug_info_req 
(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_oam_get_debug_info_req_t debug_info_req;  
    rrc_oam_get_debug_info_resp_t debug_info_resp; 
    rrc_MemPoolStats rrc_csplMemPoolStats;
    U16 transaction_id = RRC_NULL;
    S32 length_read    = RRC_NULL;
    memset_wrapper(&debug_info_req, 0, sizeof(rrc_oam_get_debug_info_req_t));
    memset_wrapper(&debug_info_resp, 0, sizeof(rrc_oam_get_debug_info_resp_t));
    memset_wrapper(&rrc_csplMemPoolStats, 0, sizeof(rrc_MemPoolStats));

    if(RRC_SUCCESS !=rrc_il_parse_rrc_oam_get_debug_info_req(&debug_info_req,
            (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    if(debug_info_req.debug_type == GET_MEM_POOL_STATS)
    {
        oamh_prepare_memPool_stats(&rrc_csplMemPoolStats);
        debug_info_resp.bitmask |= RRC_OAM_DEBUG_GET_MEM_POOL_PRESENT;
        debug_info_resp.response = RRC_SUCCESS;
	    l3_memcpy_wrapper(&debug_info_resp.csplMemPoolStats,&rrc_csplMemPoolStats,sizeof(rrc_MemPoolStats));
    }
    else
    {
        RRC_TRACE(RRC_WARNING,"%s: Wrong DebugType Received",__FUNCTION__);
        debug_info_resp.response = RRC_FAILURE;
        debug_info_resp.fail_cause = WRONG_DEBUG_TYPE;
        debug_info_resp.bitmask |= RRC_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT;
    }
    transaction_id = rrc_get_transaction_id(p_api);
    rrc_oam_il_send_rrc_oam_get_debug_info_resp(&debug_info_resp,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        transaction_id,
                        RRC_NULL);

}
/*spr 22474 end*/
/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_set_log_level_req
*   INPUTS       : void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      :
*   DESCRIPTION:
*       This function handles RRC_OAM_SET_LOG_LEVEL_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_set_log_level_req
(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_oam_set_log_level_req_t    in_message;
    rrc_oam_set_log_level_resp_t   out_message;
    S32 length_read    = RRC_NULL;
    U16 transaction_id = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    memset_wrapper(&out_message, 0, sizeof(out_message));
    if(RRC_SUCCESS !=
       rrc_il_parse_rrc_oam_set_log_level_req(
            &in_message,
            (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    transaction_id = rrc_get_transaction_id(p_api);

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    /* Set new log level */
    oamh_change_logging_parameters(p_oamh_gb_context,
        oamh_get_rrc_log_level_from_oam_log_level(in_message.log_level),
        p_oamh_gb_context->log_enabled);
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */

    out_message.response = RRC_SUCCESS;

    rrc_oam_il_send_rrc_oam_set_log_level_resp(
                        &out_message,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        transaction_id,
                        RRC_NULL);

    RRC_OAMH_UT_TRACE_EXIT();
}

/*SPR 13115 Start*/
/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_get_log_level_req
*   INPUTS       : void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      :
*   DESCRIPTION:
*       This function handles RRC_OAM_GET_LOG_LEVEL_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_get_log_level_req
(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_oam_get_log_level_resp_t   out_message;
    U16 transaction_id = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    memset_wrapper(&out_message, 0, sizeof(out_message));
    transaction_id = rrc_get_transaction_id(p_api);
    if (p_oamh_gb_context->log_enabled == RRC_TRUE)
    {
        out_message.log_on_off = OAM_LOG_ON;
        out_message.bitmask |= RRC_OAM_LOG_LEVEL_PRESENT;
        out_message.log_level = oamh_get_oam_log_level_from_rrc_log_level(p_oamh_gb_context->log_level);
    }
    else
    {
        out_message.log_on_off = OAM_LOG_OFF;
    }
    rrc_oam_il_send_rrc_oam_get_log_level_resp(
                        &out_message,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        transaction_id,
                        RRC_NULL);

    RRC_OAMH_UT_TRACE_EXIT();
}
/*SPR 13115 Stop*/
/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_log_enable_req
*
*   INPUTS       : void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      :
*   DESCRIPTION:
*       This function handles RRC_OAM_LOG_ENABLE_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_log_enable_req
(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_oam_log_enable_req_t    in_message;
    rrc_oam_log_enable_resp_t   out_message;
    S32 length_read         = RRC_NULL;
    U16 transaction_id      = RRC_NULL;
    rrc_bool_et log_enabled = RRC_TRUE;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    memset_wrapper(&out_message, 0, sizeof(out_message));

    if(RRC_SUCCESS !=
       rrc_il_parse_rrc_oam_log_enable_req(
            &in_message,
            (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    transaction_id = rrc_get_transaction_id(p_api);

    if (OAM_LOG_ON == in_message.log_on_off)
    {
        log_enabled = RRC_TRUE;
    }
    else
    {
        log_enabled = RRC_FALSE;
    }

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    /* Enable/Disable logging */
    oamh_change_logging_parameters(p_oamh_gb_context, p_oamh_gb_context->
        log_level, log_enabled);
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */

    out_message.response = RRC_SUCCESS;

    rrc_oam_il_send_rrc_oam_log_enable_resp(
                        &out_message,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        transaction_id,
                        RRC_NULL);

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_check_and_fill_la_overlap_interrat_stats
*
*   DESCRIPTION:
*       This function fills LA Overlap InterRAT Stats
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_check_and_fill_la_overlap_interrat_stats(
        rrc_oam_get_cell_stats_req_t    *in_message,
        rrc_oam_get_cell_stats_resp_t   *out_message,
        rrc_oamh_gb_context_t *p_oamh_gb_context)
{
    U8 uecc_index = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);
    if (in_message->bitmask &
            RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_REQUESTED_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_IratIncMobility_LAI_counter_enable == 1)
        {
            out_message->bitmask |=
                RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
                for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
                {
                    out_message->rrc_IratIncMobility_LAI += g_rrc_stats.
                        p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message->cell_index].rrc_IratIncMobility_LAI;
                }
            }
        }
        else
        {
            RRC_OAMH_TRACE(RRC_WARNING, "RRC_OAM_GET_CELL_STATS_REQ -"
                    " irat mobility stats requested but not configured ");
        }
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_check_and_fill_ue_assoc_s1_conn_stats
*
*   DESCRIPTION:
*       This function fills UE Associated S1 Connection related Stats
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_check_and_fill_ue_assoc_s1_conn_stats(
        rrc_oam_get_cell_stats_req_t    *in_message,
        rrc_oam_get_cell_stats_resp_t   *out_message,
        rrc_oamh_gb_context_t *p_oamh_gb_context)
{
    U8 uecc_index = RRC_NULL;
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);
    if (in_message->bitmask &
            RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_REQUESTED_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_ue_assoc_s1_conn_counter_enable == 1)
        {
            out_message->bitmask |=
                RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
                for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
                {
                    out_message->rrc_ue_assoc_s1_conn_related_stats.s1sig_connEstabAtt += g_rrc_stats.
                        p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message->cell_index].rrc_ue_assoc_s1_conn_related_stats.
                        s1sig_connEstabAtt;

                    out_message->rrc_ue_assoc_s1_conn_related_stats.s1sig_connEstabSucc += g_rrc_stats.
                        p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message->cell_index].rrc_ue_assoc_s1_conn_related_stats.
                        s1sig_connEstabSucc;
                }
            }
        }
        else
        {
            RRC_OAMH_TRACE(RRC_WARNING, "RRC_OAM_GET_CELL_STATS_REQ -"
                    " ue associated s1 connection stats requested but not configured ");
        }
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_check_and_fill_reset_cell_statistics
*
*   DESCRIPTION:
*       This function fills Reset Cell Statistics
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_check_and_fill_reset_cell_statistics(
        rrc_oam_get_cell_stats_req_t    *in_message,
        rrc_oamh_gb_context_t *p_oamh_gb_context)
{
    U8 uecc_index = RRC_NULL;
    U16 time_duration = RRC_NULL;
    /* Bug 11748 Fix Start */
    U32 rrcConnectionNumber = RRC_NULL;
    rrc_uecc_oam_stat_erab_number_t erab_sim_num_stats;
    /* Bug 11748 Fix Stop */
    /*SPR_17462_START*/
    U8 target_index = RRC_NULL;
    rrc_oam_meas_on_neigh_cell_ho_stats_list_t  
        *p_meas_on_neigh_cell_ho_target_id_list = PNULL;
    rrc_oam_inter_rat_ho_stats_list_t
        *p_inter_rat_ho_target_id_list = PNULL;
    /*SPR_17462_END*/


    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    /* Bug 11748 Fix Start */
    memset_wrapper ( &erab_sim_num_stats, 0, 
            sizeof(rrc_uecc_oam_stat_erab_number_t) );
    /* Bug 11748 Fix Stop */

    if (in_message->bitmask &
            RRC_OAM_RESET_CELL_STATISTICS_PRESENT)
    {
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
            /*SPR_17462_START*/
            p_meas_on_neigh_cell_ho_target_id_list = 
                rrc_mem_get(sizeof(rrc_oam_meas_on_neigh_cell_ho_stats_list_t));
            if(PNULL == p_meas_on_neigh_cell_ho_target_id_list)
            {
                RRC_OAMH_TRACE(RRC_FATAL, "%s: Memory allocation failed for "
                    "p_meas_on_neigh_cell_ho_target_id_list.", __FUNCTION__);
                return;
            }
            /* Initialize count with 0 */
            p_meas_on_neigh_cell_ho_target_id_list->count = 0;

            p_inter_rat_ho_target_id_list = 
                rrc_mem_get(sizeof(rrc_oam_inter_rat_ho_stats_list_t));
            if(PNULL == p_inter_rat_ho_target_id_list )
            {
                RRC_OAMH_TRACE(RRC_FATAL, "%s: Memory allocation failed for "
                    "p_inter_rat_ho_target_id_list.", __FUNCTION__);
                rrc_mem_free(p_meas_on_neigh_cell_ho_target_id_list);
                p_meas_on_neigh_cell_ho_target_id_list = PNULL;
                return;
            }
            /* Initialize count with 0 */
            p_inter_rat_ho_target_id_list->count = 0;
            /*SPR_17462_END*/
            for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
            {
                /* Bug 11748 Fix Start */
                rrcConnectionNumber = g_rrc_stats.p_rrc_stats->rrc_uecc_stats. 
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[in_message->cell_index].
                    rrc_connection_related_stats.rrcConnectionNumber;
                /* Bug 11748 Fix Stop */

                memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message->cell_index].
                        rrc_connection_related_stats, 0,
                        sizeof(rrc_oam_connection_related_stats_t));

                /* Bug 11748 Fix Start */
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats. 
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[in_message->cell_index].
                    rrc_connection_related_stats.rrcConnectionNumber = 
                       rrcConnectionNumber;

                l3_memcpy_wrapper (&erab_sim_num_stats, &g_rrc_stats.
                        p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message->cell_index].
                        rrc_erab_related_stats.erab_sim_num_stats, 
                        sizeof(rrc_uecc_oam_stat_erab_number_t));
                /* Bug 11748 Fix Stop */

                memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message->cell_index].
                        rrc_erab_related_stats, 0,
                        sizeof(rrc_oam_erab_related_stats_t));

                /* Bug 11748 Fix Start */
                l3_memcpy_wrapper (&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message->cell_index].
                        rrc_erab_related_stats.erab_sim_num_stats, &erab_sim_num_stats,
                        sizeof(rrc_uecc_oam_stat_erab_number_t));
                /* Bug 11748 Fix Stop */

                /*SPR_17462_START*/
                if(0 != g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message->cell_index].
                        rrc_handover_related_stats.
                        meas_on_neigh_cell_ho_stats_list.count)
                {
                    l3_memcpy_wrapper(p_meas_on_neigh_cell_ho_target_id_list,
                            &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[in_message->cell_index].
                            rrc_handover_related_stats.
                            meas_on_neigh_cell_ho_stats_list,
                            sizeof(rrc_oam_meas_on_neigh_cell_ho_stats_list_t));
                }
                if(0 != g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message->cell_index].
                        rrc_handover_related_stats.
                        inter_rat_ho_stats_list.count)
                {
                    l3_memcpy_wrapper(p_inter_rat_ho_target_id_list,
                            &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[in_message->cell_index].
                            rrc_handover_related_stats.
                            inter_rat_ho_stats_list,
                            sizeof(rrc_oam_inter_rat_ho_stats_list_t));
                }
                memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message->cell_index].
                        rrc_handover_related_stats, 0,
                        sizeof(rrc_oam_handover_related_stats_t));

                if(0 != p_meas_on_neigh_cell_ho_target_id_list->count)
                {
                    /* SPR 17885 Fix Start */
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                      rrc_uecc_instance_stats[uecc_index].
                      rrc_uecc_cell_stats[in_message->cell_index].
                      rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                      count = p_meas_on_neigh_cell_ho_target_id_list->count;
                    /* SPR 17885 Fix Stop */
                    for(target_index = 0; 
                            target_index < p_meas_on_neigh_cell_ho_target_id_list->count;
                            target_index ++)
                    {
                        l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[in_message->cell_index].
                            rrc_handover_related_stats.
                            meas_on_neigh_cell_ho_stats_list.
                            /*SPR_19166_START*/
                            meas_on_neigh_cell_ho_stats[target_index].ecgi,
                            &p_meas_on_neigh_cell_ho_target_id_list->
                            meas_on_neigh_cell_ho_stats[target_index].ecgi,
                            sizeof(cell_global_id_eutra_t));
                            /*SPR_19166_END*/
                    }
                    p_meas_on_neigh_cell_ho_target_id_list->count = 0;
                }
                if(0 != p_inter_rat_ho_target_id_list->count)
                {
                    /* SPR 17885 Fix Start */
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                      rrc_uecc_instance_stats[uecc_index].
                      rrc_uecc_cell_stats[in_message->cell_index].
                      rrc_handover_related_stats.inter_rat_ho_stats_list.
                      count = p_inter_rat_ho_target_id_list->count;
                    /* SPR 17885 Fix Stop */
                    for(target_index = 0; 
                            target_index < p_inter_rat_ho_target_id_list->count;
                            target_index++)
                    {
                        l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[in_message->cell_index].
                            rrc_handover_related_stats.
                            inter_rat_ho_stats_list.
                            /*SPR_19166_START*/
                            inter_rat_ho_stats[target_index].target_cgi,
                            &p_inter_rat_ho_target_id_list->
                            inter_rat_ho_stats[target_index].target_cgi,
                            sizeof(rrc_inter_rat_global_cgi_t));
                            /*SPR_19166_END*/
                    }
                    p_inter_rat_ho_target_id_list->count = 0;
                }
                /*SPR_17462_END*/

                memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message->cell_index].
                        rrc_ue_assoc_s1_conn_related_stats, 0,
                        sizeof(rrc_uecc_oam_stat_ue_assoc_s1_connection_t));

                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[in_message->cell_index].
                    rrc_IratIncMobility_LAI =  0;
            } 
            /*SPR_17462_START*/
            rrc_mem_free(p_meas_on_neigh_cell_ho_target_id_list);
            p_meas_on_neigh_cell_ho_target_id_list = PNULL;
            rrc_mem_free(p_inter_rat_ho_target_id_list);
            p_inter_rat_ho_target_id_list = PNULL;
            /*SPR_17462_END*/
        }
        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_rrc_connection_counter_enable == 1)
        {
            if (RRC_NULL != p_oamh_gb_context->timers_data.
                    timers_ids[OAMH_RRC_CONNECTION_SAMPLING_TIMER])
            {
                time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_RRC_CONNECTION_SAMPLING_TIMER]); 
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                    is_reset_done[in_message->cell_index] = RRC_TRUE;
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                    time_remaining_till_reset[in_message->cell_index] = time_duration;
            }
        }

        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_erab_counter_enable == 1)
        {
            if (RRC_NULL != p_oamh_gb_context->timers_data.
                    timers_ids[OAMH_ERAB_SAMPLING_TIMER])
            {
                time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_ERAB_SAMPLING_TIMER]);
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                    is_reset_done[in_message->cell_index] = RRC_TRUE;
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                    time_remaining_till_reset[in_message->cell_index] = time_duration;            
            }
        }

        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_handover_counter_enable == 1)
        {
            if (RRC_NULL != p_oamh_gb_context->timers_data.
                    timers_ids[OAMH_HANDOVER_SAMPLING_TIMER])
            {
                time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_HANDOVER_SAMPLING_TIMER]);
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                    is_reset_done[in_message->cell_index] = RRC_TRUE;
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                    time_remaining_till_reset[in_message->cell_index] = time_duration;            
            }
        }

        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_ue_assoc_s1_conn_counter_enable == 1)
        {
            if (RRC_NULL != p_oamh_gb_context->timers_data.
                    timers_ids[OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER])
            {
                time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER]);
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_ue_assoc_s1_conn_stats_status.
                    is_reset_done[in_message->cell_index] = RRC_TRUE;
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_ue_assoc_s1_conn_stats_status.
                    time_remaining_till_reset[in_message->cell_index] = time_duration;            
            }
        }

        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_IratIncMobility_LAI_counter_enable == 1)
        {
            if (RRC_NULL != p_oamh_gb_context->timers_data.
                    timers_ids[OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER])
            {
                time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER]);
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_IratIncMobility_LAI_stats_status.
                    is_reset_done[in_message->cell_index] = RRC_TRUE;
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_IratIncMobility_LAI_stats_status.
                    time_remaining_till_reset[in_message->cell_index] = time_duration;            
            }
        }
        
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_csc_stats.
                    rrc_csc_cell_stats[in_message->cell_index].rrc_paging_related_stats,0, 
                    sizeof(rrc_csc_oam_stat_paging_t));
        }
        
        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_paging_counter_enable == 1)
        {
            if (RRC_NULL != p_oamh_gb_context->timers_data.
                    timers_ids[OAMH_PAGING_SAMPLING_TIMER])
            {
                time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_PAGING_SAMPLING_TIMER]);
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_paging_stats_status.
                    is_reset_done[in_message->cell_index] = RRC_TRUE;
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_paging_stats_status.
                    time_remaining_till_reset[in_message->cell_index] = time_duration;            
            }
        }
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_get_cell_stats_req
*
*   INPUTS       : void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles RRC_OAM_GET_CELL_STATS_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_get_cell_stats_req
(
 void                  *p_api,
 rrc_oamh_gb_context_t *p_oamh_gb_context
 )
{
    rrc_oam_get_cell_stats_req_t                in_message;
    static rrc_oam_get_cell_stats_resp_t               out_message;
    rrc_return_t get_cell_stats_resp = RRC_SUCCESS;
    U8 uecc_index = RRC_NULL;
    S32 length_read = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    memset_wrapper(&out_message, RRC_NULL, sizeof(rrc_oam_get_cell_stats_resp_t));

    if(RRC_SUCCESS !=
            rrc_il_parse_rrc_oam_get_cell_stats_req(
                &in_message,
                (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Bad message - ignore.");
        get_cell_stats_resp = RRC_FAILURE;
    }
    
    if (!(in_message.bitmask &
                ((RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_REQUESTED_PRESENT) |
                (RRC_OAM_STATISTICS_ERAB_RELATED_STATS_REQUESTED_PRESENT) |
                (RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_REQUESTED_PRESENT) |
                (RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_REQUESTED_PRESENT) |
                (RRC_OAM_STATISTICS_PAGING_RELATED_STATS_REQUESTED_PRESENT) |
                (RRC_OAM_RESET_CELL_STATISTICS_PRESENT) |
                (RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_REQUESTED_PRESENT) |
                (RRC_OAM_STATISTICS_ALL_STATS_PRESENT))))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "invalid bitmask received in "
                "RRC_OAM_GET_CELL_STATS_REQ - ignore");
        get_cell_stats_resp = RRC_FAILURE;
    }
    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
      for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
      {
        if ((MAX_NUM_CELLS > in_message.cell_index) && 
                (RRC_FALSE == g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                 rrc_uecc_instance_stats[uecc_index].
                 rrc_uecc_cell_stats[in_message.cell_index].is_cell_configured) &&
                (RRC_FALSE == g_rrc_stats.p_rrc_stats->rrc_csc_stats.
                 rrc_csc_cell_stats[in_message.cell_index].is_cell_configured))
        {
            RRC_OAMH_TRACE(RRC_WARNING, "cell not configured: Invalid cell index");
            get_cell_stats_resp = RRC_FAILURE;
        }
      }
    }
    out_message.cell_index = in_message.cell_index;

    if (RRC_FAILURE == get_cell_stats_resp)
    {
        out_message.response = get_cell_stats_resp;
/* SPR:9279 start */
        rrc_oam_il_send_rrc_oam_get_cell_stats_resp(
                &out_message,
                RRC_OAMH_MODULE_ID,
                RRC_OAM_MODULE_ID,
                rrc_get_transaction_id(p_api),
                RRC_NULL);
/* SPR:9279 stop */
/*SPR 17588 Fix Start */
        RRC_OAMH_UT_TRACE_EXIT();
        return;
/*SPR 17588 Fix End */
    }

    if (in_message.bitmask &
            RRC_OAM_STATISTICS_ALL_STATS_PRESENT)
    {
        in_message.bitmask |=
            RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_REQUESTED_PRESENT |
            RRC_OAM_STATISTICS_ERAB_RELATED_STATS_REQUESTED_PRESENT |
            RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_REQUESTED_PRESENT |
            RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_REQUESTED_PRESENT |
            RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_REQUESTED_PRESENT |
            RRC_OAM_STATISTICS_PAGING_RELATED_STATS_REQUESTED_PRESENT;  /*GET CELL STATS FIX*/

        in_message.rrc_connection_stats.bitmask |=
            RRC_OAM_STATISTICS_REQ_CONNECTION_ALL_STATS_PRESENT;
        in_message.rrc_erab_stats.bitmask |=
            RRC_OAM_STATISTICS_REQ_ERAB_ALL_STATS_PRESENT;
        in_message.rrc_handover_stats.bitmask |=
            RRC_OAM_STATISTICS_REQ_HANDOVER_ALL_STATS_PRESENT;

    }

    /* Only those indications will be sent in response
     * for which the bits are set in received cell_stats */
    if (in_message.bitmask &
            RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_REQUESTED_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_rrc_connection_counter_enable == 1)
        {
            out_message.bitmask |=
                RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_PRESENT;
            rrc_oam_build_rrc_conn_stats(&in_message,
                    p_oamh_gb_context, &out_message.rrc_connection_related_stats);
        }
        else
        {
            RRC_OAMH_TRACE(RRC_WARNING, "RRC_OAM_GET_CELL_STATS_REQ -"
                    " rrc connection stats requested but not configured ");
        }
    }

    if (in_message.bitmask &
            RRC_OAM_STATISTICS_ERAB_RELATED_STATS_REQUESTED_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_erab_counter_enable == 1)
        {
            out_message.bitmask |=
                RRC_OAM_STATISTICS_ERAB_RELATED_STATS_PRESENT;
            rrc_oam_build_erab_stats(&in_message,
                    p_oamh_gb_context, &out_message.rrc_erab_related_stats);
        }
        else
        {
            RRC_OAMH_TRACE(RRC_WARNING, "RRC_OAM_GET_CELL_STATS_REQ -"
                    " erab stats requested but not configured ");
        }
    }

    if (in_message.bitmask &
            RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_REQUESTED_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_handover_counter_enable == 1)
        {
            out_message.bitmask |=
                RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_PRESENT;
            rrc_oam_build_handover_stats(&in_message,
                    p_oamh_gb_context, &out_message.rrc_handover_related_stats);
        }
        else
        {
             RRC_OAMH_TRACE(RRC_WARNING, "RRC_OAM_GET_CELL_STATS_REQ -"
                     " handover stats requested but not configured ");
        }
    }

    oamh_oam_check_and_fill_ue_assoc_s1_conn_stats(&in_message, &out_message, p_oamh_gb_context);

    if (in_message.bitmask &
            RRC_OAM_STATISTICS_PAGING_RELATED_STATS_REQUESTED_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_paging_counter_enable == 1)
        {
            out_message.bitmask |=
                RRC_OAM_STATISTICS_PAGING_RELATED_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
                out_message.rrc_paging_related_stats.pag_discardedNbr = g_rrc_stats.
                    p_rrc_stats->rrc_csc_stats.rrc_csc_cell_stats[in_message.cell_index].
                    rrc_paging_related_stats.pag_discardedNbr;

                out_message.rrc_paging_related_stats.pag_succNbr = g_rrc_stats.
                    p_rrc_stats->rrc_csc_stats.rrc_csc_cell_stats[in_message.cell_index].
                    rrc_paging_related_stats.pag_succNbr;
                /*CR 025 changes start */
                out_message.rrc_paging_related_stats.pag_receivedNbr = g_rrc_stats.
                    p_rrc_stats->rrc_csc_stats.rrc_csc_cell_stats[in_message.cell_index].
                    rrc_paging_related_stats.pag_receivedNbr;
                /*CR 025 changes stop */
            }
        }
        else
        {
            RRC_OAMH_TRACE(RRC_WARNING, "RRC_OAM_GET_CELL_STATS_REQ -"
                    " paging stats requested but not configured ");
        }
    }

    oamh_oam_check_and_fill_la_overlap_interrat_stats(
                &in_message, &out_message, p_oamh_gb_context);

    oamh_oam_check_and_fill_reset_cell_statistics(&in_message, p_oamh_gb_context);

    if (RRC_NULL == out_message.bitmask)
    {
        get_cell_stats_resp = RRC_FAILURE;
        RRC_OAMH_TRACE(RRC_ERROR," Stats requested are not configured : Sending FAILURE");
    }

    out_message.response = get_cell_stats_resp;
/* SPR:9279 start */
    rrc_oam_il_send_rrc_oam_get_cell_stats_resp(
            &out_message,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            rrc_get_transaction_id(p_api),
            RRC_NULL);
/* SPR:9279 stop */

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_conn_estb_stats 
 *
*   INPUTS       : rrc_uecc_oam_stat_connection_establishment_t    
*                                                     *p_rrc_conn_estb_stats
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This function builds rrc_connection_establishment related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_conn_estb_stats(
      rrc_uecc_oam_stat_connection_establishment_t    *p_rrc_conn_estb_stats, 
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    RRC_OAMH_UT_TRACE_ENTER();

    for ( cause = 0; cause < RRC_ESTABLISHMENT_CAUSE_INDEX; cause++)
    {
        p_rrc_conn_estb_stats->rrc_connEstabAtt_cause[cause] +=
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
            rrc_connection_related_stats.rrc_connection_establishment.
            rrc_connEstabAtt_cause[cause];

        p_rrc_conn_estb_stats->rrc_connEstabSucc_cause[cause] +=
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
            rrc_connection_related_stats.rrc_connection_establishment.
            rrc_connEstabSucc_cause[cause];

        p_rrc_conn_estb_stats->rrc_connEstabFail_cause[cause] +=
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
            rrc_connection_related_stats.rrc_connection_establishment.
            rrc_connEstabFail_cause[cause];
    }

    p_rrc_conn_estb_stats->rrc_connEstabAtt_sum +=
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
        rrc_connection_related_stats.rrc_connection_establishment.
        rrc_connEstabAtt_sum;

    p_rrc_conn_estb_stats->rrc_connEstabSucc_sum +=
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
        rrc_connection_related_stats.rrc_connection_establishment.
        rrc_connEstabSucc_sum;

    p_rrc_conn_estb_stats->rrc_connEstabFail_sum +=
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
        rrc_connection_related_stats.rrc_connection_establishment.
        rrc_connEstabFail_sum;

    p_rrc_conn_estb_stats->rrc_connEstabFaileNBCause_Unspecified +=
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
        rrc_connection_related_stats.rrc_connection_establishment.
        rrc_connEstabFaileNBCause_Unspecified;
     
    /*SPR 15439 Fix Start*/
    p_rrc_conn_estb_stats->rrc_ConnEstabFaileNBCause_Congestion +=
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
        rrc_connection_related_stats.rrc_connection_establishment.
        rrc_ConnEstabFaileNBCause_Congestion;
    /*SPR 15439 Fix End*/
   
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_conn_re_estb_stats 
 *
*   INPUTS       : rrc_uecc_oam_stat_connection_establishment_t    
*                                                     *p_rrc_conn_re_estb_stats
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This function builds rrc_connection_reestablishment related
 *   stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_conn_re_estb_stats(
      rrc_uecc_oam_stat_connection_reestablishment_t    *p_rrc_conn_re_estb_stats, 
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    RRC_OAMH_UT_TRACE_ENTER();

    for ( cause = 0; cause < RRC_REESTABLISHMENT_CAUSE_INDEX; cause++)
    {
        p_rrc_conn_re_estb_stats->rrc_connReEstabAtt_cause[cause] += 
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            rrc_connection_reestablishment.rrc_connReEstabAtt_cause[cause];

        p_rrc_conn_re_estb_stats->rrc_connReEstabSucc_cause[cause] += 
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            rrc_connection_reestablishment.rrc_connReEstabSucc_cause[cause];

        p_rrc_conn_re_estb_stats->rrc_connReEstabFail_cause[cause] += 
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            rrc_connection_reestablishment.rrc_connReEstabFail_cause[cause];
    }

    p_rrc_conn_re_estb_stats->rrc_connReEstabAtt_sum += 
        g_rrc_stats.p_rrc_stats->
        rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
        rrc_connection_reestablishment.rrc_connReEstabAtt_sum;

    p_rrc_conn_re_estb_stats->rrc_connReEstabSucc_sum += 
        g_rrc_stats.p_rrc_stats->
        rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
        rrc_connection_reestablishment.rrc_connReEstabSucc_sum;

    p_rrc_conn_re_estb_stats->rrc_connReEstabFail_sum += 
        g_rrc_stats.p_rrc_stats->
        rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
        rrc_connection_reestablishment.rrc_connReEstabFail_sum;
  
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_conn_setup_time_stats 
 *
*   INPUTS       : rrc_oam_connection_related_stats_t    
*                                                     *p_connection_related_stats
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This function build rrc_connection_setup_time related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_conn_setup_time_stats(
      rrc_oam_connection_related_stats_t  *p_connection_related_stats,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    
    RRC_OAMH_UT_TRACE_ENTER();
  
    for (cause = 0; cause < RRC_ESTABLISHMENT_CAUSE_INDEX; cause++)
    {
        p_connection_related_stats->rrc_connEstabTime_cause[cause].
            rrc_connEstabTime_Sum += g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            rrc_connEstabTime_cause[cause].rrc_connEstabTime_Sum;

        if ((p_connection_related_stats->rrc_connEstabTime_cause[cause].
                    rrc_connEstabTime_Max) < (g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
                        rrc_connEstabTime_cause[cause].rrc_connEstabTime_Max))
        {
            p_connection_related_stats->rrc_connEstabTime_cause[cause].
                rrc_connEstabTime_Max = g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
                rrc_connEstabTime_cause[cause].rrc_connEstabTime_Max;
        }

        p_connection_related_stats->rrc_connEstabTime_cause[cause].count = 
            (U8)(p_connection_related_stats->rrc_connEstabTime_cause[cause].
                    count + g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
                    rrc_connEstabTime_cause[cause].count);
    }
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_ue_ctxt_rel_stats 
*   INPUTS       : rrc_uecc_oam_stat_ue_context_release_t    
*                                                     *p_ue_ctxt_rel_stats
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *
 *   DESCRIPTION:This function build ue_context_release related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_ue_ctxt_rel_stats(
      rrc_uecc_oam_stat_ue_context_release_t    *p_ue_ctxt_rel_stats, 
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;

    RRC_OAMH_UT_TRACE_ENTER();
 
    for (cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_ue_ctxt_rel_stats->ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_radioNetwork[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_ue_ctxt_rel_stats->ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_transport[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_ue_ctxt_rel_stats->ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_nas[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_ue_ctxt_rel_stats->ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_protocol[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_ue_ctxt_rel_stats->ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_misc[cause];
    }

    p_ue_ctxt_rel_stats->ueCtxt_relReq_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
        ue_contxt_rel_stats.ueCtxt_relReq_sum;

    p_ue_ctxt_rel_stats->ueContext_RelSuccNbr += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
        ue_contxt_rel_stats.ueContext_RelSuccNbr;
      
    RRC_OAMH_UT_TRACE_EXIT();
}
    
/******************************************************************************
*   FUNCTION NAME: rrc_oam_build_rrc_conn_stats 
*
*   INPUTS       : rrc_oam_get_cell_stats_req_t *message    
*                  rrc_oamh_gb_context_t               *p_oamh_gb_context
*                  rrc_oam_connection_related_stats_t  *p_connection_related_stats
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function builds rrc connection related stats for the
*       RRC_OAM_GET_CELL_STATS_RESP
*
*   RETURNS:
*       none
******************************************************************************/
void rrc_oam_build_rrc_conn_stats
(
    rrc_oam_get_cell_stats_req_t        *message,
    rrc_oamh_gb_context_t               *p_oamh_gb_context,
    rrc_oam_connection_related_stats_t  *p_connection_related_stats
)
{
    U8 uecc_index = 0;
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);
    
    memset_wrapper(p_connection_related_stats, 0, 
            sizeof(rrc_oam_connection_related_stats_t));

    if (message->rrc_connection_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_CONNECTION_ALL_STATS_PRESENT)
    {
        message->rrc_connection_stats.bitmask |=
            RRC_OAM_STATISTICS_REQ_CONNECTION_ESTB_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_CONNECTION_RE_ESTB_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_CONNECTION_NUMBER_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_CONNECTION_SETUP_TIME_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_CONNECTION_UE_CTX_REL_STATS_PRESENT;
    }

    if (message->rrc_connection_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_CONNECTION_ESTB_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                bitmask & RRC_OAMH_CONNECTION_ESTB_STATS_PRESENT)
        {
            p_connection_related_stats->bitmask |= 
                RRC_OAM_STATISTICS_CONNECTION_ESTB_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                rrc_oamh_build_conn_estb_stats(&p_connection_related_stats->
                        rrc_connection_establishment,message->cell_index,uecc_index);
              }
            }
        }
    }

    if (message->rrc_connection_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_CONNECTION_RE_ESTB_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                bitmask & RRC_OAMH_CONNECTION_RE_ESTB_STATS_PRESENT)
        {
            p_connection_related_stats->bitmask |=
                RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                rrc_oamh_build_conn_re_estb_stats(&p_connection_related_stats->
                        rrc_connection_reestablishment,message->cell_index,uecc_index);
              }
            }
        }
    }

    if (message->rrc_connection_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_CONNECTION_NUMBER_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                bitmask & RRC_OAMH_CONNECTION_NUMBER_STATS_PRESENT)
        {
            p_connection_related_stats->bitmask |=
                RRC_OAM_STATISTICS_CONNECTION_NUMBER_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                p_connection_related_stats->rrcConnectionNumber += 
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[message->cell_index].rrc_connection_related_stats.
                    rrcConnectionNumber;    
              }
            }
        }
    }

    if (message->rrc_connection_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_CONNECTION_SETUP_TIME_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                bitmask & RRC_OAMH_CONNECTION_SETUP_TIME_STATS_PRESENT)
        {
            p_connection_related_stats->bitmask |=
                RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                rrc_oamh_build_conn_setup_time_stats(p_connection_related_stats,
                        message->cell_index,uecc_index);
              }
            }
        }
    }

    if (message->rrc_connection_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_CONNECTION_UE_CTX_REL_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                bitmask & RRC_OAMH_UE_CONTEXT_RELEASE_STATS_PRESENT)
        {
            p_connection_related_stats->bitmask |=
                RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                rrc_oamh_build_ue_ctxt_rel_stats(&p_connection_related_stats->
                        ue_contxt_rel_stats,message->cell_index,uecc_index);
              }
            }
        }   
    }
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_erab_setup_stats 
 *
*   INPUTS       : rrc_uecc_oam_stat_erab_setup_t *p_erab_setup_stats 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION: This function update erab_stats in rrc_oam_erab_stats_ind
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_erab_setup_stats(
      rrc_uecc_oam_stat_erab_setup_t *p_erab_setup_stats,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    U8 qci_count = RRC_NULL;
    
    RRC_OAMH_UT_TRACE_ENTER();

    for (qci_count = 0; qci_count < RRC_MAX_QCI_INDEX; qci_count++)
    {
        p_erab_setup_stats->erab_estabInitAttNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estabInitAttNbr_qci[qci_count];

        p_erab_setup_stats->erab_estabInitSuccNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estabInitSuccNbr_qci[qci_count];

        p_erab_setup_stats->erab_estabAddAttNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estabAddAttNbr_qci[qci_count];

        p_erab_setup_stats->erab_estabAddSuccNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estabAddSuccNbr_qci[qci_count];
    }

    p_erab_setup_stats->erab_estabInitAttNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabInitAttNbr_sum;

    p_erab_setup_stats->erab_estabInitSuccNbr_sum+= 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabInitSuccNbr_sum;

    for (cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_erab_setup_stats->erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_radioNetwork[cause];                            

        p_erab_setup_stats->erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_radioNetwork[cause];                            
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_erab_setup_stats->erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_transport[cause];                            

        p_erab_setup_stats->erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_transport[cause];                            
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_erab_setup_stats->erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_nas[cause];                            

        p_erab_setup_stats->erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_nas[cause];                            
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_erab_setup_stats->erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_protocol[cause];                            

        p_erab_setup_stats->erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_protocol[cause];                            
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_erab_setup_stats->erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_misc[cause];                            

        p_erab_setup_stats->erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_misc[cause];                            
    }

    p_erab_setup_stats->erab_estabInitFailNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabInitFailNbr_sum;                            

    p_erab_setup_stats->erab_estabAddAttNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabAddAttNbr_sum;

    p_erab_setup_stats->erab_estabAddSuccNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabAddSuccNbr_sum;

    p_erab_setup_stats->erab_estabAddFailNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabAddFailNbr_sum;

    for (qci_count = 0; qci_count < RRC_MAX_QCI_INDEX; qci_count++)
    {
        p_erab_setup_stats->erab_estabTime_qci[qci_count].
            erab_estabTime_Sum += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estabTime_qci[qci_count].erab_estabTime_Sum;

        if ((p_erab_setup_stats->erab_estabTime_qci[qci_count].erab_estabTime_Max) < 
                (g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                 rrc_uecc_instance_stats[uecc_index].
                 rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
                 erab_setup_stats.erab_estabTime_qci[qci_count].erab_estabTime_Max))
        {
            p_erab_setup_stats->erab_estabTime_qci[qci_count].
                erab_estabTime_Max = 
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
                erab_setup_stats.erab_estabTime_qci[qci_count].erab_estabTime_Max;
        }

        p_erab_setup_stats->erab_estabTime_qci[qci_count].
            count = (U8)(p_erab_setup_stats->erab_estabTime_qci[qci_count].count + 
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
                    erab_setup_stats.erab_estabTime_qci[qci_count].count);
    } 
      
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_erab_rel_stats 
 *
*   INPUTS       : rrc_uecc_oam_stat_erab_release_t    *p_erab_rel_stats 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION: This function build erab_release related stats 
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_erab_rel_stats(
      rrc_uecc_oam_stat_erab_release_t    *p_erab_rel_stats,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    U8 qci_count = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();
 
    for (qci_count = 0; qci_count < RRC_MAX_QCI_INDEX; qci_count++)
    {
        p_erab_rel_stats->erab_relEnbNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_relEnbNbr_qci[qci_count];

        p_erab_rel_stats->erab_relAttNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_relAttNbr_qci[qci_count];

        p_erab_rel_stats->erab_relSuccNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_relSuccNbr_qci[qci_count];

        p_erab_rel_stats->erab_relActNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_relActNbr_qci[qci_count];
    }

    p_erab_rel_stats->erab_relEnbNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_release_stats.erab_relEnbNbr_sum;

    p_erab_rel_stats->erab_relAttNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_release_stats.erab_relAttNbr_sum;

    p_erab_rel_stats->erab_relSuccNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_release_stats.erab_relSuccNbr_sum;

    p_erab_rel_stats->erab_relFailNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_release_stats.erab_relFailNbr_sum;

    p_erab_rel_stats->erab_relActNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_release_stats.erab_relActNbr_sum;

    for (cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_erab_rel_stats->erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_radioNetwork[cause];

        p_erab_rel_stats->rel_fail_cause_stats.
            erab_relFailNbr_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.rel_fail_cause_stats.
            erab_relFailNbr_cause_radioNetwork[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_erab_rel_stats->erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_transport[cause];

        p_erab_rel_stats->rel_fail_cause_stats.
            erab_relFailNbr_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.rel_fail_cause_stats.
            erab_relFailNbr_cause_transport[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_erab_rel_stats->erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_nas[cause];

        p_erab_rel_stats->rel_fail_cause_stats.
            erab_relFailNbr_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.rel_fail_cause_stats.
            erab_relFailNbr_cause_nas[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_erab_rel_stats->erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_protocol[cause];

        p_erab_rel_stats->rel_fail_cause_stats.
            erab_relFailNbr_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.rel_fail_cause_stats.
            erab_relFailNbr_cause_protocol[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_erab_rel_stats->erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_misc[cause];

        p_erab_rel_stats->rel_fail_cause_stats.
            erab_relFailNbr_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.rel_fail_cause_stats.
            erab_relFailNbr_cause_misc[cause];
    }
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_erab_modify_stats 
 *
*   INPUTS       : rrc_uecc_oam_stat_erab_modification_t    *p_erab_modify_stats
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION: This function build erab_modify related stats 
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_erab_modify_stats(
      rrc_uecc_oam_stat_erab_modification_t    *p_erab_modify_stats,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    U8 qci_count = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();

    for (qci_count = 0; qci_count < RRC_MAX_QCI_INDEX; qci_count++)
    {
        p_erab_modify_stats->erab_modQosAttNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_modQosAttNbr_qci[qci_count];

        p_erab_modify_stats->erab_modQosSuccNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_modQosSuccNbr_qci[qci_count];
    }

    p_erab_modify_stats->erab_modQosAttNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_mod_stats.erab_modQosAttNbr_sum;

    p_erab_modify_stats->erab_modQosSuccNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_mod_stats.erab_modQosSuccNbr_sum;

    p_erab_modify_stats->erab_modQosFailNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_mod_stats.erab_modQosFailNbr_sum;

    for (cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_erab_modify_stats->erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_radioNetwork[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_erab_modify_stats->erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_transport[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_erab_modify_stats->erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_nas[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_erab_modify_stats->erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_protocol[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_erab_modify_stats->erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_misc[cause];
    }
    
}


/******************************************************************************
*   FUNCTION NAME: rrc_oam_build_erab_stats 
*
*   INPUTS       : rrc_oam_get_cell_stats_req_t        *message
*                  rrc_oamh_gb_context_t               *p_oamh_gb_context
*                  rrc_oam_erab_related_stats_t        *p_erab_related_stats
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function builds erab stats for RRC_OAM_GET_CELL_STATS_RESP
*
*   RETURNS:
*       none
******************************************************************************/
void rrc_oam_build_erab_stats
(
 rrc_oam_get_cell_stats_req_t        *message,
 rrc_oamh_gb_context_t               *p_oamh_gb_context,
 rrc_oam_erab_related_stats_t        *p_erab_related_stats
 )
{
    U8 uecc_index = 0; 
    U8 qci_count = 0;
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    memset_wrapper(p_erab_related_stats, 0, sizeof(rrc_oam_erab_related_stats_t));

    if (message->rrc_erab_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_ERAB_ALL_STATS_PRESENT)
    {
        message->rrc_erab_stats.bitmask |=
            RRC_OAM_STATISTICS_REQ_ERAB_SETUP_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_ERAB_RELEASE_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_ERAB_MODIFY_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_ERAB_NUMBER_STATS_PRESENT;
    }

    if (message->rrc_erab_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_ERAB_SETUP_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                bitmask & RRC_OAMH_ERAB_SETUP_STATS_PRESENT)
        {
            p_erab_related_stats->bitmask |= 
                RRC_OAM_STATISTICS_ERAB_SETUP_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                rrc_oamh_build_erab_setup_stats(&p_erab_related_stats->erab_setup_stats,
                        message->cell_index,uecc_index);
              }
            }
        }
    }

    if (message->rrc_erab_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_ERAB_RELEASE_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                bitmask & RRC_OAMH_ERAB_RELEASE_STATS_PRESENT)
        {
            p_erab_related_stats->bitmask |= 
                RRC_OAM_STATISTICS_ERAB_RELEASE_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
               rrc_oamh_build_erab_rel_stats(&p_erab_related_stats->
                       erab_release_stats,message->cell_index,uecc_index);
              }
            }
        }
    }

    if (message->rrc_erab_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_ERAB_MODIFY_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                bitmask & RRC_OAMH_ERAB_MODIFY_STATS_PRESENT)
        {
            p_erab_related_stats->bitmask |= 
                RRC_OAM_STATISTICS_ERAB_MODIFY_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
               rrc_oamh_build_erab_modify_stats(&p_erab_related_stats->
                       erab_mod_stats,message->cell_index,uecc_index);
              }
            }
        }
    }

    if (message->rrc_erab_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_ERAB_NUMBER_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                bitmask & RRC_OAMH_ERAB_NUMBER_STATS_PRESENT)
        {
            p_erab_related_stats->bitmask |= 
                RRC_OAM_STATISTICS_ERAB_NUMBER_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                for (qci_count = 0; qci_count < RRC_MAX_QCI_INDEX; qci_count++)
                {
                    p_erab_related_stats->erab_sim_num_stats.
                        erabSimNumber_qci[qci_count] += 
                        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[message->cell_index].rrc_erab_related_stats.
                        erab_sim_num_stats.erabSimNumber_qci[qci_count];
                }

                p_erab_related_stats->erab_sim_num_stats.erabSimNumber_sum += 
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[message->cell_index].rrc_erab_related_stats.
                    erab_sim_num_stats.erabSimNumber_sum;
              }
            }
        }
    }

    RRC_OAMH_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_intra_enb_ho_stats 
 *
*   INPUTS       : rrc_oam_intra_enb_ho_stats_t    *p_intra_enb_ho_stats
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This function builds intra_enb_ho related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_intra_enb_ho_stats(
      rrc_oam_intra_enb_ho_stats_t    *p_intra_enb_ho_stats, 
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    RRC_OAMH_UT_TRACE_ENTER();

    for ( cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_intra_enb_ho_stats->ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_radioNetwork[cause];

        p_intra_enb_ho_stats->ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_radioNetwork[cause];
    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_intra_enb_ho_stats->ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_transport[cause];
        
        p_intra_enb_ho_stats->ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_transport[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_intra_enb_ho_stats->ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_nas[cause];
        
        p_intra_enb_ho_stats->ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_nas[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_intra_enb_ho_stats->ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_protocol[cause];
        
        p_intra_enb_ho_stats->ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_protocol[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_intra_enb_ho_stats->ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_misc[cause];
        
        p_intra_enb_ho_stats->ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_misc[cause];

    }

    p_intra_enb_ho_stats->ho_IntraEnbOutAtt_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        intra_enb_ho_stats.ho_IntraEnbOutAtt_sum;

    p_intra_enb_ho_stats->ho_IntraEnbOutSucc_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        intra_enb_ho_stats.ho_IntraEnbOutSucc_sum;
  
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_inter_enb_ho_stats 
 *
*   INPUTS        :rrc_oam_inter_enb_ho_stats_t    *p_inter_enb_ho_stats
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This function builds inter_enb_ho related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_inter_enb_ho_stats(
      rrc_oam_inter_enb_ho_stats_t    *p_inter_enb_ho_stats, 
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    RRC_OAMH_UT_TRACE_ENTER();

    for ( cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_inter_enb_ho_stats->ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_radioNetwork[cause];

        p_inter_enb_ho_stats->ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_radioNetwork[cause];
    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_inter_enb_ho_stats->ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_transport[cause];
        
        p_inter_enb_ho_stats->ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_transport[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_inter_enb_ho_stats->ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_nas[cause];
        
        p_inter_enb_ho_stats->ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_nas[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_inter_enb_ho_stats->ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_protocol[cause];
        
        p_inter_enb_ho_stats->ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_protocol[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_inter_enb_ho_stats->ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_misc[cause];
        
        p_inter_enb_ho_stats->ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_misc[cause];

    }

    p_inter_enb_ho_stats->ho_InterEnbOutAtt_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_enb_ho_stats.ho_InterEnbOutAtt_sum;

    p_inter_enb_ho_stats->ho_InterEnbOutSucc_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_enb_ho_stats.ho_InterEnbOutSucc_sum;

    p_inter_enb_ho_stats->ho_InterEnbOutPrepAtt += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_enb_ho_stats.ho_InterEnbOutPrepAtt;
      
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_meas_on_neigh_cell_ho_stats 
 *
*   INPUTS        :rrc_oam_handover_related_stats_t    *p_handover_related_stats
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This function build meas_on_neigh_cell_ho stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_meas_on_neigh_cell_ho_stats(
      rrc_oam_handover_related_stats_t    *p_handover_related_stats, 
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    U8 uecc_target_cell_index = RRC_NULL;
    U8 rrc_target_cell_count = RRC_NULL;
    
    RRC_OAMH_UT_TRACE_ENTER();
    
    if (RRC_NULL == uecc_index)
    {
        p_handover_related_stats->meas_on_neigh_cell_ho_stats_list = 
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            meas_on_neigh_cell_ho_stats_list;
    }
    else
    {
        for ( uecc_target_cell_index = 0;  uecc_target_cell_index < g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
                meas_on_neigh_cell_ho_stats_list.count
                && uecc_target_cell_index < RRC_MAX_CONNECTION; uecc_target_cell_index++)
        {
            for ( rrc_target_cell_count = 0;  (rrc_target_cell_count < 
                    p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.count) && 
                    (rrc_target_cell_count < RRC_MAX_CONNECTION); 
                    rrc_target_cell_count++)
            {
                if(RRC_NULL == memcmp_wrapper(&p_handover_related_stats->
                            meas_on_neigh_cell_ho_stats_list.
                            /*SPR_19166_START*/
                            meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ecgi,
                            &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                            rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                            meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ecgi,
                            sizeof(cell_global_id_eutra_t)))
                            /*SPR_19166_END*/
                {
                    break;
                }
            }
            if ((rrc_target_cell_count == p_handover_related_stats->
                    meas_on_neigh_cell_ho_stats_list.count) && 
                    (RRC_MAX_CONNECTION > rrc_target_cell_count))
            {
                p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.count++;
                l3_memcpy_wrapper (&p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                        /*SPR_19166_START*/
                        meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ecgi,
                        &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                        rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                        meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ecgi,
                        sizeof(cell_global_id_eutra_t)
                        /*SPR_19166_END*/
                        );
            }
            if (RRC_MAX_CONNECTION <= rrc_target_cell_count)
            {/* BUG 12662 Fix Start */
                RRC_OAMH_TRACE(RRC_ERROR, 
                        "Target Cells are more than 32 so unable to handle it");/* BUG 12662 Fix Stop */
                return;
            }

            for ( cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
            {
                p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_radioNetwork[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_radioNetwork[cause];
                
                p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_radioNetwork[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_radioNetwork[cause];
            }

            for ( cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
            {
                p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_transport[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_transport[cause];
                
                p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_transport[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_transport[cause];
            }

            for ( cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
            {
                p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_nas[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_nas[cause];
                
                p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_nas[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_nas[cause];
            }

            for ( cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
            {
                p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_protocol[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_protocol[cause];
                
                p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_protocol[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_protocol[cause];
            }

            for ( cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
            {
                p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_misc[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_misc[cause];
                
                p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_misc[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_misc[cause];
            }
            p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_sum +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_sum;

            p_handover_related_stats->meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_sum +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_sum;
        }
    }
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_inter_freq_ho_stats 
 *
*   INPUTS        :rrc_oam_inter_freq_ho_stats_t    *p_inter_freq_ho_stats
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This function builds inter_freq_ho related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_inter_freq_ho_stats(
      rrc_oam_inter_freq_ho_stats_t    *p_inter_freq_ho_stats, 
      U8 cell_index,
      U8 uecc_index
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    p_inter_freq_ho_stats->ho_InterFreqMeasGapOutAtt += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_freq_ho_stats.ho_InterFreqMeasGapOutAtt;

    p_inter_freq_ho_stats->ho_InterFreqMeasGapOutSucc += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_freq_ho_stats.ho_InterFreqMeasGapOutSucc;

    p_inter_freq_ho_stats->ho_InterFreqNoMeasGapOutAtt += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_freq_ho_stats.ho_InterFreqNoMeasGapOutAtt;

    p_inter_freq_ho_stats->ho_InterFreqNoMeasGapOutSucc += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_freq_ho_stats.ho_InterFreqNoMeasGapOutSucc;
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_drx_non_drx_ho_stats 
 *
*   INPUTS        :rrc_oam_drx_non_drx_ho_stats_t    *p_drx_non_drx_ho_stats
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This function build drx_ho or non_drx_ho related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_build_drx_non_drx_ho_stats(
      rrc_oam_drx_non_drx_ho_stats_t    *p_drx_non_drx_ho_stats, 
      U8 cell_index,
      U8 uecc_index
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    p_drx_non_drx_ho_stats->ho_DrxOutAtt += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        drx_non_drx_ho_stats.ho_DrxOutAtt;

    p_drx_non_drx_ho_stats->ho_DrxOutSucc += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        drx_non_drx_ho_stats.ho_DrxOutSucc;

    p_drx_non_drx_ho_stats->ho_NoDrxOutAtt += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        drx_non_drx_ho_stats.ho_NoDrxOutAtt;

    p_drx_non_drx_ho_stats->ho_NoDrxOutSucc += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        drx_non_drx_ho_stats.ho_NoDrxOutSucc;
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_build_inter_rat_ho_stats 
 *
*   INPUTS        : :rrc_oam_handover_related_stats_t *p_handover_related_stats
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION: This function builds inter_rat_ho related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
/* BUG 12662 Fix Start */
void rrc_oamh_build_inter_rat_ho_stats(
      rrc_oam_handover_related_stats_t    *p_handover_related_stats,/* BUG 12662 Fix Stop */ 
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    /* BUG 12662 Fix Start */
    U8 uecc_target_cell_index = RRC_NULL;
    U8 rrc_target_cell_count = RRC_NULL;
    /* BUG 12662 Fix Stop */
    RRC_OAMH_UT_TRACE_ENTER();

    /* BUG 12662 Fix Start */
    if (RRC_NULL == uecc_index)
    {
    	p_handover_related_stats->inter_rat_ho_stats_list = 
	        g_rrc_stats.p_rrc_stats->
	        rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
    	    rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
	        inter_rat_ho_stats_list;   
    }
    else
    {
    	for ( uecc_target_cell_index = 0;  uecc_target_cell_index < g_rrc_stats.p_rrc_stats->
	    	rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
		    rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
    		inter_rat_ho_stats_list.count &&
	    	uecc_target_cell_index < RRC_MAX_CONNECTION; uecc_target_cell_index++)
	    {
   	        for ( rrc_target_cell_count = 0;  (rrc_target_cell_count <
		        p_handover_related_stats->inter_rat_ho_stats_list.count) &&
    		    (rrc_target_cell_count < RRC_MAX_CONNECTION); 
	    	    rrc_target_cell_count++)
	        {
		
                if(RRC_NULL == memcmp_wrapper(&p_handover_related_stats->
                            inter_rat_ho_stats_list.
                            /*SPR_19166_START*/
                            inter_rat_ho_stats[rrc_target_cell_count].target_cgi,
                            &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                            rrc_handover_related_stats.inter_rat_ho_stats_list.
                            inter_rat_ho_stats[uecc_target_cell_index].target_cgi,
                            sizeof(rrc_inter_rat_global_cgi_t)))
                            /*SPR_19166_END*/
                {
                    break;
                }
	        }
            
	        if ((rrc_target_cell_count == p_handover_related_stats->
                    inter_rat_ho_stats_list.count) && 
                    (RRC_MAX_CONNECTION > rrc_target_cell_count))
            {
                p_handover_related_stats->inter_rat_ho_stats_list.count++;
                l3_memcpy_wrapper (&p_handover_related_stats->inter_rat_ho_stats_list.
                        /*SPR_19166_START*/
                        inter_rat_ho_stats[rrc_target_cell_count].target_cgi,
                        &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                        rrc_handover_related_stats.inter_rat_ho_stats_list.
                        inter_rat_ho_stats[uecc_target_cell_index].target_cgi,
                        sizeof(rrc_inter_rat_global_cgi_t));
                        /*SPR_19166_END*/
            }

            if (RRC_MAX_CONNECTION <= rrc_target_cell_count)
            {
                RRC_OAMH_TRACE(RRC_ERROR, 
                        "Target Cells are more than 32 so unable to handle it");
                return;
            }
    
        for (cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
        {
        	p_handover_related_stats->inter_rat_ho_stats_list.
		        inter_rat_ho_stats[rrc_target_cell_count].
		        ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[cause] += 
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
                inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
		        ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[cause];

        	p_handover_related_stats->inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
	    	    ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[cause] +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
                inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	    	    ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[cause];
        }

        for (cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
        {
        	p_handover_related_stats->inter_rat_ho_stats_list.
		        inter_rat_ho_stats[rrc_target_cell_count]. 
		        ho_IratOutAtt_cause.ho_IratOutAtt_Cause_transport[cause] += 
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
                inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
		        ho_IratOutAtt_cause.ho_IratOutAtt_Cause_transport[cause];

        	p_handover_related_stats->inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
	    	    ho_IratOutSucc_cause.ho_IratOutSucc_Cause_transport[cause] +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
                inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	    	    ho_IratOutSucc_cause.ho_IratOutSucc_Cause_transport[cause];
        }

        for (cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
        {
        	p_handover_related_stats->inter_rat_ho_stats_list.
		        inter_rat_ho_stats[rrc_target_cell_count].
		        ho_IratOutAtt_cause.ho_IratOutAtt_Cause_nas[cause] += 
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
                inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
		        ho_IratOutAtt_cause.ho_IratOutAtt_Cause_nas[cause];

        	p_handover_related_stats->inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
	    	    ho_IratOutSucc_cause.ho_IratOutSucc_Cause_nas[cause] +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
                inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	    	    ho_IratOutSucc_cause.ho_IratOutSucc_Cause_nas[cause];
        }

        for (cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
        {
        	p_handover_related_stats->inter_rat_ho_stats_list.
		        inter_rat_ho_stats[rrc_target_cell_count].
		        ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[cause] += 
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
                inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
		        ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[cause];

        	p_handover_related_stats->inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
	    	    ho_IratOutSucc_cause.ho_IratOutSucc_Cause_protocol[cause] +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
                inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	    	    ho_IratOutSucc_cause.ho_IratOutSucc_Cause_protocol[cause];
        }

        for (cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
        {
        	p_handover_related_stats->inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
	    	    ho_IratOutAtt_cause.ho_IratOutAtt_Cause_misc[cause] += 
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
         	    inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	    	    ho_IratOutAtt_cause.ho_IratOutAtt_Cause_misc[cause];

        	p_handover_related_stats->inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
	    	    ho_IratOutSucc_cause.ho_IratOutSucc_Cause_misc[cause] +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
         	    inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	    	    ho_IratOutSucc_cause.ho_IratOutSucc_Cause_misc[cause];
        }

    	p_handover_related_stats->inter_rat_ho_stats_list.
    		inter_rat_ho_stats[rrc_target_cell_count].ho_IratOutAtt_sum += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        	inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
		    ho_IratOutAtt_sum;

    	p_handover_related_stats->inter_rat_ho_stats_list.
    		inter_rat_ho_stats[rrc_target_cell_count].ho_IratOutSucc_sum += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        	inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	        ho_IratOutSucc_sum;

	    }
    }
    /* BUG 12662 Fix Stop */ 

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_oam_build_intra_freq_and_inter_freq_stats
*
*   DESCRIPTION:
*       This function builds IntraFreq and InterFreq HO related Stats
*
*   RETURNS:
*       None
******************************************************************************/
static void rrc_oam_build_intra_freq_and_inter_freq_stats(
        rrc_oam_get_cell_stats_req_t        *message,
        rrc_oamh_gb_context_t               *p_oamh_gb_context,
        rrc_oam_handover_related_stats_t    *p_handover_related_stats)
{
    U8 uecc_index = 0;
    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    if (message->rrc_handover_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_INTRA_FREQ_HO_RELATED_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                bitmask & RRC_OAMH_INTRA_FREQ_HO_STATS_PRESENT)
        {
            p_handover_related_stats->bitmask |=
                RRC_OAM_INTRA_FREQ_HO_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                p_handover_related_stats->
                    intra_freq_ho_stats.ho_IntraFreqOutAtt += g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[message->cell_index].rrc_handover_related_stats.
                    intra_freq_ho_stats.ho_IntraFreqOutAtt;

                p_handover_related_stats->
                    intra_freq_ho_stats.ho_IntraFreqOutSucc += g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[message->cell_index].rrc_handover_related_stats.
                    intra_freq_ho_stats.ho_IntraFreqOutSucc;
              }
            }
        }   
    }

    if (message->rrc_handover_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_INTER_FREQ_HO_RELATED_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                bitmask & RRC_OAMH_INTER_FREQ_HO_STATS_PRESENT)
        {
            p_handover_related_stats->bitmask |=
                RRC_OAM_INTER_FREQ_HO_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                rrc_oamh_build_inter_freq_ho_stats(&p_handover_related_stats->
                        inter_freq_ho_stats,message->cell_index,uecc_index);
              }
            }
        }
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_oam_build_intra_enb_inter_enb_inter_rat_stats
*
*   DESCRIPTION:
*       This function builds IntraENB, Inter ENB and InterRAT HO related Stats
*
*   RETURNS:
*       None
******************************************************************************/
static void rrc_oam_build_intra_enb_inter_enb_inter_rat_stats(
        rrc_oam_get_cell_stats_req_t        *message,
        rrc_oamh_gb_context_t               *p_oamh_gb_context,
        rrc_oam_handover_related_stats_t    *p_handover_related_stats)
{
    U8 uecc_index = 0;
    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_oamh_gb_context);
    if (message->rrc_handover_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_INTRA_ENB_HO_RELATED_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                bitmask & RRC_OAMH_INTRA_ENB_HO_STATS_PRESENT)
        {
            p_handover_related_stats->bitmask |= 
                RRC_OAM_INTRA_ENB_HO_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                rrc_oamh_build_intra_enb_ho_stats(&p_handover_related_stats->
                        intra_enb_ho_stats,message->cell_index,uecc_index);
              }
            }
        }
    }

    if (message->rrc_handover_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_INTER_ENB_HO_RELATED_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                bitmask & RRC_OAMH_INTER_ENB_HO_STATS_PRESENT)
        {
            p_handover_related_stats->bitmask |=
                RRC_OAM_INTER_ENB_HO_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                rrc_oamh_build_inter_enb_ho_stats(&p_handover_related_stats->
                        inter_enb_ho_stats,message->cell_index,uecc_index);
              }
            }
        }
    }

    if (message->rrc_handover_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_INTER_RAT_HO_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                bitmask & RRC_OAMH_INTER_RAT_HO_STATS_PRESENT)
        {
            p_handover_related_stats->bitmask |=
                RRC_OAM_INTER_RAT_HO_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              { /* BUG 12662 Fix Start */
                rrc_oamh_build_inter_rat_ho_stats(p_handover_related_stats,
                        message->cell_index,uecc_index);
		        /* BUG 12662 Fix Stop */
              }
            }
        }
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_oam_build_handover_stats 
*
*   INPUTS        :rrc_oam_get_cell_stats_req_t        *message
*                  rrc_oamh_gb_context_t               *p_oamh_gb_context
*                  rrc_oam_handover_related_stats_t    *p_handover_related_stats
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function builds handover stats for RRC_OAM_GET_CELL_STATS_RESP
*
*   RETURNS:
*       none
******************************************************************************/
void rrc_oam_build_handover_stats
(
 rrc_oam_get_cell_stats_req_t        *message,
 rrc_oamh_gb_context_t               *p_oamh_gb_context,
 rrc_oam_handover_related_stats_t    *p_handover_related_stats
 )
{
    U8 uecc_index = 0;
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    memset_wrapper(p_handover_related_stats, 0, 
            sizeof(rrc_oam_handover_related_stats_t));

    if (message->rrc_handover_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_HANDOVER_ALL_STATS_PRESENT)
    {
        message->rrc_handover_stats.bitmask |= 
            RRC_OAM_STATISTICS_REQ_INTRA_ENB_HO_RELATED_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_INTER_ENB_HO_RELATED_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_INTRA_FREQ_HO_RELATED_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_INTER_FREQ_HO_RELATED_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_DRX_NON_DRX_HO_RELATED_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_INTER_RAT_HO_STATS_PRESENT |
            RRC_OAM_STATISTICS_REQ_HANDOVER_FAILURE_NUM_STATS_PRESENT;
    }

    rrc_oam_build_intra_enb_inter_enb_inter_rat_stats(
                message, p_oamh_gb_context, p_handover_related_stats);
    rrc_oam_build_intra_freq_and_inter_freq_stats(
                message, p_oamh_gb_context, p_handover_related_stats);

    if (message->rrc_handover_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                bitmask & RRC_OAMH_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT)
        {
            p_handover_related_stats->bitmask |=
                RRC_OAM_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                rrc_oamh_build_meas_on_neigh_cell_ho_stats(p_handover_related_stats,
                        message->cell_index,uecc_index);
              }
            }
              
        }
    }

    if (message->rrc_handover_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_DRX_NON_DRX_HO_RELATED_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                bitmask & RRC_OAMH_DRX_NON_DRX_HO_STATS_PRESENT)
        {
            p_handover_related_stats->bitmask |=
                RRC_OAM_DRX_NON_DRX_HO_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                rrc_oamh_build_drx_non_drx_ho_stats(&p_handover_related_stats->
                        drx_non_drx_ho_stats,message->cell_index,uecc_index);
              }
            }
        }
    }

    if (message->rrc_handover_stats.bitmask &
            RRC_OAM_STATISTICS_REQ_HANDOVER_FAILURE_NUM_STATS_PRESENT)
    {
        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                bitmask & RRC_OAMH_HO_FAILURE_NUM_STATS_PRESENT)
        {
            p_handover_related_stats->bitmask |=
                RRC_OAM_HO_FAILURE_NUM_STATS_PRESENT;
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
              for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
              {
                p_handover_related_stats->ho_failure_num = (U8)(p_handover_related_stats->
                        ho_failure_num + g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[message->cell_index].rrc_handover_related_stats.
                        ho_failure_num);
              }
            }
        }
    }

    RRC_OAMH_UT_TRACE_EXIT();
}


/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_get_cell_status_req
*
*   INPUTS        :void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles RRC_OAM_GET_CELL_STATUS_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_get_cell_status_req
(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_oam_get_cell_status_req_t       in_message;
    rrc_uecc_oamh_get_cell_status_req_t out_message;
    S32 length_read = RRC_NULL;
    /*BUG 8599 Fix start*/
    rrc_oam_get_cell_status_resp_t      out_oam_message;
    U8 response = RRC_SUCCESS;
    /*BUG 8599 Fix stop*/

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm oamh_gb_context=%p", p_oamh_gb_context);        

    /*BUG 8599 Fix start*/
    memset_wrapper(&out_message, 0, sizeof(rrc_uecc_oamh_get_cell_status_req_t));
    memset_wrapper(&out_oam_message, 0, sizeof(rrc_oam_get_cell_status_resp_t));
    /*BUG 8599 Fix stop*/
    
    if(RRC_SUCCESS !=
       rrc_il_parse_rrc_oam_get_cell_status_req(
            &in_message,
            (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    
    if((PNULL != p_oamh_gb_context->p_p_csc_cell_ctx)&&(PNULL != 
                p_oamh_gb_context->p_p_csc_cell_ctx[in_message.cell_index]))
    {
        if(p_oamh_gb_context->p_p_csc_cell_ctx[in_message.cell_index]->
                ready_condition == RRC_FALSE)
        {
            RRC_OAMH_TRACE(RRC_WARNING, "oamh_oam_process_get_cell_stats_req:"
                    "Cell is out of service");
    /*BUG 8599 Fix start*/
                response = RRC_FAILURE;
    /*BUG 8599 Fix stop*/
        }
    }

    /*BUG 8599 Fix start*/
    if (RRC_FAILURE == response)
    {
        out_oam_message.bitmask        = 0;
        out_oam_message.cell_index     = in_message.cell_index;
        out_oam_message.response       = response;

        rrc_oam_il_send_rrc_oam_get_cell_status_resp(
                &out_oam_message,
                RRC_OAMH_MODULE_ID,
                RRC_OAM_MODULE_ID,
                rrc_get_transaction_id(p_api),out_oam_message.cell_index);
    }
    else
    {
    /*BUG 8599 Fix stop*/
        /* Get transaction ID from message */
        out_message.transaction_id = rrc_get_transaction_id(p_api);
        out_message.cell_index     = in_message.cell_index;

        rrc_oamh_send_internal_msg(RRC_UECCMD_MODULE_ID,
                               UECC_OAMH_GET_CELL_STATUS_REQ,
                               sizeof(out_message),
                               &out_message);
    /*BUG 8599 Fix start*/
    }
    /*BUG 8599 Fix stop*/
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_uecc_process_get_cell_status_resp
*
*   INPUTS        :void                  *p_msg
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles UECC_OAMH_GET_CELL_STATUS_RESP msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_uecc_process_get_cell_status_resp
(
    void                  *p_msg,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_uecc_oamh_get_cell_status_resp_t *p_in_message = PNULL;
    rrc_oam_get_cell_status_resp_t       out_message;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm oamh_gb_context=%p", p_oamh_gb_context);        
    memset_wrapper(&out_message, 0, sizeof(out_message));

    /* Precondition - p_msg should point to right message */
    if (rrc_get_api_buf_size(p_msg) !=
        RRC_API_HEADER_SIZE + sizeof(rrc_uecc_oamh_get_cell_status_resp_t))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    p_in_message = (rrc_uecc_oamh_get_cell_status_resp_t *)
                                ((U8*)p_msg + RRC_API_HEADER_SIZE);
    out_message.bitmask    = 0;
    out_message.cell_index = p_in_message->cell_index;
    out_message.response   = p_in_message->response;
    if(RRC_SUCCESS == p_in_message->response)
    {
        if(p_in_message->bitmask & UECC_OAMH_CELL_STATUS_PRESENT)
        {
            out_message.bitmask |= RRC_OAM_CELL_STATUS_PRESENT;
            out_message.cell_status = p_in_message->cell_status;
        }
    }
    rrc_oam_il_send_rrc_oam_get_cell_status_resp(
                        &out_message,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        p_in_message->transaction_id,
                        RRC_NULL);
    RRC_OAMH_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_get_ue_status_req
*
*   INPUTS        :void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles RRC_OAM_GET_UE_STATUS_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_get_ue_status_req
(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_oam_get_ue_status_req_t       in_message;
    rrc_uecc_oamh_get_ue_status_req_t out_message;
    rrc_oam_get_ue_status_resp_t      out_oam_message;
    U8 response = RRC_SUCCESS;
    S32 length_read = RRC_NULL;
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm oamh_gb_context=%p", p_oamh_gb_context);        
    memset_wrapper(&out_oam_message, 0, sizeof(out_oam_message));

    memset_wrapper(&out_message, 0, sizeof(out_message));
    if(RRC_SUCCESS !=
       rrc_il_parse_rrc_oam_get_ue_status_req(
            &in_message,
            (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }

    RRC_OAMH_TRACE(RRC_BRIEF,"[UE_INDEX:%u]", in_message.ue_index);
    
    do{
        if((PNULL == p_oamh_gb_context->p_p_csc_cell_ctx)||
                ((PNULL != p_oamh_gb_context->p_p_csc_cell_ctx)&&(PNULL == 
                    p_oamh_gb_context->p_p_csc_cell_ctx[in_message.cell_index])))
        {
            RRC_OAMH_TRACE(RRC_WARNING,"cell context not found");
            response = RRC_FAILURE;
           
        }
        else
        {
            if(p_oamh_gb_context->p_p_csc_cell_ctx[in_message.cell_index]->
                    ready_condition == RRC_FALSE)
            {
                RRC_OAMH_TRACE(RRC_WARNING, "oamh_oam_process_get_ue_status_req:"
                        "Cell is out of service");
                response = RRC_FAILURE;
                break;
            }

            out_message.bitmask = 0;
            /* Get transaction ID from message */
            out_message.transaction_id = rrc_get_transaction_id(p_api);
            out_message.cell_index     = in_message.cell_index;
            if(in_message.bitmask & RRC_OAM_UE_INDEX_PRESENT)
            {
                out_message.bitmask |= UECC_OAMH_UE_INDEX_PRESENT;
                out_message.ue_index = in_message.ue_index;
                if((RRC_GET_UECC_MODULE_ID(in_message.ue_index) - RRC_UECC_1_MODULE_ID)
                        < p_oamh_gb_context->no_of_uecc_instances)
                {
                    rrc_oamh_send_internal_msg(RRC_GET_UECC_MODULE_ID(in_message.ue_index),
                            UECC_OAMH_GET_UE_STATUS_REQ,
                            sizeof(out_message),
                            &out_message);
                }
                else
                {
                    RRC_OAMH_TRACE(RRC_WARNING,"UECC instance does not exist");
                    response = RRC_FAILURE;
                    break;
                }
            }
            else
            {
                rrc_oamh_send_internal_msg(RRC_UECCMD_MODULE_ID,
                        UECC_OAMH_GET_UE_STATUS_REQ,
                        sizeof(out_message),
                        &out_message);
            }
        } 
    }while(0);

    if (RRC_FAILURE == response)
    {
        out_oam_message.bitmask        = 0;
        out_oam_message.cell_index     = in_message.cell_index;
        out_oam_message.response       = response;

        rrc_oam_il_send_rrc_oam_get_ue_status_resp(
                &out_oam_message,
                RRC_OAMH_MODULE_ID,
                RRC_OAM_MODULE_ID,
                rrc_get_transaction_id(p_api),
                RRC_NULL);
    }    
    
    RRC_OAMH_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: oamh_uecc_process_get_ue_status_resp
*
*   INPUTS        :void                  *p_msg
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles UECC_OAMH_GET_UE_STATUS_RESP msg.
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_uecc_process_get_ue_status_resp
(
    void                  *p_msg,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_uecc_oamh_get_ue_status_resp_t *p_in_message = PNULL;
    rrc_oam_get_ue_status_resp_t       out_message;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm oamh_gb_context=%p", p_oamh_gb_context);

    memset_wrapper(&out_message, 0, sizeof(out_message));

    /* Precondition - p_msg should point to right message */
    if (rrc_get_api_buf_size(p_msg) !=
        RRC_API_HEADER_SIZE + sizeof(rrc_uecc_oamh_get_ue_status_resp_t))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Wrong message size - ignore");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    p_in_message = (rrc_uecc_oamh_get_ue_status_resp_t *)
                                ((U8*)p_msg + RRC_API_HEADER_SIZE);
    out_message.bitmask    = 0;
    out_message.cell_index = p_in_message->cell_index;
    out_message.response   = p_in_message->response;

    if(RRC_SUCCESS == p_in_message->response)
    {
        if(p_in_message->bitmask & UECC_OAMH_UE_STATUS_LIST_PRESENT)
        {
            out_message.bitmask |= RRC_OAM_UE_STATUS_PRESENT;
            out_message.ue_status_list = p_in_message->ue_status_list;
        }
    }
    rrc_oam_il_send_rrc_oam_get_ue_status_resp(
                        &out_message,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        p_in_message->transaction_id,
                        RRC_NULL);
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_reset_cell_stats_req
*
*   INPUTS        :void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles RRC_OAM_RESET_CELL_STATS_REQ msg.
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_reset_cell_stats_req
(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_oam_reset_cell_stats_req_t        in_message;
    rrc_oam_reset_cell_stats_resp_t       out_message;
    S32 length_read = RRC_NULL;
    U16 time_duration = RRC_NULL;
    rrc_bool_t reset_cell_stats_resp = RRC_SUCCESS;
    U8 uecc_index = 0;
    /* Bug 11748 Fix Start */
    U32 rrcConnectionNumber = RRC_NULL;
    rrc_uecc_oam_stat_erab_number_t erab_sim_num_stats;
    /* Bug 11748 Fix Stop */
    /*SPR_17462_START*/
    U8 target_index = RRC_NULL;
    rrc_oam_meas_on_neigh_cell_ho_stats_list_t  
        *p_meas_on_neigh_cell_ho_target_id_list = PNULL;
    rrc_oam_inter_rat_ho_stats_list_t
        *p_inter_rat_ho_target_id_list = PNULL;
    /*SPR_17462_END*/

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    memset_wrapper(&out_message, 0, sizeof(out_message));

    /* Bug 11748 Fix Start */
    memset_wrapper ( &erab_sim_num_stats, 0, 
            sizeof(rrc_uecc_oam_stat_erab_number_t) );
    /* Bug 11748 Fix Stop */

    if(RRC_SUCCESS !=
            rrc_il_parse_rrc_oam_reset_cell_stats_req(
                &in_message,
                (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Bad message - ignore.");
        reset_cell_stats_resp = RRC_FAILURE;
    }
    
    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
        /*SPR_17462_START*/
        p_meas_on_neigh_cell_ho_target_id_list = 
            rrc_mem_get(sizeof(rrc_oam_meas_on_neigh_cell_ho_stats_list_t));
        if(PNULL == p_meas_on_neigh_cell_ho_target_id_list)
        {
            RRC_OAMH_TRACE(RRC_FATAL, "%s: Memory allocation failed for "
                    "p_meas_on_neigh_cell_ho_target_id_list.", __FUNCTION__);
            return;
        }
        /* Initialize count with 0 */
        p_meas_on_neigh_cell_ho_target_id_list->count = 0;

        p_inter_rat_ho_target_id_list = 
            rrc_mem_get(sizeof(rrc_oam_inter_rat_ho_stats_list_t));
        if(PNULL == p_inter_rat_ho_target_id_list )
        {
            RRC_OAMH_TRACE(RRC_FATAL, "%s: Memory allocation failed for "
                    "p_inter_rat_ho_target_id_list.", __FUNCTION__);
            rrc_mem_free(p_meas_on_neigh_cell_ho_target_id_list);
            p_meas_on_neigh_cell_ho_target_id_list = PNULL;
            return;
        }
        /* Initialize count with 0 */
        p_inter_rat_ho_target_id_list->count = 0;
        /*SPR_17462_END*/
        for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
      {
        if ((MAX_NUM_CELLS > in_message.cell_index) &&
                (RRC_TRUE == g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                 rrc_uecc_instance_stats[uecc_index].
                 rrc_uecc_cell_stats[in_message.cell_index].
                 is_cell_configured))
        {
            /* Bug 11748 Fix Start */
            rrcConnectionNumber = g_rrc_stats.p_rrc_stats->rrc_uecc_stats. 
                rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[in_message.cell_index].
                rrc_connection_related_stats.rrcConnectionNumber;
            /* Bug 11748 Fix Stop */

            /*resetting global shared memory */
            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[in_message.cell_index].
                    rrc_connection_related_stats, 0,
                    sizeof(rrc_oam_connection_related_stats_t));

            /* Bug 11748 Fix Start */
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
               rrc_uecc_cell_stats[in_message.cell_index].rrc_connection_related_stats.
               rrcConnectionNumber = rrcConnectionNumber;
            /* Bug 11748 Fix Stop */

            if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                    is_rrc_connection_counter_enable == 1)
            {
                if (RRC_NULL != p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_RRC_CONNECTION_SAMPLING_TIMER])
                {
                    time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                            timers_ids[OAMH_RRC_CONNECTION_SAMPLING_TIMER]);
                    p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                        is_reset_done[in_message.cell_index] = RRC_TRUE;
                    p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                        time_remaining_till_reset[in_message.cell_index] = time_duration;
                }
            }

            /* Bug 11748 Fix Start */
            l3_memcpy_wrapper (&erab_sim_num_stats, &g_rrc_stats.
                    p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[in_message.cell_index].
                    rrc_erab_related_stats.erab_sim_num_stats, 
                    sizeof(rrc_uecc_oam_stat_erab_number_t));
            /* Bug 11748 Fix Stop */

            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[in_message.cell_index].
                    rrc_erab_related_stats, 0,
                    sizeof(rrc_oam_erab_related_stats_t));

            /* Bug 11748 Fix Start */
            l3_memcpy_wrapper (&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[in_message.cell_index].
                    rrc_erab_related_stats.erab_sim_num_stats, &erab_sim_num_stats,
                    sizeof(rrc_uecc_oam_stat_erab_number_t));
            /* Bug 11748 Fix Stop */

            if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                    is_erab_counter_enable == 1)
            {
                if (RRC_NULL != p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_ERAB_SAMPLING_TIMER])
                {
                    time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                            timers_ids[OAMH_ERAB_SAMPLING_TIMER]);
                    p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                        is_reset_done[in_message.cell_index] = RRC_TRUE;
                    p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                        time_remaining_till_reset[in_message.cell_index] = time_duration;            
                }
            }

            /*SPR_17462_START*/
            if(0 != g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[in_message.cell_index].
                    rrc_handover_related_stats.
                    meas_on_neigh_cell_ho_stats_list.count)
            {
                l3_memcpy_wrapper(p_meas_on_neigh_cell_ho_target_id_list,
                        &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message.cell_index].
                        rrc_handover_related_stats.
                        meas_on_neigh_cell_ho_stats_list,
                        sizeof(rrc_oam_meas_on_neigh_cell_ho_stats_list_t));
            }
            if(0 != g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[in_message.cell_index].
                    rrc_handover_related_stats.
                    inter_rat_ho_stats_list.count)
            {
                l3_memcpy_wrapper(p_inter_rat_ho_target_id_list,
                        &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[in_message.cell_index].
                        rrc_handover_related_stats.
                        inter_rat_ho_stats_list,
                        sizeof(rrc_oam_inter_rat_ho_stats_list_t));
            }
            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[in_message.cell_index].
                    rrc_handover_related_stats, 0,
                    sizeof(rrc_oam_handover_related_stats_t));
            if(0 != p_meas_on_neigh_cell_ho_target_id_list->count)
            {
                /* SPR 17885 Fix Start */
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                  rrc_uecc_instance_stats[uecc_index].
                  rrc_uecc_cell_stats[in_message.cell_index].
                  rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                  count = p_meas_on_neigh_cell_ho_target_id_list->count;
                /* SPR 17885 Fix Stop */
                for(target_index = 0; 
                        target_index < p_meas_on_neigh_cell_ho_target_id_list->count;
                        target_index++)
                {
                    l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[in_message.cell_index].
                            rrc_handover_related_stats.
                            meas_on_neigh_cell_ho_stats_list.
                            /*SPR_19166_START*/
                            meas_on_neigh_cell_ho_stats[target_index].ecgi,
                            &p_meas_on_neigh_cell_ho_target_id_list->
                            meas_on_neigh_cell_ho_stats[target_index].ecgi,
                            sizeof(cell_global_id_eutra_t));
                            /*SPR_19166_END*/
                }
                p_meas_on_neigh_cell_ho_target_id_list->count = 0;
            }
            if(0 != p_inter_rat_ho_target_id_list->count)
            {
                /* SPR 17885 Fix Start */
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                  rrc_uecc_instance_stats[uecc_index].
                  rrc_uecc_cell_stats[in_message.cell_index].
                  rrc_handover_related_stats.inter_rat_ho_stats_list.
                  count = p_inter_rat_ho_target_id_list->count;
                /* SPR 17885 Fix Stop */
                for(target_index = 0; 
                        target_index < p_inter_rat_ho_target_id_list->count;
                        target_index++)
                {
                    l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[in_message.cell_index].
                            rrc_handover_related_stats.
                            inter_rat_ho_stats_list.
                            /*SPR_19166_START*/
                            inter_rat_ho_stats[target_index].target_cgi,
                            &p_inter_rat_ho_target_id_list->
                            inter_rat_ho_stats[target_index].target_cgi,
                            sizeof(rrc_inter_rat_global_cgi_t));
                            /*SPR_19166_END*/
                }
                p_inter_rat_ho_target_id_list->count = 0;
            }
            /*SPR_17462_END*/

            if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                    is_handover_counter_enable == 1)
            {
                if (RRC_NULL != p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_HANDOVER_SAMPLING_TIMER])
                {
                    time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                            timers_ids[OAMH_HANDOVER_SAMPLING_TIMER]);
                    p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                        is_reset_done[in_message.cell_index] = RRC_TRUE;
                    p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
                        time_remaining_till_reset[in_message.cell_index] = time_duration;            
                }
            }

            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[in_message.cell_index].
                    rrc_ue_assoc_s1_conn_related_stats, 0,
                    sizeof(rrc_uecc_oam_stat_ue_assoc_s1_connection_t));

            if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                    is_ue_assoc_s1_conn_counter_enable == 1)
            {
                if (RRC_NULL != p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER])
                {
                    time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                            timers_ids[OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER]);
                    p_oamh_gb_context->rrc_oamh_stats_status.rrc_ue_assoc_s1_conn_stats_status.
                        is_reset_done[in_message.cell_index] = RRC_TRUE;
                    p_oamh_gb_context->rrc_oamh_stats_status.rrc_ue_assoc_s1_conn_stats_status.
                        time_remaining_till_reset[in_message.cell_index] = time_duration;            
                }
            }

            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[in_message.cell_index].
                rrc_IratIncMobility_LAI = 0;

            if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                    is_IratIncMobility_LAI_counter_enable == 1)
            {
                if (RRC_NULL != p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER])
                {
                    time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                            timers_ids[OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER]);
                    p_oamh_gb_context->rrc_oamh_stats_status.rrc_IratIncMobility_LAI_stats_status.
                        is_reset_done[in_message.cell_index] = RRC_TRUE;
                    p_oamh_gb_context->rrc_oamh_stats_status.rrc_IratIncMobility_LAI_stats_status.
                        time_remaining_till_reset[in_message.cell_index] = time_duration;            
                }
            }
        }
        else
        {
            RRC_OAMH_TRACE(RRC_ERROR, "Bad message: Cell is not configured"
                    " Invalid cell index");
            reset_cell_stats_resp = RRC_FAILURE;
        }
    }
    /*SPR_17462_START*/
    rrc_mem_free(p_meas_on_neigh_cell_ho_target_id_list);
    p_meas_on_neigh_cell_ho_target_id_list = PNULL;
    rrc_mem_free(p_inter_rat_ho_target_id_list);
    p_inter_rat_ho_target_id_list = PNULL;
    /*SPR_17462_END*/

    if ((MAX_NUM_CELLS > in_message.cell_index) && 
            (RRC_TRUE == g_rrc_stats.p_rrc_stats->rrc_csc_stats.
            rrc_csc_cell_stats[in_message.cell_index].is_cell_configured))
    {
        memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_csc_stats.
                rrc_csc_cell_stats[in_message.cell_index].rrc_paging_related_stats,
                0, sizeof(rrc_csc_oam_stat_paging_t));

        if (p_oamh_gb_context->rrc_oamh_stats_status.m.
                is_paging_counter_enable == 1)
        {
            if (RRC_NULL != p_oamh_gb_context->timers_data.
                    timers_ids[OAMH_PAGING_SAMPLING_TIMER])
            {
                time_duration = rrc_timer_now(p_oamh_gb_context->timers_data.
                        timers_ids[OAMH_PAGING_SAMPLING_TIMER]);
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_paging_stats_status.
                    is_reset_done[in_message.cell_index] = RRC_TRUE;
                p_oamh_gb_context->rrc_oamh_stats_status.rrc_paging_stats_status.
                    time_remaining_till_reset[in_message.cell_index] = time_duration;            
            }
        }
      }
      else
      {
        RRC_OAMH_TRACE(RRC_ERROR, "Bad message: Cell is not configured"
                " Invalid cell index");
        reset_cell_stats_resp = RRC_FAILURE;
      }
    }
    out_message.cell_index = in_message.cell_index;
    out_message.response   = reset_cell_stats_resp;

    rrc_oam_il_send_rrc_oam_reset_cell_stats_resp(
            &out_message,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            rrc_get_transaction_id(p_api),
            RRC_NULL);

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_uecc_process_cell_traffic_trace_start_error_ind
*
*   INPUTS        :void                  *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles UECC_OAMH_CELL_TRAFFIC_TRACE_START_ERROR_IND msg.
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_uecc_process_cell_traffic_trace_start_error_ind
(
    void                  *p_msg
)
{
    rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t *p_in_message = PNULL;
    rrc_oam_cell_traffic_trace_start_error_ind_t    out_message;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);

    memset_wrapper(&out_message, RRC_NULL, sizeof(out_message));
    /* Precondition - p_msg should point to right message */
    if (rrc_get_api_buf_size(p_msg) !=
        RRC_API_HEADER_SIZE + 
        sizeof(rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    p_in_message = (rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t *)
                                ((U8*)p_msg + RRC_API_HEADER_SIZE);
    out_message.cell_index = p_in_message->cell_index;
    out_message.cause = p_in_message->cause;

    rrc_oam_il_send_rrc_oam_cell_traffic_trace_start_error_ind(
                        &out_message,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        RRC_NULL,   /* Transaction id is not received in cell traffic start msg */
                        RRC_NULL);

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_uecc_process_trace_file_transfer_start_ind
*
*   INPUTS        :void                  *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles UECC_OAMH_TRACE_FILE_TRANSFER_START_IND msg.
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_uecc_process_trace_file_transfer_start_ind
(
    void                  *p_msg
)
{
    rrc_uecc_oamh_trace_file_transfer_start_ind_t *p_in_message = PNULL;
    rrc_oam_trace_file_transfer_start_ind_t    out_message;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);

    memset_wrapper(&out_message, RRC_NULL, sizeof(out_message));

    /* Precondition - p_msg should point to right message */
    if (rrc_get_api_buf_size(p_msg) !=
        RRC_API_HEADER_SIZE + 
        sizeof(rrc_uecc_oamh_trace_file_transfer_start_ind_t))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    p_in_message = (rrc_uecc_oamh_trace_file_transfer_start_ind_t *)
                                ((U8*)p_msg + RRC_API_HEADER_SIZE);

    /* Copy p_in_message into out_message */
    l3_memcpy_wrapper(&out_message, p_in_message, 
        sizeof(rrc_oam_trace_file_transfer_start_ind_t));

    rrc_oam_il_send_rrc_oam_trace_file_transfer_start_ind(
                        &out_message,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        RRC_NULL, /* Transaction id is not received in cell traffic start msg */
                        RRC_NULL);
    RRC_OAMH_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: oamh_csc_process_cell_start_ind
*
*   INPUTS        :void                  *p_msg
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles CSC_OAMH_CELL_START_IND msg.
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_csc_process_cell_start_ind
(
    void                  *p_msg,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_csc_oamh_cell_start_ind_t *p_cell_start = PNULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    /* Precondition - p_msg should point to right message */
    if (rrc_get_api_buf_size(p_msg) !=
        RRC_API_HEADER_SIZE + sizeof(rrc_csc_oamh_cell_start_ind_t))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    p_cell_start = (rrc_csc_oamh_cell_start_ind_t *)
                                ((U8*)p_msg + RRC_API_HEADER_SIZE);
    if ( (p_oamh_gb_context->p_p_csc_cell_ctx != PNULL) &&
            p_oamh_gb_context->p_p_csc_cell_ctx[p_cell_start->
            cell_index] != PNULL)
    {
        p_oamh_gb_context->p_p_csc_cell_ctx[p_cell_start->cell_index]->
            ready_condition = RRC_TRUE;   
    }
    
    RRC_OAMH_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: oamh_csc_process_cell_stop_ind
*
*   INPUTS        :void                  *p_msg
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles CSC_OAMH_CELL_STOP_IND msg.
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_csc_process_cell_stop_ind
(
    void                  *p_msg,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_csc_oamh_cell_stop_ind_t *p_in_message = PNULL;
    
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);


    /* Precondition - p_msg should point to right message */
    if (rrc_get_api_buf_size(p_msg) !=
        RRC_API_HEADER_SIZE + sizeof(rrc_csc_oamh_cell_stop_ind_t))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    p_in_message = (rrc_csc_oamh_cell_stop_ind_t *)
                                ((U8*)p_msg + RRC_API_HEADER_SIZE);

    if ( (p_oamh_gb_context->p_p_csc_cell_ctx != PNULL) &&
            p_oamh_gb_context->p_p_csc_cell_ctx[p_in_message->
            cell_index] != PNULL)
    {
        p_oamh_gb_context->p_p_csc_cell_ctx[p_in_message->cell_index]->
            ready_condition = RRC_FALSE;
    }
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_uecc_process_trace_start_error_ind
*
*   INPUTS        :void                  *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles UECC_OAMH_TRACE_START_ERROR_IND msg.
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_uecc_process_trace_start_error_ind 
(
    void                  *p_msg
)
{
/* SPR 21773 Start */
    rrc_uecc_oamh_trace_start_error_ind_t *p_in_message = PNULL;
    rrc_oam_trace_start_error_ind_t    *p_out_message;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);

    p_out_message= rrc_mem_get(sizeof(rrc_oam_trace_start_error_ind_t));
    if(PNULL == p_out_message )
    {
        RRC_OAMH_TRACE(RRC_FATAL, "Memory allocation failed for "
                "p_out_message");
        return;
    }

    /* Precondition - p_msg should point to right message */
    if (rrc_get_api_buf_size(p_msg) !=
        RRC_API_HEADER_SIZE + 
        sizeof(rrc_uecc_oamh_trace_start_error_ind_t))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        rrc_mem_free(p_out_message);
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    p_in_message = (rrc_uecc_oamh_trace_start_error_ind_t *)
                                ((U8*)p_msg + RRC_API_HEADER_SIZE);

    /* Copy p_in_message into p_out_message */
    l3_memcpy_wrapper(p_out_message, (U8*)p_in_message + sizeof(rrc_transaction_id_t), 
        sizeof(rrc_oam_trace_start_error_ind_t));

    rrc_oam_il_send_rrc_oam_trace_start_error_ind(
                        p_out_message,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        p_in_message->transaction_id,
                        RRC_NULL); 
    rrc_mem_free(p_out_message);
    /* SPR 21773 End */
    RRC_OAMH_UT_TRACE_EXIT();
}

/* SPR 22584 Changes - Starts */
/******************************************************************************
*   FUNCTION NAME: oamh_build_and_send_start_trace_error_indication
*
*   INPUTS       : cell_index_t - Cell Index
*                   rrc_trace_start_error_cause_et - Error Cause
*   OUTPUTS      : None
*   DESCRIPTION  : This function prepare
*                   RRC_OAM_CELL_TRAFFIC_TRACE_START_ERROR_IND message
*                   as per received cause.
*   RETURNS      : None
******************************************************************************/
static void oamh_build_and_send_start_trace_error_indication
(
    rrc_cell_index_t                cell_index,
    rrc_trace_start_error_cause_et  cause
)
{
    rrc_oam_cell_traffic_trace_start_error_ind_t    out_message;

    RRC_OAMH_UT_TRACE_ENTER();
    //RRC_ASSERT(PNULL != p_msg);

    out_message.cell_index = cell_index;
    out_message.cause = cause;

    rrc_oam_il_send_rrc_oam_cell_traffic_trace_start_error_ind(
        &out_message,
        RRC_OAMH_MODULE_ID,
        RRC_OAM_MODULE_ID,
        RRC_NULL,
        RRC_NULL);

    RRC_OAMH_UT_TRACE_EXIT();
}
/* SPR 22584 Changes - Ends */

/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_cell_traffic_trace_start
*
*   INPUTS        :void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles RRC_OAM_CELL_TRAFFIC_TRACE_START msg.
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_cell_traffic_trace_start
(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_oam_cell_traffic_trace_start_t          in_message;
    rrc_uecc_oamh_cell_traffic_trace_start_t    out_message;
    S32 length_read = RRC_NULL;
    /* SPR 22584 Changes - Starts */
    rrc_trace_start_error_cause_et  cause = RRC_INVALID_CAUSE;
    /* SPR 22584 Changes - Ends */

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);
    if(PNULL == p_oamh_gb_context->p_p_csc_cell_ctx)
    {
        RRC_OAMH_TRACE(RRC_WARNING, "cell context not found");
        return;
    }

    memset_wrapper(&out_message, 0, sizeof(out_message));
    if(RRC_SUCCESS !=
       rrc_il_parse_rrc_oam_cell_traffic_trace_start(
            &in_message,
            (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }

    /* SPR 22584 Changes - Starts */
    do
    {
        if((p_oamh_gb_context->p_p_csc_cell_ctx[in_message.cell_index] == PNULL)
            || (p_oamh_gb_context->p_p_csc_cell_ctx[in_message.cell_index]->
                    ready_condition == RRC_FALSE)) 
        {
            RRC_OAMH_TRACE(RRC_WARNING, "Cell := %u is OUT of SERVICE. "
                "Sending Trace Start Error Indication to OAM",
                in_message.cell_index);
            cause = RRC_CELL_INACTIVE;
            break;
        }

        /* Get transaction ID from message */
        out_message.transaction_id = rrc_get_transaction_id(p_api);
        /* Copy in_message send to UECC */
        l3_memcpy_wrapper(((U8*)&out_message) + sizeof(U16), &in_message, 
                sizeof(rrc_oam_cell_traffic_trace_start_t));
        rrc_oamh_send_internal_msg(RRC_UECCMD_MODULE_ID,
                UECC_OAMH_CELL_TRAFFIC_TRACE_START,
                sizeof(out_message),
                &out_message);
    } while(0);

    if(RRC_INVALID_CAUSE != cause)
    {
        oamh_build_and_send_start_trace_error_indication(
            in_message.cell_index, cause);
    }
    /* SPR 22584 Changes - Ends */
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_cell_traffic_trace_stop
*
*   INPUTS        :void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles RRC_OAM_CELL_TRAFFIC_TRACE_STOP msg.
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_cell_traffic_trace_stop
(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_oam_cell_traffic_trace_stop_t   in_message;
    rrc_uecc_oamh_cell_traffic_trace_stop_t out_message;
    S32 length_read = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);
    if(PNULL == p_oamh_gb_context->p_p_csc_cell_ctx)
    {
        RRC_OAMH_TRACE(RRC_WARNING, "cell context not found");
        return;
    }
    /* Reset Buffer */
    memset_wrapper(&out_message, RRC_NULL, sizeof(out_message));

    if(RRC_SUCCESS !=
       rrc_il_parse_rrc_oam_cell_traffic_trace_stop(
            &in_message,
            (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    if(p_oamh_gb_context->p_p_csc_cell_ctx[in_message.cell_index] == 
        PNULL)
    {
        RRC_OAMH_TRACE(RRC_WARNING, "oamh_oam_process_cell_traffic_trace_stop:"
                       "Cell context does not exists.. ignore");
        return;
    }
    if(p_oamh_gb_context->p_p_csc_cell_ctx[in_message.cell_index]->
        ready_condition == RRC_FALSE)
    {
        RRC_OAMH_TRACE(RRC_WARNING, "oamh_oam_process_cell_traffic_trace_stop:"
                       "Cell is out of service");
        return;
    }
    /* Copy message send to UECC */
    l3_memcpy_wrapper(&out_message, &in_message, sizeof(rrc_oam_cell_traffic_trace_stop_t));

    rrc_oamh_send_internal_msg(RRC_UECCMD_MODULE_ID,
                               UECC_OAMH_CELL_TRAFFIC_TRACE_STOP,
                               sizeof(out_message),
                               &out_message);
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_trace_file_transfer_complete_ind
*
*   INPUTS        :void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function handles RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND msg.
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_trace_file_transfer_complete_ind
(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    rrc_oam_trace_file_transfer_complete_ind_t  in_message;
    rrc_uecc_oamh_trace_file_transfer_complete_ind_t out_message;
    S32 length_read = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);
    if(PNULL == p_oamh_gb_context->p_p_csc_cell_ctx)
    {
        RRC_OAMH_TRACE(RRC_WARNING, "cell context not found");
        return;
    }
    /* Reset Buffer */
    memset_wrapper(&out_message, RRC_NULL, sizeof(out_message));

    if(RRC_SUCCESS !=
       rrc_il_parse_rrc_oam_trace_file_transfer_complete_ind(
            &in_message,
            (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    if(p_oamh_gb_context->p_p_csc_cell_ctx[in_message.cell_index]->
        ready_condition == RRC_FALSE)
    {
        RRC_OAMH_TRACE(RRC_WARNING, "oamh_oam_process_trace_file_transfer_complete_ind:"
                       "Cell is out of service");
        return;
    }
    /* Copy message send to UECC */
    l3_memcpy_wrapper(&out_message, &in_message, sizeof(rrc_oam_trace_file_transfer_complete_ind_t));

    rrc_oamh_send_internal_msg(RRC_UECCMD_MODULE_ID,
                               UECC_OAMH_TRACE_FILE_TRANSFER_COMPLETE_IND,
                               sizeof(out_message),
                               &out_message);
    RRC_OAMH_UT_TRACE_EXIT();
}

/*##################### END OF MSG HANDLERS ################################*/

/******************************************************************************
*   FUNCTION NAME: rrc_oamh_init
*
*   INPUTS        :void                  *p_validate_comm_info_int
*   OUTPUTS      : None
*   DESCRIPTION:
*       This is initialisation function of the OAMH module.
*
*   RETURNS:
*       void * (Oamh global context is returned)
******************************************************************************/
void* rrc_oamh_init
(
     void *p_validate_comm_info_int   /* Pointer on initial param */
)
{
    rrc_oamh_gb_context_t   *p_oamh_gb_context = PNULL;
    comm_info_init_param_t  *comm_info_init_param = PNULL;
    rrc_return_et           result = RRC_FAILURE;


    RRC_OAMH_UT_TRACE_ENTER();
    RRC_OAMH_TRACE(RRC_DETAILED, "Init.");
    comm_info_init_param = (comm_info_init_param_t *)p_validate_comm_info_int;

    p_oamh_gb_context = rrc_mem_get(sizeof(rrc_oamh_gb_context_t));

    if(PNULL == p_oamh_gb_context)
    {
        RRC_OAMH_TRACE(RRC_FATAL, "rrc_oamh_init: unable to Allocate Memory "
            "for OAMH global context!");
        return PNULL;
    }
    else
    {
        memset_wrapper(p_oamh_gb_context, 0, sizeof(rrc_oamh_gb_context_t));
        p_oamh_gb_context->oam_license_timer = PNULL;

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
        /* Initialise logging parameters to default values */
        p_oamh_gb_context->log_level    = RRC_BRIEF;
        p_oamh_gb_context->log_enabled  = RRC_TRUE;
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */
        oamh_reset_context(p_oamh_gb_context);
        OAMH_FSM_INIT(p_oamh_gb_context);
        p_oamh_gb_context->p_comm_info_int = (comm_info_t *)&(comm_info_init_param->comm_info);
    }
    
    result = init_shared_mem_stats();
    if (RRC_SUCCESS != result)
    {
        RRC_OAMH_TRACE(RRC_WARNING, "rrc_oamh_init:Unable to initialize Shared Memory");
    }

#ifdef LICENSE_FLAG 
    /* Starting Licensing timer */
    oamh_timer_start(OAMH_LICENSE_TIMER,
            (U32)comm_info_init_param->expiry_duration,
            &p_oamh_gb_context->timers_data);


/*    guard_timer_data.auto_delete    = RRC_TRUE;
    guard_timer_data.timer_type     = OAMH_LICENSE_TIMER;
    p_oamh_gb_context->oam_license_timer = rrc_start_timer(
                      (U32)comm_info_init_param->expiry_duration,
                      &guard_timer_data,
                      sizeof (guard_timer_data),
                      RRC_FALSE);
*/
    /* End of code */                  
#endif
    RRC_OAMH_UT_TRACE_EXIT();

    return p_oamh_gb_context;
}

/******************************************************************************
*   FUNCTION NAME: rrc_oamh_process_msg
*
*   INPUTS        :void                  *p_api
*                  void                  *p_gl_ctx
*   OUTPUTS      : None
*   DESCRIPTION:
*       This is entry point function of the OAMH module. This function receives
*       messages from external interfaces, validates and pass it to
*       respective FSM.
*
*   RETURNS:
*       RRC_BUFFER_SHOULD_BE_RELEASED (1) -
                            Encapsulating shell should release the buffer
******************************************************************************/
S32 rrc_oamh_process_msg
(
    void  *p_api,
    void  *p_gl_ctx
)
{
    rrc_oamh_gb_context_t *p_oamh_gb_context = PNULL;

    RRC_OAMH_UT_TRACE_ENTER();

    /* RRC_OAMH_TRACE(RRC_INFO, RRC_MSG_RECVD); */

    if ( (PNULL == p_api) || (PNULL == p_gl_ctx) )
    {
        RRC_OAMH_TRACE(RRC_ERROR, "rrc_oamh_process_msg: somebody call OAMH in"
            " wrong context");
        RRC_UT_TRACE_EXIT();
        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }

    if (RRC_SUCCESS != rrc_check_cspl_header(p_api))
    {
        RRC_OAMH_TRACE(RRC_WARNING,"rrc_oamh_process_msg: bad message header -"
            " ignore msg");
        RRC_UT_TRACE_EXIT();
        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }

    p_oamh_gb_context = (rrc_oamh_gb_context_t *)p_gl_ctx;

    /* Find appropriate handler for incoming message */
    oamh_dispatch_message(p_api, p_oamh_gb_context);

    RRC_OAMH_UT_TRACE_EXIT();

    /* Encapsulating shell should release the buffer */
    return RRC_BUFFER_SHOULD_BE_RELEASED;
}

/******************************************************************************
*   FUNCTION NAME: rrc_oamh_process_timer_msg
*
*   INPUTS        :rrc_timer_t timer_id
*                  void        *p_timer_buf
*                  void        *p_gl_ctx
*   OUTPUTS      : None
*   DESCRIPTION:
*       This is entry point function for timer expiry events
*
*   RETURNS:
*       None
******************************************************************************/
void rrc_oamh_process_timer_msg
(
    rrc_timer_t timer_id,
    void        *p_timer_buf,
    void        *p_gl_ctx
)
{
    /* only one timer currently supported */
    rrc_oam_init_ind_t oam_init_ind;

    oamh_timer_buf_t *p_oamh_timer_buf = PNULL;
    rrc_oamh_gb_context_t *p_oamh_gb_context = PNULL;
    /* ARGUSED */
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm timer_id=%p", timer_id);        

    /* Precondition check */
    if ((PNULL == p_timer_buf) || (PNULL == p_gl_ctx))
    {
        RRC_OAMH_TRACE(RRC_ERROR, "rrc_oamh_process_timer_msg: somebody call "
            "us in wrong context");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }

/* Added code for handling license timer expiry */
    p_oamh_timer_buf = (oamh_timer_buf_t *)p_timer_buf;
    p_oamh_gb_context = (rrc_oamh_gb_context_t *)p_gl_ctx;

    switch (p_oamh_timer_buf->timer_type)
    {
#ifdef LICENSE_FLAG
        case OAMH_LICENSE_TIMER:
             RRC_OAMH_TRACE(RRC_INFO, "LICENSE EXPIRED, PLEASE CONTACT ARICENT SUPPORT.");
            
             ALARM_MSG_L3(RRC_MODULE_ID, RRC_LICENSE_EXPIRY_ALARM_ID, MINOR_ALARM,INVALID_ERR_CAUSE_ID);
             
             exit_wrapper(0);
             break;
#endif
        case OAMH_RRC_CONNECTION_SAMPLING_TIMER:
             RRC_OAMH_TRACE(RRC_BRIEF, "rrc_oamh_process_timer_msg: OAMH_RRC_CONNECTION_SAMPLING_TIMER expired.");
             rrc_oamh_connection_timer_expiry_handler(p_oamh_gb_context);
             break;

        case OAMH_ERAB_SAMPLING_TIMER:
             RRC_OAMH_TRACE(RRC_BRIEF, "rrc_oamh_process_timer_msg: OAMH_ERAB_SAMPLING_TIMER expired.");
             rrc_oamh_erab_timer_expiry_handler(p_oamh_gb_context);
             break;

        case OAMH_HANDOVER_SAMPLING_TIMER:
             RRC_OAMH_TRACE(RRC_BRIEF, "rrc_oamh_process_timer_msg: OAMH_HANDOVER_SAMPLING_TIMER expired.");
             rrc_oamh_handover_timer_expiry_handler(p_oamh_gb_context);
             break;

        case OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER:
             RRC_OAMH_TRACE(RRC_BRIEF, "rrc_oamh_process_timer_msg: OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER expired.");
             rrc_oamh_ue_assoc_s1_conn_timer_expiry_handler(p_oamh_gb_context);
             break;

        case OAMH_PAGING_SAMPLING_TIMER:
             RRC_OAMH_TRACE(RRC_BRIEF, "rrc_oamh_process_timer_msg: OAMH_PAGING_SAMPLING_TIMER expired.");
             rrc_oamh_paging_timer_expiry_handler(p_oamh_gb_context);
             break;

        case OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER:
             RRC_OAMH_TRACE(RRC_BRIEF, "rrc_oamh_process_timer_msg: OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER expired.");
             rrc_oamh_common_la_overlap_irat_timer_expiry_handler(p_oamh_gb_context);
             break;

        case OAMH_HEALTH_MONITOR_TIMER:
             RRC_OAMH_TRACE(RRC_BRIEF, "rrc_oamh_process_timer_msg: OAMH_HEALTH_MONITOR_TIMER expired.");
             rrc_oamh_process_health_monitor_chk(p_oamh_gb_context);
             break;
        /*valgrind_fix start*/
        case OAMH_INIT_TIMER:
             rrc_oam_il_send_rrc_oam_init_ind(&oam_init_ind,
                     RRC_OAMH_MODULE_ID,
                     RRC_OAM_MODULE_ID,
                     RRC_TRANSACTION_ID_ABSENT,
                     RRC_NULL);
             RRC_OAMH_TRACE(RRC_INFO, " Init indication sent to OAM.");
	     break;
	/*valgrind_fix stop*/
        default :
        /*valgrind_fix start*/
             RRC_OAMH_TRACE(RRC_WARNING, "Invalid Timer ID %d", p_oamh_timer_buf->timer_type);
	/*valgrind_fix stop*/

    }
/* End of code*/

/*SPR 18714 Fix Start*/
    if (RRC_TRUE == p_oamh_timer_buf->auto_delete)
    {
        if (PNULL != p_timer_buf)
        {
            rrc_mem_free(p_timer_buf);
            p_timer_buf = PNULL;
        }
    }
/*SPR 18714 Fix Stop*/

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_dispatch_message
*
*   INPUTS        :void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function parse the API header and call appropriate action handler
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_dispatch_message
(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    U16 app_id = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);
    /* Call appropriate message parser according to the source of the API
     * received and get a event id */
    app_id = rrc_get_api_id(p_api);

    switch(rrc_get_src_module_id(p_api))
    {
    case RRC_OAM_MODULE_ID:
        oamh_oam_dispatch_message(
            p_api,
            app_id,
            p_oamh_gb_context);
        break;

    case RRC_CSC_MODULE_ID:
        oamh_csc_dispatch_message(
            p_api,
            app_id,
            p_oamh_gb_context);
        break;

    case RRC_UECC_MODULE_ID:
        oamh_uecc_dispatch_message(
             p_api,
             app_id,
             p_oamh_gb_context);
        break;

    case RRC_UECCMD_MODULE_ID:
        oamh_ueccmd_dispatch_message(
             p_api,
             app_id,
             p_oamh_gb_context);
        break;

    case RRC_LLIM_MODULE_ID:
        oamh_llim_dispatch_message(
             p_api,
             app_id,
             p_oamh_gb_context);
        break;

    default:
        RRC_OAMH_TRACE(RRC_WARNING, "oamh_dispatch_message:"
                       "API from unexpected module received");
        break;
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_dispatch_message
*
*   INPUTS        :void                  *p_api
*                  U16                   api_id
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function parse the API header and calls appropriate handler
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_dispatch_message
(
    void                  *p_api,
    U16                   api_id,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    /*p_msg - this buffer will be auto-released by CSPL*/
    void *p_msg     = PNULL;
    S32 length_read = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    switch (api_id)
    {
    case RRC_OAM_LOG_ENABLE_REQ:
        oamh_oam_process_log_enable_req(p_api, p_oamh_gb_context);
        break;
    case RRC_OAM_SET_LOG_LEVEL_REQ:
        oamh_oam_process_set_log_level_req(p_api, p_oamh_gb_context);
        break;
/*SPR 13115 Start*/
    case RRC_OAM_GET_LOG_LEVEL_REQ:
        oamh_oam_process_get_log_level_req(p_api, p_oamh_gb_context);
        break;
/*SPR 13115 Stop*/
    case RRC_OAM_GET_CELL_STATS_REQ:
        oamh_oam_process_get_cell_stats_req(p_api, p_oamh_gb_context);
        break;

    case RRC_OAM_GET_CELL_STATUS_REQ:
        oamh_oam_process_get_cell_status_req(p_api, p_oamh_gb_context);
        break;

    case RRC_OAM_GET_UE_STATUS_REQ:
        oamh_oam_process_get_ue_status_req(p_api, p_oamh_gb_context);
        break;

    case RRC_OAM_RESET_CELL_STATS_REQ:
        oamh_oam_process_reset_cell_stats_req(p_api, p_oamh_gb_context);
        break;

    case RRC_OAM_CELL_TRAFFIC_TRACE_START:
        oamh_oam_process_cell_traffic_trace_start(p_api,p_oamh_gb_context);
        break;

    case RRC_OAM_CELL_TRAFFIC_TRACE_STOP:
        oamh_oam_process_cell_traffic_trace_stop(p_api,p_oamh_gb_context);
        break;

    case RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND:
        oamh_oam_process_trace_file_transfer_complete_ind(p_api,p_oamh_gb_context);
        break;

    case RRC_OAM_CLEANUP_REQ:
        /* Get transaction ID from message */
        p_oamh_gb_context->cleanup_req_trans_id =
                                             rrc_get_transaction_id(p_api);
        oamh_fsm_process_event(p_oamh_gb_context,
                      OAMH_EVENT_OAM_CLEANUP_REQ,
                      api_id,/*coverity fix start*/
                      p_api);/*coverity fix stop*/
        break;

    case RRC_OAM_COMMUNICATION_INFO_REQ:
        /* Get transaction ID from message */
        p_oamh_gb_context->comm_req_trans_id = rrc_get_transaction_id(p_api);

        p_msg = rrc_mem_get(sizeof(rrc_oam_communication_info_req_t));
        if(PNULL == p_msg)
        {
            RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
            return;
        }

        if (PNULL != p_msg)
        {
            /*valgrind_fix start*/
            memset_wrapper(p_msg, 0, sizeof(rrc_oam_communication_info_req_t));
            /*valgrind_fix start*/
            /* Decode the recieved message */
            if( RRC_SUCCESS ==
                    rrc_il_parse_rrc_oam_communication_info_req(
                        (rrc_oam_communication_info_req_t *)p_msg,
                        (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                        rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                        &length_read))
            {
                oamh_fsm_process_event(p_oamh_gb_context,
                        OAMH_EVENT_OAM_COMM_INFO_REQ,
                        api_id,
                        p_msg);
            }
        }
        /*valgrind_fix start*/
        if (PNULL != p_msg)
        {
            rrc_mem_free(p_msg);
	    /* + Coverity 81479 */
            //p_msg = PNULL;
	    /* - Coverity 81479 */
        }
        /*valgrind_fix stop*/
        break;

    case RRC_OAM_PROVISION_REQ:
        /* Get transaction ID from message */
        p_oamh_gb_context->prov_req_trans_id = rrc_get_transaction_id(p_api);

        p_msg = rrc_mem_get(sizeof(rrc_oam_provision_req_t));
        if(PNULL == p_msg)
        {
            RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
            return;
        }

        if (PNULL != p_msg)
        {
		/*valgrind_fix start*/
            memset_wrapper(p_msg, 0, sizeof(rrc_oam_provision_req_t));
            /*valgrind_fix start*/
            /* Decode the recieved message */
            if( RRC_SUCCESS ==
                    rrc_il_parse_rrc_oam_provision_req(
                        (rrc_oam_provision_req_t *)p_msg,
                        (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                        rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                        &length_read))
            {
                oamh_fsm_process_event(p_oamh_gb_context,
                                       OAMH_EVENT_OAM_PROV_REQ,
                                       api_id,
                                       p_msg);
            }
            else
            {

              ALARM_MSG_L3(RRC_MODULE_ID, RRC_PROVISIONING_FAILURE_ALARM_ID,
                      MAJOR_ALARM,INVALID_ERR_CAUSE_ID);
            }
        }

        /*valgrind_fix start*/
        if (PNULL != p_msg)
        {
            rrc_mem_free(p_msg);
	    /* + Coverity 81479 */
            //p_msg = PNULL;
	    /* - Coverity 81479 */
        }
        /*valgrind_fix stop*/
        break;

    case RRC_OAM_CONFIG_STATS_REQ:
        oamh_oam_process_config_stats_req(p_api, p_oamh_gb_context);
        break;

    case RRC_OAM_PROC_SUPP_REQ:
        oamh_oam_process_proc_req(p_oamh_gb_context);/*SPR 17777 +-*/
        break; 

   /*lipa Start*/
    case RRC_OAM_ADD_LGW_REQ:
        p_oamh_gb_context->lgw_req_trans_id = rrc_get_transaction_id(p_api);
        p_msg = rrc_mem_get(sizeof(rrc_oam_add_lgw_req_t));
        if(PNULL == p_msg)
        {
            RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
            return;
        }
        if (PNULL != p_msg)
        {
            /*valgrind_fix start*/
            memset_wrapper(p_msg, 0, sizeof(rrc_oam_add_lgw_req_t));
            /*valgrind_fix start*/

            /* Decode the recieved message */
            if( RRC_SUCCESS ==
                    rrc_il_parse_rrc_oam_add_lgw_req(
                        (rrc_oam_add_lgw_req_t *)p_msg,
                        (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                        rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                        &length_read))
            {
                oamh_fsm_process_event(p_oamh_gb_context,
                        OAMH_EVENT_OAM_ADD_LGW_REQ,
                        api_id,
                        p_msg);
            }
        }
        /*valgrind_fix start*/
        if (PNULL != p_msg)
        {
            rrc_mem_free(p_msg);
	    /* + Coverity 81479 */
            //p_msg = PNULL;
	    /* - Coverity 81479 */
        }
        /*valgrind_fix stop*/
 
        break;

    case RRC_OAM_DELETE_LGW_REQ:
        p_oamh_gb_context->lgw_req_trans_id = rrc_get_transaction_id(p_api);
        p_msg = rrc_mem_get(sizeof(rrc_oam_delete_lgw_req_t));
        if(PNULL == p_msg)
        {
            RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
            return;
        }
        if (PNULL != p_msg)
        {
            /*valgrind_fix start*/
            memset_wrapper(p_msg, 0, sizeof(rrc_oam_delete_lgw_req_t));
            /*valgrind_fix start*/

            /* Decode the recieved message */
            if( RRC_SUCCESS ==
                    rrc_il_parse_rrc_oam_delete_lgw_req(
                        (rrc_oam_delete_lgw_req_t *)p_msg,
                        (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                        rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                        &length_read))
            {
                oamh_fsm_process_event(p_oamh_gb_context,
                        OAMH_EVENT_OAM_DEL_LGW_REQ,
                        api_id,
                        p_msg);
            }
        }
        /*valgrind_fix start*/
        if (PNULL != p_msg)
        {
            rrc_mem_free(p_msg);
	    /* + Coverity 81479 */
            //p_msg = PNULL;
	    /* - Coverity 81479 */
        }
        /*valgrind_fix stop*/
 
        break;
        /*lipa end*/

    /*spr 22474 start*/
    case RRC_OAM_GET_DEBUG_INFO_REQ: 
        oamh_oam_process_get_debug_info_req(p_api, p_oamh_gb_context);
    /*spr 22474 end*/
    default:
        RRC_OAMH_TRACE(RRC_WARNING, "oamh_oam_dispatch_message:"
                "unexpected API[%u]from OAM module received", api_id);
        break;
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_csc_dispatch_message
* 
*   INPUTS        :void                  *p_api
*                  U16                   api_id
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function parse the API header and calls appropriate event handler
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_csc_dispatch_message
(
    void                  *p_api,
    U16                   api_id,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    oamh_event_et event_id = OAMH_INVALID_EVENT;
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    switch(api_id)
    {
    case CSC_OAMH_INIT_IND:
        /* Check for blank API */
        event_id = OAMH_EVENT_CSC_INIT_IND;
        break;

    case CSC_OAMH_PROV_RESP:
        event_id = OAMH_EVENT_CSC_PROV_RESP;
        break;

    case CSC_OAMH_CLEANUP_RESP:
        event_id = OAMH_EVENT_CSC_CLEANUP_RESP;
        break;

    case CSC_OAMH_CELL_START_IND:
        oamh_csc_process_cell_start_ind(p_api,
                p_oamh_gb_context);
        break;

    case CSC_OAMH_CELL_STOP_IND:
        oamh_csc_process_cell_stop_ind(p_api,
                p_oamh_gb_context);
        break;
   /* SPR 20908 Start */ 
    case CSC_OAMH_OAM_COMMUNICATION_INFO_IND:
        event_id = OAMH_EVENT_CSC_COMMUNICATION_INFO_IND;
        break;
   /* SPR 20908 End */ 
    default:
        RRC_OAMH_TRACE(RRC_WARNING, "oamh_csc_dispatch_message:"
                "unexpected API from CSC module received");
        break;
    }

    if(OAMH_INVALID_EVENT != event_id)
    {
        oamh_fsm_process_event(p_oamh_gb_context,
                               event_id,
                               api_id,
                               p_api);
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_uecc_dispatch_message
*
*   INPUTS        :void                  *p_api
*                  U16                   api_id
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function parse the API header and calls appropriate event handler
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_uecc_dispatch_message
(
    void                  *p_api,
    U16                   api_id,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    switch(api_id)
    {
    case UECC_OAMH_GET_CELL_STATUS_RESP:
        oamh_uecc_process_get_cell_status_resp(p_api, p_oamh_gb_context);
        break;

    case UECC_OAMH_GET_UE_STATUS_RESP:
        oamh_uecc_process_get_ue_status_resp(p_api, p_oamh_gb_context);
        break;

    case UECC_OAMH_CELL_TRAFFIC_TRACE_START_ERROR_IND:
        oamh_uecc_process_cell_traffic_trace_start_error_ind(p_api);
        break;

    case UECC_OAMH_TRACE_FILE_TRANSFER_START_IND:
        oamh_uecc_process_trace_file_transfer_start_ind(p_api);
        break;

    case UECC_OAMH_TRACE_START_ERROR_IND:
        oamh_uecc_process_trace_start_error_ind(p_api);
        break;
    default:
        RRC_OAMH_TRACE(RRC_WARNING, "oamh_uecc_dispatch_message:"
                       "unexpected API from UECC module received");
        break;
    }

    RRC_OAMH_UT_TRACE_EXIT();
}


/******************************************************************************
*   FUNCTION NAME: oamh_ueccmd_dispatch_message
*
*   INPUTS        :void                  *p_api
*                  U16                   api_id
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function parse the API header and calls appropriate event handler
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_ueccmd_dispatch_message
(
    void                  *p_api,
    U16                   api_id,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    oamh_event_et event_id = OAMH_INVALID_EVENT;
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    switch(api_id)
    {
    case UECCMD_OAMH_INIT_IND:
        event_id = OAMH_EVENT_UECCMD_INIT_IND;
        break;

    case UECCMD_OAMH_PROV_RESP:
        event_id = OAMH_EVENT_UECCMD_PROV_RESP;
        break;

    case UECCMD_OAMH_CLEANUP_RESP:
        event_id = OAMH_EVENT_UECCMD_CLEANUP_RESP;
        break;

    default:
        RRC_OAMH_TRACE(RRC_WARNING, "oamh_ueccmd_dispatch_message:"
                       "unexpected API from UECC module received");
        break;
    }

    if(OAMH_INVALID_EVENT != event_id)
    {
        oamh_fsm_process_event(p_oamh_gb_context,
                               event_id,
                               api_id,
                               p_api);
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_llim_dispatch_message
*
*   INPUTS        :void                  *p_api
*                  U16                   api_id
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function parse the API header and calls appropriate event handler
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_llim_dispatch_message(
    void                  *p_api,
    U16                   api_id,
    rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    oamh_event_et   event_id = OAMH_INVALID_EVENT;
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    switch(api_id)
    {
    case LLIM_OAMH_INIT_IND:
        /* Check for blank API */
        event_id = OAMH_EVENT_LLIM_INIT_IND;
        break;

    case LLIM_OAMH_PROV_RESP:
        event_id = OAMH_EVENT_LLIM_PROV_RESP;
        break;

    case LLIM_OAMH_CLEANUP_RESP:
        event_id = OAMH_EVENT_LLIM_CLEANUP_RESP;
        break;

    default:
        RRC_OAMH_TRACE(RRC_WARNING, "oamh_llim_dispatch_message:"
                       "unexpected API from LLIM module received");
        break;
    }
    if(OAMH_INVALID_EVENT != event_id)
    {
        oamh_fsm_process_event(p_oamh_gb_context,
                               event_id,
                               api_id,
                               p_api);
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: oamh_timer_start 
 *
*   INPUTS        :oamh_timer_id_et        oamh_timer_id
*                  U32                     timer_duration
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function starts the OAMH timers
 *       
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 ******************************************************************************/
rrc_return_t oamh_timer_start(
        oamh_timer_id_et        oamh_timer_id,
        U32                     timer_duration,
        rrc_oamh_gb_context_t   *p_oamh_gb_context)
{
    rrc_return_t result = RRC_FAILURE;
    oamh_timer_buf_t repeated_timer_buf;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);
    RRC_ASSERT(OAMH_TIMER_LAST > oamh_timer_id);

    repeated_timer_buf.timer_type = oamh_timer_id;
    repeated_timer_buf.auto_delete = RRC_FALSE;
    repeated_timer_buf.target_submodule = OAMH_SUBMODULE;

    switch(oamh_timer_id)
    {
        case OAMH_LICENSE_TIMER:
            repeated_timer_buf.auto_delete = RRC_TRUE;

            p_oamh_gb_context->timers_data.
            timers_ids[oamh_timer_id]= rrc_start_timer(
                    timer_duration,
                    &repeated_timer_buf,
                    sizeof (oamh_timer_buf_t),
                    RRC_FALSE);

            p_oamh_gb_context->oam_license_timer = p_oamh_gb_context->timers_data.
                timers_ids[oamh_timer_id];

            p_oamh_gb_context->timers_data.timers_ids[oamh_timer_id] =
                p_oamh_gb_context->oam_license_timer;
            break;

        case OAMH_PAGING_SAMPLING_TIMER:

            p_oamh_gb_context->timers_data.
                timers_ids[oamh_timer_id] = rrc_start_timer(
                        timer_duration,
                        &repeated_timer_buf,
                        sizeof(oamh_timer_buf_t),
                        RRC_TRUE);
            break;       

        case OAMH_RRC_CONNECTION_SAMPLING_TIMER:

            p_oamh_gb_context->timers_data.
                timers_ids[oamh_timer_id] = rrc_start_timer(
                        timer_duration,
                        &repeated_timer_buf,
                        sizeof(oamh_timer_buf_t),
                        RRC_TRUE);
            break;       

        case OAMH_ERAB_SAMPLING_TIMER:

            p_oamh_gb_context->timers_data.
                timers_ids[oamh_timer_id] = rrc_start_timer(
                        timer_duration,
                        &repeated_timer_buf,
                        sizeof(oamh_timer_buf_t),
                        RRC_TRUE);
            break;      

        case OAMH_HANDOVER_SAMPLING_TIMER:

            p_oamh_gb_context->timers_data.
                timers_ids[oamh_timer_id] = rrc_start_timer(
                        timer_duration,
                        &repeated_timer_buf,
                        sizeof(oamh_timer_buf_t),
                        RRC_TRUE);
            break;       

        case OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER:

            p_oamh_gb_context->timers_data.
                timers_ids[oamh_timer_id] = rrc_start_timer(
                        timer_duration,
                        &repeated_timer_buf,
                        sizeof(oamh_timer_buf_t),
                        RRC_TRUE);
            break;       

        case OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER:

            p_oamh_gb_context->timers_data.
                timers_ids[oamh_timer_id] = rrc_start_timer(
                        timer_duration,
                        &repeated_timer_buf,
                        sizeof(oamh_timer_buf_t),
                        RRC_TRUE);
            break;      

        default :
            RRC_OAMH_TRACE(RRC_FATAL, "Invalid Timer ID , Cannot start timer");
    }

    if (oamh_timer_id < OAMH_TIMER_LAST)
    {
        if (PNULL == p_oamh_gb_context->timers_data.timers_ids[oamh_timer_id])
        {
            RRC_OAMH_TRACE(RRC_ERROR,"Timer starting error for timer ID [%d]",
                oamh_timer_id);
        }
        else
        {
            result = RRC_SUCCESS;
        }
    }

    RRC_OAMH_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 *   FUNCTION NAME: start_oamh_health_monitor_timer
 *
 *   DESCRIPTION:
 *       Starts OAMH health monitor timer if duration specified by caller
 *       greater than zero
 *
 *   RETURNS: None
 ****************************************************************************/

void start_oamh_health_monitor_timer
(
    rrc_oamh_gb_context_t   *p_oamh_gb_context,
    U32                     duration
)
{
    oamh_timer_buf_t repeated_timer_buf;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    if(duration == 0)
    {
        return;
    }
    repeated_timer_buf.timer_type = OAMH_HEALTH_MONITOR_TIMER;
    repeated_timer_buf.auto_delete = RRC_FALSE;
    repeated_timer_buf.target_submodule = OAMH_SUBMODULE;

    p_oamh_gb_context->timers_data.
        timers_ids[OAMH_HEALTH_MONITOR_TIMER] = rrc_start_timer(
                duration,
                &repeated_timer_buf,
                sizeof(oamh_timer_buf_t),
                RRC_TRUE);

    RRC_OAMH_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_stop_performance_related_timers 
 *
*   INPUTS         : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function stops performance related timers
 *
 *   RETURNS:
 *       None
 ******************************************************************************/
void rrc_oamh_stop_performance_related_timers
(
 rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    if (PNULL != p_oamh_gb_context->timers_data.
            timers_ids[OAMH_PAGING_SAMPLING_TIMER])
    {   
        rrc_stop_timer(p_oamh_gb_context->timers_data.
                timers_ids[OAMH_PAGING_SAMPLING_TIMER]);
    }
    
    if (PNULL != p_oamh_gb_context->timers_data.
            timers_ids[OAMH_RRC_CONNECTION_SAMPLING_TIMER])
    {
        rrc_stop_timer(p_oamh_gb_context->timers_data.
                timers_ids[OAMH_RRC_CONNECTION_SAMPLING_TIMER]);
    }

    if (PNULL != p_oamh_gb_context->timers_data.
            timers_ids[OAMH_ERAB_SAMPLING_TIMER])
    {
        rrc_stop_timer(p_oamh_gb_context->timers_data.
                timers_ids[OAMH_ERAB_SAMPLING_TIMER]);
    }

    if (PNULL != p_oamh_gb_context->timers_data.
            timers_ids[OAMH_HANDOVER_SAMPLING_TIMER])
    {
        rrc_stop_timer(p_oamh_gb_context->timers_data.
                timers_ids[OAMH_HANDOVER_SAMPLING_TIMER]);

    }

    if (PNULL != p_oamh_gb_context->timers_data.
            timers_ids[OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER])
    {
        rrc_stop_timer(p_oamh_gb_context->timers_data.
                timers_ids[OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER]);
    }

    if (PNULL != p_oamh_gb_context->timers_data.
            timers_ids[OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER])
    {
        rrc_stop_timer(p_oamh_gb_context->timers_data.
                timers_ids[OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER]);
    }

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_non_periodic_reporting_stats
*
*   DESCRIPTION:
*       This function processes Non Periodic Reporting Stats
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_non_periodic_reporting_stats(
        rrc_oam_config_stats_req_t        *in_message,
        rrc_oamh_gb_context_t *p_oamh_gb_context)
{
    RRC_OAMH_UT_TRACE_ENTER();
	RRC_ASSERT(PNULL != p_oamh_gb_context);
	if (in_message->bitmask & RRC_OAM_RRC_CONNECTION_RELATED_STATS_PRESENT)
	{
		p_oamh_gb_context->rrc_oamh_stats_status.m.
			is_rrc_connection_counter_enable = 1;
		p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
			bitmask = in_message->rrc_connection_stats.bitmask;
	}
	if (in_message->bitmask & RRC_OAM_ERAB_RELATED_STATS_PRESENT)
	{
		p_oamh_gb_context->rrc_oamh_stats_status.m.
			is_erab_counter_enable = 1;
		p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
			bitmask = in_message->rrc_erab_stats.bitmask;
	}
	if (in_message->bitmask & RRC_OAM_HANDOVER_RELATED_STATS_PRESENT)
	{
		p_oamh_gb_context->rrc_oamh_stats_status.m.
			is_handover_counter_enable = 1;
		p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
			bitmask = in_message->rrc_handover_stats.bitmask;
	}
	if (in_message->bitmask & RRC_OAM_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT)
	{
		p_oamh_gb_context->rrc_oamh_stats_status.m.
			is_ue_assoc_s1_conn_counter_enable = 1;
	}
	if (in_message->bitmask & RRC_OAM_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT)
	{
		p_oamh_gb_context->rrc_oamh_stats_status.m.
			is_IratIncMobility_LAI_counter_enable = 1;
	}
	if (in_message->bitmask & RRC_OAM_PAGING_RELATED_STATS_PRESENT)
	{
		p_oamh_gb_context->rrc_oamh_stats_status.m.
			is_paging_counter_enable = 1;
	}
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_process_periodic_reporting_stats
*
*   DESCRIPTION:
*       This function processes Periodic Reporting Stats
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_process_periodic_reporting_stats(
        rrc_oam_config_stats_req_t        *in_message,
        rrc_oamh_gb_context_t *p_oamh_gb_context,
        rrc_return_t          *config_stats_resp)
{
    rrc_return_t result = RRC_FAILURE;

    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    /*Starting the respective timers for the stats requested in the
     * incoming mesage */
    if (in_message->bitmask & RRC_OAM_PAGING_RELATED_STATS_PRESENT)
    {
	    result = oamh_timer_start(OAMH_PAGING_SAMPLING_TIMER,
			    ((in_message->rrc_paging_stats.sampling_period)*
                            RRC_CONVERT_TO_MILLISEC),
			    p_oamh_gb_context);

	    if (RRC_SUCCESS == result)
	    {
		    RRC_OAMH_TRACE(RRC_INFO,"OAMH_PAGING_SAMPLING_TIMER Started");
		    p_oamh_gb_context->rrc_oamh_stats_status.m.
			    is_paging_counter_enable = 1;
		    p_oamh_gb_context->rrc_oamh_stats_status.rrc_paging_stats_status.
			    sampling_period = in_message->rrc_paging_stats.sampling_period;
	    }
	    else
	    {
		    RRC_OAMH_TRACE(RRC_ERROR,"OAMH_PAGING_SAMPLING_TIMER Starting error");
		    *config_stats_resp = RRC_FAILURE;
		    return;
	    }
    }

    if (in_message->bitmask & RRC_OAM_RRC_CONNECTION_RELATED_STATS_PRESENT)
    {
	    result =  oamh_timer_start(OAMH_RRC_CONNECTION_SAMPLING_TIMER,
			    ((in_message->rrc_connection_stats.sampling_period)*
                            RRC_CONVERT_TO_MILLISEC),
			    p_oamh_gb_context);

	    if (RRC_SUCCESS != result)
	    {
		    RRC_OAMH_TRACE(RRC_ERROR, "OAMH_RRC_CONNECTION_SAMPLING_TIMER Starting error");
		    *config_stats_resp = RRC_FAILURE;
		    return;
	    }
	    else
	    {
		    RRC_OAMH_TRACE(RRC_INFO, "OAMH_RRC_CONNECTION_SAMPLING_TIMER Started");
		    p_oamh_gb_context->rrc_oamh_stats_status.m.
			    is_rrc_connection_counter_enable = 1;
		    p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
			    bitmask = in_message->rrc_connection_stats.bitmask;
		    p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
			    sampling_period = in_message->rrc_connection_stats.sampling_period;
	    }
    }

    if (in_message->bitmask & RRC_OAM_ERAB_RELATED_STATS_PRESENT)
    {
	    result =  oamh_timer_start(OAMH_ERAB_SAMPLING_TIMER,
			    ((in_message->rrc_erab_stats.sampling_period)*
                            RRC_CONVERT_TO_MILLISEC),
			    p_oamh_gb_context);

	    if (RRC_SUCCESS != result)
	    {
		    RRC_OAMH_TRACE(RRC_ERROR, "OAMH_ERAB_SAMPLING_TIMER Starting error");
		    *config_stats_resp = RRC_FAILURE;
		    return;
	    }    
	    else
	    {
		    RRC_OAMH_TRACE(RRC_INFO, "OAMH_ERAB_SAMPLING_TIMER Started");
		    p_oamh_gb_context->rrc_oamh_stats_status.m.
			    is_erab_counter_enable = 1;
		    p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
			    bitmask = in_message->rrc_erab_stats.bitmask;
		    p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
			    sampling_period = in_message->rrc_erab_stats.sampling_period;
	    }
    }

    if (in_message->bitmask & RRC_OAM_HANDOVER_RELATED_STATS_PRESENT)
    {
	    result =  oamh_timer_start(OAMH_HANDOVER_SAMPLING_TIMER,
			    ((in_message->rrc_handover_stats.sampling_period)*
                            RRC_CONVERT_TO_MILLISEC),
			    p_oamh_gb_context);

	    if (RRC_SUCCESS != result)
	    {
		    RRC_OAMH_TRACE(RRC_ERROR, "OAMH_HANDOVER_SAMPLING_TIMER Starting error");
		    *config_stats_resp = RRC_FAILURE;
		    return;
	    }
	    else
	    {
		    RRC_OAMH_TRACE(RRC_INFO, "OAMH_HANDOVER_SAMPLING_TIMER Started");
		    p_oamh_gb_context->rrc_oamh_stats_status.m.
			    is_handover_counter_enable = 1;
		    p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
			    bitmask = in_message->rrc_handover_stats.bitmask;
		    p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
			    sampling_period = in_message->rrc_handover_stats.sampling_period;
	    }
    }

    if (in_message->bitmask & RRC_OAM_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT)
    {
	    result =  oamh_timer_start(OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER,
			    ((in_message->rrc_ue_assoc_s1_conn_stats.sampling_period)*
                            RRC_CONVERT_TO_MILLISEC),
			    p_oamh_gb_context);

	    if (RRC_SUCCESS != result)
	    {
		    RRC_OAMH_TRACE(RRC_ERROR, "OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER Starting error");
		    *config_stats_resp = RRC_FAILURE;
		    return;
	    }
	    else
	    {
		    RRC_OAMH_TRACE(RRC_INFO, "OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER Started");
		    p_oamh_gb_context->rrc_oamh_stats_status.m.
			    is_ue_assoc_s1_conn_counter_enable = 1;
		    p_oamh_gb_context->rrc_oamh_stats_status.
			    rrc_ue_assoc_s1_conn_stats_status.sampling_period = in_message->
			    rrc_ue_assoc_s1_conn_stats.sampling_period;
	    }
    }

    if (in_message->bitmask & RRC_OAM_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT)
    {
	    result =  oamh_timer_start(OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER,
			    ((in_message->rrc_IratIncMobility_LAI_stats.sampling_period)*
                            RRC_CONVERT_TO_MILLISEC),
			    p_oamh_gb_context);

	    if (RRC_SUCCESS != result)
	    {
		    RRC_OAMH_TRACE(RRC_ERROR, "OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER Starting error");
		    *config_stats_resp = RRC_FAILURE;
		    return;
	    }
	    else
	    {
		    RRC_OAMH_TRACE(RRC_INFO, "OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER Started");
		    p_oamh_gb_context->rrc_oamh_stats_status.m.
			    is_IratIncMobility_LAI_counter_enable = 1;
		    p_oamh_gb_context->rrc_oamh_stats_status.
			    rrc_IratIncMobility_LAI_stats_status.sampling_period = 
			    in_message->rrc_IratIncMobility_LAI_stats.sampling_period;
	    }
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: oamh_oam_process_config_stats_req 
 *
*   INPUTS        :void                  *p_api
*                  rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function handles RRC_OAM_CONFIG_STATS_REQ msg
 *
 *   RETURNS:
 *       None
 ******************************************************************************/
void oamh_oam_process_config_stats_req
(
 void                  *p_api,
 rrc_oamh_gb_context_t *p_oamh_gb_context
 )
{
    rrc_oam_config_stats_req_t          in_message;
    rrc_oam_config_stats_resp_t         in_message_resp;
    rrc_return_t config_stats_resp = RRC_SUCCESS; 
    U8 index = RRC_NULL;
    U8 uecc_index = RRC_NULL;
    /*SPR_17462_START*/
    U8 target_index = RRC_NULL;
    rrc_oam_meas_on_neigh_cell_ho_stats_list_t  
        *p_meas_on_neigh_cell_ho_target_id_list = PNULL;
    rrc_oam_inter_rat_ho_stats_list_t
        *p_inter_rat_ho_target_id_list = PNULL;
    /*SPR_17462_END*/

    S32 length_read = RRC_NULL;
    /* Bug 11748 Fix Start */
    U32 rrcConnectionNumber = RRC_NULL;
    rrc_uecc_oam_stat_erab_number_t erab_sim_num_stats;
    /* Bug 11748 Fix Stop */

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);

    memset_wrapper(&in_message_resp, 0,
            sizeof(rrc_oam_config_stats_resp_t));
    /* Bug 11748 Fix Start */
    memset_wrapper ( &erab_sim_num_stats, 0, 
            sizeof(rrc_uecc_oam_stat_erab_number_t) );
    /* Bug 11748 Fix Stop */

    if(RRC_SUCCESS !=
            rrc_il_parse_rrc_oam_config_stats_req(
                &in_message,
                (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Bad message - ignore.");
        in_message_resp.response = RRC_FAILURE;
/* SPR:9279 start */
        rrc_oam_il_send_rrc_oam_config_stats_resp(
                &in_message_resp,
                RRC_OAMH_MODULE_ID,
                RRC_OAM_MODULE_ID,
                rrc_get_transaction_id(p_api),
                RRC_NULL);
/* SPR:9279 stop */

        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
   
    /*checking the bitmasks in the incoming message and sending 
     * failure in response if it is invalid */ 
    in_message.bitmask = (in_message.bitmask & 
            ((RRC_OAM_RRC_CONNECTION_RELATED_STATS_PRESENT)|
             (RRC_OAM_ERAB_RELATED_STATS_PRESENT)|
             (RRC_OAM_HANDOVER_RELATED_STATS_PRESENT)|
             (RRC_OAM_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT)|
             (RRC_OAM_PAGING_RELATED_STATS_PRESENT)|
             (RRC_OAM_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT)));

    if (in_message.bitmask == RRC_NULL)
    {
        RRC_OAMH_TRACE(RRC_ERROR,"Switching Off All The Statistics");
        config_stats_resp = RRC_SUCCESS;
    }
    else
    {
        if (in_message.bitmask & RRC_OAM_RRC_CONNECTION_RELATED_STATS_PRESENT)
        {
            in_message.rrc_connection_stats.bitmask = (in_message.
                    rrc_connection_stats.bitmask & 
                    ((RRC_OAM_STATISTICS_REQ_CONNECTION_ESTB_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_CONNECTION_RE_ESTB_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_CONNECTION_NUMBER_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_CONNECTION_SETUP_TIME_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_CONNECTION_UE_CTX_REL_CONFIG_STATS_PRESENT)));

            if (in_message.rrc_connection_stats.bitmask == RRC_NULL)
            {
                RRC_OAMH_TRACE(RRC_ERROR," Invalid bitmask in "
                        "RRC_OAM_CONFIG_STATS_REQ for RRC CONNECTION RELATED_STATS ");
                config_stats_resp = RRC_FAILURE;
            }
        }

        if (in_message.bitmask & RRC_OAM_ERAB_RELATED_STATS_PRESENT)
        {
            in_message.rrc_erab_stats.bitmask = (in_message.rrc_erab_stats.
                    bitmask & 
                    ((RRC_OAM_STATISTICS_REQ_ERAB_SETUP_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_ERAB_RELEASE_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_ERAB_MODIFY_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_ERAB_NUMBER_CONFIG_STATS_PRESENT)));

            if (in_message.rrc_erab_stats.bitmask == RRC_NULL)
            {
                RRC_OAMH_TRACE(RRC_ERROR," Invalid bitmask in "
                        "RRC_OAM_CONFIG_STATS_REQ for ERAB RELATED STATS ");
                config_stats_resp = RRC_FAILURE;
            }
        }

        if (in_message.bitmask & RRC_OAM_HANDOVER_RELATED_STATS_PRESENT)
        {
            in_message.rrc_handover_stats.bitmask = (in_message.
                    rrc_handover_stats.bitmask &
                    ((RRC_OAM_STATISTICS_REQ_INTRA_ENB_HO_RELATED_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_INTER_ENB_HO_RELATED_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_MEAS_ON_NEIGH_CELL_HO_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_INTRA_FREQ_HO_RELATED_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_INTER_FREQ_HO_RELATED_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_DRX_NON_DRX_HO_RELATED_CONFIG_STATS_PRESENT) |
                     (RRC_OAM_STATISTICS_REQ_INTER_RAT_HO_CONFIG_STATS_PRESENT)|
                     (RRC_OAM_STATISTICS_REQ_HO_FAILURE_NUM_CONFIG_STATS_PRESENT)));

            if (in_message.rrc_handover_stats.bitmask == RRC_NULL)
            {
                RRC_OAMH_TRACE(RRC_ERROR," Invalid bitmask in "
                        " RRC_OAM_CONFIG_STATS_REQ for HANDOVER RELATED STATS");

                config_stats_resp = RRC_FAILURE;
            }
        }
    }

    if (config_stats_resp == RRC_FAILURE)
    {
        in_message_resp.response = config_stats_resp;
/* SPR:9279 start */
        rrc_oam_il_send_rrc_oam_config_stats_resp(
                &in_message_resp,
                RRC_OAMH_MODULE_ID,
                RRC_OAM_MODULE_ID,
                rrc_get_transaction_id(p_api),
                RRC_NULL);
/* SPR:9279 stop */

        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
        /*SPR_17462_START*/
        p_meas_on_neigh_cell_ho_target_id_list = 
            rrc_mem_get(sizeof(rrc_oam_meas_on_neigh_cell_ho_stats_list_t));
        if(PNULL == p_meas_on_neigh_cell_ho_target_id_list)
        {
            RRC_OAMH_TRACE(RRC_FATAL, "%s: Memory allocation failed for "
                    "p_meas_on_neigh_cell_ho_target_id_list.", __FUNCTION__);
            return;
        }
        /* Initialize count with 0 */
        p_meas_on_neigh_cell_ho_target_id_list->count = 0;

        p_inter_rat_ho_target_id_list = 
            rrc_mem_get(sizeof(rrc_oam_inter_rat_ho_stats_list_t));
        if(PNULL == p_inter_rat_ho_target_id_list )
        {
            RRC_OAMH_TRACE(RRC_FATAL, "%s: Memory allocation failed for "
                    "p_inter_rat_ho_target_id_list.", __FUNCTION__);
            rrc_mem_free(p_meas_on_neigh_cell_ho_target_id_list);
            p_meas_on_neigh_cell_ho_target_id_list = PNULL;
            return;
        }
        /* Initialize count with 0 */
        p_inter_rat_ho_target_id_list->count = 0;
        /*SPR_17462_END*/
        for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
      {
        /* Resetting the shared memory */
        for (index = 0; index < MAX_NUM_CELLS; index++)
        {
            /* Bug 11748 Fix Start */
            rrcConnectionNumber = g_rrc_stats.p_rrc_stats->rrc_uecc_stats. 
                rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[index].
                rrc_connection_related_stats.rrcConnectionNumber;
            /* Bug 11748 Fix Stop */

            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].rrc_connection_related_stats, 0,
                    sizeof(rrc_oam_connection_related_stats_t));

            /* Bug 11748 Fix Start */
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[index].rrc_connection_related_stats.
                rrcConnectionNumber = rrcConnectionNumber;

            l3_memcpy_wrapper (&erab_sim_num_stats, &g_rrc_stats.
                    p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].
                    rrc_erab_related_stats.erab_sim_num_stats, 
                    sizeof(rrc_uecc_oam_stat_erab_number_t));
            /* Bug 11748 Fix Stop */

            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].rrc_erab_related_stats, 0,
                    sizeof(rrc_oam_erab_related_stats_t));

            /* Bug 11748 Fix Start */
            l3_memcpy_wrapper (&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].
                    rrc_erab_related_stats.erab_sim_num_stats, &erab_sim_num_stats,
                    sizeof(rrc_uecc_oam_stat_erab_number_t));
            /* Bug 11748 Fix Stop */

            /*SPR_17462_START*/
            if(0 != g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].
                    rrc_handover_related_stats.
                    meas_on_neigh_cell_ho_stats_list.count)
            {
                l3_memcpy_wrapper(p_meas_on_neigh_cell_ho_target_id_list,
                        &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[index].
                        rrc_handover_related_stats.
                        meas_on_neigh_cell_ho_stats_list,
                        sizeof(rrc_oam_meas_on_neigh_cell_ho_stats_list_t));
            }
            if(0 != g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].
                    rrc_handover_related_stats.
                    inter_rat_ho_stats_list.count)
            {
                l3_memcpy_wrapper(p_inter_rat_ho_target_id_list,
                        &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[index].
                        rrc_handover_related_stats.
                        inter_rat_ho_stats_list,
                        sizeof(rrc_oam_inter_rat_ho_stats_list_t));
            }
            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].rrc_handover_related_stats, 0,
                    sizeof(rrc_oam_handover_related_stats_t));
            if(0 != p_meas_on_neigh_cell_ho_target_id_list->count)
            {
                /* SPR 17885 Fix Start */
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                  rrc_uecc_instance_stats[uecc_index].
                  rrc_uecc_cell_stats[index].
                  rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                  count = p_meas_on_neigh_cell_ho_target_id_list->count;
                /* SPR 17885 Fix Stop */
                for(target_index = 0; 
                        target_index < p_meas_on_neigh_cell_ho_target_id_list->count;
                        target_index++)
                {
                    l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[index].
                            rrc_handover_related_stats.
                            meas_on_neigh_cell_ho_stats_list.
                        /*SPR_19166_START*/
                            meas_on_neigh_cell_ho_stats[target_index].ecgi,
                            &p_meas_on_neigh_cell_ho_target_id_list->
                            meas_on_neigh_cell_ho_stats[target_index].ecgi,
                            sizeof(cell_global_id_eutra_t));
                        /*SPR_19166_END*/
                }
                p_meas_on_neigh_cell_ho_target_id_list->count = 0;
            }
            if(0 != p_inter_rat_ho_target_id_list->count)
            {
                /* SPR 17885 Fix Start */
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                  rrc_uecc_instance_stats[uecc_index].
                  rrc_uecc_cell_stats[index].
                  rrc_handover_related_stats.inter_rat_ho_stats_list.
                  count = p_inter_rat_ho_target_id_list->count;
                /* SPR 17885 Fix Stop */
                for(target_index = 0; 
                        target_index < p_inter_rat_ho_target_id_list->count;
                        target_index++)
                {
                    l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[index].
                            rrc_handover_related_stats.
                            inter_rat_ho_stats_list.
                            /*SPR_19166_START*/
                            inter_rat_ho_stats[target_index].target_cgi,
                            &p_inter_rat_ho_target_id_list->
                            inter_rat_ho_stats[target_index].target_cgi,
                            sizeof(rrc_inter_rat_global_cgi_t));
                            /*SPR_19166_END*/
                }
                p_inter_rat_ho_target_id_list->count = 0;
            }
            /*SPR_17462_END*/

            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].rrc_ue_assoc_s1_conn_related_stats, 0,
                    sizeof(rrc_uecc_oam_stat_ue_assoc_s1_connection_t));

            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[index].rrc_IratIncMobility_LAI =  0;

            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_csc_stats.
                    rrc_csc_cell_stats[index].rrc_paging_related_stats,0, 
                    sizeof(rrc_csc_oam_stat_paging_t));
        }
      }
      /*SPR_17462_START*/
      rrc_mem_free(p_meas_on_neigh_cell_ho_target_id_list);
      p_meas_on_neigh_cell_ho_target_id_list = PNULL;
      rrc_mem_free(p_inter_rat_ho_target_id_list);
      p_inter_rat_ho_target_id_list = PNULL;
      /*SPR_17462_END*/
    }
    /*Stopping the timers if already started and resetting the global context*/
    rrc_oamh_stop_performance_related_timers(p_oamh_gb_context);

    memset_wrapper (&p_oamh_gb_context->rrc_oamh_stats_status, 0,
            sizeof(rrc_oamh_stats_t));
    memset_wrapper (&p_oamh_gb_context->timers_data, 0,
            sizeof(oamh_timer_data_t));

    do
    {
        if (in_message.rrc_periodic_reporting == RRC_OAM_STATS_PERIODICITY_ENABLED)
        {
            oamh_oam_process_periodic_reporting_stats(
                           &in_message, p_oamh_gb_context, &config_stats_resp);
        }
        else
        {
            oamh_oam_process_non_periodic_reporting_stats(&in_message,p_oamh_gb_context);
        }
    }while(0);
    
    if (RRC_FAILURE == config_stats_resp)
    {
        rrc_oamh_stop_performance_related_timers(p_oamh_gb_context);
    
        memset_wrapper (&p_oamh_gb_context->rrc_oamh_stats_status, 0,
                sizeof(rrc_oamh_stats_t));
        memset_wrapper (&p_oamh_gb_context->timers_data, 0,
                sizeof(oamh_timer_data_t));
    }
    
    /* Sending response to OAM */
    in_message_resp.response = config_stats_resp;
/* SPR:9279 start */
    rrc_oam_il_send_rrc_oam_config_stats_resp(
            &in_message_resp,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            rrc_get_transaction_id(p_api),
            RRC_NULL);
/* SPR:9279 stop */

    RRC_OAMH_UT_TRACE_EXIT();
    return;
}


/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_conn_estb_stats 
 *
*   INPUTS        :rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION: This function updates rrc_connection_establishment related
 *                 stats.
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_conn_estb_stats(
      rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    RRC_OAMH_UT_TRACE_ENTER();

    p_conn_stats_ind->rrc_connection[index].bitmask |= 
        RRC_OAM_STATISTICS_CONNECTION_ESTB_IND_STATS_PRESENT;

    for ( cause = 0; cause < RRC_ESTABLISHMENT_CAUSE_INDEX; cause++)
    {
        p_conn_stats_ind->rrc_connection[index].
            rrc_connection_establishment.rrc_connEstabAtt_cause[cause] +=
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
            rrc_connection_related_stats.rrc_connection_establishment.
            rrc_connEstabAtt_cause[cause];

        p_conn_stats_ind->rrc_connection[index].
            rrc_connection_establishment.rrc_connEstabSucc_cause[cause] +=
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
            rrc_connection_related_stats.rrc_connection_establishment.
            rrc_connEstabSucc_cause[cause];

        p_conn_stats_ind->rrc_connection[index].
            rrc_connection_establishment.rrc_connEstabFail_cause[cause] +=
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
            rrc_connection_related_stats.rrc_connection_establishment.
            rrc_connEstabFail_cause[cause];
    }

    p_conn_stats_ind->rrc_connection[index].
        rrc_connection_establishment.rrc_connEstabAtt_sum +=
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
        rrc_connection_related_stats.rrc_connection_establishment.
        rrc_connEstabAtt_sum;

    p_conn_stats_ind->rrc_connection[index].
        rrc_connection_establishment.rrc_connEstabSucc_sum +=
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
        rrc_connection_related_stats.rrc_connection_establishment.
        rrc_connEstabSucc_sum;

    p_conn_stats_ind->rrc_connection[index].
        rrc_connection_establishment.rrc_connEstabFail_sum +=
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
        rrc_connection_related_stats.rrc_connection_establishment.
        rrc_connEstabFail_sum;

    p_conn_stats_ind->rrc_connection[index].
        rrc_connection_establishment.rrc_connEstabFaileNBCause_Unspecified +=
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
        rrc_connection_related_stats.rrc_connection_establishment.
        rrc_connEstabFaileNBCause_Unspecified;

    /*SPR 15439 Fix Start*/
    p_conn_stats_ind->rrc_connection[index].
        rrc_connection_establishment.rrc_ConnEstabFaileNBCause_Congestion +=
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
        rrc_connection_related_stats.rrc_connection_establishment.
        rrc_ConnEstabFaileNBCause_Congestion;
    /*SPR 15439 Fix End*/

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_conn_re_estb_stats 
 *
*   INPUTS        :rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION: This function updates rrc_connection_establishment related
 *   DESCRIPTION:This function updates rrc_connection_reestablishment
 *               related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_conn_re_estb_stats(
      rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    RRC_OAMH_UT_TRACE_ENTER();

    p_conn_stats_ind->rrc_connection[index].bitmask |= 
        RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_IND_STATS_PRESENT;

    for ( cause = 0; cause < RRC_REESTABLISHMENT_CAUSE_INDEX; cause++)
    {
        p_conn_stats_ind->rrc_connection[index].
            rrc_connection_reestablishment.rrc_connReEstabAtt_cause[cause] += 
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            rrc_connection_reestablishment.rrc_connReEstabAtt_cause[cause];

        p_conn_stats_ind->rrc_connection[index].
            rrc_connection_reestablishment.rrc_connReEstabSucc_cause[cause] += 
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            rrc_connection_reestablishment.rrc_connReEstabSucc_cause[cause];

        p_conn_stats_ind->rrc_connection[index].
            rrc_connection_reestablishment.rrc_connReEstabFail_cause[cause] += 
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            rrc_connection_reestablishment.rrc_connReEstabFail_cause[cause];
    }

    p_conn_stats_ind->rrc_connection[index].
        rrc_connection_reestablishment.rrc_connReEstabAtt_sum += 
        g_rrc_stats.p_rrc_stats->
        rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
        rrc_connection_reestablishment.rrc_connReEstabAtt_sum;

    p_conn_stats_ind->rrc_connection[index].
        rrc_connection_reestablishment.rrc_connReEstabSucc_sum += 
        g_rrc_stats.p_rrc_stats->
        rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
        rrc_connection_reestablishment.rrc_connReEstabSucc_sum;

    p_conn_stats_ind->rrc_connection[index].
        rrc_connection_reestablishment.rrc_connReEstabFail_sum += 
        g_rrc_stats.p_rrc_stats->
        rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
        rrc_connection_reestablishment.rrc_connReEstabFail_sum;

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_conn_setup_time_stats 
 *
*   INPUTS        :rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION: This function updates rrc_connection_establishment related
 *   DESCRIPTION:This function updates rrc_connection_setup_time 
 *                related stats_
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_conn_setup_time_stats(
      rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    
    RRC_OAMH_UT_TRACE_ENTER();
    
    p_conn_stats_ind->rrc_connection[index].bitmask |= 
        RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_IND_STATS_PRESENT;
    
    for (cause = 0; cause < RRC_ESTABLISHMENT_CAUSE_INDEX; cause++)
    {
        p_conn_stats_ind->rrc_connection[index].
            rrc_connEstabTime_cause[cause].rrc_connEstabTime_Sum +=
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            rrc_connEstabTime_cause[cause].rrc_connEstabTime_Sum;

        if ((p_conn_stats_ind->rrc_connection[index].
                    rrc_connEstabTime_cause[cause].rrc_connEstabTime_Max) < 
                (g_rrc_stats.p_rrc_stats->
                 rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                 rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
                 rrc_connEstabTime_cause[cause].rrc_connEstabTime_Max))
        {
            p_conn_stats_ind->rrc_connection[index].
                rrc_connEstabTime_cause[cause].rrc_connEstabTime_Max = 
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
                rrc_connEstabTime_cause[cause].rrc_connEstabTime_Max;
        }

        p_conn_stats_ind->rrc_connection[index].
            rrc_connEstabTime_cause[cause].count = 
            (U8)(p_conn_stats_ind->rrc_connection[index].
                    rrc_connEstabTime_cause[cause].count +
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
                    rrc_connEstabTime_cause[cause].count);
    }

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_ue_ctxt_rel_stats 
 *
*   INPUTS        :rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION: This function updates rrc_connection_establishment related
 *   DESCRIPTION:This function updates ue_context_release related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_ue_ctxt_rel_stats(
      rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;

    RRC_OAMH_UT_TRACE_ENTER();
    
    p_conn_stats_ind->rrc_connection[index].bitmask |=
        RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_IND_STATS_PRESENT;

    for (cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_conn_stats_ind->rrc_connection[index].
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_radioNetwork[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_conn_stats_ind->rrc_connection[index].
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_transport[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_conn_stats_ind->rrc_connection[index].
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_nas[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_conn_stats_ind->rrc_connection[index].
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_protocol[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_conn_stats_ind->rrc_connection[index].
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
            ue_contxt_rel_stats.ueCtxt_relReq_cause.
            ueCtxt_relReq_cause_misc[cause];
    }

    p_conn_stats_ind->rrc_connection[index].
        ue_contxt_rel_stats.ueCtxt_relReq_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
        ue_contxt_rel_stats.ueCtxt_relReq_sum;

    p_conn_stats_ind->rrc_connection[index].
        ue_contxt_rel_stats.ueContext_RelSuccNbr += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
        ue_contxt_rel_stats.ueContext_RelSuccNbr;
    
    RRC_OAMH_UT_TRACE_EXIT();
}
    
/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_connection_timer_expiry_handler 
 *
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
 *   DESCRIPTION: This function updates rrc_connection_establishment related
 *   DESCRIPTION:
 *       This function is called on the expiry of
 *       OAMH_RRC_CONNECTION_SAMPLING_TIMER 
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_connection_timer_expiry_handler(
        rrc_oamh_gb_context_t *p_oamh_gb_context
        )
{
    rrc_oam_rrc_connection_stats_ind_t rrc_connection_stats_ind;
    U8 cell_index = RRC_NULL;
    U8 index = RRC_NULL;
    rrc_return_t result = RRC_FAILURE;
    U8 uecc_index = RRC_NULL;
    /* Bug 11748 Fix Start */
    U32 rrcConnectionNumber = RRC_NULL;
    /* Bug 11748 Fix Stop */

    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_INFO, "OAMH_RRC_CONNECTION_SAMPLING_TIMER expired");

    memset_wrapper(&rrc_connection_stats_ind, RRC_NULL, 
            sizeof(rrc_oam_rrc_connection_stats_ind_t));

    if (p_oamh_gb_context->rrc_oamh_stats_status.m.is_rrc_connection_counter_enable == 1)
    {
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
            for (cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
            {  
                uecc_index = 0; 

                if ((RRC_TRUE == g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[cell_index].is_cell_configured ) 
                        && (MAX_NUM_CELLS > index))
                {
                    rrc_connection_stats_ind.count++;
                    rrc_connection_stats_ind.rrc_connection[index].cell_index = cell_index;

                    if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                            is_reset_done[cell_index] == RRC_TRUE)
                    {
                        rrc_connection_stats_ind.rrc_connection[index].sampling_period = 
                            p_oamh_gb_context->rrc_oamh_stats_status.
                            rrc_connection_stats_status.time_remaining_till_reset[cell_index];

                    }
                    else
                    {
                        rrc_connection_stats_ind.rrc_connection[index].sampling_period = 
                            p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                            sampling_period;
                    }

                    for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
                    {
                        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                                bitmask & RRC_OAMH_CONNECTION_ESTB_STATS_PRESENT)
                        {
                            rrc_oamh_update_conn_estb_stats(&rrc_connection_stats_ind,index,
                                    cell_index,uecc_index);
                        }

                        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                                bitmask & RRC_OAMH_CONNECTION_RE_ESTB_STATS_PRESENT)
                        {
                            rrc_oamh_update_conn_re_estb_stats(&rrc_connection_stats_ind,index,
                                    cell_index,uecc_index);
                        }

                        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                                bitmask & RRC_OAMH_CONNECTION_NUMBER_STATS_PRESENT)
                        {
                            rrc_connection_stats_ind.rrc_connection[index].bitmask |=
                                RRC_OAM_STATISTICS_CONNECTION_NUMBER_IND_STATS_PRESENT;

                            rrc_connection_stats_ind.rrc_connection[index].rrcConnectionNumber +=
                                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                                rrc_uecc_instance_stats[uecc_index].
                                rrc_uecc_cell_stats[cell_index].rrc_connection_related_stats.
                                rrcConnectionNumber;
                        }

                        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                                bitmask & RRC_OAMH_CONNECTION_SETUP_TIME_STATS_PRESENT)
                        {
                            rrc_oamh_update_conn_setup_time_stats(&rrc_connection_stats_ind,index,
                                    cell_index,uecc_index);
                        }

                        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                                bitmask & RRC_OAMH_UE_CONTEXT_RELEASE_STATS_PRESENT)
                        {
                            rrc_oamh_update_ue_ctxt_rel_stats(&rrc_connection_stats_ind,index,
                                    cell_index,uecc_index);
                        }
                    }
                    index++;
                }
            }
        }
    }

    /*SPR_19262_START*/
    if(rrc_connection_stats_ind.count > 0)
    {
    /* Sending the indication to OAM*/
    result = rrc_oam_il_send_rrc_oam_rrc_connection_stats_ind(
            &rrc_connection_stats_ind,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            RRC_TRANSACTION_ID_ABSENT,
            RRC_NULL);

    if (RRC_FAILURE == result)
    {
        RRC_OAMH_TRACE(RRC_ERROR, "Unable to send RRC_OAM_RRC_CONNECTION_STATS_IND ");
    }
    }
    /*SPR_19262_END*/
   
    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
      /* Resetting respective global context and shared memeory*/
      for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
      {
        for (index = 0; index < MAX_NUM_CELLS; index++)
        {
            /* Bug 11748 Fix Start */
            rrcConnectionNumber = g_rrc_stats.p_rrc_stats->rrc_uecc_stats. 
                rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[index].
                rrc_connection_related_stats.rrcConnectionNumber;
            /* Bug 11748 Fix Stop */

            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].rrc_connection_related_stats, 0,
                    sizeof(rrc_oam_connection_related_stats_t));

            /* Bug 11748 Fix Start */
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[index].rrc_connection_related_stats.
                rrcConnectionNumber = rrcConnectionNumber;
            /* Bug 11748 Fix Stop */

            p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                is_reset_done[index] = RRC_FALSE;

            p_oamh_gb_context->rrc_oamh_stats_status.rrc_connection_stats_status.
                time_remaining_till_reset[index] = RRC_NULL;
        }
      }
    }
    RRC_OAMH_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_erab_setup_stats 
 *
*   INPUTS        :rrc_oam_erab_stats_ind_t    *p_erab_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION: This function updates rrc_connection_establishment related
 *   DESCRIPTION: This function update erab_stats in rrc_oam_erab_stats_ind
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_erab_setup_stats(
      rrc_oam_erab_stats_ind_t    *p_erab_stats_ind,
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    U8 qci_count = RRC_NULL;
    
    RRC_OAMH_UT_TRACE_ENTER();

    p_erab_stats_ind->erab_stats[index].bitmask |=
        RRC_OAM_STATISTICS_ERAB_SETUP_IND_STATS_PRESENT;

    for (qci_count = 0; qci_count < RRC_MAX_QCI_INDEX; qci_count++)
    {
        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estabInitAttNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estabInitAttNbr_qci[qci_count];

        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estabInitSuccNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estabInitSuccNbr_qci[qci_count];

        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estabAddAttNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estabAddAttNbr_qci[qci_count];

        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estabAddSuccNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estabAddSuccNbr_qci[qci_count];
    }

    p_erab_stats_ind->erab_stats[index].erab_setup_stats.
        erab_estabInitAttNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabInitAttNbr_sum;

    p_erab_stats_ind->erab_stats[index].erab_setup_stats.
        erab_estabInitSuccNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabInitSuccNbr_sum;

    for (cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_radioNetwork[cause];                            

        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_radioNetwork[cause];                            
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_transport[cause];                            

        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_transport[cause];                            
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_nas[cause];                            

        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_nas[cause];                            
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_protocol[cause];                            

        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_protocol[cause];                            
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_init_fail_stats.
            erab_estabInitFailNbr_cause_misc[cause];                            

        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estab_add_fail_stats.
            erab_estabAddFailNbr_cause_misc[cause];                            
    }

    p_erab_stats_ind->erab_stats[index].erab_setup_stats.
        erab_estabInitFailNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabInitFailNbr_sum;                            

    p_erab_stats_ind->erab_stats[index].erab_setup_stats.
        erab_estabAddAttNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabAddAttNbr_sum;

    p_erab_stats_ind->erab_stats[index].erab_setup_stats.
        erab_estabAddSuccNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabAddSuccNbr_sum;

    p_erab_stats_ind->erab_stats[index].erab_setup_stats.
        erab_estabAddFailNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_setup_stats.erab_estabAddFailNbr_sum;


    for (qci_count = 0; qci_count < RRC_MAX_QCI_INDEX; qci_count++)
    {
        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estabTime_qci[qci_count].erab_estabTime_Sum += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_setup_stats.erab_estabTime_qci[qci_count].
            erab_estabTime_Sum;

        if ((p_erab_stats_ind->erab_stats[index].erab_setup_stats.
                    erab_estabTime_qci[qci_count].erab_estabTime_Max) < 
                (g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                 rrc_uecc_instance_stats[uecc_index].
                 rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
                 erab_setup_stats.erab_estabTime_qci[qci_count].
                 erab_estabTime_Max))
        {
            p_erab_stats_ind->erab_stats[index].erab_setup_stats.
                erab_estabTime_qci[qci_count].erab_estabTime_Max =  
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
                erab_setup_stats.erab_estabTime_qci[qci_count].
                erab_estabTime_Max;
        }

        p_erab_stats_ind->erab_stats[index].erab_setup_stats.
            erab_estabTime_qci[qci_count].count = (U8)(p_erab_stats_ind->
                    erab_stats[index].erab_setup_stats.
                    erab_estabTime_qci[qci_count].count +
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
                    erab_setup_stats.erab_estabTime_qci[qci_count].count);
    }

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_erab_rel_stats 
 *
*   INPUTS        :rrc_oam_erab_stats_ind_t    *p_erab_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION: This function updates rrc_connection_establishment related
 *   DESCRIPTION: This function update erab_stats in rrc_oam_erab_stats_ind
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_erab_rel_stats(
      rrc_oam_erab_stats_ind_t    *p_erab_stats_ind,
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    U8 qci_count = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();
    p_erab_stats_ind->erab_stats[index].bitmask |=
        RRC_OAM_STATISTICS_ERAB_RELEASE_IND_STATS_PRESENT;

    for (qci_count = 0; qci_count < RRC_MAX_QCI_INDEX; qci_count++)
    {
        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            erab_relEnbNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_relEnbNbr_qci[qci_count];

        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            erab_relAttNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_relAttNbr_qci[qci_count];

        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            erab_relSuccNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_relSuccNbr_qci[qci_count];

        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            erab_relActNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_relActNbr_qci[qci_count];
    }

    p_erab_stats_ind->erab_stats[index].erab_release_stats.
        erab_relEnbNbr_sum += g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_release_stats.erab_relEnbNbr_sum;

    p_erab_stats_ind->erab_stats[index].erab_release_stats.
        erab_relAttNbr_sum += g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_release_stats.erab_relAttNbr_sum;

    p_erab_stats_ind->erab_stats[index].erab_release_stats.
        erab_relSuccNbr_sum += g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_release_stats.erab_relSuccNbr_sum;

    p_erab_stats_ind->erab_stats[index].erab_release_stats.
        erab_relFailNbr_sum += g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_release_stats.erab_relFailNbr_sum;

    p_erab_stats_ind->erab_stats[index].erab_release_stats.
        erab_relActNbr_sum += g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_release_stats.erab_relActNbr_sum;

    for (cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            erab_rel_enb_cause_stats.erab_relEnbNbr_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_radioNetwork[cause];

        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            rel_fail_cause_stats.erab_relFailNbr_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.rel_fail_cause_stats.
            erab_relFailNbr_cause_radioNetwork[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            erab_rel_enb_cause_stats.erab_relEnbNbr_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_transport[cause];

        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            rel_fail_cause_stats.erab_relFailNbr_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.rel_fail_cause_stats.
            erab_relFailNbr_cause_transport[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            erab_rel_enb_cause_stats.erab_relEnbNbr_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_nas[cause];

        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            rel_fail_cause_stats.erab_relFailNbr_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.rel_fail_cause_stats.
            erab_relFailNbr_cause_nas[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            erab_rel_enb_cause_stats.erab_relEnbNbr_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_protocol[cause];

        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            rel_fail_cause_stats.erab_relFailNbr_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.rel_fail_cause_stats.
            erab_relFailNbr_cause_protocol[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            erab_rel_enb_cause_stats.erab_relEnbNbr_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_rel_enb_cause_stats.
            erab_relEnbNbr_cause_misc[cause];

        p_erab_stats_ind->erab_stats[index].erab_release_stats.
            rel_fail_cause_stats.erab_relFailNbr_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_release_stats.rel_fail_cause_stats.
            erab_relFailNbr_cause_misc[cause];
    }
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_erab_modify_stats 
 *
*   INPUTS        :rrc_oam_erab_stats_ind_t    *p_erab_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION: This function update erab modify related stats in 
 *                rrc_oam_erab_stats_ind
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_erab_modify_stats(
      rrc_oam_erab_stats_ind_t    *p_erab_stats_ind,
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    U8 qci_count = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();
    p_erab_stats_ind->erab_stats[index].bitmask |=
        RRC_OAM_STATISTICS_ERAB_MODIFY_IND_STATS_PRESENT;

    for (qci_count = 0; qci_count < RRC_MAX_QCI_INDEX; qci_count++)
    {
        p_erab_stats_ind->erab_stats[index].erab_mod_stats.
            erab_modQosAttNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_modQosAttNbr_qci[qci_count];

        p_erab_stats_ind->erab_stats[index].erab_mod_stats.
            erab_modQosSuccNbr_qci[qci_count] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_modQosSuccNbr_qci[qci_count];
    }

    p_erab_stats_ind->erab_stats[index].erab_mod_stats.
        erab_modQosAttNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_mod_stats.erab_modQosAttNbr_sum;

    p_erab_stats_ind->erab_stats[index].erab_mod_stats.
        erab_modQosSuccNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_mod_stats.erab_modQosSuccNbr_sum;

    p_erab_stats_ind->erab_stats[index].erab_mod_stats.
        erab_modQosFailNbr_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
        erab_mod_stats.erab_modQosFailNbr_sum;

    for (cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_mod_stats.
            erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_radioNetwork[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_mod_stats.
            erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_transport[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_mod_stats.
            erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_nas[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_mod_stats.
            erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_protocol[cause];
    }

    for (cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_erab_stats_ind->erab_stats[index].erab_mod_stats.
            erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
            erab_mod_stats.erab_mod_fail_cause_stats.
            erab_modQosFailNbr_cause_misc[cause];
    }
}
/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_erab_timer_expiry_handler 
 *
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function is called on the expiry of
 *       OAMH_ERAB_SAMPLING_TIMER 
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_erab_timer_expiry_handler(
        rrc_oamh_gb_context_t *p_oamh_gb_context
        )
{
    static rrc_oam_erab_stats_ind_t    erab_stats_ind;
    U8 index = RRC_NULL;
    U8 cell_index = RRC_NULL;
    U8 uecc_index = RRC_NULL;
    U8 qci_count = RRC_NULL;
    rrc_return_t    result = RRC_FAILURE;
    /* Bug 11748 Fix Start */
    rrc_uecc_oam_stat_erab_number_t erab_sim_num_stats;
    /* Bug 11748 Fix Stop */

    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_INFO, "OAMH_ERAB_SAMPLING_TIMER  expired");

    memset_wrapper(&erab_stats_ind, RRC_NULL, sizeof(rrc_oam_erab_stats_ind_t));
    /* Bug 11748 Fix Start */
    memset_wrapper ( &erab_sim_num_stats, 0, 
            sizeof(rrc_uecc_oam_stat_erab_number_t) );
    /* Bug 11748 Fix Stop */

    if (p_oamh_gb_context->rrc_oamh_stats_status.m.is_erab_counter_enable == 1)
    {
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
            for (cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
            {
                uecc_index = 0;

                if ((RRC_TRUE == g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                            is_cell_configured) && (MAX_NUM_CELLS > index))
                {
                    erab_stats_ind.count++;
                    erab_stats_ind.erab_stats[index].cell_index = cell_index;

                    if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                            is_reset_done[cell_index] == RRC_TRUE)
                    {
                        erab_stats_ind.erab_stats[index].sampling_period = p_oamh_gb_context->
                            rrc_oamh_stats_status.rrc_erab_stats_status.
                            time_remaining_till_reset[cell_index];
                    }
                    else
                    {
                        erab_stats_ind.erab_stats[index].sampling_period = p_oamh_gb_context->
                            rrc_oamh_stats_status.rrc_erab_stats_status.sampling_period;
                    }

                    for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
                    {
                        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                                bitmask & RRC_OAMH_ERAB_SETUP_STATS_PRESENT)
                        {
                            rrc_oamh_update_erab_setup_stats(&erab_stats_ind,index,
                                    cell_index,uecc_index);
                        }

                        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                                bitmask & RRC_OAMH_ERAB_RELEASE_STATS_PRESENT)
                        {
                            rrc_oamh_update_erab_rel_stats(&erab_stats_ind,index,
                                    cell_index,uecc_index);
                        }

                        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                                bitmask & RRC_OAMH_ERAB_MODIFY_STATS_PRESENT)
                        {
                            rrc_oamh_update_erab_modify_stats(&erab_stats_ind,index,
                                    cell_index,uecc_index);
                        }

                        if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                                bitmask & RRC_OAMH_ERAB_NUMBER_STATS_PRESENT)
                        {
                            erab_stats_ind.erab_stats[index].bitmask |=
                                RRC_OAM_STATISTICS_ERAB_NUMBER_IND_STATS_PRESENT;

                            for (qci_count = 0; qci_count < RRC_MAX_QCI_INDEX; qci_count++)
                            {
                                erab_stats_ind.erab_stats[index].erab_sim_num_stats.
                                    erabSimNumber_qci[qci_count] += 
                                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                                    rrc_uecc_instance_stats[uecc_index].
                                    rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
                                    erab_sim_num_stats.erabSimNumber_qci[qci_count];
                            }

                            erab_stats_ind.erab_stats[index].erab_sim_num_stats.
                                erabSimNumber_sum += 
                                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                                rrc_uecc_instance_stats[uecc_index].
                                rrc_uecc_cell_stats[cell_index].rrc_erab_related_stats.
                                erab_sim_num_stats.erabSimNumber_sum;
                        }
                    }
                    index++;
                }
            }
        }
    }
    /*SPR_19262_START*/
    if(erab_stats_ind.count > 0)
	{
    result = rrc_oam_il_send_rrc_oam_erab_stats_ind(
            &erab_stats_ind,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            RRC_TRANSACTION_ID_ABSENT,
            RRC_NULL);

    if (RRC_FAILURE == result)
    {
        RRC_OAMH_TRACE(RRC_ERROR, "Unable to send RRC_OAM_ERAB_STATS_IND ");
    }
	}
    /*SPR_19262_END*/
    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
      for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
      {
        for (index = 0; index < MAX_NUM_CELLS; index++)
        {
            /* Bug 11748 Fix Start */
            l3_memcpy_wrapper (&erab_sim_num_stats, &g_rrc_stats.
                    p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].
                    rrc_erab_related_stats.erab_sim_num_stats, 
                    sizeof(rrc_uecc_oam_stat_erab_number_t));
            /* Bug 11748 Fix Stop */

            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].rrc_erab_related_stats, 0,
                    sizeof(rrc_oam_erab_related_stats_t));

            /* Bug 11748 Fix Start */
            l3_memcpy_wrapper (&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].
                    rrc_erab_related_stats.erab_sim_num_stats, &erab_sim_num_stats,
                    sizeof(rrc_uecc_oam_stat_erab_number_t));
            /* Bug 11748 Fix Stop */

            p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                is_reset_done[index] = RRC_FALSE;

            p_oamh_gb_context->rrc_oamh_stats_status.rrc_erab_stats_status.
                time_remaining_till_reset[index] = RRC_NULL;
        }
      }
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_intra_enb_ho_stats 
 *
*   INPUTS        :rrc_oam_handover_stats_ind_t    *p_handover_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_intra_enb_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    RRC_OAMH_UT_TRACE_ENTER();

    p_handover_stats_ind->handover_stats[index].bitmask |= 
        RRC_OAM_INTRA_ENB_HO_IND_STATS_PRESENT;

    for ( cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
            ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_radioNetwork[cause];

        p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
            ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_radioNetwork[cause];
    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
            ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_transport[cause];
        
        p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
            ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_transport[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
            ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_nas[cause];
        
        p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
            ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_nas[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
            ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_protocol[cause];
        
        p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
            ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_protocol[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
            ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.
            ho_IntraEnbOutAtt_Cause_misc[cause];
        
        p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
            ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
            ho_IntraEnbOutSucc_Cause_misc[cause];

    }

    p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
        ho_IntraEnbOutAtt_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        intra_enb_ho_stats.ho_IntraEnbOutAtt_sum;

    p_handover_stats_ind->handover_stats[index].intra_enb_ho_stats.
        ho_IntraEnbOutSucc_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        intra_enb_ho_stats.ho_IntraEnbOutSucc_sum;

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_inter_enb_ho_stats 
 *
*   INPUTS        :rrc_oam_handover_stats_ind_t    *p_handover_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_inter_enb_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    RRC_OAMH_UT_TRACE_ENTER();

    p_handover_stats_ind->handover_stats[index].bitmask |= 
        RRC_OAM_INTER_ENB_HO_IND_STATS_PRESENT;

    for ( cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
    {
        p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
            ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_radioNetwork[cause];

        p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
            ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_radioNetwork[cause];
    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
    {
        p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
            ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_transport[cause];
        
        p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
            ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_transport[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_transport[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
    {
        p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
            ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_nas[cause];
        
        p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
            ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_nas[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_nas[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
    {
        p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
            ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_protocol[cause];
        
        p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
            ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_protocol[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_protocol[cause];

    }

    for ( cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
    {
        p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
            ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause_misc[cause];
        
        p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
            ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_misc[cause] += 
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause_misc[cause];

    }

    p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
        ho_InterEnbOutAtt_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_enb_ho_stats.ho_InterEnbOutAtt_sum;

    p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
        ho_InterEnbOutSucc_sum += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_enb_ho_stats.ho_InterEnbOutSucc_sum;

    p_handover_stats_ind->handover_stats[index].inter_enb_ho_stats.
        ho_InterEnbOutPrepAtt += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_enb_ho_stats.ho_InterEnbOutPrepAtt;
    
    RRC_OAMH_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_meas_on_neigh_cell_ho_stats 
 *
*   INPUTS        :rrc_oam_handover_stats_ind_t    *p_handover_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This function updates meas_on_neigh_cell_ho related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_meas_on_neigh_cell_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    U8 uecc_target_cell_index = RRC_NULL;
    U8 rrc_target_cell_count = RRC_NULL;
    
    RRC_OAMH_UT_TRACE_ENTER();
    
    p_handover_stats_ind->handover_stats[index].bitmask |= 
        RRC_OAM_MEAS_ON_NEIGH_CELL_HO_IND_STATS_PRESENT;
    if (RRC_NULL == uecc_index)
    {
        p_handover_stats_ind->handover_stats[index].
            meas_on_neigh_cell_ho_stats_list = g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            meas_on_neigh_cell_ho_stats_list;
    }
    else
    {
        for ( uecc_target_cell_index = 0;  uecc_target_cell_index < g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
                meas_on_neigh_cell_ho_stats_list.count
                && uecc_target_cell_index < RRC_MAX_CONNECTION; uecc_target_cell_index++)
        {
            for ( rrc_target_cell_count = 0;  (rrc_target_cell_count < 
                        p_handover_stats_ind->handover_stats[index].
                        meas_on_neigh_cell_ho_stats_list.count) && 
                    (rrc_target_cell_count < RRC_MAX_CONNECTION); 
                    rrc_target_cell_count++)
            {
                if(RRC_NULL == memcmp_wrapper(&p_handover_stats_ind->handover_stats[index].
                            meas_on_neigh_cell_ho_stats_list.
                        /*SPR_19166_START*/
                            meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ecgi,
                            &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                            rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                            meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ecgi,
                            sizeof(cell_global_id_eutra_t)))
                        /*SPR_19166_END*/
                {
                    break;
                }
            }
            if ((rrc_target_cell_count == p_handover_stats_ind->handover_stats[index].
                        meas_on_neigh_cell_ho_stats_list.count) &&
                    (RRC_MAX_CONNECTION > rrc_target_cell_count))
            {
                p_handover_stats_ind->handover_stats[index].
                    meas_on_neigh_cell_ho_stats_list.count++;
                l3_memcpy_wrapper (&p_handover_stats_ind->handover_stats[index].
                        meas_on_neigh_cell_ho_stats_list.
                        /*SPR_19166_START*/
                        meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ecgi,
                        &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                        rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                        meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ecgi,
                        sizeof(cell_global_id_eutra_t));
                        /*SPR_19166_END*/
            }
            if (RRC_MAX_CONNECTION <= rrc_target_cell_count)
            {/* BUG 12662 Fix Start */
                RRC_OAMH_TRACE(RRC_ERROR, 
                        "Target Cells are more than 32 so unable to handle it");/* BUG 12662 Fix Stop */
                return;
            }

            for ( cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
            {
                p_handover_stats_ind->handover_stats[index].
                    meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_radioNetwork[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_radioNetwork[cause];
                
                p_handover_stats_ind->handover_stats[index].
                    meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_radioNetwork[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_radioNetwork[cause];
            }

            for ( cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
            {
                p_handover_stats_ind->handover_stats[index].
                    meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_transport[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_transport[cause];
                
                p_handover_stats_ind->handover_stats[index].
                    meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_transport[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_transport[cause];
            }

            for ( cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
            {
                p_handover_stats_ind->handover_stats[index].
                    meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_nas[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_nas[cause];
                
                p_handover_stats_ind->handover_stats[index].
                    meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_nas[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_nas[cause];
            }

            for ( cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
            {
                p_handover_stats_ind->handover_stats[index].
                    meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_protocol[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_protocol[cause];
                
                p_handover_stats_ind->handover_stats[index].
                    meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_protocol[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_protocol[cause];
            }

            for ( cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
            {
                p_handover_stats_ind->handover_stats[index].
                    meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_misc[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_cause.
                    ho_OutAttTarget_Cause_misc[cause];
                
                p_handover_stats_ind->handover_stats[index].
                    meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_misc[cause] +=
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_cause.
                    ho_OutSuccTarget_Cause_misc[cause];
            }
            p_handover_stats_ind->handover_stats[index].
                meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutAttTarget_sum +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutAttTarget_sum;

            p_handover_stats_ind->handover_stats[index].
                meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[rrc_target_cell_count].ho_OutSuccTarget_sum +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[uecc_target_cell_index].ho_OutSuccTarget_sum;
        }
    }
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_intra_freq_ho_stats 
 *
*   INPUTS        :rrc_oam_handover_stats_ind_t    *p_handover_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This function updates intra_freq_ho related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_intra_freq_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    p_handover_stats_ind->handover_stats[index].bitmask |= 
        RRC_OAM_INTRA_FREQ_HO_IND_STATS_PRESENT;
    
    p_handover_stats_ind->handover_stats[index].
        intra_freq_ho_stats.ho_IntraFreqOutAtt += g_rrc_stats.p_rrc_stats->
        rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        intra_freq_ho_stats.ho_IntraFreqOutAtt;

    p_handover_stats_ind->handover_stats[index].
        intra_freq_ho_stats.ho_IntraFreqOutSucc += g_rrc_stats.p_rrc_stats->
        rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        intra_freq_ho_stats.ho_IntraFreqOutSucc;
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_inter_freq_ho_stats 
 *
*   INPUTS        :rrc_oam_handover_stats_ind_t    *p_handover_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This function updates inter_freq_ho related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_inter_freq_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    p_handover_stats_ind->handover_stats[index].bitmask |= 
        RRC_OAM_INTER_FREQ_HO_IND_STATS_PRESENT;
    
    p_handover_stats_ind->handover_stats[index].inter_freq_ho_stats.
        ho_InterFreqMeasGapOutAtt += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_freq_ho_stats.ho_InterFreqMeasGapOutAtt;


    p_handover_stats_ind->handover_stats[index].inter_freq_ho_stats.
        ho_InterFreqMeasGapOutSucc += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_freq_ho_stats.ho_InterFreqMeasGapOutSucc;

    p_handover_stats_ind->handover_stats[index].inter_freq_ho_stats.
        ho_InterFreqNoMeasGapOutAtt += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_freq_ho_stats.ho_InterFreqNoMeasGapOutAtt;

    p_handover_stats_ind->handover_stats[index].inter_freq_ho_stats.
        ho_InterFreqNoMeasGapOutSucc += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        inter_freq_ho_stats.ho_InterFreqNoMeasGapOutSucc;
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_drx_non_drx_ho_stats 
 *
*   INPUTS        :rrc_oam_handover_stats_ind_t    *p_handover_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:This fumction updates drx_ho or non_drx_ho related stats
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_drx_non_drx_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    p_handover_stats_ind->handover_stats[index].bitmask |= 
        RRC_OAM_DRX_NON_DRX_HO_IND_STATS_PRESENT;

    p_handover_stats_ind->handover_stats[index].
        drx_non_drx_ho_stats.ho_DrxOutAtt += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        drx_non_drx_ho_stats.ho_DrxOutAtt;

    p_handover_stats_ind->handover_stats[index].
        drx_non_drx_ho_stats.ho_DrxOutSucc += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        drx_non_drx_ho_stats.ho_DrxOutSucc;

    p_handover_stats_ind->handover_stats[index].
        drx_non_drx_ho_stats.ho_NoDrxOutAtt += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        drx_non_drx_ho_stats.ho_NoDrxOutAtt;

    p_handover_stats_ind->handover_stats[index].
        drx_non_drx_ho_stats.ho_NoDrxOutSucc += 
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
        rrc_uecc_instance_stats[uecc_index].
        rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        drx_non_drx_ho_stats.ho_NoDrxOutSucc;
    
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_update_inter_rat_ho_stats 
 *
*   INPUTS        :rrc_oam_handover_stats_ind_t    *p_handover_stats_ind
*                  U8 index 
*                  U8 cell_index
*                  U8 uecc_index
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_update_inter_rat_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
)
{
    U8 cause = 0;
    /* BUG 12662 Fix Start */
    U8 uecc_target_cell_index = RRC_NULL;
    U8 rrc_target_cell_count = RRC_NULL;
    /* BUG 12662 Fix Stop */
    RRC_OAMH_UT_TRACE_ENTER();

    p_handover_stats_ind->handover_stats[index].bitmask |= 
        RRC_OAM_INTER_RAT_HO_IND_STATS_PRESENT;

    /* BUG 12662 Fix Start */
    if (RRC_NULL == uecc_index)
    {
        p_handover_stats_ind->handover_stats[index].
            inter_rat_ho_stats_list = g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
            inter_rat_ho_stats_list;
    }
    else
    {
        for ( uecc_target_cell_index = 0;  uecc_target_cell_index < g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
                inter_rat_ho_stats_list.count
                && uecc_target_cell_index < RRC_MAX_CONNECTION; uecc_target_cell_index++)
        {
            for ( rrc_target_cell_count = 0;  (rrc_target_cell_count < 
                        p_handover_stats_ind->handover_stats[index].
                        inter_rat_ho_stats_list.count) && 
                    (rrc_target_cell_count < RRC_MAX_CONNECTION); 
                    rrc_target_cell_count++)
            {
                if(RRC_NULL == memcmp_wrapper(&p_handover_stats_ind->handover_stats[index].
                            inter_rat_ho_stats_list.
                            /*SPR_19166_START*/
                            inter_rat_ho_stats[rrc_target_cell_count].target_cgi,
                            &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                            rrc_handover_related_stats.inter_rat_ho_stats_list.
                            inter_rat_ho_stats[uecc_target_cell_index].target_cgi,
                            sizeof(rrc_inter_rat_global_cgi_t)))
                            /*SPR_19166_END*/
                {
                    break;
                }
            }
            if ((rrc_target_cell_count == p_handover_stats_ind->handover_stats[index].
                        inter_rat_ho_stats_list.count) &&
                    (RRC_MAX_CONNECTION > rrc_target_cell_count))
            {
                p_handover_stats_ind->handover_stats[index].
                    inter_rat_ho_stats_list.count++;
                l3_memcpy_wrapper (&p_handover_stats_ind->handover_stats[index].
                        inter_rat_ho_stats_list.
                        /*SPR_19166_START*/
                        inter_rat_ho_stats[rrc_target_cell_count].target_cgi,
                        &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                        rrc_handover_related_stats.inter_rat_ho_stats_list.
                        inter_rat_ho_stats[uecc_target_cell_index].target_cgi,
                        sizeof(rrc_inter_rat_global_cgi_t));
                        /*SPR_19166_END*/
            }
            if (RRC_MAX_CONNECTION <= rrc_target_cell_count)
            {
                RRC_OAMH_TRACE(RRC_ERROR, 
                        "Target Cells are more than 32 so unable to handle it");
                return;
            }

        for (cause = 0; cause < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; cause++)
        {
        	p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
	    	    inter_rat_ho_stats[rrc_target_cell_count].
                ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[cause] += 
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
		        inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
    		    ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[cause];

	        p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
	    	    inter_rat_ho_stats[rrc_target_cell_count].
                ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[cause] +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
	            inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
		        ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[cause];
        }

        for (cause = 0; cause < RRC_S1AP_CAUSE_TRANSPORT_INDEX; cause++)
        {
	        p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
		        inter_rat_ho_stats[rrc_target_cell_count].
                ho_IratOutAtt_cause.ho_IratOutAtt_Cause_transport[cause] += 
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
        	    inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
		        ho_IratOutAtt_cause.ho_IratOutAtt_Cause_transport[cause];

        	p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
                ho_IratOutSucc_cause.ho_IratOutSucc_Cause_transport[cause] +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
	            inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
		        ho_IratOutSucc_cause.ho_IratOutSucc_Cause_transport[cause];
        }

        for (cause = 0; cause < RRC_S1AP_CAUSE_NAS_INDEX; cause++)
        {
        	p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
                ho_IratOutAtt_cause.ho_IratOutAtt_Cause_nas[cause] += 
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
	            inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	    	    ho_IratOutAtt_cause.ho_IratOutAtt_Cause_nas[cause];

        	p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
                ho_IratOutSucc_cause.ho_IratOutSucc_Cause_nas[cause] +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
	            inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	    	    ho_IratOutSucc_cause.ho_IratOutSucc_Cause_nas[cause];
        }

        for (cause = 0; cause < RRC_S1AP_CAUSE_PROTOCOL_INDEX; cause++)
        {
        	p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
                ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[cause] += 
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
	            inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	    	    ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[cause];

	        p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
                ho_IratOutSucc_cause.ho_IratOutSucc_Cause_protocol[cause] +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
	            inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	     	   ho_IratOutSucc_cause.ho_IratOutSucc_Cause_protocol[cause];
        }

        for (cause = 0; cause < RRC_S1AP_CAUSE_MISC_INDEX; cause++)
        {
        	p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
    		    inter_rat_ho_stats[rrc_target_cell_count].
                ho_IratOutAtt_cause.ho_IratOutAtt_Cause_misc[cause] += 
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
	            inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
    		    ho_IratOutAtt_cause.ho_IratOutAtt_Cause_misc[cause];

        	p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
	    	    inter_rat_ho_stats[rrc_target_cell_count].
                ho_IratOutSucc_cause.ho_IratOutSucc_Cause_misc[cause] +=
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
	            inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
		        ho_IratOutSucc_cause.ho_IratOutSucc_Cause_misc[cause];
        }

	    p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
    		inter_rat_ho_stats[rrc_target_cell_count].
            ho_IratOutAtt_sum += g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
	        inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	    	ho_IratOutAtt_sum;

	    p_handover_stats_ind->handover_stats[index].inter_rat_ho_stats_list.
    		inter_rat_ho_stats[rrc_target_cell_count].
            ho_IratOutSucc_sum += g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[uecc_index].
            rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
	        inter_rat_ho_stats_list.inter_rat_ho_stats[uecc_target_cell_index].
	    	ho_IratOutSucc_sum;
        }
    }
    /* BUG 12662 Fix Stop */ 
    RRC_OAMH_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_handover_timer_expiry_handler 
 *
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function is called on the expiry of
 *       OAMH_HANDOVER_SAMPLING_TIMER 
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_handover_timer_expiry_handler(
        rrc_oamh_gb_context_t *p_oamh_gb_context
        )
{
	/* coverity_fix_64029_start */
	static rrc_oam_handover_stats_ind_t    handover_stats_ind;
	U8 index = RRC_NULL;
	U8 cell_index = RRC_NULL;
	U8 uecc_index = RRC_NULL;
	rrc_return_t    result = RRC_FAILURE;
	/*SPR_17462_START*/
	U8 target_index = RRC_NULL;
	rrc_oam_meas_on_neigh_cell_ho_stats_list_t  
		*p_meas_on_neigh_cell_ho_target_id_list = PNULL;
	rrc_oam_inter_rat_ho_stats_list_t
		*p_inter_rat_ho_target_id_list = PNULL;
	/*SPR_17462_END*/

	RRC_OAMH_UT_TRACE_ENTER();
	RRC_ASSERT(PNULL != p_oamh_gb_context);

	RRC_OAMH_TRACE(RRC_INFO, "OAMH_HANDOVER_SAMPLING_TIMER expired");

	memset_wrapper(&handover_stats_ind, RRC_NULL, 
			sizeof(rrc_oam_handover_stats_ind_t));

	if (p_oamh_gb_context->rrc_oamh_stats_status.m.is_handover_counter_enable == 1)
	{
		if (PNULL != g_rrc_stats.p_rrc_stats)
		{
			for (cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
			{
            /*SPR_19262_START*/
				uecc_index = 0;
            /*SPR_19262_END*/
				if ((RRC_TRUE == g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
							rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
							is_cell_configured) && (MAX_NUM_CELLS > index))
				{
					handover_stats_ind.count++;
					handover_stats_ind.handover_stats[index].cell_index = cell_index;

					if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
							is_reset_done[cell_index] == RRC_TRUE)
					{
						handover_stats_ind.handover_stats[index].sampling_period =
							p_oamh_gb_context->rrc_oamh_stats_status.
							rrc_handover_stats_status.time_remaining_till_reset[cell_index];
					}
					else
					{
						handover_stats_ind.handover_stats[index].sampling_period = 
							p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
							sampling_period;
					}

					for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
							count && uecc_index < MAX_UECC_INSTANCES && MAX_NUM_CELLS > index;
							uecc_index++)
					{
						if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
								bitmask & RRC_OAMH_INTRA_ENB_HO_STATS_PRESENT)
						{
							rrc_oamh_update_intra_enb_ho_stats(&handover_stats_ind, index, 
									cell_index, uecc_index);
						}

						if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
								bitmask & RRC_OAMH_INTER_ENB_HO_STATS_PRESENT)
						{
							rrc_oamh_update_inter_enb_ho_stats(&handover_stats_ind, index, 
									cell_index, uecc_index);
						}

						if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
								bitmask & RRC_OAMH_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT)
						{
							rrc_oamh_update_meas_on_neigh_cell_ho_stats(&handover_stats_ind,index, 
									cell_index, uecc_index);
						}

						if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
								bitmask & RRC_OAMH_INTRA_FREQ_HO_STATS_PRESENT)
						{
							rrc_oamh_update_intra_freq_ho_stats(&handover_stats_ind, index, 
									cell_index, uecc_index);
						}

						if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
								bitmask & RRC_OAMH_INTER_FREQ_HO_STATS_PRESENT)
						{
							rrc_oamh_update_inter_freq_ho_stats(&handover_stats_ind,index, 
									cell_index, uecc_index);
						}

						if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
								bitmask & RRC_OAMH_DRX_NON_DRX_HO_STATS_PRESENT)
						{
							rrc_oamh_update_drx_non_drx_ho_stats(&handover_stats_ind,index, 
									cell_index, uecc_index);
						}

						if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
								bitmask & RRC_OAMH_INTER_RAT_HO_STATS_PRESENT)
						{
							rrc_oamh_update_inter_rat_ho_stats(&handover_stats_ind, index, 
									cell_index, uecc_index);
						}

						if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
								bitmask & RRC_OAMH_HO_FAILURE_NUM_STATS_PRESENT)
						{
							handover_stats_ind.handover_stats[index].bitmask |=
								RRC_OAM_HO_FAILURE_NUM_IND_STATS_PRESENT;

							handover_stats_ind.handover_stats[index].ho_failure_num =
								(U8)(handover_stats_ind.handover_stats[index].ho_failure_num +
										g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
										rrc_uecc_instance_stats[uecc_index].
										rrc_uecc_cell_stats[cell_index].rrc_handover_related_stats.
										ho_failure_num);
						}

						/*BUG 11287 Fix Start*/
						/*BUG 11287 Fix Stop*/
					}
					/*BUG 11287 Fix Start*/
					index++;
					/*BUG 11287 Fix Stop*/
				}
			}
		}
	}
    /*SPR_19262_START*/
    if(handover_stats_ind.count > 0)
	{
	result = rrc_oam_il_send_rrc_oam_handover_stats_ind(
			&handover_stats_ind,
			RRC_OAMH_MODULE_ID,
			RRC_OAM_MODULE_ID,
			RRC_TRANSACTION_ID_ABSENT,
			RRC_NULL);

	if (RRC_FAILURE == result)
	{
		RRC_OAMH_TRACE(RRC_ERROR, "Unable to send RRC_OAM_HANDOVER_STATS_IND ");
	}
	}
    /*SPR_19262_END*/

	if (PNULL != g_rrc_stats.p_rrc_stats)
	{
		/*SPR_17462_START*/
		p_meas_on_neigh_cell_ho_target_id_list = 
			rrc_mem_get(sizeof(rrc_oam_meas_on_neigh_cell_ho_stats_list_t));
		if(PNULL == p_meas_on_neigh_cell_ho_target_id_list)
		{
			RRC_OAMH_TRACE(RRC_FATAL, "%s: Memory allocation failed for "
					"p_meas_on_neigh_cell_ho_target_id_list.", __FUNCTION__);
			return;
		}
		/* Initialize count with 0 */
		p_meas_on_neigh_cell_ho_target_id_list->count = 0;

		p_inter_rat_ho_target_id_list = 
			rrc_mem_get(sizeof(rrc_oam_inter_rat_ho_stats_list_t));
		if(PNULL == p_inter_rat_ho_target_id_list )
		{
			RRC_OAMH_TRACE(RRC_FATAL, "%s: Memory allocation failed for "
					"p_inter_rat_ho_target_id_list.", __FUNCTION__);
			rrc_mem_free(p_meas_on_neigh_cell_ho_target_id_list);
			p_meas_on_neigh_cell_ho_target_id_list = PNULL;
			return;
		}
		/* Initialize count with 0 */
		p_inter_rat_ho_target_id_list->count = 0;
		for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
				count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
		{
			for (index = 0; index < MAX_NUM_CELLS; index++)
			{
				if(0 != g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
						rrc_uecc_instance_stats[uecc_index].
						rrc_uecc_cell_stats[index].
						rrc_handover_related_stats.
						meas_on_neigh_cell_ho_stats_list.count)
				{
					l3_memcpy_wrapper(p_meas_on_neigh_cell_ho_target_id_list,
							&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
							rrc_uecc_instance_stats[uecc_index].
							rrc_uecc_cell_stats[index].
							rrc_handover_related_stats.
							meas_on_neigh_cell_ho_stats_list,
							sizeof(rrc_oam_meas_on_neigh_cell_ho_stats_list_t));
				}
				if(0 != g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
						rrc_uecc_instance_stats[uecc_index].
						rrc_uecc_cell_stats[index].
						rrc_handover_related_stats.
						inter_rat_ho_stats_list.count)
				{
					l3_memcpy_wrapper(p_inter_rat_ho_target_id_list,
							&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
							rrc_uecc_instance_stats[uecc_index].
							rrc_uecc_cell_stats[index].
							rrc_handover_related_stats.
							inter_rat_ho_stats_list,
							sizeof(rrc_oam_inter_rat_ho_stats_list_t));
				}
				memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
						rrc_uecc_instance_stats[uecc_index].
						rrc_uecc_cell_stats[index].rrc_handover_related_stats, 0,
						sizeof(rrc_oam_handover_related_stats_t));
				if(0 != p_meas_on_neigh_cell_ho_target_id_list->count)
				{
					/* SPR 17885 Fix Start */
					g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
						rrc_uecc_instance_stats[uecc_index].
						rrc_uecc_cell_stats[index].
						rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
						count = p_meas_on_neigh_cell_ho_target_id_list->count;
					/* SPR 17885 Fix Stop */
					for(target_index = 0; 
							target_index < p_meas_on_neigh_cell_ho_target_id_list->count;
							target_index++)
					{
						l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->
								rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
								rrc_uecc_cell_stats[index].
								rrc_handover_related_stats.
								meas_on_neigh_cell_ho_stats_list.
                        /*SPR_19166_START*/
                            meas_on_neigh_cell_ho_stats[target_index].ecgi,
								&p_meas_on_neigh_cell_ho_target_id_list->
                            meas_on_neigh_cell_ho_stats[target_index].ecgi,
                            sizeof(cell_global_id_eutra_t));
                        /*SPR_19166_END*/
					}
					p_meas_on_neigh_cell_ho_target_id_list->count = 0;
				}
				if(0 != p_inter_rat_ho_target_id_list->count)
				{
					/* SPR 17885 Fix Start */
					g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
						rrc_uecc_instance_stats[uecc_index].
						rrc_uecc_cell_stats[index].
						rrc_handover_related_stats.inter_rat_ho_stats_list.
						count = p_inter_rat_ho_target_id_list->count;
					/* SPR 17885 Fix Stop */
					for(target_index = 0; 
							target_index < p_inter_rat_ho_target_id_list->count;
							target_index++)
					{
						l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->
								rrc_uecc_stats.rrc_uecc_instance_stats[uecc_index].
								rrc_uecc_cell_stats[index].
								rrc_handover_related_stats.
								inter_rat_ho_stats_list.
                            /*SPR_19166_START*/
                            inter_rat_ho_stats[target_index].target_cgi,
								&p_inter_rat_ho_target_id_list->
                            inter_rat_ho_stats[target_index].target_cgi,
                            sizeof(rrc_inter_rat_global_cgi_t));
                            /*SPR_19166_END*/
					}
					p_inter_rat_ho_target_id_list->count = 0;
				}

				p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
					is_reset_done[index] = RRC_FALSE;
				p_oamh_gb_context->rrc_oamh_stats_status.rrc_handover_stats_status.
					time_remaining_till_reset[index] = RRC_NULL;
			}
		}
		rrc_mem_free(p_meas_on_neigh_cell_ho_target_id_list);
		p_meas_on_neigh_cell_ho_target_id_list = PNULL;
		rrc_mem_free(p_inter_rat_ho_target_id_list);
		p_inter_rat_ho_target_id_list = PNULL;
		/*SPR_17462_END*/
	}
	RRC_OAMH_UT_TRACE_EXIT();
}
/* coverity_fix_64029_stop */

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_ue_assoc_s1_conn_timer_expiry_handler 
 *
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function is called on the expiry of
 *       OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER 
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_ue_assoc_s1_conn_timer_expiry_handler(
        rrc_oamh_gb_context_t *p_oamh_gb_context
        )
{
    rrc_oam_ue_assoc_s1_conn_stats_ind_t    ue_assoc_s1_conn_stats_ind;
    U8 index = RRC_NULL;
    U8 cell_index = RRC_NULL;
    U8 uecc_index = RRC_NULL;
    rrc_return_t    result = RRC_FAILURE;

    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_INFO, "OAMH_UE_ASSOC_S1_CONN_SAMPLING_TIMER expired");

    memset_wrapper(&ue_assoc_s1_conn_stats_ind, RRC_NULL, 
            sizeof(rrc_oam_ue_assoc_s1_conn_stats_ind_t));

    if (p_oamh_gb_context->rrc_oamh_stats_status.m.
            is_ue_assoc_s1_conn_counter_enable == 1)
    {
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
          for (cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
          {
            uecc_index = 0;
            if ((RRC_TRUE == g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                        is_cell_configured) && (MAX_NUM_CELLS > index))
            {
                ue_assoc_s1_conn_stats_ind.count++;
                ue_assoc_s1_conn_stats_ind.ue_assoc_s1_conn_stats[index].cell_index = 
                    cell_index; 

                if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_ue_assoc_s1_conn_stats_status.
                        is_reset_done[cell_index] == RRC_TRUE)
                {
                    ue_assoc_s1_conn_stats_ind.ue_assoc_s1_conn_stats[index].
                        sampling_period = (p_oamh_gb_context->rrc_oamh_stats_status.
                                rrc_ue_assoc_s1_conn_stats_status.time_remaining_till_reset[cell_index]);
                }
                else
                {
                    ue_assoc_s1_conn_stats_ind.ue_assoc_s1_conn_stats[index].sampling_period = 
                        p_oamh_gb_context->rrc_oamh_stats_status.rrc_ue_assoc_s1_conn_stats_status.
                        sampling_period;
                }

                for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
                {
                    ue_assoc_s1_conn_stats_ind.ue_assoc_s1_conn_stats[index].
                        s1sig_connEstabAtt += g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                        rrc_ue_assoc_s1_conn_related_stats.s1sig_connEstabAtt;

                    ue_assoc_s1_conn_stats_ind.ue_assoc_s1_conn_stats[index].
                        s1sig_connEstabSucc += g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                        rrc_ue_assoc_s1_conn_related_stats.s1sig_connEstabSucc;

                }
                index++;
            }
          }
        }
    }
    /*SPR_19262_START*/
    if(ue_assoc_s1_conn_stats_ind.count > 0)
	{
    result = rrc_oam_il_send_rrc_oam_ue_assoc_s1_conn_stats_ind(
            &ue_assoc_s1_conn_stats_ind,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            RRC_TRANSACTION_ID_ABSENT,
            RRC_NULL);

    if (RRC_FAILURE == result)
    {
        RRC_OAMH_TRACE(RRC_ERROR, "Unable to send RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND ");
    }
	}
    /*SPR_19262_END*/
    
    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
      for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
      {
        for (index = 0; index < MAX_NUM_CELLS; index++)
        {
            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[uecc_index].
                    rrc_uecc_cell_stats[index].rrc_ue_assoc_s1_conn_related_stats, 0,
                    sizeof(rrc_uecc_oam_stat_ue_assoc_s1_connection_t));

            p_oamh_gb_context->rrc_oamh_stats_status.rrc_ue_assoc_s1_conn_stats_status.
                is_reset_done[index] = RRC_FALSE;
            p_oamh_gb_context->rrc_oamh_stats_status.rrc_ue_assoc_s1_conn_stats_status.
                time_remaining_till_reset[index] = RRC_NULL;
        }
      }
    }
    RRC_OAMH_UT_TRACE_EXIT();

}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_paging_timer_expiry_handler 
 *
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function is called on the expiry of
 *       OAMH_PAGING_SAMPLING_TIMER 
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_paging_timer_expiry_handler(
        rrc_oamh_gb_context_t *p_oamh_gb_context
        )
{
    rrc_oam_paging_stats_ind_t  paging_stats_ind;
    U8 index = RRC_NULL;
    U8 cell_index = RRC_NULL;
    rrc_return_t    result = RRC_FAILURE;

    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_INFO, "OAMH_PAGING_SAMPLING_TIMER expired");

    memset_wrapper(&paging_stats_ind, RRC_NULL, sizeof(rrc_oam_paging_stats_ind_t));

    if (p_oamh_gb_context->rrc_oamh_stats_status.m.is_paging_counter_enable == 1)
    {
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
          for (cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
          {
            if ((RRC_TRUE == g_rrc_stats.p_rrc_stats->rrc_csc_stats.
                        rrc_csc_cell_stats[cell_index].is_cell_configured) 
                    && (MAX_NUM_CELLS > index))
            {
                paging_stats_ind.count++;
                paging_stats_ind.rrc_paging_stats[index].cell_index = cell_index;

                if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_paging_stats_status.
                        is_reset_done[cell_index] == RRC_TRUE)
                {
                    paging_stats_ind.rrc_paging_stats[index].sampling_period = 
                         p_oamh_gb_context->rrc_oamh_stats_status.
                         rrc_paging_stats_status.time_remaining_till_reset[cell_index];
                }
                else
                {
                    paging_stats_ind.rrc_paging_stats[index].sampling_period = p_oamh_gb_context->
                        rrc_oamh_stats_status.rrc_paging_stats_status.sampling_period;
                }

                paging_stats_ind.rrc_paging_stats[index].pag_discardedNbr = 
                    g_rrc_stats.p_rrc_stats->rrc_csc_stats.rrc_csc_cell_stats[cell_index].
                    rrc_paging_related_stats.pag_discardedNbr;

                paging_stats_ind.rrc_paging_stats[index].pag_succNbr = 
                    g_rrc_stats.p_rrc_stats->rrc_csc_stats.rrc_csc_cell_stats[cell_index].
                    rrc_paging_related_stats.pag_succNbr;
                
                /*CR 025 changes start */
                paging_stats_ind.rrc_paging_stats[index].pag_receivedNbr = 
                    g_rrc_stats.p_rrc_stats->rrc_csc_stats.rrc_csc_cell_stats[cell_index].
                    rrc_paging_related_stats.pag_receivedNbr;
                /*CR 025 changes stop */
                index++;
            }
          }
        }
    }
    /*SPR_19262_START*/
    if(paging_stats_ind.count > 0)
	{
    result = rrc_oam_il_send_rrc_oam_paging_stats_ind(
            &paging_stats_ind,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            RRC_TRANSACTION_ID_ABSENT,
            RRC_NULL);
    if (RRC_FAILURE == result)
    {
        RRC_OAMH_TRACE(RRC_ERROR, "Unable to send RRC_OAM_PAGING_STATS_IND ");
    }
	}
    /*SPR_19262_END*/
    
    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
      for (index = 0; index < MAX_NUM_CELLS; index++)
      {
       memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_csc_stats.
               rrc_csc_cell_stats[index].rrc_paging_related_stats, 0,
               sizeof(rrc_csc_oam_stat_paging_t));

        p_oamh_gb_context->rrc_oamh_stats_status.rrc_paging_stats_status.
            is_reset_done[index] = RRC_FALSE;
        p_oamh_gb_context->rrc_oamh_stats_status.rrc_paging_stats_status.
            time_remaining_till_reset[index] = RRC_NULL;
      }
    }
    RRC_OAMH_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_common_la_overlap_irat_timer_expiry_handler 
 *
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function is called on the expiry of
 *       OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER 
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oamh_common_la_overlap_irat_timer_expiry_handler(
        rrc_oamh_gb_context_t *p_oamh_gb_context
        )
{
    rrc_oam_irat_mobility_stats_ind_t   irat_mobility_stats_ind;
    U8 index = RRC_NULL;
    U8 cell_index = RRC_NULL;
    U8 uecc_index = RRC_NULL;
    rrc_return_t    result = RRC_FAILURE;

    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_INFO, "OAMH_COMMON_LA_OVERLAP_IRAT_SAMPLING_TIMER expired");
    
    memset_wrapper(&irat_mobility_stats_ind, RRC_NULL, sizeof(rrc_oam_irat_mobility_stats_ind_t));

    if (p_oamh_gb_context->rrc_oamh_stats_status.m.
            is_IratIncMobility_LAI_counter_enable == 1)
    {
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
          for (cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
          {
            uecc_index = 0;

            if ((RRC_TRUE == g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].rrc_uecc_cell_stats[cell_index].
                        is_cell_configured) && (MAX_NUM_CELLS > index))
            {
                irat_mobility_stats_ind.count++;
                irat_mobility_stats_ind.rrc_irat_mobility_stats[index].cell_index = cell_index;

                if (p_oamh_gb_context->rrc_oamh_stats_status.rrc_IratIncMobility_LAI_stats_status.
                        is_reset_done[cell_index] == RRC_TRUE)
                {
                    irat_mobility_stats_ind.rrc_irat_mobility_stats[index].sampling_period = 
                        p_oamh_gb_context->rrc_oamh_stats_status.
                        rrc_IratIncMobility_LAI_stats_status.time_remaining_till_reset[cell_index];
                }
                else
                {
                    irat_mobility_stats_ind.rrc_irat_mobility_stats[index].sampling_period = 
                        p_oamh_gb_context->rrc_oamh_stats_status.
                        rrc_IratIncMobility_LAI_stats_status.sampling_period;
                }

                for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
                {
                    irat_mobility_stats_ind.rrc_irat_mobility_stats[index].rrc_IratIncMobility_LAI += 
                        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[cell_index].rrc_IratIncMobility_LAI;

                }
                index++;
            }
          }
        }
    }
    /*SPR_19262_START*/
    if(irat_mobility_stats_ind.count > 0)
	{
    result = rrc_oam_il_send_rrc_oam_irat_mobility_stats_ind(
            &irat_mobility_stats_ind,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            RRC_TRANSACTION_ID_ABSENT,
            RRC_NULL);
    if (RRC_FAILURE == result)
    {
        RRC_OAMH_TRACE(RRC_ERROR, "Unable to send RRC_OAM_IRAT_MOBILITY_STATS_IND");
    }
	}
    /*SPR_19262_END*/
    
    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
      for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            count && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
      {
        for (index = 0; index < MAX_NUM_CELLS; index++)
        {
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[uecc_index].
                rrc_uecc_cell_stats[index].rrc_IratIncMobility_LAI = 0;

            p_oamh_gb_context->rrc_oamh_stats_status.rrc_IratIncMobility_LAI_stats_status.
                is_reset_done[index] = RRC_FALSE;
            p_oamh_gb_context->rrc_oamh_stats_status.rrc_IratIncMobility_LAI_stats_status.
                time_remaining_till_reset[index] = RRC_NULL;
        }
      }
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: oamh_oam_process_proc_req 
 *   Inputs         :p_oamh_gb_context - Pointer to the OAMH global context data
 *   DESCRIPTION:
 *       This function checks the status of global health monitoring variable
 *       based on it sends RRC_OAM_PROC_SUPP_RESP message to OAM
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void oamh_oam_process_proc_req
(
 /*SPR 17777 +-*/
 rrc_oamh_gb_context_t *p_oamh_gb_context
)
{
	/* coverity_fix_54714_start */
	U16 trans_id = RRC_NULL;
	U8 count = 0;
	U8 no_of_active_th = 0;
	U8 oamh_th = 1;
	U8 adv_logger_th = 0;
	U8 no_of_uecc_instances = 0;
	RRC_ASSERT(PNULL != p_oamh_gb_context);

	RRC_OAMH_TRACE(RRC_INFO, "[RRC_OAM_PROC_SUPP_REQ] Received");

	if(1 == p_oamh_gb_context->no_of_uecc_instances)
	{
		if(RRC_TRUE == p_oamh_gb_context->rrc_oamh_health_monitor_val[0])
		{
			no_of_active_th++;  
		}
		else
		{
			RRC_OAMH_TRACE(RRC_FATAL, "oamh_oam_process_proc_req: status of the thread %s",
					oamh_health_monitor_desriptors[0].thrd_info);
		}
	}
	else
	{
		no_of_uecc_instances = p_oamh_gb_context->no_of_uecc_instances;
		for(count = 0;count < (RRC_MAX_THREADS - 1);count++)
		{
			if(RRC_TRUE == p_oamh_gb_context->rrc_oamh_health_monitor_val[count])
			{
				no_of_active_th++;
			}
			else
			{
				RRC_OAMH_TRACE(RRC_FATAL, "oamh_oam_process_proc_req: status of the thread %s",
						oamh_health_monitor_desriptors[count].thrd_info);
			}
		}
	}
#if (LOGGER_TYPE == RRC_ADV_LOGGER)
	if(RRC_TRUE == p_oamh_gb_context->adv_logger_hm_chk)
	{
		adv_logger_th = 1;
		if(RRC_TRUE == p_oamh_gb_context->rrc_oamh_health_monitor_val[11])
		{
			no_of_active_th++;  
		}
		else
		{
			RRC_OAMH_TRACE(RRC_FATAL, "oamh_oam_process_proc_req: status of the thread %s",
					oamh_health_monitor_desriptors[11].thrd_info);
		}
	}
#endif
	if(no_of_active_th == (no_of_uecc_instances + oamh_th + adv_logger_th))
	{
		rrc_oam_send_proc_res(RRC_OAMH_MODULE_ID,trans_id);
	}
	return;
}
/* coverity_fix_54714_stop */

/******************************************************************************
 *   FUNCTION NAME: rrc_oam_send_proc_res 
 *   DESCRIPTION:
 *       This function constructs and sends RRC_OAM_PROC_SUPP_RESP message to OAM
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/
void rrc_oam_send_proc_res
(
    U16 src_module_id,
    U16 trans_id
)
{
    U8* p_msg = PNULL;

    U16   total_msg_len = CV_HDRSIZE + RRC_INTERFACE_API_HEADER_SIZE;
    U16   msg_len = 0;


    p_msg = (U8*) rrc_msg_mem_get(total_msg_len);

    if (PNULL == p_msg)
    {
        RRC_OAMH_TRACE(RRC_WARNING, "%s:rrc_msg_mem_get failed",
                __FUNCTION__);
        return;
    }

    /* Fill CSPL header */
    rrc_construct_api_header(
            p_msg,
            RRC_VERSION_ID,
            src_module_id,
            RRC_OAM_MODULE_ID,
            RRC_OAM_PROC_SUPP_RESP,
            total_msg_len);

    /* Fill interface header */
    rrc_construct_interface_api_header(
            p_msg + RRC_API_HEADER_SIZE,
            trans_id,
            src_module_id,
            RRC_OAM_MODULE_ID,
            RRC_OAM_PROC_SUPP_RESP,
            msg_len,
            0);

    /* Send message to */
    /*SPR 20172 FIX START*/
    rrc_send_message(p_msg,RRC_OAM_MODULE_ID);
    /*SPR 20172 FIX End*/
}

/******************************************************************************
 *   FUNCTION NAME: rrc_oamh_process_health_monitor_chk 
 *   Inputs:        p_oamh_gb_context - Pointer to the OAMH global context data
 *   DESCRIPTION:
 *       This function copies global health monitoring variable
 *       in OAMH global context data and resets the global health
 *       monitor variable for RRC
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/

void rrc_oamh_process_health_monitor_chk
(
    rrc_oamh_gb_context_t *p_oamh_gb_context
 )
{
      U8 count = 0;
      RRC_ASSERT(PNULL != p_oamh_gb_context);
      
      if(1 == p_oamh_gb_context->gl_oamh_timer_exp_counter)
      {
/*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL 
    	  if(PNULL != rrc_lock)
          {
              qvLock(rrc_lock);
          }
#endif
/*Lockless Implementation stop*/
 /*Bug 9305 start*/
          l3_memcpy_wrapper(&p_oamh_gb_context->rrc_oamh_health_monitor_val,
                  &rrc_health_monitor_val,RRC_MAX_THREADS * sizeof(U32));
 /*Bug 9305 end*/
          for(count = 0;count < RRC_MAX_THREADS; count++)
          {
/*Lockless Implementation start*/
              sync_fetch_and_and_wrapper(&rrc_health_monitor_val[count],0,PNULL);/*Pass NULL as lock is already acquired*/
          }
#ifndef LOCKLESS_IMPL 
    	  if(PNULL != rrc_lock)
          {
	          qvUnlock(rrc_lock);
          }
#endif
/*Lockless Implementation stop*/
          p_oamh_gb_context->gl_oamh_timer_exp_counter = 0;
      }
      else
      {
          p_oamh_gb_context->gl_oamh_timer_exp_counter++;
          rrc_update_health_monitor_variable(OAMH_HEALTH_MONITOR_EXPIRY_EVENT);
      }
      return;
}
