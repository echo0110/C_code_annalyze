/******************************************************************************
*
*   FILE NAME:
*       csc_md.c
*
*   DESCRIPTION:
*       This is the main file of the CSC module. This module provides
*       the main message handler and timer expiry handler functions of
*       RRC CSC module.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*   06 June 2009    VasylS      ---------       Cleanup support
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_defines.h"
#include "csc_logger.h"
#include "rrc_common_utils.h"
#include "csc_timers.h"
#include "csc_global_ctx.h"
#include "rrc_msg_mgmt.h"

#include "rrc_ext_api.h"
#include "rrc_intrl_api.h"

#include "csc_cell_m.h"
#include "csc_sfn_m.h"
#include "csc_oamh.h"
#include "rrc_handlers.h"
#ifdef LINUX_PC_TEST
#include "csc_cell_m_timers.h"
#endif

/*
*   Global variables
*/

/* Used in logger for identifying CSC (just for avoid string duplication) */
const S8* rrc_csc_log_facility_name = (const S8*)"eNodeB RRC CSC";


/******************************************************************************
*   FUNCTION NAME: csc_rrm_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function passes RRM messages to Cell manager submodule
*                  and check incoming API id.
*
******************************************************************************/
static void csc_rrm_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
    case RRC_RRM_CELL_SETUP_REQ:
        cell_m_rrm_cell_setup_req_process_msg(p_api, p_csc_gl_ctx);
        break;
    /*process cell_delete_req from RRM*/
    case RRC_RRM_CELL_DELETE_REQ:
        cell_m_rrm_cell_delete_req_process_msg(p_api, p_csc_gl_ctx);
        break;
    
    case RRC_RRM_CELL_RECONFIGURE_REQ:
        cell_m_rrm_cell_reconfig_req_process_msg(p_api, p_csc_gl_ctx);
        break;
   
    case RRC_RRM_UPDATED_PWS_SI_LIST_RESP:
        cell_m_rrm_updated_pws_si_list_resp_process_msg(p_api, p_csc_gl_ctx);
        break;

    case RRC_RRM_CELL_START_REQ:
        cell_m_rrm_cell_start_req_process_msg(p_api, p_csc_gl_ctx);
        break;
        
    case RRC_RRM_CELL_STOP_REQ:
        cell_m_rrm_cell_stop_req_process_msg(p_api, p_csc_gl_ctx);
        break;
 
    default:
        RRC_CSC_TRACE(RRC_WARNING, "csc_rrm_process_msg: unexpected API from "
            "RRM module received");
        break;
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: csc_s1ap_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function passes S1AP messages to Cell manager submodule
*                  and check incoming API id.
******************************************************************************/
static void csc_s1ap_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
    case S1AP_PAGING_IND:
        cell_m_s1ap_paging_ind_process_msg(p_api, p_csc_gl_ctx);
        break;
    case S1AP_PWS_REQ:
        cell_m_s1ap_pws_req(p_api, p_csc_gl_ctx);
        break;
    case S1AP_PWS_FAILURE_REQ:
        cell_m_s1ap_pws_failure_req(p_api, p_csc_gl_ctx);
        break;
    case S1AP_KILL_REQ:
        cell_m_s1ap_kill_req(p_api, p_csc_gl_ctx);
        break;
    case S1AP_KILL_FAILURE_IND:
        cell_m_s1ap_kill_failure_ind(p_api, p_csc_gl_ctx);
        break;
    default:
        RRC_CSC_TRACE(RRC_WARNING, "csc_s1ap_process_msg: unexpected API from "
            "S1AP module received");
        break;
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: csc_x2ap_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function passes X2AP messages to Cell manager submodule
*                  and check incoming API id.
******************************************************************************/
static void csc_x2ap_process_msg
(
 void            *p_api,         /* Pointer to input API buffer */
 csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
 )
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
        case X2AP_CSC_CELL_STATUS_REQ:
        cell_m_x2ap_csc_cell_status_req_process_and_send_resp_msg(p_api, p_csc_gl_ctx);
        break;
        default:
        RRC_CSC_TRACE(RRC_WARNING, "csc_x2ap_process_msg: unexpected API from "
                "X2AP module received");
        break;
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: csc_oamh_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function passes OAMH messages to CSC OAMH submodule
*                  and check incoming API id.
******************************************************************************/
static void csc_oamh_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
    case CSC_OAMH_INIT_CNF:
        csc_oamh_init_cnf_process_msg(p_api, p_csc_gl_ctx);
        break;

    case CSC_OAMH_PROV_REQ:
        csc_oamh_prov_req_process_msg(p_api, p_csc_gl_ctx);
        break;

    case CSC_OAMH_CLEANUP_REQ:
        csc_oamh_cleanup_req_process_msg(p_api, p_csc_gl_ctx);
        break;
/* SPR 20908 Start */
    case CSC_OAMH_OAM_COMMUNICATION_INFO_RESP:
        csc_oamh_communication_info_resp_process_msg(p_api, p_csc_gl_ctx);
        break;
