/******************************************************************************
*
*   FILE NAME:
*       rrc_llim_oamh.c
*
*   DESCRIPTION:
*       This is the source file of the LLIM OAMH.
*       LLIM OAMH interracts with RRC OAMH module.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   01 Apr 2009     VasylN      ---------       Initial
*   28 May 2009     VasylN      DD v0.2         Updated
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_defines.h"
#include "rrc_llim_oamh.h"
#include "rrc_intrl_api.h"
#include "rrc_oamh_intf.h"
#include "rrc_msg_mgmt.h"
#include "rrc_llim_utils.h"
#include "rrc_llim_logger.h"
#include "rrc_common_utils.h"
#include "rrc_cemh.h"
#include "rrc_uemh.h"

/******************************************************************************
*   FUNCTION NAME: check_and_fill_timers
*   INPUT        : rrc_llim_gl_ctx_t           *p_gl_ctx
*                  rrc_llim_oamh_prov_req_t    *p_prov_req
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes timers from RRC_OAM_PROVISION_REQ
*
******************************************************************************/
static rrc_return_et check_and_fill_timers
(
    rrc_llim_gl_ctx_t           *p_gl_ctx,  /* Destination */
    rrc_llim_oamh_prov_req_t    *p_prov_req /* Source */
)
{
    rrc_bool_et is_payload_correct  = RRC_TRUE;
    U16 timer_index                 = RRC_NULL;

    RRC_ASSERT(PNULL != p_gl_ctx);
    RRC_ASSERT(PNULL != p_prov_req);

    if (p_prov_req->timer_info.num_of_timers > MAX_LLIM_TIMER_INFO)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Incorrect number of timers [%u] received",
            p_prov_req->timer_info.num_of_timers);
        is_payload_correct = RRC_FALSE;
    }

    for (timer_index = 0; timer_index < p_prov_req->timer_info.num_of_timers &&
        is_payload_correct; timer_index++)
    {
        /* Check whether timer value is correct */
        if (RRC_NULL == p_prov_req->timer_info.timer_info[timer_index].timer_val)
        {
            RRC_LLIM_TRACE(RRC_ERROR, "Zero timer value [id = %u] received in "
                "LLIM_OAMH_PROV_REQ", p_prov_req->timer_info.
                timer_info[timer_index].timer_id);

            is_payload_correct = RRC_FALSE;
            break;
        }
        /* Fill local timer values */
        switch (p_prov_req->timer_info.timer_info[timer_index].timer_id)
        {
            case LLIM_CELL_SETUP_TIMER:
                p_gl_ctx->cell_cfg_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;

            case LLIM_CREATE_UE_ENTITY_TIMER:
                p_gl_ctx->create_ue_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;

            case LLIM_RECONFIG_UE_ENTITY_TIMER:
                p_gl_ctx->reconfig_ue_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                p_gl_ctx->del_rcfg_ue_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;

            case LLIM_DELETE_UE_ENTITY_TIMER:
                p_gl_ctx->delete_ue_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;

            case LLIM_CONFIG_UE_SECURITY_TIMER:
                p_gl_ctx->config_ue_security_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;

            case LLIM_CELL_RECONFIG_TIMER:
                p_gl_ctx->cell_recfg_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;
            case LLIM_SUSPEND_UE_ENTITY_TIMER:
                p_gl_ctx->suspend_ue_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;

            case LLIM_REESTABLISH_UE_ENTITY_TIMER:
                p_gl_ctx->reestablish_ue_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;
            case LLIM_RECONFIG_SECURITY_UE_ENTITY_TIMER:
               p_gl_ctx->reconfig_security_ue_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;
            case LLIM_RESUME_UE_ENTITY_TIMER:
               p_gl_ctx->resume_ue_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;
            case LLIM_CHANGE_CRNTI_TIMER:
               p_gl_ctx->change_crnti_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;
                
            case LLIM_CELL_START_TIMER:
                p_gl_ctx->cell_start_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;
                
            case LLIM_CELL_STOP_TIMER:
                p_gl_ctx->cell_stop_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;

            case LLIM_CELL_DELETE_TIMER:
                p_gl_ctx->cell_delete_duration = p_prov_req->timer_info.
                    timer_info[timer_index].timer_val;
                break;
            
            default:
                RRC_LLIM_TRACE(RRC_WARNING, "Unknown timer [id = %u] received "
                    "in LLIM_OAMH_PROV_REQ", p_prov_req->timer_info.
                    timer_info[timer_index].timer_id);

                is_payload_correct = RRC_FALSE;
                break;
        }
    }

    return ((RRC_TRUE == is_payload_correct) ? RRC_SUCCESS : RRC_FAILURE);
}

