/******************************************************************************
*
*   FILE NAME:
*       ueccmd_oamh.c
*
*   DESCRIPTION:
*       This is the source file of the UECC MD OAMH.
*       UECC MD OAMH interracts with RRC OAMH module.
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_defines.h"
#include "ueccmd_oamh.h"
#include "rrc_intrl_api.h"
#include "rrc_oamh_intf.h"
#include "rrc_msg_mgmt.h"
#include "ueccmd_utils.h"
#include "ueccmd_logger.h"
#include "rrc_common_utils.h"
#include "rrc_handlers.h"
#include "rrc_shared_stats.h"

/******************************************************************************
*   FUNCTION NAME: ueccmd_oamh_init_cnf_process_msg
*   INPUT        : void            *p_api
*                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : none
*   DESCRIPTION  : This function processes UECCMD_OAMH_INIT_CNF message
******************************************************************************/
void ueccmd_oamh_init_cnf_process_msg
(
    void            *p_api,          /* Pointer to input API buffer */
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
)
{
    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + 0)
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Stop wake up indication repeated timer */
    if (PNULL != p_ueccmd_gl_ctx->oamh_init_ind_timer)
    {
        RRC_UECCMD_TRACE(RRC_BRIEF, "Stop timer [UECCMD_OAMH_INIT_IND]");
        rrc_stop_timer(p_ueccmd_gl_ctx->oamh_init_ind_timer);
        p_ueccmd_gl_ctx->oamh_init_ind_timer = PNULL;
    }
    else
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "UECCMD_OAMH_INIT_CNF received again!");
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: ueccmd_oamh_prov_req_process_msg
*   INPUT        : void            *p_api
*                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : none
*   DESCRIPTION:    This function processes UECCMD_OAMH_PROV_REQ message
******************************************************************************/
void ueccmd_oamh_prov_req_process_msg
(
    void            *p_api,          /* Pointer to input API buffer */
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
)
{
    rrc_ueccmd_oamh_prov_req_t  *p_prov_req = PNULL;
    void                        *p_intrl_msg  = PNULL;
    rrc_uecc_oamh_prov_req_t    *p_uecc_oamh_prov_req = PNULL;
    U8                          uecc_index = RRC_NULL;
    rrc_return_et               result = RRC_SUCCESS;
    rrc_ueccmd_oamh_prov_resp_t *p_ueccmd_oamh_prov_resp = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_ueccmd_oamh_prov_req_t))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    p_prov_req = (rrc_ueccmd_oamh_prov_req_t *)((U8* )p_api + RRC_API_HEADER_SIZE);

    if(1 == p_prov_req->no_of_uecc_instances)
    {
        p_prov_req->uecc_oamh_prov_req.uecc_timers_info.
            uecc_health_monitor_timer_value = 0;
    }
    if (MAX_UECC_INSTANCES < p_prov_req->no_of_uecc_instances)
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Number of instance can't be greater than 8");
        result = RRC_FAILURE;
    }
    
    if (RRC_FAILURE == result)
    {
        /* Prepare UECCMD_OAMH_PROV_RESP message */
        p_intrl_msg = (void *)ueccmd_alloc_intrl_msg (RRC_UECCMD_MODULE_ID,
                RRC_OAMH_MODULE_ID,
                UECCMD_OAMH_PROV_RESP,
                sizeof(rrc_ueccmd_oamh_prov_resp_t));
        if (PNULL != p_intrl_msg)
        {
            p_ueccmd_oamh_prov_resp = (rrc_ueccmd_oamh_prov_resp_t *)((U8 *)
                p_intrl_msg + RRC_API_HEADER_SIZE);

            p_ueccmd_oamh_prov_resp->result = RRC_FAILURE;

            /* Send UECC_OAMH_PROV_RESP message */
            rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
        }
    }
    else
    {
        p_ueccmd_gl_ctx->num_uecc_instances = p_prov_req->no_of_uecc_instances;

        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
            memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats, RRC_NULL, 
                sizeof(rrc_uecc_stats_t));

            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.count = p_ueccmd_gl_ctx->num_uecc_instances;
        }
        p_ueccmd_gl_ctx->total_ue_supported = p_prov_req->uecc_oamh_prov_req.
            total_ue_supported;


        p_ueccmd_gl_ctx->current_proc = UECCMD_PROVISION;

        for (uecc_index = 0; uecc_index < p_ueccmd_gl_ctx->num_uecc_instances;
                uecc_index++)
        {
            /* Prepare UECC_OAMH_PROV_REQ message */
            p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_UECCMD_MODULE_ID,
                    (rrc_module_id_t)(RRC_UECC_1_MODULE_ID + uecc_index),
                     UECC_OAMH_PROV_REQ,
                     sizeof(rrc_uecc_oamh_prov_req_t));
            if (PNULL != p_intrl_msg)
            {
                /* Fill UECC_OAMH_PROV_REQ message */
                p_uecc_oamh_prov_req = (rrc_uecc_oamh_prov_req_t *)((U8 *)p_intrl_msg + 
                        RRC_API_HEADER_SIZE);

                l3_memcpy_wrapper (p_uecc_oamh_prov_req,
                        &p_prov_req->uecc_oamh_prov_req,
                        sizeof(rrc_uecc_oamh_prov_req_t));

                /* Send UECC_OAMH_PROV_RESP message */
                rrc_send_message(p_intrl_msg, (rrc_module_id_t)(RRC_UECC_1_MODULE_ID + uecc_index));
            }
        }
    }
    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: ueccmd_oamh_cleanup_req_process_msg