/* SPR 20908 End */
    default:
        RRC_CSC_TRACE(RRC_WARNING, "csc_oamh_process_msg: unexpected API from "
            "OAMH module received");
        break;
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: csc_llim_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function passes LLIM messages to Cell manager and
*                  SFN manager submodules and check incoming API id.
******************************************************************************/
static void csc_llim_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
    case CSC_LLIM_CONFIG_CELL_RESP:
        cell_m_llim_config_cell_resp_process_msg(p_api, p_csc_gl_ctx);
        break;

    case CSC_LLIM_DELETE_CELL_RESP:
        cell_m_llim_delete_cell_resp_process_msg(p_api, p_csc_gl_ctx);
        break;

    case CSC_LLIM_SFN_IND:
        sfn_m_llim_sfn_ind_process_msg(p_api, p_csc_gl_ctx);
        break;

    case CSC_LLIM_SFN_RESP:
        sfn_m_llim_sfn_resp_process_msg(p_api, p_csc_gl_ctx);
        break;
 
    case CSC_LLIM_RECONFIG_CELL_RESP:
         cell_m_llim_reconfig_cell_resp_process_msg(p_api, p_csc_gl_ctx);
             break;
    
    case CSC_LLIM_PWS_RESP:
         cell_m_llim_pws_resp_process_msg(p_api, p_csc_gl_ctx);
             break;
    
    case CSC_LLIM_UPDATED_PWS_SI_RESP:
         cell_m_llim_updated_pws_si_resp_process_msg(p_api, p_csc_gl_ctx);
             break;
             
    case CSC_LLIM_CELL_START_RESP:
         cell_m_llim_cell_start_resp_process_msg(p_api, p_csc_gl_ctx);
             break;
    
    case CSC_LLIM_CELL_STOP_RESP:
         cell_m_llim_cell_stop_resp_process_msg(p_api, p_csc_gl_ctx);
             break; 
 
    default:
        RRC_CSC_TRACE(RRC_WARNING, "csc_llim_process_msg: unexpected API from "
            "LLIM module received");
        break;
    }

    RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: csc_uecc_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function passes UECC messages to Cell manager and
*                  SFN manager submodules and check incoming API id.
******************************************************************************/
static void csc_uecc_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)

