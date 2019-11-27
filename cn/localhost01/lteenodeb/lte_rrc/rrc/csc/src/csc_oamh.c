/******************************************************************************
*
*   FILE NAME:
*       csc_oamh.c
*
*   DESCRIPTION:
*       This is the source file of the CSC OAMH.
*       CSC OAMH interracts with RRC OAMH module.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*   06 June 2009    VasylS      ---------       Cleanup support
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_defines.h"
#include "csc_oamh.h"
#include "rrc_intrl_api.h"
#include "rrc_oamh_intf.h"
#include "rrc_msg_mgmt.h"
#include "csc_utils.h"
#include "csc_logger.h"
#include "rrc_common_utils.h"
#include "csc_cell_m.h"
/* SPR 20908 Start */
#include "csc_cell_m_fsm.h"
#include "rrc_rrm_il_parser.h"
/*SPR 21096 Start*/
	U8 L2_INSTANCE_ID[MAX_NUM_CELLS];

/*SPR 21096 End*/
/* SPR 20908 End */
/******************************************************************************
*   FUNCTION NAME: csc_oamh_init_cnf_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes CSC_OAMH_INIT_CNF message
******************************************************************************/
void csc_oamh_init_cnf_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + 0)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /* Stop wake up indication repeated timer */
    if (PNULL != p_csc_gl_ctx->oamh_init_ind_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_OAMH_INIT_IND]");
        rrc_stop_timer(p_csc_gl_ctx->oamh_init_ind_timer);
        p_csc_gl_ctx->oamh_init_ind_timer = PNULL;
    }
    else
    {
        RRC_CSC_TRACE(RRC_WARNING, "CSC_OAMH_INIT_CNF received again!");
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: csc_oamh_prov_req_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  :    This function processes CSC_OAMH_PROV_REQ message
******************************************************************************/
void csc_oamh_prov_req_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    rrc_csc_oamh_prov_req_t *p_prov_req = PNULL;
    void *p_intrl_msg               = PNULL;
    rrc_bool_et is_payload_correct  = RRC_TRUE;
    U16 timer_index                 = RRC_NULL;
    rrc_timer_duration_t cell_cfg_duration = RRC_NULL;
    rrc_timer_duration_t cell_recfg_duration = RRC_NULL;
    rrc_timer_duration_t cell_del_duration = RRC_NULL;
    rrc_timer_duration_t pws_duration = RRC_NULL;
    rrc_timer_duration_t kill_duration = RRC_NULL;

    rrc_timer_duration_t cell_start_duration = RRC_NULL;
    rrc_timer_duration_t cell_stop_duration = RRC_NULL;
    rrc_return_t result = RRC_FAILURE;
    U8           cell_index = 0;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    cell_cfg_duration = p_csc_gl_ctx->cell_cfg_duration;
    cell_recfg_duration = p_csc_gl_ctx->cell_recfg_duration;
    cell_del_duration = p_csc_gl_ctx->cell_del_duration;
    pws_duration = p_csc_gl_ctx->pws_duration;
    kill_duration = p_csc_gl_ctx->kill_duration;
    cell_start_duration = p_csc_gl_ctx->cell_start_duration;
    cell_stop_duration = p_csc_gl_ctx->cell_stop_duration;

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_csc_oamh_prov_req_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    p_prov_req = (rrc_csc_oamh_prov_req_t *)((U8* )p_api + RRC_API_HEADER_SIZE);

    if(p_prov_req->bitmask & RRC_CSC_HEALTH_MONITORING_TIMER_PRESENT)
    {
        start_csc_health_monitor_timer(p_csc_gl_ctx,
                ((p_prov_req->csc_health_monitor_timer_value * 1000)/2));
    }
    p_csc_gl_ctx->max_cell_reconfig_retry_count =  p_prov_req->
        max_cell_reconfig_retry_count ;

    p_csc_gl_ctx->csc_cell_index_status =  p_prov_req->
        csc_cell_index_status ;

    p_csc_gl_ctx->no_of_uecc_instances =  p_prov_req->
       no_of_uecc_instances ;

    l3_memcpy_wrapper( p_csc_gl_ctx->null_integrity_support, 
            p_prov_req->null_integrity_support,
            sizeof(p_prov_req->null_integrity_support));

    p_csc_gl_ctx->max_num_supported_cells = p_prov_req->max_num_supported_cells;

    

    /* Check timers count */
    if (p_prov_req->oam_csc_timer_info.num_of_timers > MAX_CSC_TIMER_INFO)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Incorrect number of timers [%u] received",
            p_prov_req->oam_csc_timer_info.num_of_timers);
        is_payload_correct = RRC_FALSE;
    }
    else if (p_prov_req->max_num_supported_cells > MAX_NUM_CELLS)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Incorrect max number of cells [%u] received",
            p_prov_req->max_num_supported_cells);
        is_payload_correct = RRC_FALSE;
    }
    else
    {
        for (cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
        {
            SET_CELL_INDEX(cell_index);

            if(p_prov_req->max_num_supported_ue_per_cell[cell_index] > MAX_NUM_UE)
            {
                RRC_CSC_TRACE(RRC_WARNING, "Incorrect max number of ue per cell [%u] received for Cell Index: %u",
                        p_prov_req->max_num_supported_ue_per_cell[cell_index], cell_index);
                is_payload_correct = RRC_FALSE;
                break;
            }
        }

        if (is_payload_correct)
        {

            l3_memcpy_wrapper(p_csc_gl_ctx->max_num_supported_ue_per_cell,
                    p_prov_req->max_num_supported_ue_per_cell,
                    sizeof(p_prov_req->max_num_supported_ue_per_cell));
        }
        /* Process payload */
        for (timer_index = 0; timer_index < p_prov_req->oam_csc_timer_info.num_of_timers &&
            is_payload_correct; timer_index++)
        {
            /* Check whether timer value is correct */
            if (RRC_NULL== p_prov_req->oam_csc_timer_info.timer_info[timer_index].timer_val)
            {
                RRC_CSC_TRACE(RRC_WARNING, "Zero timer value [id = %u] "
                    "received in CSC_OAMH_PROV_REQ", p_prov_req->oam_csc_timer_info.
                    timer_info[timer_index].timer_id);

                is_payload_correct = RRC_FALSE;
                break;
            }
            /* Fill local timer values */
            switch (p_prov_req->oam_csc_timer_info.timer_info[timer_index].timer_id)
            {
                case CSC_CELL_SETUP_TIMER:
                    cell_cfg_duration = p_prov_req->oam_csc_timer_info.timer_info[timer_index].
                        timer_val;
                    break;
                
                case CSC_CELL_RECONFIG_TIMER:
                    cell_recfg_duration = p_prov_req->oam_csc_timer_info.timer_info[timer_index].
                        timer_val;
                    break;
                case CSC_CELL_DELETE_TIMER:
                    cell_del_duration = p_prov_req->oam_csc_timer_info.timer_info[timer_index].
                    timer_val;
                    break;

                case CSC_PWS_TIMER:
                    pws_duration = p_prov_req->oam_csc_timer_info.timer_info[timer_index].
                    timer_val;
                    break;
                
                case CSC_KILL_TIMER:
                    kill_duration = p_prov_req->oam_csc_timer_info.timer_info[timer_index].
                    timer_val;
                    break;

                case CSC_CELL_START_TIMER:
                    cell_start_duration = p_prov_req->oam_csc_timer_info.timer_info[timer_index].
                        timer_val;
                    break;
                
                case CSC_CELL_STOP_TIMER:
                    cell_stop_duration = p_prov_req->oam_csc_timer_info.timer_info[timer_index].
                        timer_val;
                    break;
                
                default:
                    RRC_CSC_TRACE(RRC_WARNING, "Unknown timer [id = %u] "
                        "received in CSC_OAMH_PROV_REQ", p_prov_req->oam_csc_timer_info.
                        timer_info[timer_index].timer_id);

                    is_payload_correct = RRC_FALSE;
                    break;
            }
        }
    }

    
    /* Prepare CSC_OAMH_PROV_RESP message */
    p_intrl_msg = csc_alloc_intrl_msg(  RRC_OAMH_MODULE_ID,
                                        CSC_OAMH_PROV_RESP,
                                        sizeof(rrc_csc_oamh_prov_resp_t));

    if (PNULL != p_intrl_msg)
    {
        /* Fill CSC_OAMH_PROV_RESP message */
        rrc_csc_oamh_prov_resp_t *p_prov_resp = (rrc_csc_oamh_prov_resp_t *)((U8
            *)p_intrl_msg + RRC_API_HEADER_SIZE);

        /* Initialize the CSC DB */
        result = csc_cell_ctx_db_init (p_csc_gl_ctx);

        if ((RRC_FALSE == is_payload_correct) ||
                (RRC_FAILURE == result))
        {
            /* negative response */
            p_prov_resp->result = RRC_FAILURE;
        }
        else
        {
            p_csc_gl_ctx->cell_cfg_duration = cell_cfg_duration;
            p_csc_gl_ctx->cell_recfg_duration = cell_recfg_duration ;
            p_csc_gl_ctx->cell_del_duration = cell_del_duration;
            p_csc_gl_ctx->pws_duration = pws_duration;
            p_csc_gl_ctx->kill_duration = kill_duration;
            p_csc_gl_ctx->cell_start_duration = cell_start_duration;
            p_csc_gl_ctx->cell_stop_duration = cell_stop_duration;
            p_csc_gl_ctx->sfn_offset = p_prov_req->sfn_offset;

            /* positive response */
            p_prov_resp->result = RRC_SUCCESS;
        }

        /* Send CSC_OAMH_PROV_RESP message */
        rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: csc_oamh_cleanup_req_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes CSC_OAMH_CLEANUP_REQ message
******************************************************************************/
void csc_oamh_cleanup_req_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    void *p_intrl_msg = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + 0)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /* Cleanup cells */
    cell_m_oamh_cleanup_req_process_msg(p_csc_gl_ctx);

    /* Cleanup CSC global context */
    /* Cleanup Cell DB. Already done ... */
    RRC_ASSERT(PNULL == p_csc_gl_ctx->p_p_cell_ctx);

    if (PNULL != p_csc_gl_ctx->oamh_init_ind_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_OAMH_INIT_IND]");
        rrc_stop_timer(p_csc_gl_ctx->oamh_init_ind_timer);
        p_csc_gl_ctx->oamh_init_ind_timer = PNULL;
    }

    /* Init CSC global context */
    rrc_csc_init_gl_ctx(p_csc_gl_ctx);

    /* Send report to OAMH */
    p_intrl_msg = csc_alloc_intrl_msg(RRC_OAMH_MODULE_ID, CSC_OAMH_CLEANUP_RESP,
        0);

    if (PNULL != p_intrl_msg)
    {
        /* Fill and send message */
        rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
    }

    RRC_CSC_UT_TRACE_EXIT();
}
/* SPR 20908 Start */
/******************************************************************************
*   FUNCTION NAME: csc_oamh_communication_info_resp_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes CSC_OAMH_CLEANUP_REQ message
******************************************************************************/