/******************************************************************************
*   FUNCTION NAME: rrc_llim_oamh_process_timer_msg
*   INPUT        : rrc_timer_t             timer_id
*                  rrc_llim_timer_buf_t    *p_rrc_llim_timer_buf
*                  rrc_llim_gl_ctx_t       *p_rrc_llim_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes OAMH timer message
******************************************************************************/
void rrc_llim_oamh_process_timer_msg
(
    /* Identity of the expired timer */
    rrc_timer_t             timer_id,
    /* Pointer to buffer associated with timer msg */
    rrc_llim_timer_buf_t    *p_rrc_llim_timer_buf,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t       *p_rrc_llim_gl_ctx
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != timer_id);
    RRC_ASSERT(PNULL != p_rrc_llim_timer_buf);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);


    RRC_LLIM_TRACE(RRC_BRIEF, "rrc_llim_oamh_process_timer_msg:"
            "timer id: %p expired submodule id: %u ",
            timer_id,p_rrc_llim_timer_buf->target_submodule);

    /* Currently we have only one timer for OAMH */
    if (timer_id == p_rrc_llim_gl_ctx->oamh_init_ind_timer)
    {
        /* Send wake up indication to OAMH */
        rrc_llim_send_internal_msg( RRC_OAMH_MODULE_ID, LLIM_OAMH_INIT_IND,
                                    0, PNULL);

        RRC_LLIM_TRACE(RRC_BRIEF, "Init indication sent to OAMH...");
    }
    else if(timer_id == p_rrc_llim_gl_ctx->llim_health_monitor_timer)
    {
        RRC_LLIM_TRACE(RRC_DETAILED, "processing llim health monitor time %p",timer_id);
        rrc_update_health_monitor_variable(LLIM_HEALTH_MONITOR_EXPIRY_EVENT);
    }
    else
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Unknown timer id [%p] received", timer_id);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_llim_oamh_init_cnf_processs_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes LLIM_OAMH_INIT_CNF message
******************************************************************************/
void rrc_llim_oamh_init_cnf_processs_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + 0)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Stop wake up indication repeated timer */
    if (PNULL != p_rrc_llim_gl_ctx->oamh_init_ind_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stop timer [LLIM_OAMH_INIT_IND]");
        rrc_stop_timer(p_rrc_llim_gl_ctx->oamh_init_ind_timer);
        p_rrc_llim_gl_ctx->oamh_init_ind_timer = PNULL;
    }
    else
    {
        RRC_LLIM_TRACE(RRC_WARNING, "LLIM_OAMH_INIT_CNF received again!");
    }

    RRC_LLIM_UT_TRACE_EXIT();
}