{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
    case UECC_CSC_DELETE_ALL_UE_RESP:
        cell_m_uecc_del_cell_resp_process_msg(p_api, p_csc_gl_ctx);
        break;


    default:
        RRC_CSC_TRACE(RRC_WARNING, "csc_uecc_process_msg: unexpected API from "
            "UECC module received");
        break;
    }

    RRC_CSC_UT_TRACE_EXIT();
}
#ifdef LINUX_PC_TEST
static void csc_pctest_process_msg
(
 void            *p_api,         /* Pointer to input API buffer */
 csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
 )
{
    pctest_timer_expire_to_csc_t *p_msg;
    U8 cnt = 0;
    csc_cell_m_timer_buf_t timer_buf;
    
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    
    switch (rrc_get_api_id(p_api))
    {
    case PCTEST_TIMER_EXPIRE_TO_CSC:
        p_msg = (pctest_timer_expire_to_csc_t *)((U8* )p_api + RRC_API_HEADER_SIZE);
        
        timer_buf.auto_delete = RRC_FALSE;
        timer_buf.target_submodule = p_msg->submodule;
        timer_buf.cell_index = p_msg->cellindex;
        timer_buf.cell_m_event = p_msg->event;
        
        RRC_CSC_TRACE(RRC_BRIEF,"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

        for (cnt = 0; cnt < p_msg->repeatNumber; cnt++)
        {
            RRC_CSC_TRACE(RRC_BRIEF,"------------------------------------------");
            rrc_csc_process_timer_msg(0, (void*)&timer_buf, p_csc_gl_ctx);
        }
        break;

    default:
        RRC_CSC_TRACE(RRC_WARNING, "unexpected API from PCTEST received");
        break;
    }

    RRC_CSC_UT_TRACE_EXIT();
}
#endif
/******************************************************************************
*   FUNCTION NAME: rrc_csc_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : RRC_BUFFER_SHOULD_BE_RELEASED - Encapsulating shell should release
*                  the buffer
*   DESCRIPTION  : This is the main entry point of RRC CSC module - CSPL callback. It
*                  passes all the external APIs received from exetrnal entities RRM
*                  and internal RRC entities OAMH, LLIM to appropriate
*                  internal submodule Cell manager, SFN manager, CSC OAMH based on
*                  incoming module ID and API ID.
*                  This function always returns nonzero value, i.e. the buffer is always
*                  released by the encapsulating shell.
******************************************************************************/
 S32 rrc_csc_process_msg
(
    void *p_api,        /* Pointer to input API buffer */
    void *p_gl_ctx      /* Pointer to the CSC global context data */
)
{
    csc_gl_ctx_t *p_csc_gl_ctx = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    /*RRC_CSC_TRACE(RRC_INFO, RRC_MSG_RECVD);*/

    /* SPR 19273 Fix Start */
    SET_CELL_AND_UE_INDEX(0xff, 0xffff);
    /* SPR 19273 Fix End */

    /* Precondition check */
    /* Check general message integrity too */
    if ( (PNULL == p_api) || (PNULL == p_gl_ctx) )
    {
        RRC_CSC_TRACE(RRC_ERROR, "rrc_csc_process_msg: somebody call us in "
            "wrong context");
        RRC_CSC_UT_TRACE_EXIT();
        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }

    if (RRC_SUCCESS != rrc_check_cspl_header(p_api))
    {
        RRC_CSC_TRACE(RRC_WARNING, "rrc_csc_process_msg: bad message header - "
            "ignore msg");
        RRC_CSC_UT_TRACE_EXIT();
        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }

    p_csc_gl_ctx = (csc_gl_ctx_t *)p_gl_ctx;

    switch (rrc_get_src_module_id(p_api))
    {
    case RRC_RRM_MODULE_ID:
        csc_rrm_process_msg(p_api, p_csc_gl_ctx);
        break;

    case RRC_S1AP_MODULE_ID:
        csc_s1ap_process_msg(p_api, p_csc_gl_ctx);
        break;

    case RRC_OAMH_MODULE_ID:
        csc_oamh_process_msg(p_api, p_csc_gl_ctx);
        break;

    case RRC_LLIM_MODULE_ID:
        csc_llim_process_msg(p_api, p_csc_gl_ctx);
        break;
 
    /* handle UECC message */
    case RRC_UECC_MODULE_ID:
        csc_uecc_process_msg(p_api, p_csc_gl_ctx);
           break;

    case RRC_X2AP_MODULE_ID:
           csc_x2ap_process_msg(p_api, p_csc_gl_ctx);
           break;
    
#ifdef LINUX_PC_TEST
    case RRC_PCTEST_MODULE_ID:
        csc_pctest_process_msg(p_api, p_csc_gl_ctx);
        break;
#endif

    default:
        RRC_CSC_TRACE(RRC_WARNING, "rrc_csc_process_msg: API from unexpected "
            "module received");
        break;
    }

    RRC_CSC_UT_TRACE_EXIT();

    /* Encapsulating shell should release the buffer */
    return RRC_BUFFER_SHOULD_BE_RELEASED;
}

/******************************************************************************
*   FUNCTION NAME: rrc_csc_process_timer_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This is the main entry point of RRC CSC module - CSPL callback.
*                  It handles all the timer expiry events.
*                  The timer buffer is always allocated in the rrc_start_timer() function.
*                  This buffer should always start with csc_timer_buf_t
*                  Timer buffer can be freed depending on information in csc_timer_buf_t.
******************************************************************************/
void rrc_csc_process_timer_msg
(
    rrc_timer_t timer_id,       /* Identity of the expired timer */
    void        *p_timer_buf,   /* Pointer to data associated with the timer */
    void        *p_gl_ctx       /* Pointer to the CSC global context data */
)
{
    csc_gl_ctx_t *p_csc_gl_ctx = PNULL;
    csc_timer_buf_t *p_csc_timer_buf = PNULL;

    /* Precondition check */
    if ((PNULL == p_timer_buf) || (PNULL == p_gl_ctx))
    {
        RRC_CSC_TRACE(RRC_ERROR, "rrc_csc_process_timer_msg: somebody call us "
            "in wrong context");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    p_csc_gl_ctx = (csc_gl_ctx_t *)p_gl_ctx;
    p_csc_timer_buf = (csc_timer_buf_t *)p_timer_buf;


    switch (p_csc_timer_buf->target_submodule)
    {
    case CSC_SUBMODULE_CELL_M:
        RRC_CSC_TRACE(RRC_DETAILED, "Get timer msg.");
        csc_cell_m_process_timer_msg(timer_id, p_csc_timer_buf, p_csc_gl_ctx);
        break;

    case CSC_SUBMODULE_SFN_M:
        csc_sfn_m_process_timer_msg(p_csc_timer_buf, p_csc_gl_ctx);
        break;

    case CSC_SUBMODULE_OAMH:
        RRC_CSC_TRACE(RRC_DETAILED, "Get timer msg.");
        csc_oamh_process_timer_msg(timer_id, p_csc_timer_buf, p_csc_gl_ctx);
        break;

    default:
        RRC_ASSERT(0);
        RRC_CSC_TRACE(RRC_ERROR, "rrc_csc_process_timer_msg: timer event for "
            "unexpected submodule received");
        break;
    }

    if (RRC_TRUE == p_csc_timer_buf->auto_delete)
    {
        if (PNULL != p_timer_buf)
        {
            rrc_mem_free(p_timer_buf);
        }
    }

}

