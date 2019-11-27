/******************************************************************************
*
*   FILE NAME:
*       csc_sfn_m.c
*
*   DESCRIPTION:
*       This header file contains in points of SFN manager submodule.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*   06 June 2009    VasylS      ---------       Cleanup support
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "csc_sfn_m.h"
#include "rrc_defines.h"
#include "csc_logger.h"
#include "csc_cell_ctx.h"
#include "rrc_msg_mgmt.h"
#include "csc_sfn_m_timers.h"
#include "rrc_csc_llim_intf.h"
#include "csc_paging_m.h"
#include "csc_cell_m_fsm.h"
#include "rrc_ext_api.h"
#include "csc_cell_m_timers.h"
#include "csc_utils.h"
#include "rrc_rrm_il_composer.h"
#include "rrc_intrl_api.h"

/****************************************************************************
* Local Functions Prototypes
****************************************************************************/
rrc_return_t csc_paging_build_and_send_si_paging_msg(
    rrc_sfn_t    sfn
);

/*SPR:7484 Start*/
static void sfn_m_update_csc_sfn(
    rrc_csc_llim_sfn_resp_t*  p_llim_sfn_resp,
    csc_gl_ctx_t    *p_csc_gl_ctx,   /* Pointer to the CSC global context data */
    rrc_cell_index_t    cell_index,
    rrc_bool_et      mac_update_reqd
);
/*SPR:7484 Stop*/



extern U16 fill_si_periodicity_in_sfn (U8  si_periodicity);

/******************************************************************************
*   FUNCTION NAME: sfn_m_init_sfn
*   INPUT        : rrc_sfn_t       sfn
*                  csc_cell_ctx_t  *p_cell_ctx
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function init SFN in RRC cell.
******************************************************************************/
void sfn_m_init_sfn(
    rrc_sfn_t       sfn,            /* initial SFN */
    csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    csc_sfn_m_timer_buf_t sfn_timer_data;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    if (sfn < SFN_UPPER_LIMIT)
    {
        p_cell_ctx->sfn = sfn;
        p_cell_ctx->mod_period_boud_counter = p_cell_ctx->mod_period - (sfn % p_cell_ctx->mod_period);
    }
    else
    {
        p_cell_ctx->sfn = RRC_NULL;
        RRC_CSC_TRACE(RRC_WARNING, "Init with too big SFN. Start from RRC_NULL"
            ".");
    }

    RRC_CSC_TRACE(RRC_INFO, "SFN == %d", p_cell_ctx->sfn);

    /* Start SFN timer. */
    sfn_timer_data.auto_delete      = RRC_FALSE;
    sfn_timer_data.target_submodule = CSC_SUBMODULE_SFN_M;
    sfn_timer_data.cell_index       = p_cell_ctx->cell_index;

    RRC_CSC_TRACE(RRC_BRIEF,
        "[CELL:%u] Start timer [SFN] %u ms.",
        p_cell_ctx->cell_index, p_csc_gl_ctx->sfn_period);

    p_cell_ctx->sfn_timer =
        rrc_start_timer(p_csc_gl_ctx->sfn_period, &sfn_timer_data,
            sizeof(sfn_timer_data), RRC_TRUE);

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: sfn_m_get_sfn
*   INPUT        : csc_cell_ctx_t  *p_cell_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function return current SFN in RRC cell.
******************************************************************************/
rrc_sfn_t sfn_m_get_sfn(
    csc_cell_ctx_t *p_cell_ctx  /* cell context */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);

    RRC_CSC_UT_TRACE_EXIT();

    return p_cell_ctx->sfn;
}