void csc_oamh_communication_info_resp_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    static rrc_rrm_cell_setup_req_t cell_setup_req;
    rrc_return_t    result = RRC_FAILURE;
    rrc_oamh_communication_info_resp_t *communication_info_resp;
    rrm_fail_cause_et       fail_cause = RRM_RRC_INTERNAL_FAILURE;
    S32 length_read             = RRC_NULL;
    void *p_api_cell;
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    
    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof(rrc_oamh_communication_info_resp_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }
    /* Get message payload */
    communication_info_resp = (rrc_oamh_communication_info_resp_t *)((U8* )p_api + RRC_API_HEADER_SIZE);
    /*SPR 21096 Start*/
    SET_CELL_INDEX(communication_info_resp->cell_index);
    /*SPR 21096 End*/
    p_api_cell = (void*)(p_csc_gl_ctx->p_p_cell_ctx[communication_info_resp->cell_index]->p_api);

    memset_wrapper(&cell_setup_req, 0 ,sizeof(rrc_rrm_cell_setup_req_t));

    do {
        /* Precondition - p_api_cell is the data for cell setup */
        if ((RRC_SUCCESS != rrc_il_parse_rrc_rrm_cell_setup_req(
                    &cell_setup_req,
                    (U8* )p_api_cell + RRC_FULL_INTERFACE_HEADERS_SIZE,
                    rrc_get_api_buf_size(p_api_cell) -
                    RRC_FULL_INTERFACE_HEADERS_SIZE,
                    &length_read)) || (communication_info_resp->result == RRC_FALSE))
        {
            fail_cause = RRM_RRC_INVALID_CONFIGURATION_RECV;
            RRC_CSC_TRACE(RRC_WARNING, "Bad message - ignore.");
            RRC_CSC_UT_TRACE_EXIT();
            /* Drop message */
            break;
        }
        /*SPR 21096 Start*/
        /* Updating the L2 instance info in global array as per cell_index */
        L2_INSTANCE_ID[cell_setup_req.cell_index] = cell_setup_req.cell_config_param.associated_layer2_inst; 
		/*SPR 21096 End*/
        /* Normal message processing */
        cell_m_fsm_process_event(   CELL_M_EVENT_RRM_CONFIG_CELL_REQ,
                p_api_cell,
		&cell_setup_req,
		/*SPR 21096 Start*/
		p_csc_gl_ctx->p_p_cell_ctx[communication_info_resp->cell_index],
		/*SPR 21096 End*/
		p_csc_gl_ctx);
        result = RRC_SUCCESS;
    } while (0);

    if (RRC_SUCCESS != result)
    {
        /* Send failure response */
        if (RRC_SUCCESS != csc_send_rrm_cell_setup_resp(
                rrc_get_transaction_id(p_api),
                cell_setup_req.cell_index,
                RRC_FAILURE,
                fail_cause))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "Unable to RESP FAILURE to RRM.");
        }
    }


    RRC_CSC_UT_TRACE_EXIT();
}
/* SPR 20908 End */
/******************************************************************************
*   FUNCTION NAME: csc_oamh_process_timer_msg
*   INPUT        : rrc_timer_t         timer_id
*                  csc_timer_buf_t     *p_csc_timer_buf
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes OAMH timer message
******************************************************************************/
void csc_oamh_process_timer_msg
(
    rrc_timer_t         timer_id,           /* Identity of the expired timer */
    /* Pointer to buffer associated with timer msg */
    csc_timer_buf_t     *p_csc_timer_buf,
    /* Pointer to the CSC global context data */
    csc_gl_ctx_t        *p_csc_gl_ctx
)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != timer_id);
    RRC_ASSERT(PNULL != p_csc_timer_buf);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);


    RRC_CSC_TRACE(RRC_BRIEF, "csc_oamh_process_timer_msg :"
            "timer id: %p expired, submodule id: %u",
            timer_id,p_csc_timer_buf->target_submodule);

    /* Currently we have only one timer for OAMH */
    if (timer_id == p_csc_gl_ctx->oamh_init_ind_timer)
    {
        /* Send wake up indication to OAMH */
        void *p_intrl_msg = csc_alloc_intrl_msg(RRC_OAMH_MODULE_ID,
            CSC_OAMH_INIT_IND, 0);

        if (PNULL != p_intrl_msg)
        {
            /* Fill and send message */
            rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
        }

        RRC_CSC_TRACE(RRC_BRIEF, "Init indication sent to OAMH...");
    }
    else if(timer_id == p_csc_gl_ctx->csc_health_monitor_timer)
    {
        RRC_CSC_TRACE(RRC_DETAILED, "processing csc health monitor timer %p", timer_id);
        rrc_update_health_monitor_variable(CSC_HEALTH_MONITOR_EXPIRY_EVENT);
    }
    else
    {
        RRC_CSC_TRACE(RRC_WARNING, "Unknown timer id [%p] received", timer_id);
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: start_csc_health_monitor_timer
*
*   Description:   This function starts health monitor timer
*                  for csc module if duration is greater 
*                  than zero.
*
*   RETURNS:
*       None
*
******************************************************************************/
void start_csc_health_monitor_timer
(
    csc_gl_ctx_t *p_csc_gl_ctx,
    rrc_timer_duration_t    duration
)
{
    csc_timer_buf_t repeated_timer_buf;

    RRC_CSC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    if(0 == duration)
    {
        return;
    }

    repeated_timer_buf.auto_delete = RRC_FALSE;
    repeated_timer_buf.target_submodule = CSC_SUBMODULE_OAMH;

    p_csc_gl_ctx->csc_health_monitor_timer =
        rrc_start_timer(duration,
                &repeated_timer_buf,
                sizeof(repeated_timer_buf),
                RRC_TRUE);
    RRC_CSC_UT_TRACE_EXIT();
}