*   INPUT        : void            *p_api
*                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : none
*   DESCRIPTION:    This function processes UECCMD_OAMH_CLEANUP_REQ message
******************************************************************************/
void ueccmd_oamh_cleanup_req_process_msg
(
    void            *p_api,          /* Pointer to input API buffer */
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
)
{
    void                           *p_intrl_msg  = PNULL;
    U8                             uecc_index = RRC_NULL;
    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE)
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    if (RRC_NULL == p_ueccmd_gl_ctx->num_uecc_instances)
    {
        /* Prepare CLEANUP RESP message */
        p_intrl_msg = (void *)ueccmd_alloc_intrl_msg (RRC_UECCMD_MODULE_ID,
                RRC_OAMH_MODULE_ID,
                UECCMD_OAMH_CLEANUP_RESP,
                RRC_NULL);

        if (PNULL != p_intrl_msg)
        {
            RRC_UECCMD_TRACE(RRC_INFO, "Sending CleanUp Response from UECC MD.");
            /* Send UECC_OAMH_CLEANUP_RESP message */
            rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
        }

        rrc_ueccmd_init_gl_ctx (p_ueccmd_gl_ctx);

        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Prepare UECC_OAMH_CLEANUP_REQ message */
    for (uecc_index = 0; uecc_index < p_ueccmd_gl_ctx->num_uecc_instances;
            uecc_index++)
    {
        p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_UECCMD_MODULE_ID,
                (rrc_module_id_t)(RRC_UECC_1_MODULE_ID + uecc_index),
                UECC_OAMH_CLEANUP_REQ, RRC_NULL);

        if(PNULL != p_intrl_msg)
        {
            /* Send UECC_OAMH_PROV_RESP message */
            rrc_send_message(p_intrl_msg, (rrc_module_id_t)(RRC_UECC_1_MODULE_ID + uecc_index));
        }
    }

    p_ueccmd_gl_ctx->current_proc = UECCMD_CLEANUP;

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: ueccmd_process_traffic_trace_start_msg
*   INPUT        : void            *p_api
*                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : none
*   DESCRIPTION:    This function processes RRC_OAM_CELL_TRAFFIC_TRACE_START message
******************************************************************************/
void ueccmd_process_traffic_trace_start_msg
(
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx, /* Pointer to the UECC MD global context */
    void            *p_api          /* Pointer to input API buffer */
)
{
    U8          cell_index = RRC_NULL;

    RRC_UECCMD_UT_TRACE_ENTER();
    
    cell_index =  ueccmd_get_cell_index(p_api,
            UECCMD_TRAFFIC_TRACE_START_PROC_RUNNING);

    SET_CELL_INDEX(cell_index);

    if (cell_index >= MAX_NUM_CELLS) 
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong Cell Index - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    p_ueccmd_gl_ctx->ueccmd_cell_ctx[cell_index].bitmask |= 
        UECCMD_TRAFFIC_TRACE_START_PROC_RUNNING;

    /*Reset Cell_Context for Trace Procedure If any */
    p_ueccmd_gl_ctx->ueccmd_cell_ctx[cell_index].
        uecc_resp_per_cell[UECCMD_TRAFFIC_TRACE_START].
        num_resp_recv = RRC_NULL;

    memset_wrapper(p_ueccmd_gl_ctx->ueccmd_cell_ctx[cell_index].
            uecc_resp_per_cell[UECCMD_TRAFFIC_TRACE_START].
            uecc_resp, RRC_NULL, sizeof(U8)*MAX_UECC_INSTANCES);

    ueccmd_broadcast_msg (p_api, p_ueccmd_gl_ctx);

    RRC_UECCMD_UT_TRACE_EXIT();
}