/******************************************************************************
*   FUNCTION NAME: sfn_m_llim_sfn_ind_process_msg
*   INPUT        : void            *p_api 
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function process CSC_LLIM_SFN_IND message.
******************************************************************************/
void sfn_m_llim_sfn_ind_process_msg(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
    )
{
    rrc_csc_llim_sfn_ind_t *p_llim_sfn_ind = PNULL;
    csc_cell_ctx_t *p_cell_ctx = PNULL;
    /* SPR_15768 FIX Start */
    S16             rrc_mac_sfn_gap = RRC_NULL;
    /* SPR_15768 FIX End */

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_sfn_ind_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    p_llim_sfn_ind = (rrc_csc_llim_sfn_ind_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_llim_sfn_ind->cell_index);

    /* Search in DB cell with p_llim_sfn_ind->cell_index - should be present */
    if ((PNULL == p_csc_gl_ctx->p_p_cell_ctx[p_llim_sfn_ind->cell_index]) ||
            ((p_csc_gl_ctx->p_p_cell_ctx[p_llim_sfn_ind->cell_index])->
             cell_index != p_llim_sfn_ind->cell_index))
    {
        /* ignore */
        RRC_CSC_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[p_llim_sfn_ind->cell_index];

        /* SPR_15768 FIX Start */
        if ((p_llim_sfn_ind->transaction_id == p_cell_ctx->transaction_id) &&
                (p_cell_ctx->cell_reconfig_counter > 0))
        {
            /* while calculating rrc_mac_sfn_gap, it is assumed
             * that if difference between MAC and CSC SFN is
             * more than CSC_SFN_REARRANGE_LIMIT then MAC SFN
             * is greater than CSC SFN. */
            if((p_llim_sfn_ind->sfn > p_cell_ctx->sfn) 
                    && (p_llim_sfn_ind->sfn - p_cell_ctx->sfn 
                        <= SFN_UPPER_LIMIT - CSC_SFN_REARRANGE_LIMIT)
                    && (p_llim_sfn_ind->sfn - p_cell_ctx->sfn > 0 ))
            { /* MAC SFN is greater than CSC SFN */
                rrc_mac_sfn_gap = (S16)(p_llim_sfn_ind->sfn - p_cell_ctx->sfn);
            }
            else if(p_llim_sfn_ind->sfn > p_cell_ctx->sfn)
            { /* CSC SFN has wrap around and is ahead of MAC SFN */
                rrc_mac_sfn_gap = (S16)(p_llim_sfn_ind->sfn - p_cell_ctx->sfn - SFN_UPPER_LIMIT);
            }
            else if(p_cell_ctx->sfn - p_llim_sfn_ind->sfn <= CSC_SFN_REARRANGE_LIMIT)
            { /* CSC SFN is greater than MAC SFN */
                rrc_mac_sfn_gap = (S16)(p_llim_sfn_ind->sfn - p_cell_ctx->sfn);
            }
            else 
            {  /* MAC SFN has wrap around and is ahead of CSC SFN */
                rrc_mac_sfn_gap = (S16)(p_llim_sfn_ind->sfn + SFN_UPPER_LIMIT - p_cell_ctx->sfn);
            }
            if ((S16)p_cell_ctx->cell_reconfig_counter > rrc_mac_sfn_gap)
            {
                p_cell_ctx->cell_reconfig_counter =
                    (U32)((S32)(p_cell_ctx->cell_reconfig_counter) - rrc_mac_sfn_gap);
            }
            else
                /* SPR_15768 FIX End */
            {/* SFN ERROR IND is received for SI modification paging
              * message. Because paging for SI Modification has not been
              * broadcasted, stopping cell reconfiguration procedure.
              */
                RRC_CSC_TRACE(RRC_INFO,
                        " Dropping RRC RRM cell reconfiguration message.");

                p_cell_ctx->cell_reconfig_counter = 0;
                p_cell_ctx->si_mod_flag = RRC_NULL;
                p_cell_ctx->rcfg_status_flag = RRC_FALSE;

                if (PNULL != p_cell_ctx->cell_reconfig_api)
                {
                    rrc_msg_mem_free(p_cell_ctx->cell_reconfig_api);
                    p_cell_ctx->cell_reconfig_api = PNULL;
                }

                if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(   
                            p_cell_ctx->transaction_id,
                            p_cell_ctx->cell_index,
                            RRC_FAILURE,
                            RRM_RRC_INTERNAL_FAILURE))
                {
                    RRC_CSC_TRACE(RRC_WARNING, 
                            "Unable to send Failure RESP to RRM");
                }
            }
            /* SPR_15768 FIX Start */
        }
        /* eMTC changes stop */

        /* SPR_15768 FIX End */
        /*SPR:7484 Start*/

        sfn_m_update_csc_sfn(p_llim_sfn_ind, p_csc_gl_ctx,p_llim_sfn_ind->cell_index, RRC_FALSE);
        /* eMTC changes stop */
        /*SPR:7484 Stop*/
    }
    RRC_CSC_UT_TRACE_EXIT();
}





/******************************************************************************
*   FUNCTION NAME: sfn_m_llim_sfn_resp_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function process CSC_LLIM_SFN_RESP message.
******************************************************************************/
void sfn_m_llim_sfn_resp_process_msg(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    rrc_csc_llim_sfn_resp_t *p_llim_sfn_resp = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_csc_llim_sfn_resp_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    p_llim_sfn_resp = (rrc_csc_llim_sfn_resp_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_llim_sfn_resp->cell_index);

    /* Search in DB cell with p_llim_sfn_resp->cell_index - should be present */
    if ((PNULL == p_csc_gl_ctx->p_p_cell_ctx[p_llim_sfn_resp->cell_index]) ||
        (p_csc_gl_ctx->p_p_cell_ctx[p_llim_sfn_resp->cell_index]->
             cell_index != p_llim_sfn_resp->cell_index))
    {
        /* ignore */
        RRC_CSC_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
         /*SPR:7484 Start*/
             sfn_m_update_csc_sfn(p_llim_sfn_resp, p_csc_gl_ctx,p_llim_sfn_resp->cell_index, RRC_TRUE);
         /*SPR:7484 Stop*/
      /* eMTC changes stop */
    }
    RRC_CSC_UT_TRACE_EXIT();
}





/******************************************************************************
*   FUNCTION NAME: sfn_m_stop_sfn
*   INPUT        : csc_cell_ctx_t  *p_cell_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function stop SFN in RRC cell.
******************************************************************************/
void sfn_m_stop_sfn(
    csc_cell_ctx_t  *p_cell_ctx    /* cell context */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);

    if (PNULL != p_cell_ctx->sfn_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF,
            "[CELL:%u] Stop timer [CELL_M_SFN_TIMER]",
            p_cell_ctx->cell_index);
        rrc_stop_timer(p_cell_ctx->sfn_timer);
        p_cell_ctx->sfn_timer = PNULL;
    }
    RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: csc_rrm_build_and_send_updated_pws_si_req
*   INPUT        : csc_gl_ctx_t        *p_csc_gl_ctx
*                  csc_cell_ctx_t *p_cell_ctx
*                  rrc_warning_info_list_t   *rrc_warning_info_list
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function process Sends Updated PWS SI req
******************************************************************************/