/******************************************************************************
*   FUNCTION NAME: rrc_llim_oamh_prov_req_processs_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes LLIM_OAMH_PROV_REQ message
*
******************************************************************************/
void rrc_llim_oamh_prov_req_processs_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_llim_oamh_prov_req_t    *p_prov_req = PNULL;
    rrc_llim_gl_ctx_t           temp_gl_ctx;
    rrc_llim_oamh_prov_resp_t   prov_resp;
    rrc_bool_t is_payload_correct = RRC_TRUE;
    U8     cell_index             = RRC_NULL;
    U8     loop                   = RRC_NULL;
    /* +- SPR 18268 */
    rrc_ue_index_t     ue_index               = RRC_NULL;
    /* +- SPR 18268 */
    U16    total_ue_supported     = RRC_NULL;



    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    memset_wrapper(&temp_gl_ctx, RRC_NULL, sizeof(rrc_llim_gl_ctx_t));

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_llim_oamh_prov_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    p_prov_req = (rrc_llim_oamh_prov_req_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    if(p_prov_req->bitmask & RRC_LLIM_HEALTH_MONITORING_TIMER_PRESENT)
    {
        start_llim_health_monitor_timer(p_rrc_llim_gl_ctx,
                ((p_prov_req->llim_health_monitor_timer_value * 1000)/2));
    }

    /* Check maximum number of supported UEs for all the cells */
    for (cell_index=0; cell_index<MAX_NUM_CELLS; cell_index++)
    {
        SET_CELL_INDEX(cell_index);

        if (p_prov_req->max_num_supported_ue_per_cell[cell_index] > RRC_MAX_NUM_SUPPORTED_UE)
        {
            RRC_LLIM_TRACE(RRC_ERROR, "Incorrect maximum number of supported UEs "
                    "[%u] received for CellIndex [%d]", p_prov_req->
                    max_num_supported_ue_per_cell[cell_index], cell_index);
            is_payload_correct = RRC_FALSE;
        }
        total_ue_supported = (U16)(total_ue_supported + p_prov_req->
            max_num_supported_ue_per_cell[cell_index]);

    }

    if ( (is_payload_correct == RRC_TRUE) &&
            p_prov_req->no_of_uecc_instances > MAX_UECC_INSTANCES)
    {
        /* Incorrect Number of UECC instances received */
        is_payload_correct = RRC_FALSE;
    }
    else if ((is_payload_correct == RRC_TRUE) && 
        (RRC_FAILURE == check_and_fill_timers(&temp_gl_ctx, p_prov_req)))
    {
        /* Incorrect timers received */
        is_payload_correct = RRC_FALSE;
    }
    /*Initialize UECC context db */
    else if ( (is_payload_correct == RRC_TRUE) && 
            (PNULL == (temp_gl_ctx.p_p_uecc_ctx = (rrc_llim_uecc_ctx_t  **)
                       rrc_mem_get((p_prov_req->no_of_uecc_instances) * sizeof(rrc_llim_uecc_ctx_t *))) ))
    {
        /* Not enough memory */
        is_payload_correct = RRC_FALSE;
    }
    else if ( is_payload_correct == RRC_TRUE ) 
    {
        for ( loop = 0; loop<p_prov_req->no_of_uecc_instances; loop++)
        {
            if (PNULL == (temp_gl_ctx.p_p_uecc_ctx[loop] = ((rrc_llim_uecc_ctx_t  *)
                    rrc_mem_get(sizeof(rrc_llim_uecc_ctx_t)))))
            {
                /* Not enough memory */
                is_payload_correct = RRC_FALSE;
                break;
            }
        }
    }
    if (is_payload_correct == RRC_TRUE)
    {
        for ( loop = 0; loop<p_prov_req->no_of_uecc_instances; loop++)
        {
            if (PNULL == (temp_gl_ctx.p_p_uecc_ctx[loop]->p_p_ue_ctx = (rrc_llim_ue_ctx_t * *)
                    rrc_mem_get(total_ue_supported * sizeof(rrc_llim_ue_ctx_t*))))
            {
                /* Not enough memory */
                is_payload_correct = RRC_FALSE;
                break;
            }
            memset_wrapper (temp_gl_ctx.p_p_uecc_ctx[loop]->p_p_ue_ctx,
                    RRC_NULL, (total_ue_supported * sizeof(rrc_llim_ue_ctx_t*)));
        }
    }
    /*Initialize Cell context db */
    if ( (is_payload_correct == RRC_TRUE) && 
            (PNULL == (temp_gl_ctx.p_p_cell_ctx = (struct _rrc_llim_cell_ctx_t * *)
                       rrc_mem_get((MAX_NUM_CELLS) * sizeof(struct _rrc_llim_cell_ctx_t *))) ))
    {
        /* Not enough memory */
        is_payload_correct = RRC_FALSE;
    }
    if (RRC_FALSE == is_payload_correct)
    {
        /* Roll back */
        if(temp_gl_ctx.p_p_uecc_ctx != PNULL)
        {
            for ( loop = 0; loop<p_prov_req->no_of_uecc_instances; loop++)
            {
                if (temp_gl_ctx.p_p_uecc_ctx[loop] != PNULL)
                {
                    if ( temp_gl_ctx.p_p_uecc_ctx[loop]->p_p_ue_ctx != PNULL)
                    {
                        rrc_mem_free(temp_gl_ctx.p_p_uecc_ctx[loop]->p_p_ue_ctx);
                    }
                    rrc_mem_free(temp_gl_ctx.p_p_uecc_ctx[loop]);
                }
            }
            rrc_mem_free(temp_gl_ctx.p_p_uecc_ctx);

        }
        if( temp_gl_ctx.p_p_cell_ctx != PNULL)
        {
            rrc_mem_free(temp_gl_ctx.p_p_cell_ctx);
        }

        /* negative response */
        prov_resp.result = RRC_FAILURE;
    }
    else
    {
        memset_wrapper (temp_gl_ctx.p_p_cell_ctx,
                    RRC_NULL, MAX_NUM_CELLS * sizeof(struct _rrc_llim_cell_ctx_t *));
        /* Fill global timer values with a local values in case of correct
         *  payload */
        p_rrc_llim_gl_ctx->oamh_init_ind_duration       = temp_gl_ctx.
            oamh_init_ind_duration;
        p_rrc_llim_gl_ctx->cell_cfg_duration            = temp_gl_ctx.
            cell_cfg_duration;
        p_rrc_llim_gl_ctx->cell_recfg_duration            = temp_gl_ctx.
            cell_recfg_duration;
        p_rrc_llim_gl_ctx->create_ue_duration           = temp_gl_ctx.
            create_ue_duration;
        p_rrc_llim_gl_ctx->reconfig_ue_duration         = temp_gl_ctx.
            reconfig_ue_duration;
        p_rrc_llim_gl_ctx->del_rcfg_ue_duration         = temp_gl_ctx.
            del_rcfg_ue_duration;
        p_rrc_llim_gl_ctx->delete_ue_duration           = temp_gl_ctx.
            delete_ue_duration;
        p_rrc_llim_gl_ctx->config_ue_security_duration  = temp_gl_ctx.
            config_ue_security_duration;

        p_rrc_llim_gl_ctx->suspend_ue_duration  = temp_gl_ctx.
            suspend_ue_duration;

        p_rrc_llim_gl_ctx->reestablish_ue_duration = temp_gl_ctx.
            reestablish_ue_duration;

        p_rrc_llim_gl_ctx->reconfig_security_ue_duration = temp_gl_ctx.
            reconfig_security_ue_duration;

        p_rrc_llim_gl_ctx->resume_ue_duration = temp_gl_ctx.
            resume_ue_duration;
        p_rrc_llim_gl_ctx->change_crnti_duration = temp_gl_ctx.
            change_crnti_duration;

        p_rrc_llim_gl_ctx->cell_start_duration            = temp_gl_ctx.
            cell_start_duration;
        
        p_rrc_llim_gl_ctx->cell_stop_duration            = temp_gl_ctx.
            cell_stop_duration;
        
        p_rrc_llim_gl_ctx->cell_delete_duration          = temp_gl_ctx.
            cell_delete_duration;


       l3_memcpy_wrapper (&(p_rrc_llim_gl_ctx->max_num_supported_ue), 
               &(p_prov_req->max_num_supported_ue_per_cell), 
               sizeof(p_prov_req->max_num_supported_ue_per_cell));

       if (PNULL != p_rrc_llim_gl_ctx->p_p_uecc_ctx)
       {
           /*Check that Ue context for all existing UECC instance is null or
            * not */
           for (loop = 0; loop < p_rrc_llim_gl_ctx->no_of_uecc_instances; loop++)
           {
               if (p_rrc_llim_gl_ctx->p_p_uecc_ctx[loop] != PNULL)
               {
                   if( p_rrc_llim_gl_ctx->p_p_uecc_ctx[loop]->p_p_ue_ctx != PNULL)
                   {
                       for (ue_index = 0; ue_index < total_ue_supported;
                               ue_index++)
                       {
                           SET_UE_INDEX(ue_index);

                           if( p_rrc_llim_gl_ctx->p_p_uecc_ctx[loop]->
                                   p_p_ue_ctx[ue_index] != PNULL)
                           {
                               rrc_mem_free(p_rrc_llim_gl_ctx->
                                       p_p_uecc_ctx[loop]->p_p_ue_ctx[ue_index]);

                               p_rrc_llim_gl_ctx->
                                   p_p_uecc_ctx[loop]->p_p_ue_ctx[ue_index] = PNULL;
                           }
                       }

                       rrc_mem_free(p_rrc_llim_gl_ctx->p_p_uecc_ctx[loop]->
                               p_p_ue_ctx);

                       p_rrc_llim_gl_ctx->p_p_uecc_ctx[loop]->
                           p_p_ue_ctx = PNULL;
                   }
                   rrc_mem_free(p_rrc_llim_gl_ctx->p_p_uecc_ctx[loop]);
                   p_rrc_llim_gl_ctx->p_p_uecc_ctx[loop] = PNULL;
               }
           }
           rrc_mem_free(p_rrc_llim_gl_ctx->p_p_uecc_ctx);
           p_rrc_llim_gl_ctx->p_p_uecc_ctx = PNULL;
       }

        p_rrc_llim_gl_ctx->p_p_uecc_ctx = temp_gl_ctx.p_p_uecc_ctx;

        p_rrc_llim_gl_ctx->no_of_uecc_instances          = 
            p_prov_req->no_of_uecc_instances;

       /* Initialize Cell DB */
        if (PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx)
        {
            for(loop=0; loop<MAX_NUM_CELLS; loop++)
            {
                SET_CELL_INDEX(loop);

                if ( p_rrc_llim_gl_ctx->p_p_cell_ctx[loop] != PNULL)
                {
                    rrc_mem_free(p_rrc_llim_gl_ctx->p_p_cell_ctx[loop]);
                    p_rrc_llim_gl_ctx->p_p_cell_ctx[loop] = PNULL;
                }
            }
            rrc_mem_free(p_rrc_llim_gl_ctx->p_p_cell_ctx);
            p_rrc_llim_gl_ctx->p_p_cell_ctx = PNULL;
        }

        p_rrc_llim_gl_ctx->p_p_cell_ctx = temp_gl_ctx.p_p_cell_ctx;

        /* positive response */
        prov_resp.result = RRC_SUCCESS;
    }

    /* Send response */
    rrc_llim_send_internal_msg( RRC_OAMH_MODULE_ID, LLIM_OAMH_PROV_RESP,
                                sizeof(prov_resp), &prov_resp);

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_llim_oamh_cleanup_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes LLIM_OAMH_CLEANUP_REQ message
******************************************************************************/
void rrc_llim_oamh_cleanup_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    U8     cell_index = RRC_NULL;
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + 0)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Cleanup UEs */
    /* Now UE Context will be cleaned per uecc basis */
    /*coverity fix start*/
    rrc_uemh_oamh_cleanup_req_process_msg(p_api, p_rrc_llim_gl_ctx); 

    /* Cleanup cells */
    rrc_cemh_oamh_cleanup_req_process_msg(p_api, p_rrc_llim_gl_ctx);
    /*coverity fix stop*/


    /* RRC_ASSERT(0 == p_rrc_llim_gl_ctx->max_num_supported_ue); */

    /* Cleanup Cell DB. Already done ... */
    if ( PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx)
    {
        for (cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
        {
            RRC_ASSERT(PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]);
        }
    }

    if (PNULL != p_rrc_llim_gl_ctx->oamh_init_ind_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stop timer [LLIM_OAMH_INIT_IND]");
        rrc_stop_timer(p_rrc_llim_gl_ctx->oamh_init_ind_timer);
        p_rrc_llim_gl_ctx->oamh_init_ind_timer = PNULL;
    }

    /* Init LLIM global context */
    rrc_llim_init_gl_ctx(p_rrc_llim_gl_ctx);

    /* Send report to OAMH */
    rrc_llim_send_internal_msg( RRC_OAMH_MODULE_ID, LLIM_OAMH_CLEANUP_RESP,
                                0, PNULL);

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: start_llim_health_monitor_timer
*
*       This function starts health monitor timer
*       for llim module if duration is greater 
*       than zero
*
*   RETURNS:
*       None
*
******************************************************************************/
void start_llim_health_monitor_timer
(
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx,
    rrc_timer_duration_t    duration
)        
{
    rrc_llim_timer_buf_t repeated_timer_buf;
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    if(0 == duration)
    {
        return;
    }

    repeated_timer_buf.auto_delete = RRC_FALSE;
    repeated_timer_buf.target_submodule = LLIM_SUBMODULE_OAMH;

    p_rrc_llim_gl_ctx->llim_health_monitor_timer =
        rrc_start_timer(duration,
                &repeated_timer_buf,
                sizeof(repeated_timer_buf),
                RRC_TRUE);

    RRC_LLIM_UT_TRACE_EXIT();
}