/*lipa start */

/******************************************************************************
*   FUNCTION NAME: ueccmd_process_add_del_lgw_ind_msg
*
*   DESCRIPTION:
*       This function
*
*   RETURNS:
*       NONE
*
******************************************************************************/
void ueccmd_process_add_del_lgw_ind_msg
(
    ueccmd_gl_ctx_t           *p_ueccmd_gl_ctx,  /* Pointer to the UECCMD global context */
    void                      *p_api           /* Pointer to input API buffer */
)
{
    uecc_oamh_add_del_lgw_ind_t *p_uecc_oamh_add_del_lgw_ind = PNULL;
    ueccmd_oamh_add_del_lgw_ind_t *p_ueccmd_add_del_lgw_ind = PNULL;
    U8 uecc_index = RRC_NULL ;
    void                           *p_intrl_msg  = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT (p_ueccmd_gl_ctx != PNULL);
    RRC_ASSERT (p_api != PNULL);

    /* Precondition - p_api should point to right message */
    if( rrc_get_api_buf_size(p_api) != (RRC_API_HEADER_SIZE + sizeof(ueccmd_oamh_add_del_lgw_ind_t)))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    p_ueccmd_add_del_lgw_ind = 
     (ueccmd_oamh_add_del_lgw_ind_t *)((U8* )p_api + RRC_API_HEADER_SIZE);

    for (uecc_index = 0; uecc_index < p_ueccmd_gl_ctx->num_uecc_instances;
                uecc_index++)
    {
        /* Prepare UECC_OAMH_add_del_lgw_IND message */
        p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_UECCMD_MODULE_ID,
                (rrc_module_id_t)(RRC_UECC_1_MODULE_ID + uecc_index),
                UECC_OAMH_ADD_DEL_LGW_IND,
                sizeof(uecc_oamh_add_del_lgw_ind_t));
        if (PNULL != p_intrl_msg)
        {
            /* Fill UECC_OAMH_ADD_DEL_LGW_IND message */
            p_uecc_oamh_add_del_lgw_ind = (uecc_oamh_add_del_lgw_ind_t *)((U8 *)p_intrl_msg + 
                    RRC_API_HEADER_SIZE);

            l3_memcpy_wrapper (p_uecc_oamh_add_del_lgw_ind,
                    p_ueccmd_add_del_lgw_ind,
                    sizeof(uecc_oamh_add_del_lgw_ind_t));

            /* Send UECC_OAMH_ADD_DEL_LGW_IND message */
            rrc_send_message(p_intrl_msg, (rrc_module_id_t)(RRC_UECC_1_MODULE_ID + uecc_index));
        }
    }

   RRC_UECCMD_UT_TRACE_EXIT();
}

/*lipa end*/