void csc_rrm_build_and_send_updated_pws_si_req(
               csc_gl_ctx_t        *p_csc_gl_ctx,
               csc_cell_ctx_t *p_cell_ctx,
               rrc_warning_info_list_t   *rrc_warning_info_list)
{
 
    rrc_return_et res = RRC_FAILURE;
    rrc_rrm_updated_pws_si_list_req_t updated_pws_si;
    U8       warning_count = RRC_NULL;    
    csc_cell_m_timer_buf_t guard_timer_data;

    RRC_CSC_UT_TRACE_ENTER();
    
    /* Fill and send message */
    memset_wrapper(&updated_pws_si, 0, 
            sizeof (
                rrc_rrm_updated_pws_si_list_req_t));

    updated_pws_si.cell_index = p_cell_ctx->cell_index;
    updated_pws_si.warning_info_list.count = rrc_warning_info_list->count;
 
    for (warning_count = 0; warning_count < rrc_warning_info_list->count;
             warning_count++)
    {
       l3_memcpy_wrapper (updated_pws_si.warning_info_list.warning_info[warning_count]. \
          message_identifier, rrc_warning_info_list->warning_info[warning_count]. \
                message_identifier, MSG_ID_OCTET_SIZE);
       l3_memcpy_wrapper (updated_pws_si.warning_info_list.warning_info[warning_count]. \
          serial_number, rrc_warning_info_list->warning_info[warning_count]. \
               serial_number, SERIAL_NUMBER_OCTET_SIZE);
    }

    guard_timer_data.auto_delete        = RRC_TRUE;
    guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
    guard_timer_data.cell_index         = p_cell_ctx->cell_index;
    guard_timer_data.cell_m_event       =
                            CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED;
    RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_UPDATED_PWS_TIMER] %u ms.",
                               p_csc_gl_ctx->pws_duration);

    if(PNULL != p_cell_ctx->guard_timer)
    {
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    p_cell_ctx->guard_timer = rrc_start_timer(
                             p_csc_gl_ctx->pws_duration,
                             &guard_timer_data,
                             sizeof (guard_timer_data),
                             RRC_FALSE);

    if(PNULL == p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_ERROR,"CSC_UPDATED_PWS_TIMER start timer failed");
        return;
    }

    res = rrc_rrm_il_send_rrc_rrm_updated_pws_si_list_req(
            &updated_pws_si,
        RRC_CSC_MODULE_ID,RRC_RRM_MODULE_ID, p_cell_ctx->transaction_id,
        p_cell_ctx->cell_index);

    if(RRC_FAILURE == res)
    {
        RRC_CSC_TRACE(RRC_FATAL,
                "Unable to sent RRC_RRM_UPDATED_PWS_SI_LIST_REQ to RRM");
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: get_sfn_sf_from_llim
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *                  
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function sends CSC_LLIM_SFN_REQ message to LLIM.
 *                  state CELL_M_STATE_CELL_IN_SERVICE;
 *                  event CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED
 ******************************************************************************/
static void get_sfn_sf_from_llim(
        csc_cell_ctx_t  *p_cell_ctx    /* cell context */
        )
{
    void *p_intrl_msg = PNULL;
    RRC_CSC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cell_ctx);

/*CSR#53522 fix start*/
    if(RRC_TRUE == p_cell_ctx->sfn_sync_ongoing)
    {
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }
/*CSR#53522 fix stop*/

    p_intrl_msg = csc_alloc_intrl_msg(
            RRC_LLIM_MODULE_ID,
            CSC_LLIM_SFN_REQ,
            sizeof (rrc_csc_llim_sfn_req_t));

    if (PNULL != p_intrl_msg)
    {
        /* Fill and send message */
        rrc_csc_llim_sfn_req_t *p_csc_llim_sfn_req =
            (rrc_csc_llim_sfn_req_t *)((U8 *)p_intrl_msg +
                    RRC_API_HEADER_SIZE);

        p_csc_llim_sfn_req->cell_index = p_cell_ctx->cell_index;
        rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
/*CSR#53522 fix start*/
        p_cell_ctx->sfn_sync_ongoing = RRC_TRUE;
/*CSR#53522 fix stop*/
    }
    else
    {
        RRC_CSC_TRACE(RRC_ERROR, "Memory allocation failure.");
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: csc_sfn_tmr_msg_chk_paging_reconf_counter
*
*       This function at SFN timer expiry handles paging
*       counter and cell reconfig counter.
*
*   RETURNS:
*       None
*
******************************************************************************/
static void csc_sfn_tmr_msg_chk_paging_reconf_counter(
    /* Pointer to the CSC global context data */
    csc_gl_ctx_t        *p_csc_gl_ctx,
    /* Pointer to the CSC cell context data */
    csc_cell_ctx_t 	*p_cell_ctx
)
{
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_cell_ctx);
    /*Check the Paging Counter */
    if (p_cell_ctx->mod_period_boud_counter > 0)
    {
        /* -- Decrement the counter first */
        p_cell_ctx->mod_period_boud_counter--;
        if ((p_cell_ctx->mod_period_boud_counter - CSC_SFN_OFFSET) == 0)
        {
            if (p_cell_ctx->si_mod_flag)
            {
                p_cell_ctx->cell_reconfig_counter = 
                    CSC_SFN_OFFSET + p_cell_ctx->mod_period ;
                RRC_CSC_TRACE(RRC_INFO,"csc_reconfig_counter : [%d]", 
                        p_cell_ctx->cell_reconfig_counter);                                    

                if((p_cell_ctx->etws_paging_counter > PWS_OFFSET) &&
                        (p_cell_ctx->cmas_paging_counter > PWS_OFFSET))
                {
                    csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                            PAGE_SI_ETWS_CMAS,
                            p_cell_ctx
                            );
 
                }
                else if (p_cell_ctx->etws_paging_counter > PWS_OFFSET) 
                {
                    csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                            PAGE_SI_ETWS,
                            p_cell_ctx
                            );
                }
                else if (p_cell_ctx->cmas_paging_counter > PWS_OFFSET)
                {
                    csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                            PAGE_SI_CMAS,
                            p_cell_ctx
                            );
                }
                else
                {
                    csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                            PAGE_SI,
                            p_cell_ctx
                            );
                }
                /*Resetting the si_mod_flag to zero*/
                p_cell_ctx->si_mod_flag = RRC_NULL;
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }
            /* Page UEs */
            csc_paging_page_ues(p_cell_ctx);/*SPR 17777 +-*/
        }
        else
        {
            /* Page UEs*/
            csc_paging_page_ues(p_cell_ctx);/*SPR 17777 +-*/
        }
    }
    else
    {
        /* Modification period boundary counter reaches zero*/
        /* Page UEs*/
        csc_paging_page_ues(p_cell_ctx);/*SPR 17777 +-*/

        p_cell_ctx->mod_period_boud_counter = p_cell_ctx->mod_period - 
            (p_cell_ctx->sfn % p_cell_ctx->mod_period);
    }
    /*Check the Cell Reconfig counter */
    if (p_cell_ctx->cell_reconfig_counter > 0)
    {
        /*Waiting for Cell Reconfig Api (due to SI Mod) timer expiry */
        /* -- Decrement the counter first */
        p_cell_ctx->cell_reconfig_counter--;

        if (p_cell_ctx->cell_reconfig_counter < CELL_RECONFIG_OFFSET)
        {
            RRC_CSC_TRACE(RRC_INFO,"Sending Cell Reconfig API at SFN = %d", 
                    p_cell_ctx->sfn);
            /*Build and send Cell Reconfig(due to SI Modificaiton) API*/
            csc_send_cell_reconfig_msg(p_csc_gl_ctx, p_cell_ctx);
            p_cell_ctx->cell_reconfig_counter = 0;
        }
    }
    if ((p_cell_ctx->cell_reconfig_counter > CSC_SFN_OFFSET) &&
            (p_cell_ctx->etws_paging_counter > PWS_OFFSET) &&
            (p_cell_ctx->cmas_paging_counter > 0) && 
            ((p_cell_ctx->cmas_paging_counter - PWS_OFFSET) == 0))
    {
        csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                PAGE_SI_ETWS,
                p_cell_ctx
                );
    }    
    else if ((p_cell_ctx->cell_reconfig_counter > CSC_SFN_OFFSET) &&
            (p_cell_ctx->cmas_paging_counter > PWS_OFFSET) &&
            (p_cell_ctx->etws_paging_counter > 0) && 
            ((p_cell_ctx->etws_paging_counter - PWS_OFFSET) == 0))
    {
        csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                PAGE_SI_CMAS,
                p_cell_ctx
                );
    }    
    else if ((p_cell_ctx->cell_reconfig_counter > 0) && 
            ((p_cell_ctx->cell_reconfig_counter - CSC_SFN_OFFSET) == 0) &&
            (p_cell_ctx->cmas_paging_counter > PWS_OFFSET) &&
            (p_cell_ctx->etws_paging_counter > PWS_OFFSET))
    {
        csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                PAGE_ETWS_CMAS,
                p_cell_ctx
                );
    }    
    else if (((p_cell_ctx->cell_reconfig_counter > CSC_SFN_OFFSET) ||
                (p_cell_ctx->etws_paging_counter > PWS_OFFSET)) &&
            (p_cell_ctx->cmas_paging_counter > 0) && 
            ((p_cell_ctx->cmas_paging_counter - PWS_OFFSET) == 0))
    {
        if (p_cell_ctx->cell_reconfig_counter > CSC_SFN_OFFSET)
        {
            csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                    PAGE_SI,
                    p_cell_ctx
                );
        }
        else if (p_cell_ctx->etws_paging_counter > PWS_OFFSET)
        {
            csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                    PAGE_ETWS,
                    p_cell_ctx
                );
        }
    }    
    else if (((p_cell_ctx->cell_reconfig_counter > CSC_SFN_OFFSET) ||
                (p_cell_ctx->cmas_paging_counter > PWS_OFFSET)) &&
            (p_cell_ctx->etws_paging_counter > 0) && 
            ((p_cell_ctx->etws_paging_counter - PWS_OFFSET) == 0))
    {
        if (p_cell_ctx->cell_reconfig_counter > CSC_SFN_OFFSET)
        {
            csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                    PAGE_SI,
                    p_cell_ctx
                );
        }
        else if (p_cell_ctx->cmas_paging_counter > PWS_OFFSET)
        {
            csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                    PAGE_CMAS,
                    p_cell_ctx
                );

        }
    }    
    else if ((p_cell_ctx->cell_reconfig_counter > 0) && 
            ((p_cell_ctx->cell_reconfig_counter - CSC_SFN_OFFSET) == 0) &&
            ((p_cell_ctx->cmas_paging_counter > PWS_OFFSET) ||
             (p_cell_ctx->etws_paging_counter > PWS_OFFSET)))
    {
        if (p_cell_ctx->etws_paging_counter > PWS_OFFSET)
        {
            csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                    PAGE_ETWS,
                    p_cell_ctx
                );
        }
        else if (p_cell_ctx->cmas_paging_counter > PWS_OFFSET)
        {
            csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                    PAGE_CMAS,
                    p_cell_ctx
                );
        }
    }    
}

/******************************************************************************
*   FUNCTION NAME: csc_sfn_m_process_timer_msg
*   INPUT        : csc_timer_buf_t     *p_csc_timer_buf
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function process SFN manager timer message
*                  Increment SFN and limit it to SFN_UPPER_LIMIT
******************************************************************************/
void csc_sfn_m_process_timer_msg(
    /* Pointer to buffer associated with timer msg */
    csc_timer_buf_t     *p_csc_timer_buf,
    /* Pointer to the CSC global context data */
    csc_gl_ctx_t        *p_csc_gl_ctx
)
{
    csc_cell_ctx_t *p_cell_ctx = PNULL;
    csc_sfn_m_timer_buf_t *p_sfn_m_timer_buf = (csc_sfn_m_timer_buf_t *)
                                        p_csc_timer_buf;
    cmas_info_t    *cmas_node = PNULL;
    cmas_info_t    *temp_cmas_node = PNULL;
    static rrc_bool_et    request_update_from_rrm = RRC_FALSE;
    static rrc_warning_info_list_t  warning_info_list = {0,{}};
    /*cdma_fix bug 10186 start*/
    U8 sfn_threshold_reached = RRC_FALSE;
    /*cdma_fix bug 10186 stop*/

    RRC_ASSERT(PNULL != p_sfn_m_timer_buf);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(sizeof (csc_sfn_m_timer_buf_t) == sizeof (csc_timer_buf_t) + 
         sizeof (rrc_cell_index_t));
    RRC_ASSERT(CSC_SUBMODULE_SFN_M == p_sfn_m_timer_buf->target_submodule);

    /* We have only cell specific timer event */
    /* Search in DB cell with p_cell_m_timer_buf->cell_index -
     * should be present */
    if ((PNULL == p_csc_gl_ctx->p_p_cell_ctx[p_sfn_m_timer_buf->cell_index]) ||
        (p_csc_gl_ctx->p_p_cell_ctx[p_sfn_m_timer_buf->cell_index]->
            cell_index != p_sfn_m_timer_buf->cell_index))
    {
        /* ignore */
        RRC_CSC_TRACE(RRC_ERROR, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[p_sfn_m_timer_buf->cell_index];


        if(p_cell_ctx->etws_counter > 0)
        {
            /*Decrement ETWS counter*/
            p_cell_ctx->etws_counter--; 
        }

        if(p_cell_ctx->etws_paging_counter > 0)
        {
            /*Decrement ETWS paging counter*/
            p_cell_ctx->etws_paging_counter--;
        }
        
        if(p_cell_ctx->cmas_paging_counter > 0)
        {
            /*Decrement CMAS paging counter*/
            p_cell_ctx->cmas_paging_counter--;
        }

        for ( cmas_node = (cmas_info_t *)(st_get_first_node     
                ((const search_tree_t *)&(p_cell_ctx->cmas_info_list)));
              cmas_node; cmas_node = (cmas_info_t *)(st_get_next_node
                ((const search_tree_node_t *)cmas_node)))
        {
            if (PNULL != temp_cmas_node)
            {
                st_delete_node (&(p_cell_ctx->cmas_info_list),
                                &(temp_cmas_node->anchor));
                rrc_mem_free(temp_cmas_node);
                temp_cmas_node = PNULL;

            }
            cmas_node->cmas_time_elapsed += CSC_SFN_TICK;
            if ( RRC_FALSE == cmas_node->indefinite_broadcast )
            {
                /*Decrement CMAS counter*/
                if ( cmas_node->cmas_counter > 0 )
                {
                    cmas_node->cmas_counter--;
                }
            }
            if (((cmas_node->cmas_counter < CSC_SFN_TICK) && 
                     (RRC_FALSE == cmas_node->indefinite_broadcast))&& 
                    warning_info_list.count < MAX_WARNING_MESSAGE)
            {
                request_update_from_rrm = RRC_TRUE;
                if ( warning_info_list.count < MAX_WARNING_MESSAGE )
                {
                    l3_memcpy_wrapper(warning_info_list.warning_info[warning_info_list.count]. \
                            message_identifier, cmas_node->sib12_info.msg_id,
                            MSG_ID_OCTET_SIZE);
                    l3_memcpy_wrapper(warning_info_list.warning_info[warning_info_list.count]. \
                            serial_number, cmas_node->sib12_info.serial_number,
                            SERIAL_NUMBER_OCTET_SIZE);
                    warning_info_list.count++;
                    /* Storing the node temporarily so that can be deleted later on */
                    temp_cmas_node = cmas_node;
                }
            }
            if ( 0 == st_get_node_count ((const search_tree_t *)
                        &(p_cell_ctx->cmas_info_list)))
            {
                p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_12));
            }
        }
        if (PNULL != temp_cmas_node)
        {
            st_delete_node (&(p_cell_ctx->cmas_info_list),
                            &(temp_cmas_node->anchor));
            rrc_mem_free(temp_cmas_node);
            if ( 0 == st_get_node_count ((const search_tree_t *)
                        &(p_cell_ctx->cmas_info_list)))
            {
                p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_12));
            }
        }

        /* Process message */
        p_cell_ctx->sfn = (rrc_sfn_t)((p_cell_ctx->sfn + 1) % SFN_UPPER_LIMIT);

        /* If SFN, SF & Timestamp is already requested from L2, then don't perform the following steps */
        if ((RRC_FALSE == p_cell_ctx->sfn_sf_timestamp_requested) &&
            (0 != p_cell_ctx->sib8_periodicity))
        {
            (p_cell_ctx->sib8_new_segments_generate_count)++;
#ifdef RRC_DEBUG
             RRC_CSC_TRACE(RRC_DETAILED, "segment count=%d, sib8_periodicity=%d", p_cell_ctx->sib8_new_segments_generate_count,
                     p_cell_ctx->sib8_periodicity);
#endif

             /*cdma_fix bug 10186 start*/
            /* if SIB8 new segment generation threshold reaches */
             if(RRC_FALSE == p_cell_ctx->proc_buffer_taken)
             {
                 if(((p_cell_ctx->sib8_periodicity * MAX_SIB8_INSTANCES) - (p_cell_ctx->sib8_new_segments_generate_count))
                         <= SIB8_NEW_SEG_GENERATE_THRESHOLD )
                 {
                     sfn_threshold_reached = RRC_TRUE;
                     p_cell_ctx->proc_buffer_taken = RRC_TRUE;
                     RRC_CSC_TRACE(RRC_DETAILED, "Taking proc_buffer first time, SFN[%d]", p_cell_ctx->sfn);
                 }
             }
             else
             {
                 if(0 == ((p_cell_ctx->sib8_periodicity * MAX_SIB8_INSTANCES) - (p_cell_ctx->sib8_new_segments_generate_count)))
                 {
                     sfn_threshold_reached = RRC_TRUE;
                     RRC_CSC_TRACE(RRC_DETAILED, "proc_buffer already taken SFN[%d]", p_cell_ctx->sfn);
                 }
             }

	     /*cdma_fix bug 10186 stop*/ 
	     if(sfn_threshold_reached)
	     {
		 RRC_CSC_TRACE(RRC_INFO, "SFN threshold reached");
		 if((p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask & CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG) ||
			 (RRC_TRUE == p_cell_ctx->sib8_reconfig_sched_info_present)) 
		 {
		     RRC_CSC_TRACE(RRC_INFO, "p_cell_ctx->recfg_cell_param.presence_bitmask = %d"
			     " p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask=%d",
			     p_cell_ctx->recfg_cell_param.presence_bitmask,
			     p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask);
		     p_cell_ctx->sib8_new_segments_generate_count = 0;

                    p_cell_ctx->sfn_sf_timestamp_requested = RRC_TRUE;

			/*following function will request for sfn, sfn & timestamp from L2 via LLIM and 
			 * will wait for CSC_LLIM_SFN_IND API to come to csc_md module which in turn
			 * calls the sfn_m_update_csc_sfn() function. */
                    get_sfn_sf_from_llim(p_cell_ctx);
                }
            }
        }
/* SPR 11229 Fix Start */
        else if(0 != p_cell_ctx->sib8_periodicity)
        {
            (p_cell_ctx->sib8_new_segments_generate_count)++;
        }
/* SPR 11229 Fix Stop */

        if (PNULL != p_cell_ctx->etws_info)
        {
          if ((p_cell_ctx->etws_counter < CSC_SFN_TICK) &&
                    (warning_info_list.count < MAX_WARNING_MESSAGE))
          {
            if ((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask)
            {
                l3_memcpy_wrapper (warning_info_list.warning_info[warning_info_list.count]. \
                    message_identifier, p_cell_ctx->etws_info->sib10_info.msg_id,
                    MSG_ID_OCTET_SIZE);
                l3_memcpy_wrapper (warning_info_list.warning_info[warning_info_list.count]. \
                    serial_number, p_cell_ctx->etws_info->sib10_info.serial_number,
                    SERIAL_NUMBER_OCTET_SIZE);
                warning_info_list.count++;
            }
            else if ((8 << RRM_RRC_SIB_TYPE_11) & p_cell_ctx->pws_sibs_bitmask)
            {
                l3_memcpy_wrapper (warning_info_list.warning_info[warning_info_list.count]. \
                    message_identifier, p_cell_ctx->etws_info->sib11_info. \
                        sib_type_11_Info.msg_id, MSG_ID_OCTET_SIZE);
                l3_memcpy_wrapper (warning_info_list.warning_info[warning_info_list.count]. \
                    serial_number, p_cell_ctx->etws_info->sib11_info. \
                        sib_type_11_Info.serial_number, SERIAL_NUMBER_OCTET_SIZE);
                warning_info_list.count++;
            }
            request_update_from_rrm = RRC_TRUE;
            rrc_mem_free(p_cell_ctx->etws_info);
            p_cell_ctx->etws_info = PNULL;
            p_cell_ctx->etws_counter = 0;
            p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_10));
            p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_11));
          }
        }

        if  (( RRC_TRUE == request_update_from_rrm ) && 
                (CELL_M_STATE_CELL_IN_SERVICE == CELL_M_FSM_GET_CURRENT_STATE(p_cell_ctx)))
        {
            /*send updated PWS scheduling info req to RRM*/
            csc_rrm_build_and_send_updated_pws_si_req(p_csc_gl_ctx,
                                             p_cell_ctx, 
                                             &warning_info_list);
            p_cell_ctx->warning_info_list = warning_info_list;
            /* Reset the count of warnings */
            warning_info_list.count = 0;
            request_update_from_rrm = RRC_FALSE;
        }

	csc_sfn_tmr_msg_chk_paging_reconf_counter(p_csc_gl_ctx, p_cell_ctx);
    }
}
/******************************************************************************
*   FUNCTION NAME: sfn_m_update_csc_sfn
*   INPUT        : rrc_csc_llim_sfn_resp_t*  p_llim_sfn_resp
*                  rrc_cell_index_t    cell_index
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function updates the CSC SFN. If Cell Reconfiguration has been
*                  triggered by RRM, this function will also update the SNF counters
*                  maintained for handling Cell Reconfiguration procedure.
******************************************************************************/
/*SPR:7484 Start*/
static void sfn_m_update_csc_sfn(
    rrc_csc_llim_sfn_resp_t*  p_llim_sfn_resp,
    csc_gl_ctx_t    *p_csc_gl_ctx,   /* Pointer to the CSC global context data */
    rrc_cell_index_t    cell_index,
    rrc_bool_et      mac_update_reqd
)
{
/*SPR:7484 Stop*/
    csc_cell_ctx_t  *p_cell_ctx = PNULL;
    
    S16             rrc_mac_sfn_gap = RRC_NULL;
    rrc_sfn_t sfn_current = RRC_NULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);


    p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[cell_index];

    /* klocwork Fix */
    if (PNULL == p_cell_ctx)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Cell Context with Cell Id : [%d] not found",
                         cell_index);
        return;
    }
    /* klocwork Fix */


/*CSR#53522 fix start*/
    p_cell_ctx->sfn_sync_ongoing = RRC_FALSE;
/*CSR#53522 fix stop*/
    RRC_CSC_TRACE(RRC_INFO, "Received MAC SFN: [%u]. CSC Current SFN: [%u].",
            p_llim_sfn_resp->sfn, p_cell_ctx->sfn);
    /* Process message */
    if (p_llim_sfn_resp->sfn < SFN_UPPER_LIMIT)
    {
        if (p_llim_sfn_resp->sfn !=  p_cell_ctx->sfn)
        {
            /* while calculating rrc_mac_sfn_gap, it is assumed
             * that if difference between MAC and CSC SFN is
             * more than CSC_SFN_REARRANGE_LIMIT then MAC SFN
             * is greater than CSC SFN. */
            if((p_llim_sfn_resp->sfn > p_cell_ctx->sfn) 
                && (p_llim_sfn_resp->sfn - p_cell_ctx->sfn 
                <= SFN_UPPER_LIMIT - CSC_SFN_REARRANGE_LIMIT)
                && (p_llim_sfn_resp->sfn - p_cell_ctx->sfn > 0 ))
            { /* MAC SFN is greater than CSC SFN */
                rrc_mac_sfn_gap = (S16)(p_llim_sfn_resp->sfn - p_cell_ctx->sfn);
            }
            else if(p_llim_sfn_resp->sfn > p_cell_ctx->sfn)
            { /* CSC SFN has wrap around and is ahead of MAC SFN */
                rrc_mac_sfn_gap = (S16)(p_llim_sfn_resp->sfn - p_cell_ctx->sfn - SFN_UPPER_LIMIT);
            }
            else if(p_cell_ctx->sfn - p_llim_sfn_resp->sfn <= CSC_SFN_REARRANGE_LIMIT)
            { /* CSC SFN is greater than MAC SFN */
                rrc_mac_sfn_gap = (S16)(p_llim_sfn_resp->sfn - p_cell_ctx->sfn);
            }
            else 
            {  /* MAC SFN has wrap around and is ahead of CSC SFN */
                rrc_mac_sfn_gap = (S16)(p_llim_sfn_resp->sfn + SFN_UPPER_LIMIT - p_cell_ctx->sfn);
            }

            if(0 < rrc_mac_sfn_gap)
            { /* MAC SFN is ahead of CSC SFN. Rearrange paging records */
                csc_paging_rearrange_ues(p_cell_ctx, p_llim_sfn_resp->sfn);/*SPR 17777+-*/
            }

            /* Update CSC SFN counter SFN received from MAC */
            sfn_current = p_cell_ctx->sfn;
            p_cell_ctx->sfn = p_llim_sfn_resp->sfn;

            /* Updating the Modification period counter*/
            if(rrc_mac_sfn_gap > (S16)p_cell_ctx->mod_period_boud_counter)
            { /* to keep mod_period_boud_counter greater than zero,
               * mod_period has been added in its value. */

                p_cell_ctx->mod_period_boud_counter += 
                    p_cell_ctx->mod_period - 
                    ((U16)rrc_mac_sfn_gap  % p_cell_ctx->mod_period);
            }
            else
            {
                p_cell_ctx->mod_period_boud_counter =
                (U32)((S32)(p_cell_ctx->mod_period_boud_counter) - rrc_mac_sfn_gap);
            }

            /* The above calculation may result in setting value of 
             * mod_period_boud_counter greater than mod_period. To ensure
             * that mod_period_boud_counter is within mod_period, doing
             * its module with mod_period. */
            p_cell_ctx->mod_period_boud_counter %= p_cell_ctx->mod_period;

            if (0 == (p_cell_ctx->mod_period_boud_counter - CSC_SFN_OFFSET))
            {
                if (p_cell_ctx->si_mod_flag)
                {
                    p_cell_ctx->cell_reconfig_counter =
                        CSC_SFN_OFFSET + p_cell_ctx->mod_period;
                    RRC_CSC_TRACE(RRC_INFO,"csc_reconfig_counter : [%d]",
                            p_cell_ctx->cell_reconfig_counter);
                    /*Send PAGING API (due to SI Modification)*/
                    if((p_cell_ctx->etws_paging_counter > PWS_OFFSET) &&
                         (p_cell_ctx->cmas_paging_counter > PWS_OFFSET))
                    {
                        csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                                    PAGE_SI_ETWS_CMAS,
                                    p_cell_ctx
                );
                    }
                    else if (p_cell_ctx->etws_paging_counter > PWS_OFFSET)
                    {
                        csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                                  PAGE_SI_ETWS,
                                  p_cell_ctx
                );
                    }
                    else if (p_cell_ctx->cmas_paging_counter > PWS_OFFSET)
                    {
                        csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                                    PAGE_SI_CMAS,
                                    p_cell_ctx
                );
                    }
                    else
                    {
                        csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn,
                                    PAGE_SI,
                                    p_cell_ctx
                );
                    }

                    /*Resetting the si_mod_flag to zero*/
                    p_cell_ctx->si_mod_flag = RRC_NULL;
                    return;
                }
            }

            /* Decrementing Cell Reconfig counter */
            if (p_cell_ctx->cell_reconfig_counter > 0)
            {
                if (rrc_mac_sfn_gap < (S16)p_cell_ctx->cell_reconfig_counter)
                    p_cell_ctx->cell_reconfig_counter =
                        (U32)((S32)(p_cell_ctx->cell_reconfig_counter) - rrc_mac_sfn_gap);
                else
                { /* SFN counter has crossed the modification boundary and 
                   * that is why, sending Cell reconfig failure to RRM */
                    RRC_CSC_TRACE(RRC_INFO,"Current Cell Reconfig Counter = %u"
                        " and gap with MAC SFN = %d."
                        " Dropping RRC RRM cell reconfiguration message.", 
                        p_cell_ctx->cell_reconfig_counter, rrc_mac_sfn_gap);

                    p_cell_ctx->cell_reconfig_counter = 0;
                    p_cell_ctx->si_mod_flag = RRC_NULL;
                    p_cell_ctx->rcfg_status_flag = RRC_FALSE;

                    if (PNULL != p_cell_ctx->cell_reconfig_api)
                    {
                        rrc_msg_mem_free(p_cell_ctx->cell_reconfig_api);
                        p_cell_ctx->cell_reconfig_api = PNULL;
                    }
        
                    if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(   
                        p_cell_ctx->transaction_id,
                        p_cell_ctx->cell_index,
                        RRC_FAILURE,
                        RRM_RRC_INTERNAL_FAILURE))
                    {
                        RRC_CSC_TRACE(RRC_WARNING, 
                            "Unable to send Failure RESP to RRM");
                    }
                    RRC_CSC_UT_TRACE_EXIT();
                    return;
                }
                RRC_CSC_TRACE(RRC_DETAILED,"New cell reconfig counter : [%d]",
                        p_cell_ctx->cell_reconfig_counter);

                if (p_cell_ctx->cell_reconfig_counter < CELL_RECONFIG_OFFSET)
                {
                    RRC_TRACE(RRC_INFO,
                            "Sending Cell Reconfiguration API at sfn : [%d]",
                            p_cell_ctx->sfn);
                    csc_send_cell_reconfig_msg(p_csc_gl_ctx, p_cell_ctx);
                    p_cell_ctx->cell_reconfig_counter = 0;
                    p_cell_ctx->si_mod_flag = RRC_NULL;
                }
            }
        }
        /*At this point, fresh SFN, SF & Timestamp values are received from MAC. So high priority thread will
         * be invoked here which will use these values and generate the cdma base time. Using this CDMA base
         * time, new set of SIB8 segments will be generated and packed and sent to MAC using MAC_RECONFIG_REQ
         * message 
         * Note: No state transition will be done here and MAC_RECONFIG_RESP will be handled in CELL_IN_SERVICE
         * state for MAC_ONLY_MODIFY scenario. */
         
        /* Update CSC SF with SF received from MAC */
        p_cell_ctx->sf = p_llim_sfn_resp->sf;

        /* Update CSC Timestamp with Timestamp received from MAC */
        p_cell_ctx->timestamp = p_llim_sfn_resp->timestamp;


        U64 ts_sec = p_cell_ctx->timestamp.ts_sec;
        U32 ts_nsec = p_cell_ctx->timestamp.ts_nsec;

        RRC_CSC_TRACE(RRC_INFO, "SFN=[%d] SF=[%d], Timestamp=[%llu]sec [%u]nsec in %s",
                             p_cell_ctx->sfn, p_cell_ctx->sf, ts_sec, ts_nsec, __FUNCTION__);

        if((p_cell_ctx->sfn_sf_timestamp_requested == RRC_TRUE) &&
                ((p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask & CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG) ||
                 (RRC_TRUE == p_cell_ctx->sib8_reconfig_sched_info_present)))
        {
            RRC_CSC_TRACE(RRC_INFO, "[%s]: RESUME HIGH PRIORITY THREAD",__FUNCTION__);
            if(RRC_SUCCESS != resume_high_priority_thread(p_cell_ctx, p_cell_ctx->sfn, p_cell_ctx->sf, p_cell_ctx->timestamp))
            {
                RRC_CSC_TRACE(RRC_ERROR, 
                        "Couldn't generate the CDMA Base Time in %s",__FUNCTION__);
            }
            else
            {
                RRC_CSC_TRACE(RRC_INFO, 
                        "CDMA Base Time generated successfully in %s",__FUNCTION__);
		/*SPR:7484 Start*/
                if(RRC_TRUE == mac_update_reqd) /*send MAC Reconfig in case of mac_sfn_resp and not in mac_sfn_ind*/
                { 
                    if(RRC_SUCCESS != pack_and_send_new_sib8_segments(p_cell_ctx, p_csc_gl_ctx))
                    {
                        RRC_CSC_TRACE(RRC_INFO, 
                                "pack_sib8_segments failed in %s",__FUNCTION__);
                    }
                }
		/*SPR:7484 Stop*/
            }
        }
        else
        {        
            RRC_CSC_TRACE(RRC_BRIEF, "LN:%d in %s p_cell_ctx->sfn_sf_timestamp_requested=%d,"
                    "p_cell_ctx->recfg_cell_param.presence_bitmask=%d",__LINE__, __FUNCTION__, 
                    p_cell_ctx->sfn_sf_timestamp_requested, 
                    p_cell_ctx->recfg_cell_param.presence_bitmask);
        }
        RRC_CSC_TRACE(RRC_INFO, "[RECV] [RRC_MAC_MODULE_ID->RRC:RRC_MAC_SFN_CNF(102)]"
            " Received MAC SFN: [%u] CSC Current SFN: [%u] SF=[%d],"
            " Timestamp=[%llu]sec [%u]nsec in %s ,CSC Updated SFN: [%u] ,New Mod counter : [%d]",
            p_llim_sfn_resp->sfn,sfn_current,p_cell_ctx->sf, p_cell_ctx->timestamp.ts_sec,
            p_cell_ctx->timestamp.ts_nsec, __FUNCTION__,
            p_cell_ctx->sfn,p_cell_ctx->mod_period_boud_counter);
    }
    else
    {
        RRC_CSC_TRACE(RRC_WARNING,
                "Attempt to update with too big SFN: [%u]."
                " Received SFN ignored", p_llim_sfn_resp->sfn);
    }

    RRC_CSC_TRACE(RRC_INFO, "CSC Updated SFN: [%u]", p_cell_ctx->sfn);
    RRC_CSC_TRACE(RRC_INFO,"New Mod counter : [%d]", 
            p_cell_ctx->mod_period_boud_counter);
    RRC_CSC_UT_TRACE_EXIT();
}
