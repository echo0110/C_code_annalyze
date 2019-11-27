/******************************************************************************
 *
 *   FILE NAME:
 *       csc_cell_m_fsm.c
 *
 *   DESCRIPTION:
 *       This header file contains implementation of Cell manager FSM,
 *       all Cell manager states and handlers.
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   02 June 2009    VasylS      ---------       Initial
 *   06 June 2009    VasylS      ---------       Cleanup support
 *
 *   Copyright (c) 2009, Aricent Inc. All Rights Reserved
 *
 ******************************************************************************/
#include "csc_cell_m_fsm.h"
#include "rrc_defines.h"
#include "csc_logger.h"
#include "rrc_common_utils.h"
#include "csc_cell_m_timers.h"
#include "rrc_msg_mgmt.h"
#include "csc_utils.h"
#include "rrc_intrl_api.h"
#include "rrc_rrm_intf.h"
#include "rrc_rrm_il_composer.h"
#include "rrc_csc_llim_intf.h"
#include "rrc_csc_uecc_intf.h"
#include "csc_bch_m.h"
#include "csc_sfn_m.h"
#include "rrc_s1ap_csc_intf.h"
#include "csc_paging_m.h"
#include "rrc_x2apCsc_intf.h"
#include "rrc_oamh_intf.h"
#include "rrc_shared_stats.h"
#include "rrc_cemh_fsm.h"

#ifdef LINUX_PC_TEST
#define RRC_CSC_SFN_SYNC_TIMER_DEFAULT_DURATION    500*1000
#define    CSC_SFN_SYNC_TIMER_REPEATED        RRC_FALSE
#define    CSC_SFN_SYNC_TIMER_AUTODELETE      RRC_TRUE

#else 
#ifdef RRC_CSC_DEBUG_FT
#define RRC_CSC_SFN_SYNC_TIMER_DEFAULT_DURATION    500
#define    CSC_SFN_SYNC_TIMER_REPEATED        RRC_FALSE
#define    CSC_SFN_SYNC_TIMER_AUTODELETE      RRC_TRUE

#else
#define RRC_CSC_SFN_SYNC_TIMER_DEFAULT_DURATION    500

#define    CSC_SFN_SYNC_TIMER_REPEATED        RRC_TRUE
#define    CSC_SFN_SYNC_TIMER_AUTODELETE      RRC_FALSE
#endif /*RRC_CSC_DEBUG_FT*/
#endif /*LINUX_PC_TEST*/

#define CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,cell_index)\
    if(p_csc_gl_ctx->p_p_cell_ctx[cell_index]->explicit_start_required == RRC_CSC_AUTOMATIC_CELL_START)\
{\
    CELL_M_FSM_SET_STATE(p_cell_ctx,CELL_M_STATE_CELL_IN_SERVICE);\
}\
else\
{\
    CELL_M_FSM_SET_STATE(p_cell_ctx,CELL_M_STATE_CELL_OUT_OF_SERVICE);\
}\

extern int sfn_sf_available;
extern int cdma_sys_time_available;
extern csc_cell_ctx_t  *gp_cell_ctx;

extern pthread_mutex_t region_mutex;
//extern pthread_cond_t sfn_sf_space_available;
extern pthread_cond_t sfn_sf_data_available;
//extern pthread_cond_t cdma_time_space_available;
extern pthread_cond_t cdma_time_data_available;

/*
 *   Global definitions
 */

#ifdef RRC_UT_FRAMEWORK
void   *pLlimCellConfig = PNULL;
void   *pLlimCellStart = PNULL;
void   *pLlimCellStop = PNULL;
void   *pUeccInitSetupInd = PNULL;
void   *pLlimDeleteAllUeReq = PNULL;
#endif

const S8 * CELL_M_FSM_STATES_NAMES[] =
{
    (const S8*)"CELL_M_STATE_IDLE",
    (const S8*)"CELL_M_STATE_W_FOR_LLIM_CONFIG_CELL_RESP",
    (const S8*)"CELL_M_STATE_CELL_IN_SERVICE",
    (const S8*)"CELL_M_STATE_CELL_OUT_OF_SERVICE",
    (const S8*)"CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP",
    (const S8*)"CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP",
    (const S8*)"CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP",
    (const S8*)"CELL_M_STATE_W_FOR_CELL_DELETE_REQ",
    (const S8*)"CELL_M_STATE_W_FOR_PWS_RESP",
    (const S8*)"CELL_M_STATE_W_FOR_KILL_RESP",
    (const S8*)"CELL_M_STATE_W_FOR_PWS_FAILURE_RESP",
    (const S8*)"CELL_M_STATE_W_FOR_LLIM_CELL_START_RESP",
    (const S8*)"CELL_M_STATE_W_FOR_LLIM_CELL_STOP_RESP",

};

const S8 * CELL_M_FSM_EVENTS_NAMES[] =
{
    (const S8*)"CELL_M_EVENT_RRM_CONFIG_CELL_REQ",
    (const S8*)"CELL_M_EVENT_LLIM_CONFIG_CELL_RESP",
    (const S8*)"CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED",
    (const S8*)"CELL_M_EVENT_LLIM_DELETE_CELL_RESP",
    (const S8*)"CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED",
    (const S8*)"CELL_M_EVENT_OAMH_CLEANUP_REQ",
    (const S8*)"CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED",
    (const S8*)"CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP",
    (const S8*)"CELL_M_EVENT_RRM_DELETE_CELL_REQ",
    (const S8*)"CELL_M_EVENT_RRM_RECONFIG_CELL_REQ",
    (const S8*)"CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP",
    (const S8*)"CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED" ,
    (const S8*)"CELL_M_EVENT_ENB_MGR_PWS_REQ",
    (const S8*)"CELL_M_EVENT_LLIM_PWS_RESP",
    (const S8*)"CELL_M_EVENT_PWS_TIMER_EXPIRED",
    (const S8*)"CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ",
    (const S8*)"CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP",
    (const S8*)"CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP",
    (const S8*)"CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED",
    (const S8*)"CELL_M_EVENT_ENB_MGR_KILL_REQ",
    (const S8*)"CELL_M_EVENT_KILL_TIMER_EXPIRED",
    (const S8*)"CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND",
    (const S8*)"CELL_M_EVENT_RRM_CELL_START_REQ",
    (const S8*)"CELL_M_EVENT_RRM_CELL_STOP_REQ",
    (const S8*)"CELL_M_EVENT_LLIM_CELL_START_RESP",
    (const S8*)"CELL_M_EVENT_LLIM_CELL_STOP_RESP",
    (const S8*)"CELL_M_EVENT_CELL_START_TIMER_EXPIRED",
    (const S8*)"CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED",
    (const S8*)"CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED",

};




/*
 *   Cell manager FSM types
 */

typedef void (*cell_m_fsm_act_func_ptr)(void *, void *, csc_cell_ctx_t *,
        csc_gl_ctx_t *);

static void csc_update_cell_config
(
 csc_cell_ctx_t  *p_cell_ctx /* cell context*/
 );
static void csc_update_sib_type_1_info
(
 rrc_rrm_cell_setup_req_t   *p_cell_config_req,
 broadcast_config_info_t    *p_broadcast_info
 );
static void csc_update_sib_type_2_info
(
 rrc_rrm_cell_setup_req_t   *p_cell_config_req,
 broadcast_config_info_t    *p_broadcast_info
 );
static void csc_update_sib_type_3_info
(
 rrc_rrm_cell_setup_req_t   *p_cell_config_req,
 broadcast_config_info_t    *p_broadcast_info
 );
static void csc_update_sib_type_4_info
(
 rrc_rrm_cell_setup_req_t   *p_cell_config_req,
 broadcast_config_info_t    *p_broadcast_info
 );
static void csc_update_sib_type_5_info
(
 rrc_rrm_cell_setup_req_t   *p_cell_config_req,
 broadcast_config_info_t    *p_broadcast_info
 );
static void csc_update_sib_type_6_info
(
 rrc_rrm_cell_setup_req_t   *p_cell_config_req,
 broadcast_config_info_t    *p_broadcast_info
 );
static void csc_update_sib_type_7_info
(
 rrc_rrm_cell_setup_req_t   *p_cell_config_req,
 broadcast_config_info_t    *p_broadcast_info
 );
static void csc_update_sib_type_8_info
(
 rrc_rrm_cell_setup_req_t   *p_cell_config_req,
 broadcast_config_info_t    *p_broadcast_info
 );
static void csc_update_sib_type_9_info
(
 rrc_rrm_cell_setup_req_t   *p_cell_config_req,
 broadcast_config_info_t    *p_broadcast_info
 );
static void csc_send_llim_recfg_req_with_old_cfg
(
 csc_cell_ctx_t        *p_cell_ctx,    /* csc cell context */
 csc_gl_ctx_t          *p_csc_gl_ctx,  /* Pointer to csc global context*/
 rrc_reconfig_mode_et   mode           /* reconfig mode */
 );
static void csc_send_llim_recfg_req_with_old_cfg_pws(
        csc_cell_ctx_t   *p_cell_ctx,  /* csc cell context  */
        csc_gl_ctx_t    *p_csc_gl_ctx,  /* Pointer to csc global context*/
        rrc_reconfig_mode_et mode); /* reconfig mode */
static void csc_update_mib_info
(
 rrc_rrm_cell_setup_req_t   *p_cell_set_req,
 broadcast_config_info_t    *p_broadcast_info
 );

#ifdef LTE_EMBMS_SUPPORTED
static void csc_update_sib_type_13_info(
		rrc_rrm_cell_setup_req_t   *p_cell_set_req,
		broadcast_config_info_t    *p_broadcast_info);
#endif
static void cell_m_fsm_w_llim_reconf_cell_state_llim_resp(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */
static void cell_m_fsm_cell_in_service_llim_reconf_cell_resp(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */
static void cell_m_fsm_llim_reconfig_resp_rollback(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void cell_m_fsm_cell_reconfig_cell_req(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void csc_update_phy_channel_config
(
 rrc_rrm_cell_setup_req_t       *p_cell_set_req,
 non_broadcast_config_info_t    *p_non_broadcast_info
 );

static void csc_update_mac_config
(
 rrc_rrm_cell_setup_req_t       *p_cell_set_req,
 non_broadcast_config_info_t    *p_non_broadcast_info
 );

static void csc_update_cdma2000_cell_param
(
 csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
 cdma2000_cell_param_t    *p_cdma2000_param
 );

static void csc_update_broadcast_info
(
 csc_cell_ctx_t        *p_cell_ctx     /*Cell context*/
 );
static U8 csc_get_sfn_gap
(
 sib_type_2_Info_t *p_sib_type_2_info 
 );


static void cell_m_fsm_llim_reconfig_resp_pws_failure_rollback(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
        U8               fail_cause,
        csc_cell_ctx_t  *p_cell_ctx) ;   /* cell context */

static void cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
        U8               fail_cause,
        csc_cell_ctx_t  *p_cell_ctx) ;   /* cell context */

static void cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
        U8               fail_cause,
        csc_cell_ctx_t  *p_cell_ctx) ;   /* cell context */

rrc_return_et validate_and_fill_etws_warning(
        csc_cell_ctx_t  *p_cell_ctx,      /* cell context */
        U32             *p_counter_value); /* Value of counter to be set */

rrc_return_et validate_and_fill_cmas_warning(
        csc_cell_ctx_t  *p_cell_ctx,      /* cell context */
        U32             *p_counter_value); /* Value of counter to be set */
rrc_return_et check_duplicate_warning_message(
        csc_cell_ctx_t  *p_cell_ctx);   /* cell context */
void init_pws_info(
        csc_cell_ctx_t  *p_cell_ctx);      /* cell context */

rrc_return_et fill_new_etws_and_cmas_scheduling_info (
      csc_cell_ctx_t               *p_cell_ctx); /* cell context */

void update_pws_broadcast_config (
      csc_cell_ctx_t               *p_cell_ctx); /* cell context */

static void cell_m_fsm_w_llim_resp_rrm_reconfig_cell_req(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void cell_m_fsm_w_llim_cell_start_resp(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void cell_m_fsm_rrm_cell_start_req_failure(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void cell_m_fsm_rrm_cell_stop_req_failure(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void cell_m_fsm_w_llim_cell_stop_resp(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void cell_m_csc_llim_cell_start_timer_expired(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void cell_m_csc_llim_cell_stop_timer_expired(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void cell_m_fsm_w_uecc_del_resp_cell_stop_timer_expired(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void cell_m_fsm_cell_xxx_state_rrm_delete_req(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void csc_send_s1ap_cell_register_info(
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void csc_send_uecc_setup_ind(
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */
static void cell_m_fsm_cell_out_of_service_cell_reconfig(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void cell_m_fsm_build_and_send_cell_stop_req_to_llim(
        void            *p_api,           /* p_api */
        void            *p_msg,           /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,      /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx );  /* Pointer to the CSC global context data */

static void cell_m_fsm_rrm_cell_stop_req_send_to_llim(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx ); /* Pointer to the CSC global context data */

static void cell_m_fsm_cell_out_of_service_state_cleanup_req(
        void            *p_api,         /* PNULL */
        void            *p_msg,         /* PNULL */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */
static void cell_m_fsm_regenerate_rand_val(
        rrc_timer_t     timer_id,       /* timer_id - unused */
        void            *p_timer_buf,   /* p_csc_cell_m_timer_buf_t - unused */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */

static void get_cdma2000_rand_val(
        U32 seed,/* Value of the seed */ 
        U32 min, /* Minimum value of the rand */ 
        U32 max, /* Maximum Value of the rand */
        U32 *rand_val); /* rand value that will be generated*/
 
static void csc_get_new_encoded_sibs_info(
                csc_cell_ctx_t  *p_cell_ctx);

static void csc_get_new_encoded_sibs_info_recfg(
                csc_cell_ctx_t  *p_cell_ctx,
                        sib_type_1_Info_t  *p_sib_type_1_info);

static U8 csc_check_si_mapping(
        csc_cell_ctx_t  *p_cell_ctx);

static rrc_return_et cell_reconfig_phyMacMode_mac_reconfig_fail(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx);

static rrc_return_et cell_reconfig_upon_phyMacMode_mac_timer_exp(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx);

static rrc_return_et cell_reconfig_upon_phyMacMode_phy_timer_exp(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx);

static rrc_return_et cell_reconfig_upon_mac_only_mode_timer_exp(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx);

static rrc_return_et  cell_reconfig_upon_phy_mode_timer_exp(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx);

static rrc_return_et cell_reconfig_phyMacMode_phy_reconfig_fail(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx);

static rrc_return_et process_cell_reconfig_ind(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx);



 static void cell_m_fsm_w_llim_conf_cell_state_llim_resp(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx);  /* Pointer to the CSC global context data */


static rrc_return_et cell_reconfig_mac_rlc_rlc_reconfig_fail(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx);

/*
 *   Cell manager states
 */
/*
 *   CELL_M_STATE_IDLE state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_idle_state_config_cell_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function start cell configuration.
 *                  state CELL_M_STATE_IDLE; event CELL_M_EVENT_RRM_CONFIG_CELL_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_idle_state_config_cell_req(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    rrc_rrm_cell_setup_req_t        *p_config_cell_req  =
        (rrc_rrm_cell_setup_req_t *)p_msg;
    csc_cell_m_timer_buf_t          guard_timer_data;
    void                            *p_intrl_msg        = PNULL;
    rrc_rrm_cell_setup_req_t        *p_cfg_cell_param   = &p_cell_ctx->
        cfg_cell_param;
    U32                             mod_period = RRC_NULL;
    rrc_sfn_t                       sfn_curr     = RRC_NULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_config_cell_req);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(p_config_cell_req->cell_index == p_cell_ctx->cell_index);

    rrc_return_et ret_val = RRC_SUCCESS;

    /*Allocating memory for Temp SI Scheduling Info*/
    p_cell_ctx->p_temp_si_schdl_info = rrc_mem_get(sizeof (si_schdl_info_t));

    if (PNULL == p_cell_ctx->p_temp_si_schdl_info)
    {
        RRC_CSC_TRACE(RRC_FATAL,"Unable to allocate Memeory");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }
    memset_wrapper(p_cell_ctx->p_temp_si_schdl_info, 0, sizeof (si_schdl_info_t));


    p_cell_ctx->sib8_periodicity = 0;

    /* Special case on init operation */
    *p_cfg_cell_param = *p_config_cell_req;
    p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer     = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
    p_cell_ctx->null_integrity_support  = p_csc_gl_ctx->
             null_integrity_support[p_config_cell_req->cell_index];

    /* 1. set guard timer */
    guard_timer_data.auto_delete        = RRC_TRUE;
    guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
    guard_timer_data.cell_index         = p_cell_ctx->cell_index;
    guard_timer_data.cell_m_event       =
        CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED;

    RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_SETUP_TIMER] %u ms.",
            p_csc_gl_ctx->cell_cfg_duration);
    p_cell_ctx->guard_timer = rrc_start_timer(
            p_csc_gl_ctx->cell_cfg_duration,
            &guard_timer_data, 
            sizeof (guard_timer_data), 
            RRC_FALSE);


    /* calculate the Mod period and store in cell context */
        mod_period = csc_get_mod_period(&p_cfg_cell_param->
                sib_type_2_Info);

    if ( RRC_FAILURE == mod_period )
    /* eMTC changes stop */
    {
        RRC_CSC_TRACE(RRC_FATAL, "Invalid Modificaiton period");
        /* SPR 23608/SES-43 Fix Start */
        if (PNULL != p_cell_ctx->guard_timer)
        {
            RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_SETUP_TIMER]");
            /* Stop Timer */
            rrc_stop_timer(p_cell_ctx->guard_timer);
            p_cell_ctx->guard_timer = PNULL;
        }
        /* SPR 23608/SES-43 Fix End */

        /* Send RRC_RRM_CELL_SETUP_RESP */
        if (RRC_SUCCESS != csc_send_rrm_cell_setup_resp(
                p_cell_ctx->transaction_id,
                p_cell_ctx->cell_index,
                RRC_FAILURE,
                RRM_RRC_INTERNAL_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
            RRC_CSC_UT_TRACE_EXIT();
            return;
        }
        else
        {
            RRC_CSC_TRACE(RRC_WARNING, "Sent RESP FAILURE TO RRM");
        }

        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /*Set the modification period boundary counter*/
    p_cell_ctx->mod_period = mod_period;
    sfn_curr = p_cell_ctx->sfn;
    p_cell_ctx->mod_period_boud_counter = mod_period - (sfn_curr % mod_period);

    /* calculate the SFN GAP  and store in global context */
    p_cell_ctx->sfn_gap = csc_get_sfn_gap(&p_cfg_cell_param->sib_type_2_Info);
        
    /* 2. send CSC_LLIM_CONFIG_CELL_REQ */
    p_intrl_msg = csc_alloc_intrl_msg(
            RRC_LLIM_MODULE_ID,
            CSC_LLIM_CONFIG_CELL_REQ,
            sizeof(rrc_csc_llim_config_cell_req_t));

#ifdef RRC_UT_FRAMEWORK
    pLlimCellConfig =  rrc_mem_get(sizeof(rrc_csc_llim_config_cell_req_t) +
                                     RRC_API_HEADER_SIZE);
#endif
    if (PNULL != p_intrl_msg)
    {
        /* Fill and send message */
        rrc_csc_llim_config_cell_req_t *p_llim_cfg_cell_req =
            (rrc_csc_llim_config_cell_req_t *)((U8 *)p_intrl_msg +
                                               RRC_API_HEADER_SIZE);

        /* Set the bitmask to 0. */
        p_llim_cfg_cell_req->llim_cell_setup_req.presence_bitmask = RRC_NULL;

        p_llim_cfg_cell_req->transaction_id     = p_cell_ctx->transaction_id;
        p_llim_cfg_cell_req->cell_index         = p_cell_ctx->cell_index;
        /* Filling the Modification period */
        p_llim_cfg_cell_req->mod_period         = p_cell_ctx->mod_period;
        /* Filling the SFN GAP*/
        p_llim_cfg_cell_req->sfn_gap            = p_cell_ctx->sfn_gap;

        /* In case cell is out of service, LLIM must intimate this to MAC/PHY. */ 
        if (CELL_SETUP_REQ_API_EXPLICIT_START_REQUIRED_PRESENCE_FLAG &
                p_cfg_cell_param->presence_bitmask)
        {
            /* Save the explicit_start_required value in the csc global context
             * to decide CSC FSM state after cell is successfully configured.
             * */
            p_csc_gl_ctx->p_p_cell_ctx[p_llim_cfg_cell_req->cell_index]->
                explicit_start_required = RRC_CSC_EXPLICIT_CELL_START;

            p_llim_cfg_cell_req->llim_cell_setup_req.explicit_start_required  = 
                                  p_cfg_cell_param->explicit_start_required;

            p_llim_cfg_cell_req->llim_cell_setup_req.presence_bitmask  |= 
                                  CELL_SETUP_REQ_API_EXPLICIT_START_REQUIRED_FLAG;
        }

        /* Check and fill MIB, SIB1 and SIs info */
        ret_val = bch_m_mib_sib1_sis(p_llim_cfg_cell_req,
            p_cfg_cell_param, p_cell_ctx);

        if (RRC_SUCCESS == ret_val)
        {

            p_llim_cfg_cell_req->llim_cell_param.mib_info =  
                            p_cell_ctx->cfg_cell_param.mib_info;
            p_llim_cfg_cell_req->llim_cell_param.sib_type_1_Info =  
                            p_cell_ctx->cfg_cell_param.sib_type_1_Info;
            p_llim_cfg_cell_req->llim_cell_param.sib_type_2_Info =  
                            p_cell_ctx->cfg_cell_param.sib_type_2_Info;
            p_llim_cfg_cell_req->llim_cell_param.cell_config_param =  
                            p_cell_ctx->cfg_cell_param.cell_config_param;
#ifdef RRC_UT_FRAMEWORK
             l3_memcpy_wrapper(pLlimCellConfig,p_intrl_msg,
                        (sizeof(rrc_csc_llim_config_cell_req_t) +
                                     RRC_API_HEADER_SIZE));
#endif
#ifdef LTE_EMBMS_SUPPORTED
            if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
            {
                p_llim_cfg_cell_req->llim_cell_param.cell_config_param.mac_config.bitmask |= 
                    RRC_RRM_SIB_13_INFO_PRESENCE_FLAG;
                p_llim_cfg_cell_req->llim_cell_param.cell_config_param.mac_config.sib_type_13_info = 
                    p_cell_ctx->cfg_cell_param.sib_type_13_Info;
            }
            p_llim_cfg_cell_req->llim_cell_param.cell_config_param.mac_config.mbms_updated_area_info_list = 
                p_cell_ctx->cfg_cell_param.mbms_updated_area_info_list;
        /* SPR 18672 Fix Start */
            if(p_cell_ctx->cfg_cell_param.presence_bitmask & 
                  CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG)
            {
                p_llim_cfg_cell_req->llim_cell_param.cell_config_param.mac_config.bitmask |= 
                    RRC_RRM_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG;
                p_llim_cfg_cell_req->llim_cell_param.cell_config_param.mac_config.
                    m2ap_reserved_mcch_related_bcch_info_list = 
                    p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list;
            }
        /* SPR 18672 Fix End */
            if(p_cell_ctx->cfg_cell_param.presence_bitmask & 
                  CELL_SETUP_REQ_API_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG)
            {
                p_llim_cfg_cell_req->llim_cell_param.cell_config_param.mac_config.bitmask |= 
                   RRC_RRM_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG ;
                p_llim_cfg_cell_req->llim_cell_param.cell_config_param.mac_config.mbms_transmission_power = 
                    p_cell_ctx->cfg_cell_param.mbms_transmission_power;
            }

#endif

            rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
            CELL_M_FSM_SET_STATE(p_cell_ctx,
                    CELL_M_STATE_W_FOR_LLIM_CONFIG_CELL_RESP);
        }
        else
        {
            /* Bad incoming info */
            RRC_CSC_TRACE(RRC_WARNING, "Bad message - ignore.");

            if (p_intrl_msg)
            {
                /* CSC didn't send message - free memory */
                rrc_msg_mem_free(p_intrl_msg);
            }

            /* Send RRC_RRM_CELL_SETUP_RESP */
            if (RRC_SUCCESS != csc_send_rrm_cell_setup_resp(
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                        RRM_RRC_INTERNAL_FAILURE))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }
            else
            {
                RRC_CSC_TRACE(RRC_WARNING, "Sent RESP FAILURE TO RRM");
            }
            return;
        }
    }
    /* In case of troubles rollback all changes */
    /*SPR 20172 Fix Start*/
    else
    /*SPR 20172 Fix End*/
    {
        /* Stop guard timer */
        if (PNULL != p_cell_ctx->guard_timer)
        {
            RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_SETUP_TIMER]");
            rrc_stop_timer(p_cell_ctx->guard_timer);
            p_cell_ctx->guard_timer = PNULL;
        }

        /* Delete from Cell DB and free */
        csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);
    }
    RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: resume_high_priority_thread
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *                  rrc_sfn_t          sfn,
 *                  rrc_sf_t           sf,
 *                  rrc_timestamp_t    timestamp
 *   OUTPUT       : none
 *   DESCRIPTION  : This function resumes high priority thread to calculate the base
 *                  cdma time
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
rrc_return_et resume_high_priority_thread( 
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        rrc_sfn_t          sfn,
        rrc_sf_t           sf,
        rrc_timestamp_t    timestamp)
{
    rrc_return_et           response = RRC_FAILURE;
    U32 cdma_base_time = 0;
    RRC_CSC_UT_TRACE_ENTER();
    
    /* SPR 19273 Fix Start */
    SET_CELL_INDEX(p_cell_ctx->cell_index);
    /* SPR 19273 Fix End */

    while (1) {
        RRC_CSC_TRACE(RRC_INFO,"Inside while loop of %s", __FUNCTION__);
        pthread_mutex_lock_wrapper(&region_mutex);
        p_cell_ctx->sfn = sfn;
        p_cell_ctx->sf = sf;
        p_cell_ctx->timestamp = timestamp;
        gp_cell_ctx = p_cell_ctx;
        RRC_CSC_TRACE(RRC_INFO, "\n[%s] Received from L2 SFN:%d, SF:%d, sys_time:[%llu]ts_sec ts_nsec[%u]", 
                __FUNCTION__, p_cell_ctx->sfn, p_cell_ctx->sf, (p_cell_ctx->timestamp.ts_sec),(p_cell_ctx->timestamp.ts_nsec));
        sfn_sf_available = RRC_TRUE;
        pthread_cond_signal_wrapper(&sfn_sf_data_available);

        /************************************************/

        if (cdma_sys_time_available == RRC_FALSE) {
            pthread_cond_wait_wrapper(&cdma_time_data_available,&region_mutex);
        }

        cdma_base_time = p_cell_ctx->cdma_base_time;
        RRC_CSC_TRACE(RRC_INFO, "\n[%s] Used Platform p_cell_ctx->cdma_base_time:[%u]msec", __FUNCTION__, cdma_base_time);
        cdma_sys_time_available = RRC_FALSE;
        pthread_mutex_unlock_wrapper(&region_mutex);

        response = RRC_SUCCESS;
        break; /*consume first cdma base time instance and break from loop*/
    }

    RRC_CSC_UT_TRACE_EXIT();
    return response;
}

/******************************************************************************
 *   FUNCTION NAME: csc_fill_llim_recfg_cell_req_n_cell_ctxt_recfg_params
 *
 *       This function fills llim_recfg_req message
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
static void csc_fill_llim_recfg_cell_req_n_cell_ctxt_recfg_params(
        rrc_csc_llim_reconfig_cell_req_t *p_llim_recfg_cell_req,
        csc_cell_ctx_t  *p_cell_ctx    /* cell context */
        /*SPR 17777 +-*/
)
{
    U8  new_sfn_gap = RRC_NULL;
    U32 new_mod_period = RRC_NULL;
#ifdef LTE_EMBMS_SUPPORTED
    /* SPR 18251 Fix Start */
    U8  count = RRC_ZERO;
    /* SPR 18251 Fix End */
#endif

    RRC_ASSERT(PNULL != p_llim_recfg_cell_req);
    RRC_ASSERT(PNULL != p_cell_ctx);
    /* L3 crash during debug mode - removed RRC_ASSERT : 21691*/
    RRC_CSC_UT_TRACE_ENTER();

    p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG;
    p_cell_ctx->recfg_cell_param.presence_bitmask |= CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG;

    /*MIB*/
    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG;
    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mib_info = p_cell_ctx->cfg_cell_param.mib_info;

    p_cell_ctx->recfg_cell_param.broadcast_info.mib_info = p_cell_ctx->cfg_cell_param.mib_info;
    p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG;


    /*SIB1*/
    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG;
    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_1_Info =  p_cell_ctx->cfg_cell_param.sib_type_1_Info;

    p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG;
    p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_1_Info = p_cell_ctx->cfg_cell_param.sib_type_1_Info;


    /*SIB2*/
    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG;
    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_2_Info =  p_cell_ctx->cfg_cell_param.sib_type_2_Info;

    p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG;
    p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info = p_cell_ctx->cfg_cell_param.sib_type_2_Info;

    /*SIB3*/
    if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG)
    {
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_3_Info = p_cell_ctx->cfg_cell_param.sib_type_3_Info;
    }

    /*SIB4*/
    if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_4_INFO_PRESENCE_FLAG)
    {
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_4_Info = p_cell_ctx->cfg_cell_param.sib_type_4_Info;
    }

    /*SIB5*/
    if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_5_INFO_PRESENCE_FLAG)
    {
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_5_Info = p_cell_ctx->cfg_cell_param.sib_type_5_Info;
    }

    /*SIB6*/
    if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_6_INFO_PRESENCE_FLAG)
    {
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_6_Info = p_cell_ctx->cfg_cell_param.sib_type_6_Info;
    }

    /*SIB7*/
    if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_7_INFO_PRESENCE_FLAG)
    {
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_7_Info = p_cell_ctx->cfg_cell_param.sib_type_7_Info;
    }

    /*SIB8*/
    if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
    {
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_8_Info = p_cell_ctx->cfg_cell_param.sib_type_8_Info;
    }

    /*SIB9*/
    if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_9_INFO_PRESENCE_FLAG)
    {
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_9_Info = p_cell_ctx->cfg_cell_param.sib_type_9_Info;
    }
#ifdef LTE_EMBMS_SUPPORTED
    if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
    {
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG;
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_13_info = p_cell_ctx->cfg_cell_param.sib_type_13_Info;
        /* SPR 18251 Fix Start */
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_13_info =  p_cell_ctx->cfg_cell_param.sib_type_13_Info;
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mbms_updated_area_info_list.num_valid_mbsfn_area_info =
            p_cell_ctx->cfg_cell_param.mbms_updated_area_info_list.num_valid_mbsfn_area_info;
        for(count = 0; count < p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mbms_updated_area_info_list.num_valid_mbsfn_area_info;
                count++)
        {
            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mbms_updated_area_info_list.mbms_updated_area_info[count].area_id =
                p_cell_ctx->cfg_cell_param.mbms_updated_area_info_list.mbms_updated_area_info[count].area_id;
            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mbms_updated_area_info_list.mbms_updated_area_info[count].area_index =
                p_cell_ctx->cfg_cell_param.mbms_updated_area_info_list.mbms_updated_area_info[count].area_index;
            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mbms_updated_area_info_list.mbms_updated_area_info[count].flag = UNCHANGE;
            /* SPR 18480 Fix not required here because while updating info in 
             * p_cell_ctx->cfg_cell_param after reconfiguration, this has been taken care of */
        }
    }
    /* SPR 18672 Fix Start */
    if (p_cell_ctx->cfg_cell_param.presence_bitmask 
            & CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG )
    {
        p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
            CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG;
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |=
            CELL_RECONFIG_REQ_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG ;
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.m2ap_reserved_mcch_related_bcch_info_list = 
            p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list;
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mbms_updated_area_info_list.num_valid_mbsfn_area_info =
            p_cell_ctx->cfg_cell_param.mbms_updated_area_info_list.num_valid_mbsfn_area_info;
        for(count = 0; count < p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mbms_updated_area_info_list.num_valid_mbsfn_area_info;
                count++)
        {
            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mbms_updated_area_info_list.mbms_updated_area_info[count].area_id =
                p_cell_ctx->cfg_cell_param.mbms_updated_area_info_list.mbms_updated_area_info[count].area_id;
            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mbms_updated_area_info_list.mbms_updated_area_info[count].area_index =
                p_cell_ctx->cfg_cell_param.mbms_updated_area_info_list.mbms_updated_area_info[count].area_index;
            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mbms_updated_area_info_list.mbms_updated_area_info[count].flag = UNCHANGE;
            /* SPR 18480 Fix not required here because while updating info in 
             * p_cell_ctx->cfg_cell_param after reconfiguration, this has been taken care of */
        }
    }
    /* SPR 18672 Fix End */
    if (p_cell_ctx->cfg_cell_param.presence_bitmask 
            & CELL_SETUP_REQ_API_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG )
    {
        p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= CELL_RECONFIG_REQ_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG;
        p_llim_recfg_cell_req->cell_reconfig.mbms_transmission_power = 
            p_cell_ctx->cfg_cell_param.mbms_transmission_power;
    }
    /* SPR 18251 Fix End */
#endif


    /*filling  p_cell_ctx->recfg_cell_param with non_broadcast_info*/
    /*In cell setup these are mandatory parameters, so we'll fill all of these parameters*/
    p_cell_ctx->recfg_cell_param.presence_bitmask |= CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG;
    p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= CELL_RECONFIG_REQ_NON_BROADCAST_INFO_PRESENCE_FLAG;
    p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.cell_parameters.bitmask |=
        RRC_RRM_RECONFIG_PHY_CELL_PARAMS_DL_EARFCN_PRESENT |
        RRC_RRM_RECONFIG_PHY_CELL_PARAMS_NUM_OF_ANTENNAS |
        RRC_RRM_RECONFIG_PHY_CELL_PARAMS_DL_CYCLIC_PREFIX |
        RRC_RRM_RECONFIG_PHY_CELL_PARAMS_RB_SIZE |
        RRC_RRM_RECONFIG_PHY_CELL_ID |
        RRC_RRM_RECONFIG_PHY_CELL_MAX_RS_EPRE_PRESENT;
    p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.cell_parameters.dl_earfcn \
        = p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.dl_earfcn;
	p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.cell_parameters.num_of_antennas \
        = p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.num_of_antennas;
    p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.cell_parameters.dl_cyclic_prefix \
        = p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.dl_cyclic_prefix;
    p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.cell_parameters.rb_size \
        = p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.rb_size;
    p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.cell_parameters.phys_cell_id \
        = p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.phys_cell_id;
    p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.cell_parameters.max_rs_epre \
        = p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.max_rs_epre;

    p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.sync_signals = p_cell_ctx->cfg_cell_param.cell_config_param.sync_signals;

    p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.dci_format_for_si_msgs =
        p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.dci_format_for_si_msgs;
    p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.bitmask |= RRC_RRM_RECONFIG_DCI_FORMAT_FOR_SI_MESSAGES_PRESENT;

    if(RRC_RRM_RBS_PER_TBS_DCI_1A_PRESENT &  p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.bitmask |= RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1A_PRESENT;
        l3_memcpy_wrapper(p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.num_rbs_per_tbs_dci_1a,
                p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.num_rbs_per_tbs_dci_1a, RBS_FOR_DCI_1A_SIZE);
    }
    if(RRC_RRM_RBS_PER_TBS_DCI_1C_PRESENT &  p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.bitmask |= RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1C_PRESENT;
        l3_memcpy_wrapper(p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.num_rbs_per_tbs_dci_1c,
                p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.num_rbs_per_tbs_dci_1c, RBS_FOR_DCI_1C_SIZE);
    }
    if(RRC_RRM_SI_TRANSMISSION_INFO_PRESENT &  p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.bitmask |= RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT;
        p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.si_transmission_info = 
            p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.si_transmission_info;                                                                         
    }
    if(RRC_RRM_UL_SYNC_LOSS_TIMER_PRESENT &  p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.bitmask |= RRC_RRM_RECONFIG_UL_SYNC_LOSS_TIMER_PRESENT;
        p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.ul_sync_loss_timer = 
            p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.ul_sync_loss_timer;                                                                           
    }
    if(RRC_RRM_PUCCH_CQI_SINR_THRESHOLD_PRESENT &  p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.bitmask |= 
            RRC_RRM_RECONFIG_PUCCH_CQI_SINR_THRESHOLD_PRESENT;
        p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.pucch_cqi_sinr_value = 
            p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.pucch_cqi_sinr_value;                                                                          
    }
    if(RRC_RRM_N_GAP_PRESENT &  p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.bitmask |= RRC_RRM_RECONFIG_N_GAP_PRESENT;
        p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info.mac_reconfig_params.n_gap = 
            p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.n_gap;

    }



    /*Storing the reconfig Mode(First time with NO_MODE,else with PHY_ONLY or MAC_ONLY*/
    p_llim_recfg_cell_req->mode = MAC_ONLY;

    new_mod_period = csc_get_mod_period(
            &p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info);
    if (new_mod_period != p_cell_ctx->mod_period)
    {
        /*Setting the bitmask for Mod Period*/
        p_llim_recfg_cell_req->bitmask |= RRC_CSC_MOD_PERIOD_PRESENT; 
        p_llim_recfg_cell_req->mod_period = new_mod_period;
    }

    new_sfn_gap = csc_get_sfn_gap(
            &p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info);

    if (new_sfn_gap != p_cell_ctx->sfn_gap)
    {
        /*Setting the bitmask for Mod Period*/
        p_llim_recfg_cell_req->bitmask |= RRC_CSC_SFN_GAP_PRESENT; 
        p_llim_recfg_cell_req->sfn_gap = new_sfn_gap;
    }


    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: pack_and_send_new_sib8_segments
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function packs and sends MAX_SIB8_SEGMENTS to MAC in mac_reconfig_req message
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
rrc_return_et pack_and_send_new_sib8_segments(
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx
)
{
    void            *p_intrl_msg        = PNULL;
    rrc_return_et response = RRC_FAILURE;
    U32 new_mod_period = RRC_NULL;
    U8  new_sfn_gap = RRC_NULL;
    U8  si_mapping = RRC_NULL;
#ifdef LTE_EMBMS_SUPPORTED
    U8 count = 0;
    /*SPR 18480 FIX Start */
    U8    area_count = RRC_ZERO;
    /*SPR 18480 FIX End */
#endif

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_CSC_UT_TRACE_ENTER();

    if (p_cell_ctx->rcfg_status_flag == RRC_FALSE)
    {
        p_cell_ctx->rcfg_status_flag = RRC_TRUE;
    }
    else
    {
        RRC_CSC_TRACE(RRC_WARNING, "Cell Reconfiguration Already Ongoing.");
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    if (PNULL != p_cell_ctx->p_temp_si_schdl_info)
    {
        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;
    }

    /*Allocating memory for Temp SI Scheduling Info*/
    p_cell_ctx->p_temp_si_schdl_info = rrc_mem_get(sizeof (si_schdl_info_t));

    if( PNULL == p_cell_ctx->p_temp_si_schdl_info )
    {
        RRC_CSC_TRACE(RRC_FATAL, "Unable to allocate Memory");
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    memset_wrapper(p_cell_ctx->p_temp_si_schdl_info, 0, sizeof (si_schdl_info_t));

    p_cell_ctx->sib_modify_source = CSC_TRIGGERED_SIB8_MODIFY;

    /* 5. Fill and send CSC_LLIM_RECONFIG_CELL_REQ to LLIM*/
    p_intrl_msg = csc_alloc_intrl_msg( 
            RRC_LLIM_MODULE_ID,
            CSC_LLIM_RECONFIG_CELL_REQ,
            sizeof(rrc_csc_llim_reconfig_cell_req_t));

    if (PNULL != p_intrl_msg)
    {
        /* Fill and send message */
        rrc_csc_llim_reconfig_cell_req_t *p_llim_recfg_cell_req =
            (rrc_csc_llim_reconfig_cell_req_t *)((U8 *)p_intrl_msg +
                    RRC_API_HEADER_SIZE);

        /*Storing Transaction Id and Cell index in 
         *           rrc_csc_llim_reconfig_cell_req_t*/
        p_llim_recfg_cell_req->transaction_id = p_cell_ctx->transaction_id;

        /*Cell Index*/
        p_llim_recfg_cell_req->cell_index = p_cell_ctx->cell_index;

        /*Storing the cell Reconfig(ONGOING) parameter in "rrc_csc_llim_reconfig_cell_req_t"*/
        p_llim_recfg_cell_req->cell_reconfig.presence_bitmask = 0;

        /*filling p_llim_recfg_cell_req*/
        if(!p_cell_ctx->cell_reconfig_done)
        {
            RRC_CSC_TRACE(RRC_INFO, "[%s] RRM Triggered Cell Reconfiguration not yet done", __FUNCTION__);
            /*packing the recfg_cell_param so that it look like the message has
             * come from RRM interface as rrc_rrm_cell_reconfig_req_t message*/
        
            /*filling p_llim_recfg_cell_req and p_cell_ctx->recfg_cell_param
             * both*/

            memset_wrapper(&p_cell_ctx->recfg_cell_param, 0, sizeof(rrc_rrm_cell_reconfig_req_t));
    
    	    csc_fill_llim_recfg_cell_req_n_cell_ctxt_recfg_params(p_llim_recfg_cell_req,
                    p_cell_ctx);/*SPR 17777 +-*/

            /* SI_MAPPING NOT CHANGED */
            si_mapping = csc_check_si_mapping(p_cell_ctx);
            RRC_CSC_TRACE(RRC_INFO,"si_mapping = %d", si_mapping);

            p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = 
                SI_MAPPING_NOT_CHANGED;
            RRC_CSC_TRACE(RRC_INFO,"SI MAPPING NOT CHANGED ");

            csc_get_new_encoded_sibs_info_recfg(p_cell_ctx,
                        &(p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_1_Info));

            /* Set the flag in cell context */

            /* Updating Si scheduling Info */
            /* Update the SIBs contents */
            /*csc_update_broadcast_info_for_cdma2000(p_cell_ctx);*/
        }
        else /*at least one cell_reconfig is done*/
        {
            RRC_CSC_TRACE(RRC_INFO, "[%s] RRM Triggered Cell Reconfiguration done", __FUNCTION__);
            if ( CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG &
                    p_cell_ctx->recfg_cell_param.presence_bitmask)
            {
                p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
                    CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG;
                p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask =
                    p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask;
                if ( CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG &
                        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
                {
                    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mib_info =
                        p_cell_ctx->recfg_cell_param.broadcast_info.mib_info;
                }
                if ( CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG &
                        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
                {
                    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_1_Info =
                        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_1_Info;
                }
                if ( CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG &
                        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
                {
                    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_2_Info =
                        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info;
                }
#ifdef LTE_EMBMS_SUPPORTED
                if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
                        & CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
                {
                    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= CELL_RECONFIG_REQ_SIB_13_INFO_PRESENCE_FLAG;
                    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_13_info =
                        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_13_info;
                }
#endif
            }

#ifdef LTE_EMBMS_SUPPORTED
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
        {
            p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
                CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG;

            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |=
                CELL_RECONFIG_REQ_SIB_13_INFO_PRESENCE_FLAG;
        }
        /* SPR 18672 Fix Start */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG )
        {
                p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
                    CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG;
                p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |=
                            CELL_RECONFIG_REQ_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG ;
                p_llim_recfg_cell_req->cell_reconfig.broadcast_info.m2ap_reserved_mcch_related_bcch_info_list = 
                        p_cell_ctx->recfg_cell_param.broadcast_info.m2ap_reserved_mcch_related_bcch_info_list;
            }
            if(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask & CELL_RECONFIG_REQ_API_MBMS_AREA_INFO_LIST_FLAG)
            {
                /* before storing mbms_updated_area_info_list, we will modify this list to exclude the deleted areas,
                 * so that delete req  for same not sent again */
                for(count = RRC_ZERO; count < p_cell_ctx->recfg_cell_param.broadcast_info.
                        mbms_updated_area_info_list.num_valid_mbsfn_area_info; count++)
                {
                    if(p_cell_ctx->recfg_cell_param.broadcast_info.
                            mbms_updated_area_info_list.mbms_updated_area_info[count].flag != DELETED)
                    {
                        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.
                            mbms_updated_area_info_list.mbms_updated_area_info[area_count] = 
                            p_cell_ctx->recfg_cell_param.broadcast_info.
                            mbms_updated_area_info_list.mbms_updated_area_info[count];
                        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.
                            mbms_updated_area_info_list.mbms_updated_area_info[area_count].flag = UNCHANGE; 

                        area_count++;
                        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.
                            mbms_updated_area_info_list.num_valid_mbsfn_area_info = area_count;
                    }
                }
            }
        /* SPR 18672 Fix End */
	if (p_cell_ctx->recfg_cell_param.presence_bitmask 
			& CELL_RECONFIG_REQ_API_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG )
    {
                p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= CELL_RECONFIG_REQ_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG;
              p_llim_recfg_cell_req->cell_reconfig.mbms_transmission_power = 
                            p_cell_ctx->recfg_cell_param.mbms_transmission_power;
    }
#endif
            if ( CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG &
                    p_cell_ctx->recfg_cell_param.presence_bitmask)
            {
                p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
                    CELL_RECONFIG_REQ_NON_BROADCAST_INFO_PRESENCE_FLAG;
                p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info =
                    p_cell_ctx->recfg_cell_param.non_broadcast_info;
            }
            /*SPR_19066_START*/
            /*Code Removed*/
            /*SPR_19066_END*/
            /* CLPC Start */
            if (CELL_RECONFIG_DOWNLINK_POWER_CONTROL_COMMON_PRESENT &
                    p_cell_ctx->recfg_cell_param.presence_bitmask)
            {
                p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
                    CELL_RECONFIG_REQ_MAC_DOWNLINK_POWER_CONTROL_PRESENCE_FLAG;

                p_llim_recfg_cell_req->cell_reconfig.downlink_power_control_common =
                    p_cell_ctx->recfg_cell_param.downlink_power_control_common;
            }
            /* CLPC Stop */

            if ( CELL_RECONFIG_REQ_API_CONTENTION_FREE_RACH_TIMER_PRESENT &
                    p_cell_ctx->recfg_cell_param.presence_bitmask)
            {
                p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
                    CELL_RECONFIG_REQ_CONTENTION_FREE_RACH_TIMER_PRESENCE_FLAG;
                p_llim_recfg_cell_req->cell_reconfig.contention_free_rach_timer =
                    p_cell_ctx->recfg_cell_param.contention_free_rach_timer;
            }

            /*Storing the reconfig Mode(First time with NO_MODE,else with PHY_ONLY or MAC_ONLY*/
            p_llim_recfg_cell_req->mode = MAC_ONLY;


            new_mod_period = csc_get_mod_period(
                    &p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info);

            if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG)
            {
                if (new_mod_period != p_cell_ctx->mod_period)
                {
                    /*Setting the bitmask for Mod Period*/
                    p_llim_recfg_cell_req->bitmask |= RRC_CSC_MOD_PERIOD_PRESENT;
                    p_llim_recfg_cell_req->mod_period = new_mod_period;
                }
                new_sfn_gap = csc_get_sfn_gap(
                        &p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info);

                if (new_sfn_gap != p_cell_ctx->sfn_gap)
                {
                    /*Setting the bitmask for Mod Period*/
                    p_llim_recfg_cell_req->bitmask |= RRC_CSC_SFN_GAP_PRESENT;
                    p_llim_recfg_cell_req->sfn_gap = new_sfn_gap;
                }
            }

            if (p_cell_ctx->recfg_cell_param.presence_bitmask &
                    CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG)
            {
                if (PNULL != p_csc_gl_ctx)
                {
                    /* Store the SFN for Non- Broadcast,using SFN = SFN(curr) + OFFSET */
                    p_llim_recfg_cell_req->sfn = (rrc_sfn_t)(p_cell_ctx->sfn +
                            p_csc_gl_ctx->sfn_offset) ;
                }
            }

            /* SI_MAPPING NOT CHANGED */
            si_mapping = csc_check_si_mapping(p_cell_ctx);
            RRC_CSC_TRACE(RRC_INFO,"si_mapping = %d", si_mapping);

            /* Set the flag in cell context */
            p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = 
                SI_MAPPING_NOT_CHANGED;
            RRC_CSC_TRACE(RRC_BRIEF,"SI MAPPING NOT CHANGED ");

            if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG)
            {
                csc_get_new_encoded_sibs_info_recfg(p_cell_ctx,
                        &(p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_1_Info));
            }
            else
            {
                csc_get_new_encoded_sibs_info(p_cell_ctx);
            }

            /* Update the SIBs contents */
            csc_update_broadcast_info(p_cell_ctx);
            /* SPR 15561 Fix Start */
            p_cell_ctx->cell_reconfig_done = RRC_FALSE;
            /* SPR 15561 Fix End */
        }


	p_cell_ctx->sib8_periodicity = 0;

	if (RRC_SUCCESS == bch_m_mib_sib1_sis_recfg(
                    p_llim_recfg_cell_req,
                    &p_cell_ctx->recfg_cell_param,
                    p_cell_ctx))
        {
            RRC_CSC_TRACE(RRC_INFO,"Cell Reconfiguration Received at "\
                    "SFN : [%d]", p_cell_ctx->sfn);

            p_llim_recfg_cell_req->sfn = (rrc_sfn_t)((p_cell_ctx->sfn +
                        p_cell_ctx->mod_period_boud_counter + p_cell_ctx->mod_period)
                    % SFN_UPPER_LIMIT);
/* SPR 11229 Fix Start */
            if ( p_cell_ctx->last_sent_sib8_sfn == LAST_SENT_SIB8_SFN_UNINIT )
            {
                if ( p_cell_ctx->sib8_periodicity >0)
                {
                    p_cell_ctx->last_sent_sib8_sfn = p_llim_recfg_cell_req->sfn -
                                       ( p_llim_recfg_cell_req->sfn%p_cell_ctx->sib8_periodicity ) ;
 
                    RRC_CSC_TRACE(RRC_DETAILED,"Updated the last_sent_sib8_sfn[%d] with "\
                                     "CDMA System Time [%llu] for the first SIB8 reconfiguration",
                                     p_cell_ctx->last_sent_sib8_sfn ,
                                     p_cell_ctx->cdma_base_time );
                }
                else
                {
                    RRC_CSC_TRACE(RRC_ERROR,"Periodicity of SIB8 can't be zero");
                }
            }
/* SPR 11229 Fix Stop */
            
            p_cell_ctx->cell_reconfig_api = p_intrl_msg;
            
            /*Store in cell context*/
            if(PNULL != p_intrl_msg)
            {
                rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
            }

            
            response = RRC_SUCCESS;
        }
        else
        {
            /* Bad incoming info */
            RRC_CSC_TRACE(RRC_ERROR, "Bad message - UNABLE TO ENCODE SI/SIB AND MIBs.");
            if (p_intrl_msg)
            {
                /* CSC didn't send message - free memory */
                rrc_msg_mem_free(p_intrl_msg);
                response = RRC_FAILURE;
            }
            p_cell_ctx->rcfg_status_flag = RRC_FALSE;
        }
    }  /* In case of troubles rollback all changes */
    else
    {
        RRC_CSC_TRACE(RRC_FATAL, "UNABLE TO ALLOCATE MEMORY ");
        response = RRC_FAILURE;
    }
    RRC_CSC_UT_TRACE_EXIT();
    return response;
}



/*
 *   CELL_M_STATE_W_FOR_LLIM_CONFIG_CELL_RESP state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_conf_cell_state_llim_resp
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function report cell configured if LLIM is Ok.
 *                  state CELL_M_STATE_W_FOR_LLIM_CONFIG_CELL_RESP;
 *                  event CELL_M_EVENT_LLIM_CONFIG_CELL_RESP
 *
 *   RETURNS      : None
 *
 ******************************************************************************/

 void cell_m_fsm_w_llim_conf_cell_state_llim_resp(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    csc_cell_m_timer_buf_t          rand_regenerate_timer_data;
    U32 rand_max_limit = 0,
        rand_min_limit = 0 ;  
#ifndef RRC_UNIT_TEST_FRAMEWORK
    csc_cell_m_timer_buf_t            sfn_sync_timer_data;
#endif

    rrc_csc_llim_config_cell_resp_t *p_config_cell_resp =
        (rrc_csc_llim_config_cell_resp_t *)p_msg;

    rrc_return_et           response = RRC_SUCCESS;

    rrc_csc_oamh_cell_stop_ind_t *p_csc_oamh_stop_ind = PNULL; 
    rrc_csc_oamh_cell_start_ind_t *p_csc_oamh_start_ind = PNULL; 
    /*SPR 21554 Start*/
    U8 count;
    /*SPR 21554 End*/
    void *p_intrl_msg = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_config_cell_resp);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(p_config_cell_resp->cell_index == p_cell_ctx->cell_index);
    
    RRC_CSC_TRACE(RRC_DETAILED,
        "Unused variable in fsm p_api=%p", p_api);

    /* Precondition - should be the same transaction */
    if (p_config_cell_resp->transaction_id != p_cell_ctx->transaction_id)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /* Stop guard timer */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_SETUP_TIMER]");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    if (RRC_SUCCESS == p_config_cell_resp->response)  
    {
        p_cell_ctx->cell_setup_done = RRC_TRUE;
        if (p_csc_gl_ctx->p_p_cell_ctx[p_config_cell_resp->cell_index]->
                explicit_start_required == RRC_CSC_AUTOMATIC_CELL_START)
        {
            /*SPR_19262_START*/
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
                g_rrc_stats.p_rrc_stats->rrc_csc_stats.
                    rrc_csc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_TRUE;
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[0].
                    rrc_uecc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_TRUE;
            }
            /*SPR_19262_END*/
            if (p_config_cell_resp->bitmask & RRC_CSC_LLIM_SFN_INFO_PRESENT)
            {
                /* Init SFN */


#ifndef RRC_UNIT_TEST_FRAMEWORK
                sfn_m_init_sfn(p_config_cell_resp->sfn, p_cell_ctx,p_csc_gl_ctx);

                /* Start CELL_M_SFN_SYNC_TIMER timer */
                sfn_sync_timer_data.auto_delete = CSC_SFN_SYNC_TIMER_AUTODELETE;
                sfn_sync_timer_data.target_submodule = CSC_SUBMODULE_CELL_M;
                sfn_sync_timer_data.cell_index = p_cell_ctx->cell_index;
                sfn_sync_timer_data.cell_m_event = CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED;

                RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CELL_M_SFN_SYNC_TIMER] %u ms.",
                        RRC_CSC_SFN_SYNC_TIMER_DEFAULT_DURATION);

                p_cell_ctx->mac_sfn_sync_timer =  rrc_start_timer(
                        RRC_CSC_SFN_SYNC_TIMER_DEFAULT_DURATION,
                        &sfn_sync_timer_data,
                        sizeof (sfn_sync_timer_data),
                        CSC_SFN_SYNC_TIMER_REPEATED);
#endif

                /* Send CSC_S1AP_CELL_REG to S1AP */
                csc_send_s1ap_cell_register_info(
                        p_cell_ctx,    
                        p_csc_gl_ctx); 

                /*  Send CSC_OAMH_CELL_START_IND to OAMH */
                p_intrl_msg = csc_alloc_intrl_msg(
                        RRC_OAMH_MODULE_ID,
                        CSC_OAMH_CELL_START_IND,
                        sizeof (rrc_csc_oamh_cell_start_ind_t));

                if (PNULL != p_intrl_msg)
                {
                    /*Fill and send message */
                    p_csc_oamh_start_ind =
                        (rrc_csc_oamh_cell_start_ind_t *)((U8 *)p_intrl_msg +
                                RRC_API_HEADER_SIZE);
                    p_csc_oamh_start_ind->cell_index = p_cell_ctx->cell_index;

                    rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
                }

                /* Since S1AP REG REQ has been Sent , set the flag to false */
                p_cell_ctx->rrc_rrm_s1ap_dereg_flag = RRC_FALSE;

                /* Switch state */
                /*during cell setup, perform all this only when SIB-8 comes in Cell Setup */
                if((p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG) && 
                   ((p_config_cell_resp->bitmask & RRC_CSC_LLIM_SF_INFO_PRESENT) && 
                   (p_config_cell_resp->bitmask & RRC_CSC_LLIM_TIMESTAMP_INFO_PRESENT)))
                {
                  /*cdma_fix bug 10186 start*/
                  /*Perform following operation only when CDMA system time came
                   * enabled in SIB8 parameters during cell setup*/
                  if(SIB_8_SYS_TIME_INFO_PRESENCE_FLAG & p_cell_ctx->cfg_cell_param.sib_type_8_Info.presence_bitmask)  
                  {
                  /*cdma_fix bug 10186 start*/
                    p_cell_ctx->sfn_sf_timestamp_requested = RRC_TRUE;
                    if(RRC_SUCCESS != resume_high_priority_thread(p_cell_ctx, p_config_cell_resp->sfn, 
                                                     p_config_cell_resp->sf, p_config_cell_resp->timestamp))
                    {
                        RRC_CSC_TRACE(RRC_INFO, \
                                "Couldn't generate the CDMA Base Time in %s",__FUNCTION__);
                    }
                    else
                    {
                        RRC_CSC_TRACE(RRC_INFO, \
                                "CDMA Base Time generated successfully in %s",__FUNCTION__);
                        if(RRC_SUCCESS != pack_and_send_new_sib8_segments(p_cell_ctx, p_csc_gl_ctx))
                        {
                            RRC_CSC_TRACE(RRC_INFO, \
                                "pack_sib8_segments failed in %s",__FUNCTION__);
                        }
                        else
                        {
                            p_cell_ctx->mac_reconfig_with_sib8_segments = RRC_TRUE;
                            /* STATE TRANSITION */
                            CELL_M_FSM_SET_STATE(p_cell_ctx,
                                    CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP);
                        }
                    }
                  }
                }
                if (RRC_TRUE != p_cell_ctx->mac_reconfig_with_sib8_segments)
                {
                   CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
                }  
                
                /* Bug 8690 Fix Start */
                /*x2ap_start*/
                /*Send X2AP_CSC_CELL_ADD_IND*/ 
                p_intrl_msg = csc_alloc_intrl_msg( RRC_X2AP_MODULE_ID,
                     X2AP_CSC_CELL_ADD_IND,
                     sizeof (x2ap_csc_cell_add_t));

                if (PNULL != p_intrl_msg)
                {
                  /* Fill and send message */
                  x2ap_csc_cell_add_t *p_x2ap_csc_cell_add_ind = (x2ap_csc_cell_add_t *)(
                    (U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

                  p_x2ap_csc_cell_add_ind->cell_index = (U8)p_cell_ctx->cell_index;

                  /*SPR_16723_START*/
                  l3_memcpy_wrapper(p_x2ap_csc_cell_add_ind->eutran_cell_id,
                          p_cell_ctx->cfg_cell_param.sib_type_1_Info.cell_access_related_info.
                          cell_Id,
                          CELL_ID_OCTET_SIZE);

                  p_x2ap_csc_cell_add_ind->phys_cell_id =
                      (U16)p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.phys_cell_id; 

                  p_x2ap_csc_cell_add_ind->dl_earfcn = 
                      p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.dl_earfcn;
                  /*SPR 21554 Fix Start*/
                  memset_wrapper (&p_x2ap_csc_cell_add_ind->broadcast_plmn_info, 
                          RRC_NULL, sizeof(x2ap_broadcast_plmn_t) );
                  p_x2ap_csc_cell_add_ind->broadcast_plmn_info.num_bcast_plmn_id=
                      p_cell_ctx->cfg_cell_param.sib_type_1_Info.cell_access_related_info.plmn_Id_info_list.count;                 
		  /* SPR 21833 Start */ 
                  for(count=RRC_NULL;
				count<p_x2ap_csc_cell_add_ind->broadcast_plmn_info.num_bcast_plmn_id;
				count++)
		  /* SPR 21833 End */
                  {
                      csc_paging_plmn_identity_rrc_to_s1ap(p_x2ap_csc_cell_add_ind->broadcast_plmn_info.plmn_identity[count].plmn_id,
                              &p_cell_ctx->cfg_cell_param.sib_type_1_Info.cell_access_related_info.plmn_Id_info_list.plmn_identity_info[count].plmn_identity);

                  }
                  /*SPR 21554 Fix End*/

                  /* ULARFCN will be present in FDD mode only */
                  if(0 == (p_cell_ctx->cfg_cell_param.sib_type_1_Info.presence_bitmask & SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG))
                  {
                    if (p_cell_ctx->cfg_cell_param.sib_type_2_Info.freq_info.presence_bitmask & FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG)
                    {
                      p_x2ap_csc_cell_add_ind->ul_earfcn = 
                             p_cell_ctx->cfg_cell_param.sib_type_2_Info.freq_info.ul_carrier_freq;
                    }
                    else 
                    {
                      p_x2ap_csc_cell_add_ind->ul_earfcn = UL_DL_ARFCN_DIFFRENCE +
                              p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.dl_earfcn;
                    }
                  }
                  /* ULARFCN is equal to DLEARFCN in TDD */
                  else
                  {
                    p_x2ap_csc_cell_add_ind->ul_earfcn = p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.dl_earfcn;
                  }
                  /*SPR_16723_END*/

                  rrc_send_message(p_intrl_msg, RRC_X2AP_MODULE_ID);
                }

                /*x2ap_end*/
                /* Bug 8690 Fix End */

                /* Since S1AP REG REQ has been Sent , set the flag to false */
                /*p_cell_ctx->rrc_rrm_s1ap_dereg_flag = RRC_FALSE;*/
            }
            else
            {
                response = RRC_FAILURE;
            }
        }
        else 
        {
            /*  Send CSC_OAMH_CELL_STOP_IND to OAMH */
            p_intrl_msg = csc_alloc_intrl_msg( 
                    RRC_OAMH_MODULE_ID,
                    CSC_OAMH_CELL_STOP_IND,
                    sizeof (rrc_csc_oamh_cell_stop_ind_t));

            if (PNULL != p_intrl_msg)
            {
                /*Fill and send message */
                p_csc_oamh_stop_ind = 
                    (rrc_csc_oamh_cell_stop_ind_t *)((U8 *)p_intrl_msg +
                            RRC_API_HEADER_SIZE);
                p_csc_oamh_stop_ind->cell_index = p_cell_ctx->cell_index;

                rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
            }

            /* Switch state */
            CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_OUT_OF_SERVICE);
        }
        
        /* Generation of RAND value from seed given by RRM */ 
        if (response == RRC_SUCCESS)
        {
        if (CELL_SETUP_REQ_API_CDMA2000_PARAM_PRESENCE_FLAG & 
             p_cell_ctx->cfg_cell_param.presence_bitmask)
        {
              rand_min_limit = 0 ;
              rand_max_limit = UINT_MAX ; 

 
           if (CDMA2000_PARAM_RAND_MIN_PRESENCE_FLAG &
               p_cell_ctx->cfg_cell_param.cdma2000_cell_param.cdma2000_rand.bitmask)
           {
               rand_min_limit = p_cell_ctx->cfg_cell_param.cdma2000_cell_param. 
                                cdma2000_rand.rand_min ;
           }

           if (CDMA2000_PARAM_RAND_MAX_PRESENCE_FLAG &
             p_cell_ctx->cfg_cell_param.cdma2000_cell_param.cdma2000_rand.bitmask)
           {
               rand_max_limit = p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
                                cdma2000_rand.rand_max ;
           }

           if (rand_max_limit < rand_min_limit)
           {
              RRC_CSC_TRACE(RRC_ERROR, "RAND Max = %u received from RRM is less than RAND min= %u "
                            , rand_max_limit,rand_min_limit);

              RRC_CSC_TRACE(RRC_BRIEF, "Therefore made RAND Max =  RAND min ");
              rand_max_limit = rand_min_limit;
 
             p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
                                cdma2000_rand.rand_max	=
                      p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
                                cdma2000_rand.rand_min ;
           }
           RRC_CSC_TRACE(RRC_BRIEF,"CDMA: RAND seed = %u\n RAND min = %u\n RAND max = %u\n", 
                p_cell_ctx->cfg_cell_param.cdma2000_cell_param.cdma2000_rand.rand_seed
                 ,rand_min_limit,rand_max_limit);
           get_cdma2000_rand_val(p_cell_ctx->
                         cfg_cell_param.cdma2000_cell_param.
                              cdma2000_rand.rand_seed, 
                              rand_min_limit, 
                              rand_max_limit,
                         &(p_cell_ctx->cdma2000_rand));

            RRC_CSC_TRACE(RRC_BRIEF,"CDMA: RAND value = %u ", 
                           p_cell_ctx->cdma2000_rand);

           /*Start the RAND timer */
           rand_regenerate_timer_data.auto_delete = RRC_TRUE;
           rand_regenerate_timer_data.target_submodule = CSC_SUBMODULE_CELL_M;
           rand_regenerate_timer_data.cell_index    = p_cell_ctx->cell_index;
           rand_regenerate_timer_data.cell_m_event  =
            CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED;

           /*Multiplying by 1000 to change the duration received in sec to
            * msec*/
           RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_RAND_REGENERATE_TIMER] %u ms.",
              (p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
               cdma2000_rand.rand_regenerate_timer)*1000);
    
           p_cell_ctx->rand_regenerate_timer = rrc_start_timer(
                 (p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
                 cdma2000_rand.rand_regenerate_timer)*1000,
                 &rand_regenerate_timer_data, 
                 sizeof (rand_regenerate_timer_data), 
                 RRC_FALSE);
         }

        /* Send CSC_UECC_SETUP_IND to UECC*/
        csc_send_uecc_setup_ind(
                p_cell_ctx,    
                p_csc_gl_ctx); 


        /* Updating Si scheduling Info */
        update_si_scheduling_info(p_cell_ctx);


        /*Store Duplexing Mode in Cell Context*/
        if( p_cell_ctx->cfg_cell_param.sib_type_1_Info.presence_bitmask &
                SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG)
        {
            p_cell_ctx->duplexing_mode = DUPLEXING_MODE_TDD;
        }
        /* HD-FDD start */
        else
        {
          if(RRC_RRM_DUPLEXING_MODE_PRESENT & p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask)
          {    
              p_cell_ctx->duplexing_mode = (duplexing_mode_et)p_cell_ctx->
                  cfg_cell_param.cell_config_param.mac_config.duplexing_mode;
          }
          else
          {
              p_cell_ctx->duplexing_mode = DUPLEXING_MODE_FDD;
          }
        }
        /* HD-FDD stop */

        if (RRC_TRUE != p_cell_ctx->mac_reconfig_with_sib8_segments) 
        {
            /* Send RRC_RRM_CELL_SETUP_RESP */
            if (RRC_SUCCESS !=  csc_send_rrm_cell_setup_resp( 
                        p_cell_ctx->transaction_id,
                        p_cell_ctx->cell_index,
                        RRC_SUCCESS,
                        RRM_RRC_NO_ERROR))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP TO RRM");
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }
	    }
        }
        /*SPR_19262_START*/
        /* Code Removed */
        /*SPR_19262_END*/
    }
    else
    {
        response = RRC_FAILURE;
    }

    if (response == RRC_FAILURE)
    {
        /* Send RRC_RRM_CELL_SETUP_RESP */

        if (RRC_SUCCESS !=  csc_send_rrm_cell_setup_resp( 
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_LL_CONFIG_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
            RRC_CSC_UT_TRACE_EXIT();
            return;
        }

        /* Delete from Cell DB and free */
        csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);
    }
    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_conf_cell_state_timer_expired
 *   INPUT        : rrc_timer_t     timer_id
 *                  void            *p_timer_buf
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function stop cell configuration and delete cell.
 *                  state CELL_M_STATE_W_FOR_LLIM_CONFIG_CELL_RESP;
 *                  event CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_conf_cell_state_timer_expired(
        rrc_timer_t     timer_id,       /* timer_id - unused */
        void            *p_timer_buf,   /* p_csc_cell_m_timer_buf_t - unused */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    void *p_intrl_msg = PNULL;
    csc_cell_m_timer_buf_t  guard_timer_data;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    RRC_CSC_TRACE(RRC_DETAILED,
        "Unused variable in fsm timer_id=%p, timer_buff=%p", timer_id, p_timer_buf);
    /* Timer expired and is not valid any more */
    RRC_CSC_TRACE(RRC_BRIEF, "Timer [CSC_CELL_SETUP_TIMER] expired");
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */

    /* Set guard timer */
    guard_timer_data.auto_delete        = RRC_TRUE;
    guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
    guard_timer_data.cell_index         = p_cell_ctx->cell_index;
    guard_timer_data.cell_m_event       =
        CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED;
/* NBIOT TIMER FIX */
/* NBIOT TIMER FIX */
    RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_DELETE_TIMER] %u ms.",
            p_csc_gl_ctx->cell_del_duration);

    p_cell_ctx->guard_timer =
        rrc_start_timer(p_csc_gl_ctx->cell_del_duration, &guard_timer_data,
                sizeof (guard_timer_data), RRC_FALSE);
            /* NBIOT TIMER FIX */
    /* NBIOT TIMER FIX */
    /* Send CSC_LLIM_DELETE_CELL_REQ */
    p_intrl_msg = csc_alloc_intrl_msg( 
            RRC_LLIM_MODULE_ID,
            CSC_LLIM_DELETE_CELL_REQ,
            sizeof (rrc_csc_llim_delete_cell_req_t));

    if (PNULL != p_intrl_msg)
    {
        /* Fill and send message */
        rrc_csc_llim_delete_cell_req_t *p_llim_delete_cell_req =
            (rrc_csc_llim_delete_cell_req_t *)((U8 *)p_intrl_msg +
                                               RRC_API_HEADER_SIZE);
        p_llim_delete_cell_req->transaction_id  = p_cell_ctx->transaction_id;
        p_llim_delete_cell_req->cell_index      = p_cell_ctx->cell_index;
            /* SPR 18354 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
            if((p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
                  || (p_cell_ctx->cfg_cell_param.presence_bitmask & 
                  CELL_SETUP_REQ_API_EMBMS_AREA_INFO_LIST_PRESENCE_FLAG))
            {
                p_llim_delete_cell_req->mbms_updated_area_info_list = p_cell_ctx->
                    cfg_cell_param.mbms_updated_area_info_list;
            }
            if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG)
            {
                p_llim_delete_cell_req->m2ap_reserved_mcch_related_bcch_info_list = 
                    p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list;
            }

#endif
            /* SPR 18354 Fix End */

        rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
    }

    CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP);
    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_conf_cell_state_cleanup_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function abort ongoing operation and clear Cell DB.
 *                  state CELL_M_STATE_W_FOR_LLIM_CONFIG_CELL_RESP;
 *                  event CELL_M_EVENT_OAMH_CLEANUP_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_conf_cell_state_cleanup_req(
        void            *p_api,
        void            *p_msg,
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    RRC_CSC_UT_TRACE_ENTER();

    /*coverity fix start*/
    /*coverity fix stop*/
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    RRC_CSC_TRACE(RRC_DETAILED,
        "Unused variable in fsm p_api=%p and p_msg=%p", p_api, p_msg);

    /* Delete from Cell DB and free */
    csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);

    RRC_CSC_UT_TRACE_EXIT();
}
/*
 *   CELL_M_STATE_CELL_OUT_OF_SERVICE state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_cell_out_of_service_state_cleanup_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *   
 *   DESCRIPTION  : This function stop SFN timer and clear Cell DB.
 *                  state CELL_M_STATE_CELL_OUT_OF_SERVICE
 *                  event CELL_M_EVENT_OAMH_CLEANUP_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_cell_out_of_service_state_cleanup_req(
        void            *p_api,
        void            *p_msg,
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    RRC_CSC_UT_TRACE_ENTER();

    /*coverity fix start*/
    /*coverity fix stop*/
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    RRC_CSC_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p and p_msg=%p", p_api, p_msg);
    /* Delete from Cell DB and free */
    csc_cell_ctx_out_of_service_cleanup(p_cell_ctx,p_csc_gl_ctx);

    RRC_CSC_UT_TRACE_EXIT();
}


/*
 *   CELL_M_STATE_CELL_IN_SERVICE state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_cell_in_service_state_cleanup_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function stop SFN timer and clear Cell DB.
 *                  state CELL_M_STATE_CELL_IN_SERVICE
 *                  event CELL_M_EVENT_OAMH_CLEANUP_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_cell_in_service_state_cleanup_req(
        void            *p_api,
        void            *p_msg,
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    RRC_CSC_UT_TRACE_ENTER();

    /*coverity fix start*/
    /*coverity fix stop*/
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    RRC_CSC_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p and p_msg=%p", p_api, p_msg);
            
    /* Delete from Cell DB and free */
    csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);

    RRC_CSC_UT_TRACE_EXIT();
}

/*
 *   CELL_M_STATE_CELL_IN_SERVICE state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_cell_in_service_sfn_sync_timer_expired
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function sends CSC_LLIM_SFN_REQ message to LLIM.
 *                  state CELL_M_STATE_CELL_IN_SERVICE;
 *                  event CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_cell_in_service_sfn_sync_timer_expired(
        void            *p_api,         /* PNULL */
        void            *p_msg,         /* PNULL */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    void *p_intrl_msg = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_CSC_TRACE(RRC_DETAILED, "Unused variable in fsm p_api=%p, p_msg=%p"
            " and csc_gl_ctx=%p", p_api, p_msg, p_csc_gl_ctx);
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

#ifdef RRC_CSC_DEBUG_FT
    /* it's called only once for testing purposes */
    p_cell_ctx->mac_sfn_sync_timer = PNULL;
#endif /*RRC_CSC_DEBUG_FT*/

    RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_reconf_cell_state_timer_expired
 *   INPUT        : rrc_timer_t     timer_id
 *                  void            *p_timer_buf
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function stop cell Reconfiguration 
 *                  state CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP;
 *                  event CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_reconf_cell_state_timer_expired(
        rrc_timer_t     timer_id,       /* timer_id - unused */
        void            *p_timer_buf,   /* p_csc_cell_m_timer_buf_t - unused */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    RRC_CSC_TRACE(RRC_DETAILED,
        "Unused variable in fsm timer_id=%p, timer_buff=%p", timer_id, p_timer_buf);
    /* Timer expired and is not valid any more */
    RRC_CSC_TRACE(RRC_BRIEF, "Timer [CSC_CELL_RECONFIG_TIMER] expired");
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
    if (p_csc_gl_ctx->max_cell_reconfig_retry_count  == p_cell_ctx->retry_count)
    {
        /* 1. Send RRC_RRC_LL_CONFIG_FAILURE */
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(   
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_INTERNAL_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
            RRC_CSC_UT_TRACE_EXIT();
            return;
        }

        p_cell_ctx->rcfg_status_flag = RRC_FALSE;
        p_cell_ctx->retry_count = RRC_NULL; 

        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;

        /* 2. State Transition */

        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx, p_cell_ctx->cell_index);
        
    }
    else
    {
        /* 1. Send Reconfig with Old Configuration */
        csc_send_llim_recfg_req_with_old_cfg(p_cell_ctx,p_csc_gl_ctx, NO_MODE);
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/*
 *   CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_del_cell_state_llim_resp
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function report failure of cell configuration.
 *                  state CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP;
 *                  event CELL_M_EVENT_LLIM_DELETE_CELL_RESP
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_del_cell_state_llim_resp(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    rrc_csc_llim_delete_cell_resp_t *p_delete_cell_resp =
        (rrc_csc_llim_delete_cell_resp_t *)p_msg;

    rrm_rrc_cell_del_resp_t cell_del_resp;
    void *p_intrl_msg = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_delete_cell_resp);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(p_delete_cell_resp->cell_index == p_cell_ctx->cell_index);

    RRC_CSC_TRACE(RRC_DETAILED,
        "Unused variable in fsm p_api=%p", p_api);

    /* Precondition - should be the same transaction */
    if (p_delete_cell_resp->transaction_id != p_cell_ctx->transaction_id)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /* 1. Stop guard timer */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_DELETE_TIMER]");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    if (p_delete_cell_resp->response == RRC_SUCCESS)
    {
        if (p_cell_ctx->rrc_rrm_del_cell_flag == 1)
        {

            /* 3. Send RRC_RRM_CELL_DELETE_RESP */

            memset_wrapper ( &cell_del_resp, RRC_NULL, 
                      sizeof(rrm_rrc_cell_del_resp_t) );
            cell_del_resp.cell_index          = p_cell_ctx->cell_index;
            cell_del_resp.response            = RRC_SUCCESS;
            
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
                g_rrc_stats.p_rrc_stats->rrc_csc_stats.
                    rrc_csc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_FALSE;
                /*SPR_19262_START*/
		g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[
                    0].rrc_uecc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_FALSE;
                /*SPR_19262_END*/
            }
            if(RRC_SUCCESS != rrc_rrm_il_send_rrm_rrc_cell_del_resp(&cell_del_resp,
                        RRC_CSC_MODULE_ID,RRC_RRM_MODULE_ID, 
                        p_cell_ctx->transaction_id, p_cell_ctx->cell_index))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP TO RRM");
                RRC_CSC_UT_TRACE_EXIT();
            }
            /* Set all flags to RRC_FALSE */
            p_cell_ctx->rrc_rrm_del_cell_flag = RRC_FALSE;
            p_cell_ctx->rrc_rrm_s1ap_dereg_flag = RRC_FALSE; 
        }
        else
        {
            /* 2. Send RRC_RRM_CELL_SETUP_RESP */
            if (RRC_SUCCESS != csc_send_rrm_cell_setup_resp( 
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_LL_CONFIG_FAILURE))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }
            else
            {
                RRC_CSC_TRACE(RRC_WARNING, "Sent RESP FAILURE TO RRM");
            }
            return;
        }
        /*x2ap_start*/
        /*Send X2AP_CSC_CELL_ADD_IND*/ 
         p_intrl_msg = csc_alloc_intrl_msg( RRC_X2AP_MODULE_ID,
                X2AP_CSC_CELL_DEL_IND,
                sizeof (x2ap_csc_cell_del_t));

        if (PNULL != p_intrl_msg)
        {
            /* Fill and send message */
            x2ap_csc_cell_del_t *p_x2ap_csc_cell_del_ind = (x2ap_csc_cell_del_t *)(
                    (U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

            p_x2ap_csc_cell_del_ind->cell_index = (U8)p_cell_ctx->cell_index;

            rrc_send_message(p_intrl_msg, RRC_X2AP_MODULE_ID);
        }
        /*x2ap_end*/
    }
    else
    {
        if (p_cell_ctx->rrc_rrm_del_cell_flag == 1)
        {
            /* Send RRC_RRM_CELL_DELETE_RESP FAILURE  */
            if (RRC_SUCCESS != csc_send_rrm_cell_del_resp_failure(
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_INTERNAL_FAILURE))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
                RRC_CSC_UT_TRACE_EXIT();
            }
            else
            {
                RRC_CSC_TRACE(RRC_WARNING, "Sent RESP FAILURE TO RRM");
            }
            /* Resetting the flag */
            p_cell_ctx->rrc_rrm_del_cell_flag = RRC_FALSE;
            p_cell_ctx->rrc_rrm_s1ap_dereg_flag = RRC_FALSE; 
            
        }
        else
        {
            /* 2. Send RRC_RRM_CELL_SETUP_RESP */
            if (RRC_SUCCESS != csc_send_rrm_cell_setup_resp( 
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_LL_CONFIG_FAILURE))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }
            else
            {
                RRC_CSC_TRACE(RRC_WARNING, "Sent RESP FAILURE TO RRM");
            }
            return;
        }
    }
    /* Delete from Cell DB and free */
    csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);

    RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_del_cell_state_timer_expired
 *   INPUT        : rrc_timer_t     timer_id
 *                  void            *p_timer_buf
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function report failure of cell configuration.
 *                  state CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP;
 *                  event CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_del_cell_state_timer_expired(
        rrc_timer_t     timer_id,       /* timer_id - unused */
        void            *p_timer_buf,   /* p_csc_cell_m_timer_buf_t - unused */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    
    RRC_CSC_TRACE(RRC_DETAILED,
        "Unused variable in fsm timer_id=%p, timer_buff=%p", timer_id, p_timer_buf);
    /* Timer expired and is not valid any more */
    RRC_CSC_TRACE(RRC_BRIEF, "Timer [CSC_CELL_DELETE_TIMER] expired");
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */

    /*send RRM SUCCESS while deleting the cell triggered by RRM */ 
    if (p_cell_ctx->rrc_rrm_del_cell_flag == 1)
    {
        rrm_rrc_cell_del_resp_t cell_del_resp;

        /* 3. Send RRC_RRM_CELL_DEL_RESP */
        cell_del_resp.presence_bitmask    = CELL_DEL_RESP_API_FAIL_CAUSE_PRESENCE_FLAG;
        cell_del_resp.cell_index          = p_cell_ctx->cell_index;
        cell_del_resp.response            = RRC_FAILURE;
        cell_del_resp.fail_cause          = RRM_RRC_INTERNAL_FAILURE;

        if (RRC_SUCCESS !=rrc_rrm_il_send_rrm_rrc_cell_del_resp(&cell_del_resp,
                    RRC_CSC_MODULE_ID,RRC_RRM_MODULE_ID, 
                    p_cell_ctx->transaction_id, p_cell_ctx->cell_index))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP TO RRM");
            RRC_CSC_UT_TRACE_EXIT();
        }
        RRC_CSC_TRACE(RRC_BRIEF, "CELL DELETE RESPONSE SENT TO RRM");
        p_cell_ctx->rrc_rrm_del_cell_flag = 0;
    }
    else 
    {
        /* 1. Send RRC_RRM_CELL_SETUP_RESP */
        if (RRC_SUCCESS != csc_send_rrm_cell_setup_resp( 
                p_cell_ctx->transaction_id,
                p_cell_ctx->cell_index,
                RRC_FAILURE,
                    RRM_RRC_LL_CONFIG_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
            RRC_CSC_UT_TRACE_EXIT();
            return;
        }
        else
        {
            RRC_CSC_TRACE(RRC_WARNING, "Sent RESP FAILURE TO RRM");
        }
        return;

    }

    /* Delete from Cell DB and free */
    csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);

    RRC_CSC_UT_TRACE_EXIT();
}




/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_del_cell_state_cleanup_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function abort ongoing operation and clear Cell DB.
 *                  state CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP;
 *                  event CELL_M_EVENT_OAMH_CLEANUP_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_del_cell_state_cleanup_req(
        void            *p_api,
        void            *p_msg,
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    RRC_CSC_UT_TRACE_ENTER();

    /*coverity fix start*/
    /*coverity fix stop*/
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    /*coverity fix start*/
    RRC_CSC_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p and p_msg=%p", p_api, p_msg);
    /*coverity fix stop*/

    /* Delete from Cell DB and free */
    csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);

    RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_cell_in_service_state_rrm_delete_cell_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function abort ongoing operation and clear Cell DB.
 *                  state CELL_M_STATE_CELL_IN_SERVICE
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_cell_in_service_state_rrm_delete_cell_req(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
#ifdef RRC_DEBUG
    /* GDB Fix - compiler warning */
    rrc_rrm_cell_del_req_t *p_del_cell_req  =
        (rrc_rrm_cell_del_req_t *)p_msg;
#endif
    void *p_intrl_msg        = PNULL;
    s1ap_cell_dreg_req_t *p_s1ap_cell_dreg_req = PNULL;
    csc_cell_m_timer_buf_t guard_timer_data;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
#ifdef RRC_DEBUG
    /* GDB Fix - compiler warning */
    RRC_ASSERT(PNULL != p_del_cell_req);
    RRC_ASSERT(p_del_cell_req->cell_index == p_cell_ctx->cell_index);
#endif
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    

    /* Since the Cell Delete Req has been invoked, the flag is set to true */
    p_cell_ctx->rrc_rrm_del_cell_flag = RRC_TRUE;

    p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);

    if(PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "STOP CELL TIMER; Unused variable p_msg=%p",p_msg);/*SPR 17777 +-*/
        /* Stop guard timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;

    }

    /* 1. set guard timer */
    guard_timer_data.auto_delete        = RRC_TRUE;
    guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
    guard_timer_data.cell_index         = p_cell_ctx->cell_index;
    guard_timer_data.cell_m_event       =
        CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED;

    RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_DEL_TIMER] %u ms.",
            p_csc_gl_ctx->cell_del_duration);
    p_cell_ctx->guard_timer = rrc_start_timer(p_csc_gl_ctx->cell_del_duration,
            &guard_timer_data, sizeof (guard_timer_data), RRC_FALSE);

    /* Checking if the S1AP_DEREG_REQ and UECC_DEL_ALL_UE has already been sent or not */
    if(p_cell_ctx->rrc_rrm_s1ap_dereg_flag == RRC_TRUE)
    {

        RRC_CSC_TRACE(RRC_WARNING, "S1AP_CELL_DEREG_REQ and CSC_UECC_DEL_ALL_UE_REQ"
                "messages already sent during Cell Stop Request.");

        /* 2. Send CSC_LLIM_DELETE_CELL_REQ */
        p_intrl_msg = csc_alloc_intrl_msg( 
                RRC_LLIM_MODULE_ID,
                CSC_LLIM_DELETE_CELL_REQ,
                sizeof (rrc_csc_llim_delete_cell_req_t));

#ifdef RRC_UT_FRAMEWORK
    pLlimDeleteAllUeReq = rrc_mem_get(sizeof(rrc_csc_uecc_delete_all_ue_req_t) +
                                    RRC_API_HEADER_SIZE);
#endif

        if (PNULL != p_intrl_msg)
        {
            /* Fill and send message */
            rrc_csc_llim_delete_cell_req_t *p_llim_delete_cell_req =
                (rrc_csc_llim_delete_cell_req_t *)((U8 *)p_intrl_msg +
                        RRC_API_HEADER_SIZE);
            p_llim_delete_cell_req->transaction_id  = p_cell_ctx->transaction_id;
            p_llim_delete_cell_req->cell_index      = p_cell_ctx->cell_index;
            /* SPR 18354 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
            if((p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
                  || (p_cell_ctx->cfg_cell_param.presence_bitmask & 
                  CELL_SETUP_REQ_API_EMBMS_AREA_INFO_LIST_PRESENCE_FLAG))
            {
                p_llim_delete_cell_req->mbms_updated_area_info_list = p_cell_ctx->
                    cfg_cell_param.mbms_updated_area_info_list;
            }
            if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG)
            {
                p_llim_delete_cell_req->m2ap_reserved_mcch_related_bcch_info_list = 
                    p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list;
            }

#endif
            /* SPR 18354 Fix End */
#ifdef RRC_UT_FRAMEWORK
        l3_memcpy_wrapper(pLlimDeleteAllUeReq,p_intrl_msg,
                  (sizeof(rrc_csc_uecc_delete_all_ue_req_t) +
                   RRC_API_HEADER_SIZE));
#endif

            rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
            CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP);
        }
        else
        {
            /* Bad incoming info */
            RRC_CSC_TRACE(RRC_WARNING, 
                    "unable to send CSC_LLIM_DELETE_CELL_REQ message");
        }
    }
    else  
    {
        /* 5. Send S1AP_CELL_DEREG_REQ */
        p_intrl_msg = csc_alloc_intrl_msg( 
                RRC_S1AP_MODULE_ID,
                S1AP_CELL_DEREG_REQ,
                sizeof (s1ap_cell_dreg_req_t));
        if (PNULL != p_intrl_msg)
        {
            /* Fill and send message */
            p_s1ap_cell_dreg_req = (s1ap_cell_dreg_req_t *)(
                    (U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

            p_s1ap_cell_dreg_req->cell_index = p_cell_ctx->cell_index;
            rrc_send_message(p_intrl_msg, RRC_S1AP_MODULE_ID);
        }
        else
        {
            /* Bad incoming info */
            RRC_CSC_TRACE(RRC_WARNING, "unable to send S1AP_CELL_DEREG_REQ message");
            return;
        }

        /* 2. send CSC_UECC_DEL_ALL_UE_REQ */
        p_intrl_msg = csc_alloc_intrl_msg( 
                RRC_UECC_MODULE_ID,
                CSC_UECC_DELETE_ALL_UE_REQ,
                sizeof (rrc_csc_uecc_delete_all_ue_req_t));

        if (PNULL != p_intrl_msg)
        {
            /* Fill and send message */
            rrc_csc_uecc_delete_all_ue_req_t *p_uecc_del_cell_req = 
                (rrc_csc_uecc_delete_all_ue_req_t *)((U8 *)p_intrl_msg +
                        RRC_API_HEADER_SIZE);
            p_uecc_del_cell_req->cell_index = p_cell_ctx->cell_index;
            p_uecc_del_cell_req->is_cell_stopped = RRC_FALSE;
	    /* SPR 21458 Start */
	    if(RRC_TRUE == p_cell_ctx->l2_instance_reset_flag)
	    {
		p_uecc_del_cell_req->l2_instance_reset_flag = RRC_TRUE;
	    }
	    /* SPR 21458 End */
            rrc_send_message(p_intrl_msg, RRC_UECCMD_MODULE_ID);

            p_cell_ctx->rrc_rrm_s1ap_dereg_flag = RRC_TRUE;

            CELL_M_FSM_SET_STATE(p_cell_ctx,
                    CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP);
        }
        else
        {
            /* Bad incoming info */
            RRC_CSC_TRACE(RRC_WARNING, "unable to send CSC_UECC_DEL_CELL_REQ message");
            return;
        }
    }
    RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_cell_xxx_state_rrm_delete_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function aborts ongoing operation and clear Cell DB.
 *                  state CELL_M_STATE_W_FOR_LLIM_CELL_START_RESP
 *                  state CELL_M_STATE_W_FOR_LLIM_CELL_STOP_RESP
 *                  state CELL_M_STATE_CELL_OUT_OF_SERVICE
 *                  Event: 
 *                  CELL_M_EVENT_RRM_DELETE_CELL_REQ 
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_cell_xxx_state_rrm_delete_req(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
#ifdef RRC_DEBUG
    /* GDB Fix - compiler warning */
    rrc_rrm_cell_del_req_t *p_del_cell_req =
                (rrc_rrm_cell_del_req_t *)p_msg;
#endif

    csc_cell_m_timer_buf_t          guard_timer_data;
    void                            *p_intrl_msg            = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
#ifdef RRC_DEBUG
    /* GDB Fix - compiler warning */
    RRC_ASSERT(PNULL != p_del_cell_req);
    RRC_ASSERT(p_del_cell_req->cell_index == p_cell_ctx->cell_index);
#endif
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    

    p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);

    /* IF THIS FUNCTION IS INVOKED IN  CELL_M_STATE_W_FOR_LLIM_CELL_START/STOP_RESP, 
     * stopping of the timer is  required here. 
     * If this function is invoked in out of service state
     * p_cell_ctx->guard_timer will be PNULL */
    if(PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "STOP CELL TIMER;Unused variable p_msg=%p",p_msg);/*SPR 17777 +-*/
        /* Stop guard timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;

    }
    /* set guard timer */
    guard_timer_data.auto_delete        = RRC_TRUE;
    guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
    guard_timer_data.cell_index         = p_cell_ctx->cell_index;
    guard_timer_data.cell_m_event       =
        CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED;

    RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_DEL_TIMER] %u ms.",
            p_csc_gl_ctx->cell_del_duration);
    p_cell_ctx->guard_timer = rrc_start_timer(p_csc_gl_ctx->cell_del_duration,
            &guard_timer_data, sizeof (guard_timer_data), RRC_FALSE);

    /* CSR 00052493 start*/

    /* Setting the flag, to send appropriate CELL_DELETE response to RRM  */
    p_cell_ctx->rrc_rrm_del_cell_flag = RRC_TRUE;

    /* 2. send CSC_UECC_DEL_ALL_UE_REQ */
    p_intrl_msg = csc_alloc_intrl_msg( 
            RRC_UECC_MODULE_ID,
            CSC_UECC_DELETE_ALL_UE_REQ,
            sizeof (rrc_csc_uecc_delete_all_ue_req_t));

    if (PNULL != p_intrl_msg)
    {
        /* Fill and send message */
        rrc_csc_uecc_delete_all_ue_req_t *p_uecc_del_cell_req = 
            (rrc_csc_uecc_delete_all_ue_req_t *)((U8 *)p_intrl_msg +
                    RRC_API_HEADER_SIZE);
        p_uecc_del_cell_req->cell_index = p_cell_ctx->cell_index;
        p_uecc_del_cell_req->is_cell_stopped = RRC_FALSE;

        rrc_send_message(p_intrl_msg, RRC_UECCMD_MODULE_ID);

        p_cell_ctx->rrc_rrm_s1ap_dereg_flag = RRC_TRUE;

        CELL_M_FSM_SET_STATE(p_cell_ctx,
                CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP);
    }
    /* CSR 00052493 stop*/
    else
    {
        /* Bad incoming info */
        RRC_CSC_TRACE(RRC_WARNING, 
                "CSC_UECC_DELETE_CELL_REQ memory allocation failed");
        /* Stop guard timer */
        if (PNULL != p_cell_ctx->guard_timer)
        {
            rrc_stop_timer(p_cell_ctx->guard_timer);
            p_cell_ctx->guard_timer = PNULL;
        } 
        return;
    } 

    RRC_CSC_UT_TRACE_EXIT();

}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_cell_in_w_llim_reconfig_state_rrm_delete_cell_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function abort ongoing operation and clear Cell DB.
 *                  state CELL_M_STATE_CELL_IN_SERVICE
 *                  or    CELL_M_STATE_CELL_OUT_OF_SERVICE
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_cell_in_w_llim_reconfig_state_rrm_delete_cell_req(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
#ifdef RRC_DEBUG
    /* GDB Fix - compiler warning */
    rrc_rrm_cell_del_req_t        *p_del_cell_req  =
        (rrc_rrm_cell_del_req_t *)p_msg;
#endif
    csc_cell_m_timer_buf_t guard_timer_data;
    void *p_intrl_msg        = PNULL;
    s1ap_cell_dreg_req_t *p_s1ap_cell_dreg_req = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
#ifdef RRC_DEBUG
    /* GDB Fix - compiler warning */
    RRC_ASSERT(PNULL != p_del_cell_req);
    RRC_ASSERT(p_del_cell_req->cell_index == p_cell_ctx->cell_index);
#endif
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    

    p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);

    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_RECONFIG_TIMER];Unused variable p_msg=%p",p_msg);/*SPR 17777 +-*/
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    /*  set guard timer */
    guard_timer_data.auto_delete        = RRC_TRUE;
    guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
    guard_timer_data.cell_index         = p_cell_ctx->cell_index;
    guard_timer_data.cell_m_event       =
        CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED;

    RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_DEL_TIMER] %u ms.",
            p_csc_gl_ctx->cell_del_duration);
    p_cell_ctx->guard_timer = rrc_start_timer(p_csc_gl_ctx->cell_del_duration,
            &guard_timer_data, sizeof (guard_timer_data), RRC_FALSE);

    /* Send S1AP_CELL_DEREG_REQ only if the cell was in_service state,
     * Before Cell reconfiguration proc is triggered .*/
    /* Send S1AP_CELL_DEREG_REQ */
    if(p_cell_ctx->rrc_rrm_s1ap_dereg_flag == RRC_TRUE)
    {

        RRC_CSC_TRACE(RRC_WARNING, "S1AP_CELL_DEREG_REQ and CSC_UECC_DEL_ALL_UE_REQ"
                "messages already sent during Cell Stop Request.");

        /* 2. Send CSC_LLIM_DELETE_CELL_REQ */
        p_intrl_msg = csc_alloc_intrl_msg( 
                RRC_LLIM_MODULE_ID,
                CSC_LLIM_DELETE_CELL_REQ,
                sizeof (rrc_csc_llim_delete_cell_req_t));

        if (PNULL != p_intrl_msg)
        {
            /* Fill and send message */
            rrc_csc_llim_delete_cell_req_t *p_llim_delete_cell_req =
                (rrc_csc_llim_delete_cell_req_t *)((U8 *)p_intrl_msg +
                        RRC_API_HEADER_SIZE);
            p_llim_delete_cell_req->transaction_id  = p_cell_ctx->transaction_id;
            p_llim_delete_cell_req->cell_index      = p_cell_ctx->cell_index;
            /* SPR 18354 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
            if((p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
                  || (p_cell_ctx->cfg_cell_param.presence_bitmask & 
                  CELL_SETUP_REQ_API_EMBMS_AREA_INFO_LIST_PRESENCE_FLAG))
            {
                p_llim_delete_cell_req->mbms_updated_area_info_list = p_cell_ctx->
                    cfg_cell_param.mbms_updated_area_info_list;
            }
            if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG)
            {
                p_llim_delete_cell_req->m2ap_reserved_mcch_related_bcch_info_list = 
                    p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list;
            }

#endif
            /* SPR 18354 Fix End */

            rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
            CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP);
        }
        else
        {
            /* Bad incoming info */
            RRC_CSC_TRACE(RRC_WARNING, 
                    "unable to send CSC_LLIM_DELETE_CELL_REQ message");
        }
    }
    else 
    {
    p_intrl_msg = csc_alloc_intrl_msg( 
            RRC_S1AP_MODULE_ID,
            S1AP_CELL_DEREG_REQ,
            sizeof (s1ap_cell_dreg_req_t));
    if (PNULL != p_intrl_msg)
    {
        /* Fill and send message */
        p_cell_ctx->rrc_rrm_del_cell_flag = 1;
        p_s1ap_cell_dreg_req = (s1ap_cell_dreg_req_t *)(
                (U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

        p_s1ap_cell_dreg_req->cell_index = p_cell_ctx->cell_index;

        rrc_send_message(p_intrl_msg, RRC_S1AP_MODULE_ID);
        RRC_CSC_TRACE(RRC_BRIEF, "S1AP_CELL_DREG_REQ sent to S1AP");
    }
    else
    {
        /* Bad incoming info */
        RRC_CSC_TRACE(RRC_WARNING, "unable to send S1AP_CELL_DREG_REQ message");
        return;
    }

    /* 2. send CSC_UECC_DEL_CELL_REQ */
    p_intrl_msg = csc_alloc_intrl_msg( 
            RRC_UECC_MODULE_ID,
            CSC_UECC_DELETE_ALL_UE_REQ,
            sizeof(rrc_csc_uecc_delete_all_ue_req_t));
    if (PNULL != p_intrl_msg)
    {
        /* Fill and send message */
        rrc_csc_uecc_delete_all_ue_req_t *p_uecc_del_cell_req = 
            (rrc_csc_uecc_delete_all_ue_req_t *)((U8 *)p_intrl_msg +
                                                 RRC_API_HEADER_SIZE);
        p_uecc_del_cell_req->cell_index = p_cell_ctx->cell_index;
        p_uecc_del_cell_req->is_cell_stopped = RRC_FALSE;

        rrc_send_message(p_intrl_msg, RRC_UECCMD_MODULE_ID);

        CELL_M_FSM_SET_STATE(p_cell_ctx,
                CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP);

            p_cell_ctx->rrc_rrm_s1ap_dereg_flag = RRC_TRUE;
    }
    else
    {
        /* Bad incoming info */
        RRC_CSC_TRACE(RRC_WARNING, "unable to send CSC_UECC_DEL_CELL_REQ message");
        return;
    }

    }
    RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_uecc_delete_ue_resp
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function abort ongoing operation and clear Cell DB.
 *                  state  CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_uecc_delete_ue_resp(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
#ifdef RRC_DEBUG
    /* GDB Fix - compiler warning */
  rrc_csc_uecc_delete_all_ue_resp_t *p_delete_cell_resp =
    (rrc_csc_uecc_delete_all_ue_resp_t *)p_msg;
#endif
  void                            *p_intrl_msg = PNULL;
  RRC_CSC_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_api);
#ifdef RRC_DEBUG
    /* GDB Fix - compiler warning */
  RRC_ASSERT(PNULL != p_delete_cell_resp);
  RRC_ASSERT(p_delete_cell_resp->cell_index == p_cell_ctx->cell_index);
 #endif
  RRC_ASSERT(PNULL != p_cell_ctx);
  RRC_ASSERT(PNULL != p_csc_gl_ctx);

  if(p_cell_ctx->rrc_rrm_stop_cell_flag == RRC_TRUE)
  {
      /* send cell stop request to LLIM */
      cell_m_fsm_build_and_send_cell_stop_req_to_llim(p_api,
              p_msg,
              p_cell_ctx,
              p_csc_gl_ctx);
  }
  else
  {
	  /* 2. Send CSC_LLIM_DELETE_CELL_REQ */
	  p_intrl_msg = csc_alloc_intrl_msg( 
			  RRC_LLIM_MODULE_ID,
			  CSC_LLIM_DELETE_CELL_REQ,
			  sizeof (rrc_csc_llim_delete_cell_req_t));

	  if (PNULL != p_intrl_msg)
	  {
		  /* Fill and send message */
		  rrc_csc_llim_delete_cell_req_t *p_llim_delete_cell_req =
			  (rrc_csc_llim_delete_cell_req_t *)((U8 *)p_intrl_msg +
					  RRC_API_HEADER_SIZE);
		  p_llim_delete_cell_req->transaction_id  = p_cell_ctx->transaction_id;
		  p_llim_delete_cell_req->cell_index      = p_cell_ctx->cell_index;
		  /*SPR 21369 Start*/
		  p_llim_delete_cell_req->l2_instance_reset_flag =p_cell_ctx->l2_instance_reset_flag; 
		  /*SPR 21369 End*/
            /* SPR 18354 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
            if((p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
                  || (p_cell_ctx->cfg_cell_param.presence_bitmask & 
                  CELL_SETUP_REQ_API_EMBMS_AREA_INFO_LIST_PRESENCE_FLAG))
            {
                p_llim_delete_cell_req->mbms_updated_area_info_list = p_cell_ctx->
                    cfg_cell_param.mbms_updated_area_info_list;
            }
            if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG)
            {
                p_llim_delete_cell_req->m2ap_reserved_mcch_related_bcch_info_list = 
                    p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list;
            }

#endif
            /* SPR 18354 Fix End */



		  rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
		  CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP);
	  }
	  else
	  {
		  /* Bad incoming info */
		  RRC_CSC_TRACE(RRC_WARNING, 
				  "unable to send CSC_LLIM_DELETE_CELL_REQ message");
	  }
  }
  RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_uecc_del_ue_state_timer_exp
 *   INPUT        : rrc_timer_t     timer_id
 *                  void            *p_timer_buf
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *   
 *   DESCRIPTION  : This function abort ongoing operation and clear Cell DB.
 *                  state   CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_uecc_del_ue_state_timer_exp(
        rrc_timer_t     timer_id,       /* timer_id - unused */
        void            *p_timer_buf,   /* p_csc_cell_m_timer_buf_t - unused */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    RRC_CSC_TRACE(RRC_DETAILED,
            "Unused variable in fsm timer_id=%p and p_timer_buf=%p", timer_id, p_timer_buf);
    /* Timer expired and is not valid any more */
    RRC_CSC_TRACE(RRC_BRIEF, "Timer [CSC_CELL_DELETE_TIMER] expired");
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
/* SPR 14702 Fix Start */
    rrm_rrc_cell_del_resp_t   cell_del_resp;
/* SPR 14702 Fix Stop */

    /* SPR 2938: If the response from UECC is not received at CSC due to any reason
     * and CSC Cell Delete Timer Expires in CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP 
     * state( waiting for CSC_UECC_DELETE_ALL_UE_RESP ), then the Failure will be 
     * sent to RRM. The Cell Delete Request will NOT be sent to LLIM  and the cell context 
     * will NOT be cleaned up. CSC will delete the cell if and only if all UEs
     * have been released successfully by UECC/LLIM (and all lower layers).  */

/* SPR 14702 Fix Start */
    /* Send RRM success while deleting the cell triggered by RRM */     
/*SPR 9766 Fix Start*/
    if (p_cell_ctx->rrc_rrm_del_cell_flag ==  RRC_TRUE)
    {
        /* Send RRC_RRM_CELL_DELETE_RESP */

        memset_wrapper ( &cell_del_resp, RRC_NULL, 
            sizeof(rrm_rrc_cell_del_resp_t) );
        cell_del_resp.cell_index          = p_cell_ctx->cell_index;
        cell_del_resp.response            = RRC_FAILURE;
        cell_del_resp.fail_cause          = RRM_RRC_INTERNAL_FAILURE;
        cell_del_resp.presence_bitmask    = CELL_DEL_RESP_API_FAIL_CAUSE_PRESENCE_FLAG;
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
            g_rrc_stats.p_rrc_stats->rrc_csc_stats.
                rrc_csc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_FALSE;
        }
        if(RRC_SUCCESS != rrc_rrm_il_send_rrm_rrc_cell_del_resp(&cell_del_resp,
                        RRC_CSC_MODULE_ID,RRC_RRM_MODULE_ID, 
                        p_cell_ctx->transaction_id,p_cell_ctx->cell_index))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
            RRC_CSC_UT_TRACE_EXIT();
            return;
        }
        /* Set all flags to RRC_FALSE */
        p_cell_ctx->rrc_rrm_s1ap_dereg_flag = 0; 
        p_cell_ctx->rrc_rrm_del_cell_flag = 0;
    }

    /* STATE TRANSITION*/
    CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index) ; 
/*SPR 9766 Fix Stop*/
/* SPR 14702 Fix Stop */
/*SPR_19262_START*/
    if (p_cell_ctx->rrc_rrm_del_cell_flag ==  RRC_TRUE)
    {
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
            g_rrc_stats.p_rrc_stats->rrc_csc_stats.
                rrc_csc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_FALSE;
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[
                0].rrc_uecc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_FALSE;
        }
    }
    /*SPR_19262_END*/

    RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_uecc_del_ue_state_clean_up_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function abort ongoing operation and clear Cell DB.
 *                  state   CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_uecc_del_ue_state_clean_up_req(
        void            *p_api,
        void            *p_msg,
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    RRC_CSC_UT_TRACE_ENTER();

    /*coverity fix start*/
    /*coverity fix stop*/
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    
    RRC_CSC_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p and p_msg=%p", p_api, p_msg);

    /* 1. Stop guard timer */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_DELETE_TIMER]");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    /* Delete from Cell DB and free */
    csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);

    RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_reconfig_cell_resp_state_clean_up_req 
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function abort ongoing operation and clear Cell DB.
 *                  state   CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP
 *
 *   
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_reconfig_cell_resp_state_clean_up_req(
        void            *p_api,
        void            *p_msg,
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    RRC_CSC_UT_TRACE_ENTER();

    /*coverity fix start*/
    /*coverity fix stop*/
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    /*coverity fix start*/
    RRC_CSC_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p and p_msg=%p", p_api, p_msg);
    /*coverity fix stop*/

    /* 1. Stop guard timer */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_DELETE_TIMER]");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    /* Delete from Cell DB and free */
    csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);

    RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_for_llim_reconfig_cell_resp_sfn_sync_timer_expired 
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function ignores the SYNC_TIMER_EXPIRY event 
 *                  state   CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_for_llim_reconfig_cell_resp_sfn_sync_timer_expired(
        void            *p_api,         /* PNULL */
        void            *p_msg,         /* PNULL */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    RRC_CSC_TRACE(RRC_DETAILED, "Unused variable in fsm p_api=%p, p_msg=%p"
            " and csc_gl_ctx=%p", p_api, p_msg, p_csc_gl_ctx);
    /* 1. Stop guard timer */
    if (RRC_TRUE == p_cell_ctx->mac_reconfig_with_sib8_segments)
    {
        RRC_CSC_TRACE(RRC_INFO, "Ignoring event SYNC_LOSS_TIMER_EXPIRY as extended Cell Setup "
                "ongoing for SIB-8 configuration");
    }
    else
    {
        RRC_CSC_TRACE(RRC_WARNING, "[%s] ignore event received in state [%s]",
            RRC_CELL_M_FSM_NAME,
            CELL_M_FSM_STATES_NAMES[CELL_M_FSM_GET_CURRENT_STATE(p_cell_ctx)]);
    }

    RRC_CSC_UT_TRACE_EXIT();
}



/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_cell_del_state_clean_up_req 
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  :  This function abort ongoing operation and clear Cell DB.
 *                   state   CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP
 *   RETURNS: None
 *
 ******************************************************************************/
static void cell_m_fsm_w_cell_del_state_clean_up_req(
        void            *p_api,
        void            *p_msg,
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    RRC_CSC_UT_TRACE_ENTER();

    /*coverity fix start*/
    /*coverity fix stop*/
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    
    /*coverity fix start*/
    RRC_CSC_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p and p_msg=%p", p_api, p_msg);
    /*coverity fix stop*/

    /* Delete from Cell DB and free */
    csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);

    RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_conf_cell_state_rrm_del_cell_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function abort ongoing operation and clear Cell DB.
 *                  state CELL_M_STATE_W_FOR_LLIM_CONFIG_CELL_RESP  
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_conf_cell_state_rrm_del_cell_req(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
#ifdef RRC_DEBUG
    /* GDB Fix - compiler warning */
    rrc_rrm_cell_del_req_t          *p_del_cell_req  =
                                    (rrc_rrm_cell_del_req_t *)p_msg;
#endif
    csc_cell_m_timer_buf_t          guard_timer_data;
    void                            *p_intrl_msg            = PNULL;
    rrc_csc_llim_delete_cell_req_t  *p_llim_delete_cell_req = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
#ifdef RRC_DEBUG
    /* GDB Fix - compiler warning */
    RRC_ASSERT(PNULL != p_del_cell_req);
    RRC_ASSERT(p_del_cell_req->cell_index == p_cell_ctx->cell_index);
#endif
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    


    p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);

    if(PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "STOP CELL CONFIG TIMER; Unused variable p_msg=%p",p_msg);/*SPR 17777 +-*/
        /* Stop guard timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;

    }
    /* 1. set guard timer */
    guard_timer_data.auto_delete        = RRC_TRUE;
    guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
    guard_timer_data.cell_index         = p_cell_ctx->cell_index;
    guard_timer_data.cell_m_event       =
        CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED;

    RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_DEL_TIMER] %u ms.",
            p_csc_gl_ctx->cell_del_duration);
    p_cell_ctx->guard_timer = rrc_start_timer(p_csc_gl_ctx->cell_del_duration,
            &guard_timer_data, sizeof (guard_timer_data), RRC_FALSE);

    /* 2. Send CSC_LLIM_DELETE_CELL_REQ */
    p_intrl_msg = csc_alloc_intrl_msg(  
            RRC_LLIM_MODULE_ID,
            CSC_LLIM_DELETE_CELL_REQ,
            sizeof (rrc_csc_llim_delete_cell_req_t));
    if (PNULL != p_intrl_msg)
    {
        /* Fill and send message */
        p_cell_ctx->rrc_rrm_del_cell_flag = 1;
        p_llim_delete_cell_req =
            (rrc_csc_llim_delete_cell_req_t *)((U8 *)p_intrl_msg +
                                               RRC_API_HEADER_SIZE);
        p_llim_delete_cell_req->transaction_id  = p_cell_ctx->transaction_id;
        p_llim_delete_cell_req->cell_index      = p_cell_ctx->cell_index;
            /* SPR 18354 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
            if((p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
                  || (p_cell_ctx->cfg_cell_param.presence_bitmask & 
                  CELL_SETUP_REQ_API_EMBMS_AREA_INFO_LIST_PRESENCE_FLAG))
            {
                p_llim_delete_cell_req->mbms_updated_area_info_list = p_cell_ctx->
                    cfg_cell_param.mbms_updated_area_info_list;
            }
            if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG)
            {
                p_llim_delete_cell_req->m2ap_reserved_mcch_related_bcch_info_list = 
                    p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list;
            }

#endif
            /* SPR 18354 Fix End */

        rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
        RRC_CSC_TRACE(RRC_BRIEF, "CSC_LLIM_DELETE_CELL_REQ sent to LLIM");
        /* State should be set if and only if message is sent to LLIM */
        CELL_M_FSM_SET_STATE(p_cell_ctx,
                CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP);
    }
    else
    {
        /* Bad incoming info */
        RRC_CSC_TRACE(RRC_WARNING, 
                "CSC_LLIM_DELETE_CELL_REQ memory allocation failed");
        /* Stop guard timer */
        if (PNULL != p_cell_ctx->guard_timer)
        {
            rrc_stop_timer(p_cell_ctx->guard_timer);
        }    
        return;
    } 
    RRC_CSC_UT_TRACE_EXIT();
}

/*******************************************************************************
 *   FUNCTION NAME: csc_check_si_mapping
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *   OUTPUT       : none
 *   DESCRIPTION: This function checks the Schduling Info (SI Mapping) received
 *       in SIB_TYPE_1_INFO by 
 *   1) XORng the bitmask of cell Config and cell Reconfig(explained below),
 *      if this result is false then go to step 2
 *   2) By comparing 
 *      -- SI Info Count stored in cell context
 *      -- Sib bitmask mapped to SI stored in cell context
 *
 *   RETURNS:
 *       enum (SI_MAPPING_CHANGED /SI_MAPPING_NOT_CHANGED )
 ******************************************************************************/
static U8 csc_check_si_mapping(
        csc_cell_ctx_t  *p_cell_ctx)
{
    scheduling_info_t *p_l_elem = PNULL;

    static rrc_rrm_cell_reconfig_req_t recfg_req;
    U8  old_schdl_info_count    = 0;  
    U8  new_schdl_info_count    = 0;  
    U8  si_map_flag             = 0;
    U8  schdl_counter           = 0;
    U8  SIB                     = 0;
    U16 mapped_sibs_mask        = 0;
    U16 schdl_sibs_mask         = 0;

    RRC_CSC_UT_TRACE_ENTER(); 

    recfg_req = p_cell_ctx->recfg_cell_param;

    if (!(recfg_req.broadcast_info.presence_bitmask &
                CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG))
    {
        l3_memcpy_wrapper(p_cell_ctx->p_temp_si_schdl_info, 
                &p_cell_ctx->si_schdl_info, sizeof (p_cell_ctx->si_schdl_info));
        /* eMTC changes stop */

        RRC_CSC_UT_TRACE_EXIT(); 
        if (((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask) ||
            ((8 << RRM_RRC_SIB_TYPE_11) & p_cell_ctx->pws_sibs_bitmask) ||
            ((8 << RRM_RRC_SIB_TYPE_12) & p_cell_ctx->pws_sibs_bitmask))
        {
            /*PWS is on going and RRM has not sent new SI scheduling list*/
            return INVALID_SI_MAPPING_PWS;
        }
        else
        {
            return SI_MAPPING_NOT_CHANGED;
        }
    }
    /* eMTC changes stop */
    /* contela bug fix */
    /* There is no new addition of SIBs */
    /* Check the schuling info counts*/
    new_schdl_info_count = recfg_req.broadcast_info.sib_type_1_Info.
        scheduling_info_list.count;
    /* contela bug fix */

    old_schdl_info_count = p_cell_ctx->si_schdl_info.schdl_count;
    for (schdl_counter = 0; schdl_counter < old_schdl_info_count; 
            schdl_counter++)
    {
        p_l_elem = recfg_req.broadcast_info.sib_type_1_Info.
            scheduling_info_list.scheduling_info + schdl_counter;
        mapped_sibs_mask = 0;

        for (SIB = 0; SIB < p_l_elem->sib_mapping_info.count; SIB++)
        {
            mapped_sibs_mask |= (U16)(8 << p_l_elem->sib_mapping_info.
                    sib_type[SIB]);
        }
        /* Check the bitmask of the sibs scheduled in one SI mapping */ 
        if (mapped_sibs_mask != p_cell_ctx->si_schdl_info.si_bitmask[schdl_counter])
        {
            si_map_flag = 1;
        }
        /* eMTC changes stop */

        schdl_sibs_mask |= mapped_sibs_mask;    
    }

    for ( ; schdl_counter < new_schdl_info_count; 
            schdl_counter++)
    { 
        p_l_elem = recfg_req.broadcast_info.sib_type_1_Info.
            scheduling_info_list.scheduling_info + schdl_counter;

        mapped_sibs_mask = 0;

        for (SIB = 0; SIB < p_l_elem->sib_mapping_info.count; SIB++)
        {
            mapped_sibs_mask |= (U16)(8 << p_l_elem->sib_mapping_info.
                    sib_type[SIB]);
        }

        schdl_sibs_mask |= mapped_sibs_mask;    

    }

    /* Storing the bitmask(Temp) for all the SIBs in SI Mappings */
    p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask = 
        schdl_sibs_mask;
    /* eMTC changes stop */

    if (old_schdl_info_count != new_schdl_info_count)
    {
        if (schdl_sibs_mask != (p_cell_ctx->si_schdl_info.curr_schdl_sibs_bitmask
                    | p_cell_ctx->pws_sibs_bitmask))
        {
            return SI_MAPPING_CHANGED;
        }
        /* eMTC changes stop */
    }
    if (((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask) ||
        ((8 << RRM_RRC_SIB_TYPE_11) & p_cell_ctx->pws_sibs_bitmask) ||
        ((8 << RRM_RRC_SIB_TYPE_12) & p_cell_ctx->pws_sibs_bitmask))
    {
        /*If PWS is on going then RRM must send sib10, sib11 or sib12 in scheduling info*/
        if(RRC_NULL == (schdl_sibs_mask & 
                     p_cell_ctx->pws_sibs_bitmask))
        {
            RRC_CSC_UT_TRACE_EXIT();
            return INVALID_SI_MAPPING_PWS;
        }
    }


    if(si_map_flag)
    {
        RRC_CSC_UT_TRACE_EXIT(); 
        return SI_MAPPING_CHANGED;      
    }
    else
    {
        RRC_CSC_UT_TRACE_EXIT(); 
        return SI_MAPPING_NOT_CHANGED;
    }
}

/*******************************************************************************
 *   FUNCTION NAME: csc_check_si_mapping_pws
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *                  scheduling_info_list_t *p_scheduling_info_list
 *   OUTPUT       : none
 *   DESCRIPTION: This function checks the Scheduling Info (SI Mapping) received
 *   By comparing 
 *      -- SI Info Count stored in cell context
 *      -- Sib bitmask mapped to SI stored in cell context
 *
 *   RETURNS:
 *       enum (SI_MAPPING_CHANGED /SI_MAPPING_NOT_CHANGED )
 ******************************************************************************/
static U8 csc_check_si_mapping_pws(
        csc_cell_ctx_t         *p_cell_ctx,
        scheduling_info_list_t *p_scheduling_info_list) 
{
    scheduling_info_t *p_old_l_elem = PNULL;
    scheduling_info_t *p_new_l_elem = PNULL;

    U8  old_schdl_info_count    = RRC_NULL;  
    U8  new_schdl_info_count    = RRC_NULL;  
    U8  new_schdl_counter       = RRC_NULL;
    U8  old_schdl_counter       = RRC_NULL;
    U8  old_SIB                     = RRC_NULL;
    U8  new_SIB                     = RRC_NULL;
    U16 mapped_sibs_mask        = RRC_NULL;
    U16 pws_bitmask             = 0;
    rrc_bool_et non_pws_rel_sibs_present = RRC_FALSE;

    RRC_CSC_UT_TRACE_ENTER(); 


    /* There is no new addition of SIBs */
    /* Check the schuling info counts*/
    new_schdl_info_count = p_scheduling_info_list->count;
    old_schdl_info_count = p_cell_ctx->cfg_cell_param.sib_type_1_Info. \
                      scheduling_info_list.count;
    if (old_schdl_info_count < new_schdl_info_count)
    {
        return SI_MAPPING_CHANGED;
    }
    else
    {
        while ((old_schdl_counter < old_schdl_info_count) ||
               (new_schdl_counter < new_schdl_info_count)) 
        {
          new_SIB = RRC_NULL;
          old_SIB = RRC_NULL;

          /* If New SI containing PWS SIBS is added in the end of old SI */
          if (new_schdl_counter == new_schdl_info_count)
          {
            p_old_l_elem = p_cell_ctx->cfg_cell_param.sib_type_1_Info.
                scheduling_info_list.scheduling_info + old_schdl_counter;
            p_new_l_elem = p_scheduling_info_list->scheduling_info + 
                                                    new_schdl_counter;
            while (old_SIB < p_old_l_elem->sib_mapping_info.count)
            {
                /* New Added SIs can only contain SIB10, SIB11 or SIB12 */
                if (!((RRM_RRC_SIB_TYPE_10 == 
                             p_old_l_elem->sib_mapping_info.sib_type[old_SIB]) ||
                            (RRM_RRC_SIB_TYPE_11 == 
                             p_old_l_elem->sib_mapping_info.sib_type[old_SIB]) ||
                            (RRM_RRC_SIB_TYPE_12 == 
                             p_old_l_elem->sib_mapping_info.sib_type[old_SIB])))
                {
                    return SI_MAPPING_CHANGED;
                }
                else
                {
                    old_SIB++;
                }
            }
            old_schdl_counter++;
          }
          else
          {
            p_old_l_elem = p_cell_ctx->cfg_cell_param.sib_type_1_Info.
                scheduling_info_list.scheduling_info + old_schdl_counter;

            p_new_l_elem = p_scheduling_info_list->scheduling_info + 
                                                    new_schdl_counter;
            /* If old SI contains greter SIB count than new SI */
            if (p_new_l_elem->sib_mapping_info.count >
                     p_old_l_elem->sib_mapping_info.count) 
            {
                return SI_MAPPING_CHANGED;
            }
            if ( ( RRC_NULL == p_old_l_elem->sib_mapping_info.count ) &&
                   ( RRC_NULL == p_new_l_elem->sib_mapping_info.count) )
            {
                non_pws_rel_sibs_present = RRC_TRUE;
            }
            while ((old_SIB < p_old_l_elem->sib_mapping_info.count) ||
                   (new_SIB < p_new_l_elem->sib_mapping_info.count))
            {
              /* If New SIB is added in the end of old SIB */ 
              if (new_SIB == p_new_l_elem->sib_mapping_info.count)
              {
                /* New Added SIBs can only be SIB10, SIB11 or SIB12 */
                  if (!((RRM_RRC_SIB_TYPE_10 == 
                               p_old_l_elem->sib_mapping_info.sib_type[old_SIB]) ||
                              (RRM_RRC_SIB_TYPE_11 == 
                               p_old_l_elem->sib_mapping_info.sib_type[old_SIB]) ||
                              (RRM_RRC_SIB_TYPE_12 == 
                               p_old_l_elem->sib_mapping_info.sib_type[old_SIB])))
                  {
                      return SI_MAPPING_CHANGED;
                  }
                  else
                  {
                      old_SIB++;
                  }
              }
              else
              {
                /* Either the SIBs should map with old SIBs or new SIBs can
                   only be SIB10, SIB11 or SIB12 */
                if (p_new_l_elem->sib_mapping_info.sib_type[new_SIB] ==
                        p_old_l_elem->sib_mapping_info.sib_type[old_SIB])
                {
                    if ((RRM_RRC_SIB_TYPE_10 == 
                             p_old_l_elem->sib_mapping_info.sib_type[old_SIB]) ||
                            (RRM_RRC_SIB_TYPE_11 == 
                             p_old_l_elem->sib_mapping_info.sib_type[old_SIB]) ||
                            (RRM_RRC_SIB_TYPE_12 == 
                             p_old_l_elem->sib_mapping_info.sib_type[old_SIB]))
                    {
                        pws_bitmask |= (U16)(8 << p_new_l_elem->
                                sib_mapping_info.sib_type[new_SIB]);
                    }
                    non_pws_rel_sibs_present = RRC_TRUE;
                    old_SIB++;
                    new_SIB++;
                }
                else if ((RRM_RRC_SIB_TYPE_10 ==
                           p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) ||
                         (RRM_RRC_SIB_TYPE_11 ==
                           p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) ||
                         (RRM_RRC_SIB_TYPE_12 ==
                          p_new_l_elem->sib_mapping_info.sib_type[new_SIB]))
                {
                    pws_bitmask |= (U16)(8 << p_new_l_elem->
                            sib_mapping_info.sib_type[new_SIB]);
                    old_SIB++;
                    new_SIB++;
                }
                else
                {
                   return SI_MAPPING_CHANGED;
                }
              }
              mapped_sibs_mask |= (U16)(8 << p_new_l_elem->
                        sib_mapping_info.sib_type[new_SIB]);
            }
            if ( RRC_TRUE == non_pws_rel_sibs_present)
            {
                old_schdl_counter++;
                new_schdl_counter++;        
                non_pws_rel_sibs_present = RRC_FALSE;
            }
            else /*If SI containing SIB10, SIB11 or SIB12 is added in the beg or in between */
            {
                old_schdl_counter++;        
            }
          }
        }
        if ( pws_bitmask != p_cell_ctx->pws_sibs_bitmask)
        {
            return SI_MAPPING_CHANGED;
        }
        /* Storing the bitmask(Temp) for all the SIBs in SI Mappings */
        p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask =
            mapped_sibs_mask;
    }

    RRC_CSC_UT_TRACE_EXIT(); 
    return SI_MAPPING_NOT_CHANGED;
}
/*******************************************************************************
 *   FUNCTION NAME: csc_check_pws_scheduling_info
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *                  s1ap_pws_req_t *p_enb_pws_req
 *   OUTPUT       : none
 *   DESCRIPTION: This function checks the Schduling Info (SI Mapping) received
 *       in PWS req from S1AP 
 *   1) By comparing 
 *      -- SI Info Count stored in cell context
 *      -- Sib bitmask mapped to SI stored in cell context
 *
 *   RETURNS:
 *       enum (RRC_SUCCESS/RRC_FAILURE )
 ******************************************************************************/
static U8 csc_check_pws_scheduling_info(
        csc_cell_ctx_t  *p_cell_ctx,
        s1ap_pws_req_t *p_enb_pws_req)
{
    scheduling_info_t *p_old_l_elem = PNULL;
    scheduling_info_t *p_new_l_elem = PNULL;
    scheduling_info_list_t *p_scheduling_info_list = PNULL;

    U8  old_schdl_info_count    = RRC_NULL;  
    U8  new_schdl_info_count    = RRC_NULL;  
    U8  new_schdl_counter       = RRC_NULL;
    U8  old_schdl_counter       = RRC_NULL;
    U8  old_SIB                 = RRC_NULL;
    U8  new_SIB                 = RRC_NULL;
    U16 mapped_sibs_mask        = RRC_NULL;
    U16 warning_related_mask    = RRC_NULL;
    rrc_bool_et non_pws_rel_sibs_present = RRC_FALSE;
/* SPR 5611 Start */
    rrc_bool_et prim_sec_etws_changed = RRC_FALSE;
    rrc_bool_et prim_sec_order_changed = RRC_FALSE;
/* SPR 5611 End */

    RRC_CSC_UT_TRACE_ENTER(); 


    p_scheduling_info_list = &p_enb_pws_req->scheduling_info_list;
    /* There is no new addition of SIBs */
    /* Check the scheduling info counts*/
    new_schdl_info_count = 
        p_scheduling_info_list->count;
   
    /* Initialise the bitmask if it is the first request */
    if (( p_cell_ctx->etws_info == PNULL ) &&
           ( 0 == st_get_node_count ((const search_tree_t *)
                        &(p_cell_ctx->cmas_info_list))))
    {
        p_cell_ctx->pws_sibs_bitmask = RRC_NULL;
    }
    old_schdl_info_count = p_cell_ctx->cfg_cell_param.sib_type_1_Info.
               scheduling_info_list.count;

    while ((old_schdl_counter < old_schdl_info_count) ||
            (new_schdl_counter < new_schdl_info_count)) 
    {
        new_SIB = RRC_NULL;
        old_SIB = RRC_NULL;

        /* SPR 5611 Start */
        if (new_schdl_counter == new_schdl_info_count)
        {
            break;
        }
        /* SPR 5611 End */

        /* If New SI containing PWS SIBS is added in the end of old SI */
        if (old_schdl_counter == old_schdl_info_count)
        {
            p_new_l_elem = p_scheduling_info_list->scheduling_info + 
                new_schdl_counter;
            while (new_SIB < p_new_l_elem->sib_mapping_info.count)
            {
                /* New Added SIs can only contain SIB10, SIB11 or SIB12 */
                if ((RRM_RRC_SIB_TYPE_10 == 
                            p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) ||
                        (RRM_RRC_SIB_TYPE_11 == 
                         p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) ||
                        (RRM_RRC_SIB_TYPE_12 == 
                         p_new_l_elem->sib_mapping_info.sib_type[new_SIB]))
                {
                    warning_related_mask |= (U16)(8 << p_new_l_elem->
                            sib_mapping_info.sib_type[new_SIB]);
                    p_cell_ctx->pws_sibs_bitmask |= warning_related_mask;
                    mapped_sibs_mask |=  p_cell_ctx->pws_sibs_bitmask;
                    new_SIB++;
                }
                else
                {
                    return SI_MAPPING_CHANGED;
                }
            }
            new_schdl_counter++;
        }
        else
        {
            p_old_l_elem = p_cell_ctx->cfg_cell_param.sib_type_1_Info.
                scheduling_info_list.scheduling_info + old_schdl_counter;

            p_new_l_elem = p_scheduling_info_list->scheduling_info + 
                new_schdl_counter;

            if ( ( RRC_NULL == p_old_l_elem->sib_mapping_info.count ) &&
                    ( RRC_NULL == p_new_l_elem->sib_mapping_info.count) )
            {
                non_pws_rel_sibs_present = RRC_TRUE;
            }

            while  ((new_SIB < MAX_SIB_MESSAGE_1) &&
                    ((old_SIB < p_old_l_elem->sib_mapping_info.count) ||
                     (new_SIB < p_new_l_elem->sib_mapping_info.count)))
            {
                /* If New SIB is added in the end of old SIB */ 
                if (old_SIB == p_old_l_elem->sib_mapping_info.count)
                {
                    /* New Added SIBs can only be SIB10, SIB11 or SIB12 */
                    /* SPR 5611 Start */
                    if ((RRM_RRC_SIB_TYPE_10 == 
                                p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) ||
                            (RRM_RRC_SIB_TYPE_11 == 
                             p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) ||
                            (RRM_RRC_SIB_TYPE_12 == 
                             p_new_l_elem->sib_mapping_info.sib_type[new_SIB]))
                        /* SPR 5611 End */
                    {
                        warning_related_mask |= (U16)(8 << p_new_l_elem->
                                sib_mapping_info.sib_type[new_SIB]);
                        p_cell_ctx->pws_sibs_bitmask |= warning_related_mask;
                        new_SIB++;
                    }
                    else
                    {
                        return SI_MAPPING_CHANGED;
                    }
                }
                else
                {
                    /* Either the SIBs should map with old SIBs or new SIBs can
                       only be SIB10, SIB11 or SIB12 */
                    if (p_new_l_elem->sib_mapping_info.sib_type[new_SIB] ==
                            p_old_l_elem->sib_mapping_info.sib_type[old_SIB])
                    {
                        if (((RRM_RRC_SIB_TYPE_10 == 
                                        p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) ||
                                    (RRM_RRC_SIB_TYPE_11 == 
                                     p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) ||
                                    (RRM_RRC_SIB_TYPE_12 == 
                                     p_new_l_elem->sib_mapping_info.sib_type[new_SIB])))
                        {
                            warning_related_mask |= (U16)(8 << p_new_l_elem->
                                    sib_mapping_info.sib_type[new_SIB]);
                        }
                        non_pws_rel_sibs_present = RRC_TRUE;
                        /* SPR 5611 Start */
                        if ((1 == (old_schdl_info_count - (old_schdl_counter + 1))) &&
                                (RRC_NULL == (new_schdl_info_count - (new_schdl_counter + 1))))
                        {
                            p_old_l_elem = p_cell_ctx->cfg_cell_param.sib_type_1_Info.
                                scheduling_info_list.scheduling_info + (old_schdl_counter + 1);

                            p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << 
                                        p_old_l_elem->sib_mapping_info.sib_type[old_SIB]));
                        }
                        /* SPR 5611 End */
                        old_SIB++;
                        new_SIB++;
                    }
                    else
                    {
                        /* New Added SIBs can only be SIB10, SIB11 or SIB12 */
                        /* SPR 5611 Start */
                        if (((RRM_RRC_SIB_TYPE_10 == 
                                        p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) &&
                                    (RRM_RRC_SIB_TYPE_11 == 
                                     p_old_l_elem->sib_mapping_info.sib_type[old_SIB])) ||
                                ((RRM_RRC_SIB_TYPE_11 == 
                                  p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) &&
                                 (RRM_RRC_SIB_TYPE_10 == 
                                  p_old_l_elem->sib_mapping_info.sib_type[old_SIB])))
                            /* SPR 5611 End */
                        {
                            warning_related_mask |= (U16)(8 << p_new_l_elem->
                                    sib_mapping_info.sib_type[new_SIB]);
                            p_cell_ctx->pws_sibs_bitmask |= warning_related_mask;
                            p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << 
                                        p_old_l_elem->sib_mapping_info.sib_type[old_SIB]));
                            prim_sec_order_changed = RRC_TRUE;
                            /* SPR 5611 Start */
                            non_pws_rel_sibs_present = RRC_TRUE;
                            /* SPR 5611 End */
                            new_SIB++;
                            old_SIB++;
                        }
                        /* SPR 5611 Start */
                        else if ((RRM_RRC_SIB_TYPE_10 == 
                                    p_old_l_elem->sib_mapping_info.sib_type[old_SIB]) ||
                                (RRM_RRC_SIB_TYPE_11 == 
                                 p_old_l_elem->sib_mapping_info.sib_type[old_SIB]))
                        {
                            if ((p_cell_ctx->pws_sibs_bitmask != warning_related_mask) &&
                                    (RRC_FALSE == prim_sec_order_changed))
                            {
                                p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << 
                                            p_old_l_elem->sib_mapping_info.sib_type[old_SIB]));
                            }
                            prim_sec_etws_changed = RRC_TRUE;
                            old_SIB++;
                            if ((1 == p_new_l_elem->sib_mapping_info.count) &&
                                    (1 == p_old_l_elem->sib_mapping_info.count))
                            {
                                new_SIB++;
                            }
                        }
                        else if ((RRM_RRC_SIB_TYPE_10 == 
                                    p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) ||
                                (RRM_RRC_SIB_TYPE_11 == 
                                 p_new_l_elem->sib_mapping_info.sib_type[new_SIB]) ||
                                (RRM_RRC_SIB_TYPE_12 == 
                                 p_new_l_elem->sib_mapping_info.sib_type[new_SIB]))
                            /* SPR 5611 End */
                        {
                            warning_related_mask |= (U16)(8 << p_new_l_elem->
                                    sib_mapping_info.sib_type[new_SIB]);
                            p_cell_ctx->pws_sibs_bitmask |= warning_related_mask;
                            new_SIB++;
                            /* SPR 5611 Start */
                            if ((1 == p_new_l_elem->sib_mapping_info.count) &&
                                    (1 == p_old_l_elem->sib_mapping_info.count))
                            {
                                old_SIB++;
                            }
                            /* SPR 5611 End */
                        }
                        else
                        {
                            return SI_MAPPING_CHANGED;
                        }
                    }
                }
                if(new_SIB < MAX_SIB_MESSAGE_1)
                {
                    mapped_sibs_mask |= (U16)(8 << p_new_l_elem->
                            sib_mapping_info.sib_type[new_SIB]);
                }
            }
            if ( RRC_TRUE == non_pws_rel_sibs_present)
            {
                old_schdl_counter++;
                new_schdl_counter++;
                /* SPR 5611 Start */
                non_pws_rel_sibs_present = RRC_FALSE;     
                prim_sec_etws_changed = RRC_FALSE;
            }
            /* If SI List containing SIB 10, SIB 11 is replaced with SIB 10 or SIB11 */
            else if ( RRC_TRUE == prim_sec_etws_changed )
            {
                old_schdl_counter++;
            }
            /* SPR 5611 End */
            else /*If SI containing SIB10, SIB11 or SIB12 is added in the beg or in between */
            {
                new_schdl_counter++;        
            }
        }
    }
    /* Storing the bitmask(Temp) for all the SIBs in SI Mappings */
    p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask =
        mapped_sibs_mask;
    if (warning_related_mask != (p_cell_ctx->pws_sibs_bitmask))
    {
        RRC_CSC_UT_TRACE_EXIT(); 
        return SI_MAPPING_CHANGED;      
    }

    RRC_CSC_UT_TRACE_EXIT(); 
    return SI_MAPPING_NOT_CHANGED;      
}

/*******************************************************************************
 *   FUNCTION NAME: csc_get_new_encoded_sibs_info
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *   OUTPUT       : none               
 *   DESCRIPTION:    This function do the following functionality
 *   FOR Each SI MAPPING
 *   1) Checks whether there are any SIBs mapped in SI Mapping as sent by RRM 
 *      in Cell Reconfig
 *   2) If found then 
 *      -- Find the list of all the SIBs info present in this SI MApping
 *
 *   RETURNS:
 *       None
 ******************************************************************************/
static void csc_get_new_encoded_sibs_info(
        csc_cell_ctx_t  *p_cell_ctx)
{
    new_scheduling_info_list_t   *p_new_schdl_info = PNULL;

    U8      si_schdl_count     = 0;
    U16     sib_mapping_count  = 0;
    U8      new_si_count       = 0;
/* Cov_122413_Fix_Start */

    RRC_CSC_UT_TRACE_ENTER(); 
    RRC_ASSERT(PNULL != p_cell_ctx);

    p_new_schdl_info = &p_cell_ctx->p_temp_si_schdl_info->new_schdl_info;

    for (si_schdl_count = 0; si_schdl_count < p_cell_ctx->si_schdl_info.
            schdl_count;si_schdl_count++)
    {
        /* Fill the SI INDEX */
        p_new_schdl_info->scheduling_info[new_si_count].si_index = 
            (U8)(si_schdl_count + 1);

        /* Fill the Periodicity */  
        p_new_schdl_info->scheduling_info[new_si_count].si_periodicity = 
            p_cell_ctx->cfg_cell_param.sib_type_1_Info.scheduling_info_list.
            scheduling_info[si_schdl_count].si_periodicity;  

        /* Copy the Sibs Info in local variable */  
        p_new_schdl_info->scheduling_info[new_si_count].sib_mapping_info.count = 
            p_cell_ctx->cfg_cell_param.sib_type_1_Info.scheduling_info_list.
            scheduling_info[si_schdl_count].sib_mapping_info.count;

        for (sib_mapping_count= 0; sib_mapping_count < p_new_schdl_info->
                scheduling_info[new_si_count].sib_mapping_info.count; 
                sib_mapping_count++)
        {
            p_new_schdl_info->scheduling_info[new_si_count].sib_mapping_info.
                sib_type[sib_mapping_count]= 
                p_cell_ctx->cfg_cell_param.sib_type_1_Info.scheduling_info_list.
                scheduling_info[si_schdl_count].sib_mapping_info.
                sib_type[sib_mapping_count];
        }
        new_si_count++;
    }
    /* Update the new schduling info count */
    p_new_schdl_info->count = new_si_count;     
    /* eMTC changes stop */
/* Cov_122413_Fix_End */

    RRC_CSC_UT_TRACE_EXIT(); 
}

/*******************************************************************************
 *   FUNCTION NAME: csc_get_new_encoded_sibs_info_recfg
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *                  sib_type_1_Info_t  *p_sib_type_1_info
 *   OUTPUT       : none               
 *   DESCRIPTION:    This function do the following functionality
 *   FOR Each SI MAPPING
 *   1) Checks whether there are any SIBs mapped in SI Mapping as sent by RRM 
 *      in Cell Reconfig
 *   2) If found then 
 *      -- Find the list of all the SIBs info present in this SI MApping
 *
 *   RETURNS:
 *       None
 ******************************************************************************/
static void csc_get_new_encoded_sibs_info_recfg(
        csc_cell_ctx_t  *p_cell_ctx,
        sib_type_1_Info_t  *p_sib_type_1_info)
{
    new_scheduling_info_list_t   *p_new_schdl_info = PNULL;

    U8      si_schdl_count     = 0;
    U16     sib_mapping_count  = 0;
    U8      new_si_count       = 0;


    RRC_CSC_UT_TRACE_ENTER(); 
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_sib_type_1_info);

    p_new_schdl_info = &p_cell_ctx->p_temp_si_schdl_info->new_schdl_info;

    for (si_schdl_count = 0; si_schdl_count < p_sib_type_1_info->
            scheduling_info_list.count;si_schdl_count++)
    {
        /* Fill the SI INDEX */
        p_new_schdl_info->scheduling_info[new_si_count].si_index = 
            (U8)(si_schdl_count + 1);

        /* Fill the Periodicity */  
        p_new_schdl_info->scheduling_info[new_si_count].si_periodicity = 
            p_sib_type_1_info->scheduling_info_list.
            scheduling_info[si_schdl_count].si_periodicity;  

        /* Copy the Sibs Info in local variable */  
        p_new_schdl_info->scheduling_info[new_si_count].sib_mapping_info.count = 
            p_sib_type_1_info->scheduling_info_list.
            scheduling_info[si_schdl_count].sib_mapping_info.count;

        for (sib_mapping_count= 0; sib_mapping_count < p_new_schdl_info->
                scheduling_info[new_si_count].sib_mapping_info.count; 
                sib_mapping_count++)
        {
            p_new_schdl_info->scheduling_info[new_si_count].sib_mapping_info.
                sib_type[sib_mapping_count]= 
                p_sib_type_1_info->scheduling_info_list.
                scheduling_info[si_schdl_count].sib_mapping_info.
                sib_type[sib_mapping_count];
        }
        new_si_count++;
    }
    /* Update the new schduling info count */
    p_new_schdl_info->count = new_si_count;     
    /* eMTC changes stop */

    RRC_CSC_UT_TRACE_EXIT(); 
}

/*******************************************************************************
 *   FUNCTION NAME: csc_get_mod_period
 *   INPUT        : sib_type_2_Info_t *p_sib_type_2_info
 *   OUTPUT       : none
 *   DESCRIPTION  : This function will calculate the Modification period
 *    -- First it will calculate the value from enums then calculate Mod period
 *    using
 *   -- Mod_period = Mod_period_coefficient * Default paging cyle
 *       STATE: CELL_M_STATE_CELL_IN_SERVICE;
 *       EVENT: CELL_M_EVENT_RRM_RECONFIG_CELL_REQ
 *
 *   RETURNS:
 *       Modification Period (U16)
 *
 ******************************************************************************/
U32 csc_get_mod_period(
        sib_type_2_Info_t *p_sib_type_2_info)
{

    U8      mod_period_coeff = 0;
    U16     default_paging_cycle = 0;
    U32 mod_period = RRC_NULL;
    RRC_CSC_UT_TRACE_ENTER();

    switch (p_sib_type_2_info->radio_resource_config_common_sib.bcch_config.
            mod_period_coeff)
    {
        case RRM_RRC_MOD_PERIOD_COEFF_N2:
            mod_period_coeff = 2;
            break;
        case RRM_RRC_MOD_PERIOD_COEFF_N4:
            mod_period_coeff = 4;
            break;
        case RRM_RRC_MOD_PERIOD_COEFF_N8:
            mod_period_coeff = 8;
            break;
        case RRM_RRC_MOD_PERIOD_COEFF_N16:
            mod_period_coeff = 16;
            break;
        default:
            RRC_CSC_TRACE(RRC_BRIEF,
                    "Received Invalid Mod period Coefficient from BCCH Config");  
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
    }

    switch(p_sib_type_2_info->radio_resource_config_common_sib.pcch_config.
            default_paging_cycle)
    {
        case RRM_RRC_DEF_PAG_CYCLE_RF32:
            default_paging_cycle = 32;
            break;
        case RRM_RRC_DEF_PAG_CYCLE_RF64:
            default_paging_cycle = 64;
            break;
        case RRM_RRC_DEF_PAG_CYCLE_RF128:
            default_paging_cycle = 128;
            break;
        case RRM_RRC_DEF_PAG_CYCLE_RF256:
            default_paging_cycle = 256;
            break;
        default:
            RRC_CSC_TRACE(RRC_BRIEF,
                    "Received Invalid Default Paging cycle from PCCH Config");  
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
    }
    /* Store modification period into cell context */
    mod_period = (U32)(mod_period_coeff * default_paging_cycle);

    RRC_CSC_UT_TRACE_EXIT();
    return mod_period;
}
/******************************************************************************
 *   FUNCTION NAME: csc_get_sfn_gap
 *   INPUT        : sib_type_2_Info_t *p_sib_type_2_info
 *   OUTPUT       : none
 *   DESCRIPTION  : This function will calculate the SFN GAP
 *       -- USING (T div N) (where T= Default paging cycle)
 *           -- N = min(T,nb) ;
 *               --nb = value of nb bits (generate from enum)
 *       STATE: CELL_M_STATE_CELL_IN_SERVICE;
 *       EVENT: CELL_M_EVENT_RRM_RECONFIG_CELL_REQ
 *
 *   RETURNS:
 *       sfn Gap (U8)
 *
 ****************************************************************************/
static U8 csc_get_sfn_gap(
        sib_type_2_Info_t *p_sib_type_2_info)
{
    U16   nb = 0;
    U16   nb_value = 0;
    U16   default_paging_cycle = 0;
    U16   N = 0;
    U8    sfn_gap = 0;

    RRC_CSC_UT_TRACE_ENTER();

    switch(p_sib_type_2_info->radio_resource_config_common_sib.
            pcch_config.default_paging_cycle)
    {
        case RRM_RRC_DEF_PAG_CYCLE_RF32:
            default_paging_cycle = 32;
            break;
        case RRM_RRC_DEF_PAG_CYCLE_RF64:
            default_paging_cycle = 64;
            break;
        case RRM_RRC_DEF_PAG_CYCLE_RF128:
            default_paging_cycle = 128;
            break;
        case RRM_RRC_DEF_PAG_CYCLE_RF256:
            default_paging_cycle = 256;
            break;
        default:
            RRC_CSC_TRACE(RRC_BRIEF,
                    "Received Invalid Default Paging cycle from PCCH Config ");  
            RRC_CSC_UT_TRACE_EXIT();
            break;
    }
    /* Check the change in  modification period */
    nb = p_sib_type_2_info->radio_resource_config_common_sib.pcch_config.nb;

    switch(nb)
    {
        case RRM_RRC_NB_FOUR_T:
            nb_value = (U16)(default_paging_cycle * 4);
            break;
        case RRM_RRC_NB_TWO_T:
            nb_value = (U16)(default_paging_cycle * 2);
            break;
        case RRM_RRC_NB_ONE_T:
            nb_value = default_paging_cycle ;
            break;
        case RRM_RRC_NB_HALF_T:
            nb_value = default_paging_cycle/2;
            break;
        case RRM_RRC_NB_QUARTER_T:
            nb_value = default_paging_cycle/4;
            break;
        case RRM_RRC_NB_ONE_EIGHTH_T:
            nb_value = default_paging_cycle/8;
            break;
        case RRM_RRC_NB_ONE_SIXTEENTH_T:
            nb_value = default_paging_cycle/16;
            break;
        case RRM_RRC_NB_ONE_THIRTY_SECOND_T:
            nb_value = default_paging_cycle/32;
            break;
        default:
            RRC_CSC_TRACE(RRC_BRIEF," Received Invalid nb from PCCH Config ");  
            break;
    }

    if(default_paging_cycle > nb_value)
        N = nb_value;
    else
        N = default_paging_cycle;

    if (RRC_NULL != N)
        sfn_gap = (U8)(default_paging_cycle / N);
    RRC_CSC_UT_TRACE_EXIT();
    return sfn_gap;
}


/******************************************************************************
 *   FUNCTION NAME: csc_send_cell_reconfig_msg
 *   INPUT        : csc_gl_ctx_t    *p_csc_gl_ctx
 *                  csc_cell_ctx_t  *p_cell_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function send the  Cell Reconfig API  to LLIM when SI
 *   Modification Cell Reconfig   counter expired 
 *       state CELL_M_STATE_CELL_IN_SERVICE;
 *       event : CELL RECONFIG COUNTER EXPIRED
 *
 *   RETURNS:
 *       None
 *
 *****************************************************************************/
void csc_send_cell_reconfig_msg(
        csc_gl_ctx_t    *p_csc_gl_ctx, /* Pointer to the CSC global context data */
        csc_cell_ctx_t  *p_cell_ctx)   /* cell context */
{
    void *p_intrl_msg = PNULL;
    csc_cell_m_timer_buf_t guard_timer_data;
    /* SPR 14352 Fix Start */
    rrc_csc_llim_reconfig_cell_req_t *p_llim_recfg_cell_req = PNULL;
    /* SPR 14352 Fix Stop */

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    /* Timer expired and is not valid any more */
    RRC_CSC_TRACE(RRC_BRIEF,
            "SI Mod Cell Reconfig Counter expired; Sending CELL RECONFIG REQ");
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
    guard_timer_data.auto_delete        = RRC_TRUE;
    guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
    guard_timer_data.cell_index         = p_cell_ctx->cell_index;
    guard_timer_data.cell_m_event       =
        CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED;
    RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_RECONFIG_TIMER] %u ms.",
            p_csc_gl_ctx->cell_recfg_duration);

    p_cell_ctx->guard_timer = rrc_start_timer(
            p_csc_gl_ctx->cell_recfg_duration,
            &guard_timer_data,
            sizeof (guard_timer_data),
            RRC_FALSE);

    /*  Send Cell Reconfig API due to SI Modification to LLIM */
    /* Fetch the csc_llim_cell_reconfig API */
    p_intrl_msg = p_cell_ctx->cell_reconfig_api;
    if(PNULL == p_intrl_msg)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return;
    }

    if(PNULL != p_intrl_msg)
    {
        /* SPR 14352 Fix Start */
        p_llim_recfg_cell_req = (rrc_csc_llim_reconfig_cell_req_t *)
            ((U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

        if (p_cell_ctx->mod_period_boud_counter <= CSC_SFN_OFFSET)
        {
            p_llim_recfg_cell_req->sfn += p_cell_ctx->mod_period;
        }
        /* eMTC changes stop */

        /* SPR 14352 Fix Stop */

        rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
        
        p_cell_ctx->cell_reconfig_api = PNULL;
        /* STATE TRANSITION */
        CELL_M_FSM_SET_STATE(p_cell_ctx,
                CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP);
    }
    else
    {
        RRC_CSC_TRACE(RRC_WARNING," Cell Reconfig API not found in cell context ");
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_out_of_service_cell_rcfg_fill_llim_recfg_req
 *
 *       This function fills llim_cell_recfg message in state
 *       cell out of service.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
static rrc_return_et cell_out_of_service_cell_rcfg_fill_llim_recfg_req(
        rrc_csc_llim_reconfig_cell_req_t *p_llim_recfg_cell_req,
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
    	rrc_rrm_cell_reconfig_req_t *p_reconfig_cell_req,
	void            *p_intrl_msg
)
{
    U32             new_mod_period = RRC_NULL;
    U8              new_sfn_gap    = RRC_NULL; 
    U8              si_mapping     = RRC_NULL; 
    U8              si_index     = RRC_NULL; 


    RRC_CSC_UT_TRACE_ENTER();
    if ( CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG & 
	    p_cell_ctx->recfg_cell_param.presence_bitmask)
    {
	p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
	    CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG;
	p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask =
	    p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask;
	if ( CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG &
		p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
	{
	    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mib_info =
		p_cell_ctx->recfg_cell_param.broadcast_info.mib_info;
	}
	if ( CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG &
		p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
	{
	    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_1_Info =
		p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_1_Info;
	}
	if ( CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG &
		p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
	{
	    p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_2_Info =
		p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info;
	}
    }
    if ( CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG & 
	    p_cell_ctx->recfg_cell_param.presence_bitmask)
    {
	p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= 
	    CELL_RECONFIG_REQ_NON_BROADCAST_INFO_PRESENCE_FLAG;
	p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info =
	    p_cell_ctx->recfg_cell_param.non_broadcast_info;
    }
    /*SPR_19066_START*/
    /*Code Removed*/
    /*SPR_19066_END*/
    
    if ( CELL_RECONFIG_REQ_API_CONTENTION_FREE_RACH_TIMER_PRESENT & 
	    p_cell_ctx->recfg_cell_param.presence_bitmask)
    {
	p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= 
	    CELL_RECONFIG_REQ_CONTENTION_FREE_RACH_TIMER_PRESENCE_FLAG;
	p_llim_recfg_cell_req->cell_reconfig.contention_free_rach_timer =
	    p_cell_ctx->recfg_cell_param.contention_free_rach_timer;
    }

    /*Storing the reconfig Mode(First time with NO_MODE,else with PHY_ONLY
      or MAC_ONLY*/
    p_llim_recfg_cell_req->mode = NO_MODE;


    new_mod_period = csc_get_mod_period(
	    &p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info);

    if (p_reconfig_cell_req->presence_bitmask &
	    CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG)
    {
	/* Check whether SIB_2 is present in original Reconfig bitmask */
	if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask &
		CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG)
	{
	    if (new_mod_period != p_cell_ctx->mod_period)
	    {
		/*Setting the bitmask for Mod Period*/
		p_llim_recfg_cell_req->bitmask |= RRC_CSC_MOD_PERIOD_PRESENT;
		p_llim_recfg_cell_req->mod_period = new_mod_period;
	    }
	    new_sfn_gap = csc_get_sfn_gap(
		    &p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info);

	    if (new_sfn_gap != p_cell_ctx->sfn_gap)
	    {
		/*Setting the bitmask for Mod Period*/
		p_llim_recfg_cell_req->bitmask |= RRC_CSC_SFN_GAP_PRESENT;
		p_llim_recfg_cell_req->sfn_gap = new_sfn_gap;
	    }

	}
    }

    if (p_reconfig_cell_req->presence_bitmask &
	    CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG)
    {
	p_llim_recfg_cell_req->sfn = RRC_NULL; 
    }

    /* ALERT : NEED TO CHECK whether SI MAPPING CHANGED OR NOT THEN FILL THE
     * ASN ENCODED FORM FOR MIB,SIB AND SIs
     * Check and fill MIB, SIB1 and SIs info
     * Check for the change in SI Mapping */

    si_mapping = csc_check_si_mapping(p_cell_ctx);
    if (SI_MAPPING_CHANGED == si_mapping)
    {
	RRC_CSC_TRACE(RRC_BRIEF,"SI MAPPING CHANGED :: STATE : OUT OF SERVICE ");
	/* Set the flag in cell context */
	p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = SI_MAPPING_CHANGED;
	if(p_reconfig_cell_req->presence_bitmask & CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG)
	{
	    if(!(p_reconfig_cell_req->broadcast_info.presence_bitmask & CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG) &&
		    (CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG & p_cell_ctx->cfg_cell_param.presence_bitmask))
	    {
		p_cell_ctx->sib_modify_source = RRM_TRIGGERED_SIB8_MODIFY;
		p_cell_ctx->sib8_reconfig_sched_info_present = RRC_TRUE;
	    }
	    else
	    {
		p_cell_ctx->sib8_reconfig_sched_info_present = RRC_FALSE;
	    }
	}
    }
    else if (SI_MAPPING_NOT_CHANGED == si_mapping)
    {
	/* SI_MAPPING NOT CHANGED */
	RRC_CSC_TRACE(RRC_BRIEF,"SI MAPPING NOT CHANGED ");

	/* Set the flag in cell context */
	p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = 
	    SI_MAPPING_NOT_CHANGED;

	for (si_index = 0; si_index < MAX_SCHDLING_COUNT; si_index++)
	{
	    p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask |=
		p_cell_ctx->p_temp_si_schdl_info->si_bitmask[si_index];
	}

	if (p_reconfig_cell_req->broadcast_info.presence_bitmask &
		CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG)
	{
	    csc_get_new_encoded_sibs_info_recfg(p_cell_ctx,
		    &(p_reconfig_cell_req->broadcast_info.sib_type_1_Info));
	}
	else
	{
	    csc_get_new_encoded_sibs_info(p_cell_ctx);
	}

	/* Update the SIBs contents */
	csc_update_broadcast_info(p_cell_ctx);
    }
    else if(INVALID_SI_MAPPING_PWS == si_mapping)
    {
	/* Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
	    rrc_stop_timer(p_cell_ctx->guard_timer);
	    p_cell_ctx->guard_timer = PNULL;
	}
	if (p_intrl_msg)
	{
	    /* CSC didn't send message - free memory */
	    rrc_msg_mem_free(p_intrl_msg);
	    RRC_CSC_TRACE(RRC_WARNING, 
		    "CSC didn't send CSC_LLIM_RECONFIG_CELL_REQ to LLIM - free memory");
	}
	/*PWS is going on and RRM has not sent PWS information 
	 * in Scheduling Info for Broadcasting,
	 * Send failure to RRM*/
	if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
		    p_cell_ctx->transaction_id, 
		    p_cell_ctx->cell_index,
		    RRC_FAILURE, 
		    RRM_RRC_INTERNAL_FAILURE))
	{
	    RRC_CSC_TRACE(RRC_WARNING, 
		    "Unable to send RESP FAILURE TO RRM");
	    RRC_CSC_UT_TRACE_EXIT();
	    return RRC_FAILURE;
	}
	p_cell_ctx->rcfg_status_flag = RRC_FALSE;
	p_cell_ctx->retry_count = RRC_NULL; 
	p_cell_ctx->si_mod_flag = RRC_NULL; 
	rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
	p_cell_ctx->p_temp_si_schdl_info = PNULL;
	RRC_CSC_UT_TRACE_EXIT();
	return RRC_FAILURE;
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_cell_out_of_service_cell_reconfig
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function start cell Reconfiguration.
 *                  STATE: CELL_M_STATE_CELL_OUT_OF_SERVICE; 
 *                   EVENT: CELL_M_EVENT_RRM_RECONFIG_CELL_REQ
 *   RETURNS      : None
 * 
 ******************************************************************************/
static void cell_m_fsm_cell_out_of_service_cell_reconfig(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    rrc_rrm_cell_reconfig_req_t *p_reconfig_cell_req  =
        (rrc_rrm_cell_reconfig_req_t *)p_msg;

    s1ap_cell_area_info_update_ind_t *p_s1ap_cell_area_info_update_ind = PNULL; 

    csc_cell_m_timer_buf_t guard_timer_data;
    void            *p_intrl_msg        = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_reconfig_cell_req);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(p_reconfig_cell_req->cell_index == p_cell_ctx->cell_index);

    if (p_cell_ctx->rcfg_status_flag == RRC_FALSE)
    {
        p_cell_ctx->rcfg_status_flag = RRC_TRUE;
    }
    else
    {
        RRC_CSC_TRACE(RRC_WARNING, "Cell Reconfiguration Already Ongoing.");
        /*  Send RRC_RRM_CELL_RECONFIG_RESP */
        csc_send_rrm_cell_reconfig_resp_failure( 
                rrc_get_transaction_id(p_api),
                p_cell_ctx->cell_index,
                RRC_FAILURE, RRM_RRC_INTERACTION_WITH_OTHER_PROCEDURE_FAILURE);
        RRC_CSC_UT_TRACE_EXIT();
        return;        
    }

    p_cell_ctx->sib8_periodicity = 0;

    if(p_reconfig_cell_req->presence_bitmask & CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG)
    {
        if(p_reconfig_cell_req->broadcast_info.presence_bitmask & CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
        {
            p_cell_ctx->sib_modify_source = RRM_TRIGGERED_SIB8_MODIFY;
        }
        else
        {
            p_cell_ctx->sib_modify_source = RRM_TRIGGERED_SIB8_NO_MODIFY;
        }
    }

    p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);

    if (PNULL != p_cell_ctx->p_temp_si_schdl_info)
    {
        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;
    }

    /*Check if only Emergency ID has come*/
    if (p_reconfig_cell_req->presence_bitmask ==
            CELL_RECONFIG_REQ_API_EMERGENCY_AREA_ID_PRESENCE_FLAG)
    {
        /*Send only cell area info update ind to S1AP*/
        p_intrl_msg = csc_alloc_intrl_msg( RRC_S1AP_MODULE_ID,
                S1AP_CELL_AREA_INFO_UPDATE_IND,
                sizeof (s1ap_cell_area_info_update_ind_t));
        if (PNULL != p_intrl_msg)
        {
            /* Fill and send message */
            p_s1ap_cell_area_info_update_ind = 
                (s1ap_cell_area_info_update_ind_t *)
                ((U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

            p_s1ap_cell_area_info_update_ind->cell_area_info.bitmask |=
                S1AP_CELL_INFO_UPDATE_IND_EMERGENCY_ID_PRESENT;
            p_s1ap_cell_area_info_update_ind->cell_index = p_cell_ctx->cell_index;
            l3_memcpy_wrapper(p_s1ap_cell_area_info_update_ind->cell_area_info.
                    emergency_id,
                    p_cell_ctx->recfg_cell_param.
                    emergency_area_id,EMERGENCY_AREA_SIZE);
            
            rrc_send_message(p_intrl_msg,RRC_S1AP_MODULE_ID);
            RRC_CSC_TRACE(RRC_INFO,
                    "S1AP_CELL_AREA_INFO_UPDATE_IND sent to S1AP");
            return;

        }
    }

    /*Allocating memory for Temp SI Scheduling Info*/
    p_cell_ctx->p_temp_si_schdl_info = rrc_mem_get(sizeof (si_schdl_info_t));

    if( PNULL == p_cell_ctx->p_temp_si_schdl_info )
    {
        RRC_CSC_TRACE(RRC_FATAL, "Unable to allocate Memeory");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    memset_wrapper(p_cell_ctx->p_temp_si_schdl_info, 0, sizeof (si_schdl_info_t));
    /* 1.Get Transaction ID and store in cell context*/
    p_cell_ctx->transaction_id = rrc_get_transaction_id(p_api);

    /* 2.Storing the Values of Cell Reconfiguration(ongoing) in csc cell context*/
    p_cell_ctx->recfg_cell_param = *p_reconfig_cell_req;

    guard_timer_data.auto_delete        = RRC_TRUE;
    guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
    guard_timer_data.cell_index         = p_cell_ctx->cell_index;
    guard_timer_data.cell_m_event       =
        CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED;

    RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_RECONFIG_TIMER] %u ms.",
            p_csc_gl_ctx->cell_recfg_duration);

    if(PNULL != p_cell_ctx->guard_timer)
    {
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    p_cell_ctx->guard_timer = rrc_start_timer(
            p_csc_gl_ctx->cell_recfg_duration,
            &guard_timer_data, 
            sizeof (guard_timer_data), 
            RRC_FALSE);


    /* 4.Initialise the RETRY COUNT to 0*/
    p_cell_ctx->retry_count = 0;

    /* 5. Fill and send CSC_LLIM_RECONFIG_CELL_REQ to LLIM*/
    p_intrl_msg = csc_alloc_intrl_msg( 
            RRC_LLIM_MODULE_ID,
            CSC_LLIM_RECONFIG_CELL_REQ,
            sizeof(rrc_csc_llim_reconfig_cell_req_t));

    if (PNULL != p_intrl_msg)
    {
        /* Fill and send message */
        rrc_csc_llim_reconfig_cell_req_t *p_llim_recfg_cell_req =
            (rrc_csc_llim_reconfig_cell_req_t *)((U8 *)p_intrl_msg +
                    RRC_API_HEADER_SIZE);

        /*Storing Transaction Id and Cell index in 
         *           rrc_csc_llim_reconfig_cell_req_t*/
        p_llim_recfg_cell_req->transaction_id = p_cell_ctx->transaction_id;

        /*Cell Index*/
        p_llim_recfg_cell_req->cell_index = p_cell_ctx->cell_index;

        /*Storing the cell Reconfig(ONGOING) parameter in 
         *              "rrc_csc_llim_reconfig_cell_req_t"*/
        p_llim_recfg_cell_req->cell_reconfig.presence_bitmask = 0;

	if (RRC_FAILURE == (cell_out_of_service_cell_rcfg_fill_llim_recfg_req
		(p_llim_recfg_cell_req, p_cell_ctx, p_reconfig_cell_req, p_intrl_msg)))
		return;

        /* SFN will be sent as zero as cell is OUT OF SERVICE */
        p_llim_recfg_cell_req->sfn = RRC_NULL;

        if (RRC_SUCCESS == bch_m_mib_sib1_sis_recfg(
                    p_llim_recfg_cell_req,
                    &p_cell_ctx->recfg_cell_param,
                    p_cell_ctx))
        {
            /* No need to wait for the modification period 
             * as the cell is in out of service state.*/
            if (PNULL != p_intrl_msg)
            {
                rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
                /* STATE TRANSITION */
                CELL_M_FSM_SET_STATE(p_cell_ctx,
                        CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP);
            }
        }
        else
        {
            /* Bad incoming info */
            RRC_CSC_TRACE(RRC_ERROR,
                    "Bad message - UNABLE TO ENCODE SI/SIB AND MIBs.");

            if (p_intrl_msg)
            {
                /* CSC didn't send message - free memory */
                rrc_msg_mem_free(p_intrl_msg);
            }

            /* Resetting the si_mod_flag to NULL*/
            p_cell_ctx->si_mod_flag = RRC_NULL;

            /* Stop guard timer */
            if (PNULL != p_cell_ctx->guard_timer)
            {
                rrc_stop_timer(p_cell_ctx->guard_timer);
                p_cell_ctx->guard_timer = PNULL;
            }

            /*  Send RRC_RRM_CELL_RECONFIG_RESP */
            if( RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE, RRM_RRC_INTERNAL_FAILURE))
            {
                RRC_CSC_TRACE(RRC_WARNING, 
                        "Unable to send RESP FAILURE TO RRM");
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }

            p_cell_ctx->rcfg_status_flag = RRC_FALSE;

            /* STATE TRANSITION */
            CELL_M_FSM_SET_STATE(p_cell_ctx,
                    CELL_M_STATE_CELL_OUT_OF_SERVICE);
        }
    }
    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: update_cell_info_si_mapping_unchanged 
 *  
 *  this function updates csc context with cell info if SI mapping not changed
 *  
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
rrc_return_et update_cell_info_si_mapping_unchanged(
        csc_cell_ctx_t  *p_cell_ctx,
	rrc_csc_llim_reconfig_cell_req_t *p_llim_recfg_cell_req,
        rrc_rrm_cell_reconfig_req_t *p_reconfig_cell_req,
        void            *p_intrl_msg 
        )
{
    U8              si_index     = RRC_NULL;
    rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list = PNULL;
    /* SI_MAPPING NOT CHANGED */
    RRC_CSC_TRACE(RRC_BRIEF,"SI MAPPING NOT CHANGED ");

    /* Set the flag in cell context */
    p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = 
        SI_MAPPING_NOT_CHANGED;
    for (si_index = 0; si_index < MAX_SCHDLING_COUNT; si_index++)
    {
        p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask |=
            p_cell_ctx->p_temp_si_schdl_info->si_bitmask[si_index];
    }
    /* eMTC changes stop */

    if (p_reconfig_cell_req->broadcast_info.presence_bitmask &
            CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG)
    {
        csc_get_new_encoded_sibs_info_recfg(p_cell_ctx,
                &(p_reconfig_cell_req->broadcast_info.sib_type_1_Info));
    }
    else
    {
        csc_get_new_encoded_sibs_info(p_cell_ctx);
    }

    /* Update the SIBs contents */
    csc_update_broadcast_info(p_cell_ctx);

    if ((p_reconfig_cell_req->presence_bitmask & 
                CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG))
    {
        if (RRC_SUCCESS == bch_m_mib_sib1_sis_recfg(
                    p_llim_recfg_cell_req,
                    &p_cell_ctx->recfg_cell_param,
                    p_cell_ctx))
        {
            RRC_CSC_TRACE(RRC_INFO,"Cell Reconfiguration Received at "\
                    "SFN : [%d]", p_cell_ctx->sfn);

				p_llim_recfg_cell_req->sfn = (rrc_sfn_t)((p_cell_ctx->sfn +
							p_cell_ctx->mod_period_boud_counter + p_cell_ctx->mod_period)
						% SFN_UPPER_LIMIT);
            /*Store in cell context*/
            p_cell_ctx->cell_reconfig_api = p_intrl_msg;
        }
        else
        {
            /* Bad incoming info */
            RRC_CSC_TRACE(RRC_ERROR, 
                    "Bad message - UNABLE TO ENCODE SI/SIB AND MIBs.");
            if (p_intrl_msg)
            {
                /* CSC didn't send message - free memory */
                rrc_msg_mem_free(p_intrl_msg);
            }

            /* Stop guard timer */
            if(PNULL != p_cell_ctx->guard_timer)
            {
                rrc_stop_timer(p_cell_ctx->guard_timer);
                p_cell_ctx->guard_timer = PNULL;
            }
            /* . Send RRC_RRM_CELL_SETUP_RESP */
            if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
                        p_cell_ctx->transaction_id, 
                        p_cell_ctx->cell_index,
                        RRC_FAILURE, 
                        RRM_RRC_INTERNAL_FAILURE))
            {
                RRC_CSC_TRACE(RRC_WARNING, 
                        "Unable to send RESP FAILURE TO RRM");
            /* SPR 20044 Fix Start */
                /* Code Removed */
            }
            p_cell_ctx->rcfg_status_flag = RRC_FALSE;
            return RRC_FAILURE;
            /* SPR 20044 Fix Stop */
        }
    }
    else
    {
        if (!p_cell_ctx->si_mod_flag)
        /* eMTC changes stop */
        {
            if ((p_reconfig_cell_req->presence_bitmask & 
                        CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG) &&
                    (p_reconfig_cell_req->non_broadcast_info.presence_bitmask &
                     CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG) &&
                    (p_reconfig_cell_req->non_broadcast_info.mac_reconfig_params.bitmask &
                     RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT) )
            {
                p_rrc_si_transmission_info_list =
                    &p_reconfig_cell_req->non_broadcast_info.
                    mac_reconfig_params.si_transmission_info;
                if (RRC_SUCCESS ==
                        bch_m_update_si_transmission_info_sis_recfg(
                            p_llim_recfg_cell_req,
                            &p_cell_ctx->cfg_cell_param,
                            p_cell_ctx,
                            p_rrc_si_transmission_info_list))
                {
                    RRC_CSC_TRACE(RRC_INFO,"Cell Reconfiguration Received at "\
                            "SFN : [%d]", p_cell_ctx->sfn);

                    p_llim_recfg_cell_req->sfn = (rrc_sfn_t)((p_cell_ctx->sfn +
                                p_cell_ctx->mod_period_boud_counter + p_cell_ctx->mod_period)
                            % SFN_UPPER_LIMIT);
                    /* eMTC changes stop */

                    /*Store in cell context*/
                    p_cell_ctx->cell_reconfig_api = p_intrl_msg;
                    /*Coverity 87523 Fix Start*/
                    if(PNULL != p_intrl_msg)
                    {
                        rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
                    }
                    /*Coverity 87523 Fix End*/
                }
                else
                {
                    /* Bad incoming info */
                    RRC_CSC_TRACE(RRC_ERROR, 
                            "Bad message - UNABLE TO ENCODE SI/SIB AND MIBs.");
                    if (p_intrl_msg)
                    {
                        /* CSC didn't send message - free memory */
                        rrc_msg_mem_free(p_intrl_msg);
                    }

                    /* Stop guard timer */
                    if(PNULL != p_cell_ctx->guard_timer)
                    {
                        rrc_stop_timer(p_cell_ctx->guard_timer);
                        p_cell_ctx->guard_timer = PNULL;
                    }
                    /* . Send RRC_RRM_CELL_SETUP_RESP */
                    if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
                                p_cell_ctx->transaction_id, 
                                p_cell_ctx->cell_index,
                                RRC_FAILURE, 
                                RRM_RRC_INTERNAL_FAILURE))
                    {
                        RRC_CSC_TRACE(RRC_WARNING, 
                                "Unable to send RESP FAILURE TO RRM");
                        return RRC_FAILURE;
                    }
                    p_cell_ctx->rcfg_status_flag = RRC_FALSE;
                }
                /*Coverity 87523 Fix Code Moved*/
            }
        /* SPR 18089:start */
            if (p_reconfig_cell_req->presence_bitmask & 
                       CELL_RECONFIG_REQ_API_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG)
            {
                if (RRC_SUCCESS == bch_m_mib_sib1_sis_recfg(
                            p_llim_recfg_cell_req,
                            &p_cell_ctx->recfg_cell_param,
                            p_cell_ctx))
                {
                    RRC_CSC_TRACE(RRC_INFO,"Cell Reconfiguration Received at "\
                            "SFN : [%d]", p_cell_ctx->sfn);

                    p_llim_recfg_cell_req->sfn = (rrc_sfn_t)((p_cell_ctx->sfn +
                                p_cell_ctx->mod_period_boud_counter + p_cell_ctx->mod_period)
                            % SFN_UPPER_LIMIT);
                    /*Store in cell context*/
                    p_cell_ctx->cell_reconfig_api = p_intrl_msg;
                    p_llim_recfg_cell_req->mode = MAC_ONLY;
                    if(PNULL != p_intrl_msg)
                    {
                        rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
                    }
                }
                else
                {
                    /* Bad incoming info */
                    RRC_CSC_TRACE(RRC_ERROR, 
                            "Bad message - UNABLE TO ENCODE SI/SIB AND MIBs.");
                    if (p_intrl_msg)
                    {
                        /* CSC didn't send message - free memory */
                        rrc_msg_mem_free(p_intrl_msg);
                    }

                    /* Stop guard timer */
                    if(PNULL != p_cell_ctx->guard_timer)
                    {
                        rrc_stop_timer(p_cell_ctx->guard_timer);
                        p_cell_ctx->guard_timer = PNULL;
                    }
                    /* . Send RRC_RRM_CELL_SETUP_RESP */
                    if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
                                p_cell_ctx->transaction_id, 
                                p_cell_ctx->cell_index,
                                RRC_FAILURE, 
                                RRM_RRC_INTERNAL_FAILURE))
                    {
                        RRC_CSC_TRACE(RRC_WARNING, 
                                "Unable to send RESP FAILURE TO RRM");
                        return RRC_FAILURE;
                    }
                    p_cell_ctx->rcfg_status_flag = RRC_FALSE;
                }

            }
            
        /* SPR 18089:end */

            /* STATE TRANSITION */
            CELL_M_FSM_SET_STATE(p_cell_ctx,
                    CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP);
        }
    }
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: update_cell_info_si_mapping_changed 
 *
 *       This function updates cell info in csc context if SI mapping changed
 *       
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
rrc_return_et update_cell_info_si_mapping_changed(
        csc_cell_ctx_t  *p_cell_ctx,
		rrc_csc_llim_reconfig_cell_req_t *p_llim_recfg_cell_req,
        rrc_rrm_cell_reconfig_req_t *p_reconfig_cell_req,
        void            *p_intrl_msg 
        )
{
    if(p_reconfig_cell_req->presence_bitmask & CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG)
    {
        if(!(p_reconfig_cell_req->broadcast_info.presence_bitmask & CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG) &&
                (CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG & p_cell_ctx->cfg_cell_param.presence_bitmask))
        {
            p_cell_ctx->sib_modify_source = RRM_TRIGGERED_SIB8_MODIFY;
            p_cell_ctx->sib8_reconfig_sched_info_present = RRC_TRUE;
        }
        else
        {
            p_cell_ctx->sib8_reconfig_sched_info_present = RRC_FALSE;
        }
    }
    RRC_CSC_TRACE(RRC_INFO,"Cell Reconfiguration Received at SFN : [%d]",
            p_cell_ctx->sfn);

    if (RRC_SUCCESS == bch_m_mib_sib1_sis_recfg(
                p_llim_recfg_cell_req,
                &p_cell_ctx->recfg_cell_param, 
                p_cell_ctx))
    {
        if(!p_cell_ctx->si_mod_flag)
        /* eMTC changes stop */
        {
            rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);

            /* STATE TRANSITION */
            CELL_M_FSM_SET_STATE(p_cell_ctx,
                    CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP);
        }
        else
        {
            p_llim_recfg_cell_req->sfn = (rrc_sfn_t)((p_cell_ctx->sfn +
                        p_cell_ctx->mod_period_boud_counter + p_cell_ctx->mod_period)
                    % SFN_UPPER_LIMIT);
            /* eMTC changes stop */

            /*Store in cell context*/
            p_cell_ctx->cell_reconfig_api = p_intrl_msg;
        }
    }
    else
    {
        /* Bad incoming info */
        RRC_CSC_TRACE(RRC_ERROR, 
                "Bad message - UNABLE TO ENCODE SI/SIB AND MIBs.");

        if (p_intrl_msg)
        {
            /* CSC didn't send message - free memory */
            rrc_msg_mem_free(p_intrl_msg);
        }

        /* Resetting the si_mod_flag to NULL*/
        p_cell_ctx->si_mod_flag = RRC_NULL;
        /* eMTC changes stop */

        /* Stop guard timer */
        if (PNULL != p_cell_ctx->guard_timer)
        {
            rrc_stop_timer(p_cell_ctx->guard_timer);
            p_cell_ctx->guard_timer = PNULL;
        }
        /*  Send RRC_RRM_CELL_RECONFIG_RESP */
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE, RRM_RRC_INTERNAL_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, 
                    "Unable to send RESP FAILURE TO RRM");
            return RRC_FAILURE;
        }
        p_cell_ctx->rcfg_status_flag = RRC_FALSE;
    }
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: update_cell_info_mib_sib 
 *
 *       This function updates mib and sib info info csc context
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void update_cell_info_mib_sib(csc_cell_ctx_t  *p_cell_ctx,
     	rrc_csc_llim_reconfig_cell_req_t *p_llim_recfg_cell_req,
		rrc_rrm_cell_reconfig_req_t *p_reconfig_cell_req,
        csc_gl_ctx_t    *p_csc_gl_ctx
        /*SPR 17777 +-*/
        )
{
    U32             new_mod_period = RRC_NULL;
    U8              new_sfn_gap = RRC_NULL;
    /* SA_FIX Start */
#ifdef LTE_EMBMS_SUPPORTED
    U8              count = 0;
    U8              index;
    m2ap_reserved_mcch_related_bcch_info_t *m2ap_reserved_mcch_related_bcch_info_p = PNULL;
#endif
    /* SA_FIX End */

    RRC_CSC_UT_TRACE_ENTER();
    if ( CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG & 
            p_cell_ctx->recfg_cell_param.presence_bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
            CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG;
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask =
            p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask;
        if ( CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG &
                p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
        {
            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mib_info =
                p_cell_ctx->recfg_cell_param.broadcast_info.mib_info;
        }
        if ( CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG &
                p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
        {
            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_1_Info =
                p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_1_Info;
        }
        if ( CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG &
                p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
        {
            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_2_Info =
                p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info;
#ifdef LTE_EMBMS_SUPPORTED
        /* SIB_2_DISABLE  */
            if((!(p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info.
                presence_bitmask & SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG)) && 
                (p_cell_ctx->cfg_cell_param.sib_type_2_Info.presence_bitmask & SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG))
            {
                 p_llim_recfg_cell_req->cell_reconfig.broadcast_info.is_mbsfn_disable_sib2 = RRC_TRUE;
            }
#endif
        }
#ifdef LTE_EMBMS_SUPPORTED
        if (CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG  &
                p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
        {
            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_13_info =
                p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_13_info;
        /* SPR 18093 Fix Start */
        /* SPR 18093 Fix End */
        }
        /* SPR 18093 Fix Start */
        if (CELL_RECONFIG_REQ_API_MBMS_AREA_INFO_LIST_FLAG &
                p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
        {
            p_llim_recfg_cell_req->cell_reconfig.broadcast_info.
                mbms_updated_area_info_list =
                p_cell_ctx->recfg_cell_param.broadcast_info.mbms_updated_area_info_list;
            /* SA_FIX Start */
            /* check if area to be deleted/created present in reserved list ,then keep
             * this info in p_llim_recfg_cell_req so that delete area not sent
             * to RLC for those areas */
            /* memset */
            memset_wrapper (p_llim_recfg_cell_req->cell_reconfig.broadcast_info.reserved_area_index , 0xff,
                    MAX_MBMS_UPDATED_AREA_LIST);
            for(index = 0; index < p_cell_ctx->recfg_cell_param.broadcast_info.
                    mbms_updated_area_info_list.num_valid_mbsfn_area_info; index++)
            {
                if(DELETED == p_cell_ctx->recfg_cell_param.broadcast_info.
                        mbms_updated_area_info_list.mbms_updated_area_info[index].flag)
                {
                    if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG)
                    {
                        m2ap_reserved_mcch_related_bcch_info_p = get_m2ap_reserved_mcch_related_bcch_info(
                                &(p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list),
                                p_cell_ctx->recfg_cell_param.broadcast_info.
                                mbms_updated_area_info_list.mbms_updated_area_info[index].area_id);
                    }

                    if(PNULL != m2ap_reserved_mcch_related_bcch_info_p)
                    {
                        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.reserved_area_index[count] = 
                            p_cell_ctx->recfg_cell_param.broadcast_info.
                            mbms_updated_area_info_list.mbms_updated_area_info[index].area_index;
                        count++;    

                    }
                }
            }
            /* SA_FIX End */
        }
        /* SPR 18093 Fix End */
#endif
    }
    if ( CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG & 
            p_cell_ctx->recfg_cell_param.presence_bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= 
            CELL_RECONFIG_REQ_NON_BROADCAST_INFO_PRESENCE_FLAG;
        p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info =
            p_cell_ctx->recfg_cell_param.non_broadcast_info;
    }
    /*SPR_19066_START*/
    /*Code Removed*/
    /*SPR_19066_END*/
    /* CLPC Start */

    if (CELL_RECONFIG_DOWNLINK_POWER_CONTROL_COMMON_PRESENT &
            p_cell_ctx->recfg_cell_param.presence_bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
            CELL_RECONFIG_REQ_MAC_DOWNLINK_POWER_CONTROL_PRESENCE_FLAG;

        p_llim_recfg_cell_req->cell_reconfig.downlink_power_control_common =
            p_cell_ctx->recfg_cell_param.downlink_power_control_common;
    }

    /* CLPC Stop */
    if ( CELL_RECONFIG_REQ_API_CONTENTION_FREE_RACH_TIMER_PRESENT & 
            p_cell_ctx->recfg_cell_param.presence_bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= 
            CELL_RECONFIG_REQ_CONTENTION_FREE_RACH_TIMER_PRESENCE_FLAG;
        p_llim_recfg_cell_req->cell_reconfig.contention_free_rach_timer =
            p_cell_ctx->recfg_cell_param.contention_free_rach_timer;
    }
    /*Coverity 94199:Start*/
#ifdef LTE_EMBMS_SUPPORTED
        /* SPR 18672 Fix Start */
    if (CELL_RECONFIG_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG  &
            p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= 
            CELL_RECONFIG_REQ_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG;
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.m2ap_reserved_mcch_related_bcch_info_list
            = p_cell_ctx->recfg_cell_param.broadcast_info.m2ap_reserved_mcch_related_bcch_info_list;
    }
        /* SPR 18672 Fix End */
    /* SPR 18089:start */
    if (CELL_RECONFIG_REQ_API_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG  &
            p_cell_ctx->recfg_cell_param.presence_bitmask)
    /* SPR 18089:end */
    {
        p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= 
            CELL_RECONFIG_REQ_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG;
        p_llim_recfg_cell_req->cell_reconfig.mbms_transmission_power
            = p_cell_ctx->recfg_cell_param.mbms_transmission_power;
    }
    /* SPR 18093 Fix Start */
    if (CELL_RECONFIG_REQ_API_MBMS_AREA_INFO_LIST_FLAG  &
            p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
    {
        p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask |= 
            CELL_RECONFIG_REQ_MCCH_UPDATED_AREA_INFO_LIST;
        p_llim_recfg_cell_req->cell_reconfig.
            broadcast_info.mbms_updated_area_info_list
            = p_cell_ctx->recfg_cell_param.broadcast_info.mbms_updated_area_info_list;
    }
    /* SPR 18093 Fix End */
#endif
    /*Coverity 94199:End*/

    /*Storing the reconfig Mode(First time with NO_MODE,else with PHY_ONLY
     *          * or MAC_ONLY*/
    p_llim_recfg_cell_req->mode = NO_MODE;

    new_mod_period = csc_get_mod_period(
            &p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info);

    if (p_reconfig_cell_req->presence_bitmask &
            CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG)
    {
        /* Check whether SIB_2 is present in original Reconfig bitmask */
        if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask &
                CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG)
        {

            if (new_mod_period != p_cell_ctx->mod_period)
            {
                /*Setting the bitmask for Mod Period*/
                p_llim_recfg_cell_req->bitmask |= RRC_CSC_MOD_PERIOD_PRESENT; 
                p_llim_recfg_cell_req->mod_period = new_mod_period;
            }
            new_sfn_gap = csc_get_sfn_gap(
                    &p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info);

            if (new_sfn_gap != p_cell_ctx->sfn_gap)
            {
                /*Setting the bitmask for Mod Period*/
                p_llim_recfg_cell_req->bitmask |= RRC_CSC_SFN_GAP_PRESENT; 
                p_llim_recfg_cell_req->sfn_gap = new_sfn_gap;
            }

        }
    }
    if (p_reconfig_cell_req->presence_bitmask &
            CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG)
    {
        /* Store the SFN for Non- Broadcast,using SFN = SFN(curr) + OFFSET */
        p_llim_recfg_cell_req->sfn = (rrc_sfn_t)(p_cell_ctx->sfn +
                p_csc_gl_ctx->sfn_offset) ;
    }
    RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: fill_cell_reconfig_info_in_csc_context
 *
 *       This function resumes high priority thread to calculate the base
 *       cdma time
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/

static rrc_return_et fill_cell_reconfig_info_in_csc_context(
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx,
        rrc_rrm_cell_reconfig_req_t *p_reconfig_cell_req,
        void            *p_intrl_msg 
        )
{
    U8              si_mapping     = RRC_NULL;

/* Fill and send message */
    rrc_csc_llim_reconfig_cell_req_t *p_llim_recfg_cell_req =
	    (rrc_csc_llim_reconfig_cell_req_t *)((U8 *)p_intrl_msg +
			    RRC_API_HEADER_SIZE);

    RRC_CSC_UT_TRACE_ENTER();
    /*Storing Transaction Id and Cell index in 
     *           rrc_csc_llim_reconfig_cell_req_t*/
    p_llim_recfg_cell_req->transaction_id = p_cell_ctx->transaction_id;

    /*Cell Index*/
    p_llim_recfg_cell_req->cell_index = p_cell_ctx->cell_index;

    /*Storing the cell Reconfig(ONGOING) parameter in 
     *              "rrc_csc_llim_reconfig_cell_req_t"*/
    p_llim_recfg_cell_req->cell_reconfig.presence_bitmask = 0;


    /*reconfig cell for system information*/
    update_cell_info_mib_sib(p_cell_ctx,
			p_llim_recfg_cell_req,
            p_reconfig_cell_req,
            p_csc_gl_ctx);/*SPR 17777 +-*/

    /* ALERT : NEED TO CHECK whether SI MAPPING CHANGED OR NOT THEN FILL THE
     *          * ASN ENCODED FORM FOR MIB,SIB AND SIs*/
    /* Check and fill MIB, SIB1 and SIs info */
    /* Check for the change in SI Mapping */

    si_mapping = csc_check_si_mapping(p_cell_ctx);

    if (SI_MAPPING_CHANGED == si_mapping)
    {
        RRC_CSC_TRACE(RRC_BRIEF,"SI MAPPING CHANGED ");

        /* Set the flag in cell context */
        p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = SI_MAPPING_CHANGED;
        if (RRC_FAILURE == update_cell_info_si_mapping_changed(p_cell_ctx, 
                    p_llim_recfg_cell_req,p_reconfig_cell_req, p_intrl_msg))
        {
            return RRC_FAILURE;
        }
        /* eMTC changes stop */

    }
    else if (SI_MAPPING_NOT_CHANGED == si_mapping)
    {
        if (RRC_FAILURE == update_cell_info_si_mapping_unchanged(p_cell_ctx, 
                   p_llim_recfg_cell_req,p_reconfig_cell_req, p_intrl_msg))
        {
            return RRC_FAILURE;
        }

    }
    else if(INVALID_SI_MAPPING_PWS == si_mapping)
    {
        /* Stop guard timer */
        if (PNULL != p_cell_ctx->guard_timer)
        {
            rrc_stop_timer(p_cell_ctx->guard_timer);
            p_cell_ctx->guard_timer = PNULL;
        }
        if (p_intrl_msg)
        {
            /* CSC didn't send message - free memory */
            rrc_msg_mem_free(p_intrl_msg);
        }
        /*PWS is going on and RRM has not sent PWS information 
         * in Scheduling Info for Broadcasting,
         * Send failure to RRM*/
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
                    p_cell_ctx->transaction_id, 
                    p_cell_ctx->cell_index,
                    RRC_FAILURE, 
                    RRM_RRC_INTERNAL_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, 
                    "Unable to send RESP FAILURE TO RRM");
            return RRC_FAILURE;
        }
        p_cell_ctx->rcfg_status_flag = RRC_FALSE;
        p_cell_ctx->retry_count = RRC_NULL; 
        p_cell_ctx->si_mod_flag = RRC_NULL; 
        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;
        /* eMTC changes stop */

    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_cell_reconfig_cell_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function start cell Reconfiguration.
 *                  STATE: CELL_M_STATE_CELL_IN_SERVICE; 
 *                  EVENT: CELL_M_EVENT_RRM_RECONFIG_CELL_REQ
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_cell_reconfig_cell_req(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	rrc_rrm_cell_reconfig_req_t *p_reconfig_cell_req  =
		(rrc_rrm_cell_reconfig_req_t *)p_msg;

	s1ap_cell_area_info_update_ind_t *p_s1ap_cell_area_info_update_ind = PNULL; 

	csc_cell_m_timer_buf_t guard_timer_data;
	void            *p_intrl_msg        = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_reconfig_cell_req);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
	RRC_ASSERT(p_reconfig_cell_req->cell_index == p_cell_ctx->cell_index);

	if (p_reconfig_cell_req->presence_bitmask == 0)
	{
		RRC_CSC_TRACE(RRC_WARNING, "Cell Reconfiguration received Bitmask zero");
		/*  Send RRC_RRM_CELL_RECONFIG_RESP */
		csc_send_rrm_cell_reconfig_resp_failure(
				rrc_get_transaction_id(p_api),
				p_cell_ctx->cell_index,
				RRC_FAILURE,RRM_RRC_INVALID_CONFIGURATION_RECV);
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	p_cell_ctx->sib8_periodicity = 0;

	if (p_cell_ctx->rcfg_status_flag == RRC_FALSE)
	{
		p_cell_ctx->rcfg_status_flag = RRC_TRUE;
	}
	else
	{
		RRC_CSC_TRACE(RRC_WARNING, "Cell Reconfiguration Already Ongoing.");
		/*  Send RRC_RRM_CELL_RECONFIG_RESP */
		csc_send_rrm_cell_reconfig_resp_failure( 
				rrc_get_transaction_id(p_api),
				p_cell_ctx->cell_index,
				RRC_FAILURE, RRM_RRC_INTERACTION_WITH_OTHER_PROCEDURE_FAILURE);
		RRC_CSC_UT_TRACE_EXIT();
		return;        
	}

	if(p_reconfig_cell_req->presence_bitmask & CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG)
	{
		if(p_reconfig_cell_req->broadcast_info.presence_bitmask & CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
		{
			p_cell_ctx->sib_modify_source = RRM_TRIGGERED_SIB8_MODIFY;
		}
		else
		{
			p_cell_ctx->sib_modify_source = RRM_TRIGGERED_SIB8_NO_MODIFY;
		}
	}

	p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);

	if (PNULL != p_cell_ctx->p_temp_si_schdl_info)
	{
		rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
		p_cell_ctx->p_temp_si_schdl_info = PNULL;
	}
    /* eMTC changes stop */

	/*Check if only Emergency ID has come*/
	if (p_reconfig_cell_req->presence_bitmask ==
			CELL_RECONFIG_REQ_API_EMERGENCY_AREA_ID_PRESENCE_FLAG)
	{
		/*Send only cell area info update ind to S1AP*/
		p_intrl_msg = csc_alloc_intrl_msg( RRC_S1AP_MODULE_ID,
				S1AP_CELL_AREA_INFO_UPDATE_IND,
				sizeof (s1ap_cell_area_info_update_ind_t));
		if (PNULL != p_intrl_msg)
		{
			/* Fill and send message */
			p_s1ap_cell_area_info_update_ind = 
				(s1ap_cell_area_info_update_ind_t *)
				((U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

			p_s1ap_cell_area_info_update_ind->cell_area_info.bitmask |=
				S1AP_CELL_INFO_UPDATE_IND_EMERGENCY_ID_PRESENT;
			p_s1ap_cell_area_info_update_ind->cell_index = p_cell_ctx->cell_index;
			l3_memcpy_wrapper(p_s1ap_cell_area_info_update_ind->cell_area_info.
					emergency_id,
					p_cell_ctx->recfg_cell_param.
					emergency_area_id,EMERGENCY_AREA_SIZE);


			rrc_send_message(p_intrl_msg,RRC_S1AP_MODULE_ID);
			RRC_CSC_TRACE(RRC_INFO,
					"S1AP_CELL_AREA_INFO_UPDATE_IND sent to S1AP");
			return;

		}
	}
	/*Check whether System Info has been modified or not 
	 * If modified, then first send the PAGING API and then Cell Reconfiguration
	 *               Request to LLIM */
	if (p_reconfig_cell_req->presence_bitmask &
			CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG)
	{
		/* System Info has been modified */
		/*Set the SI MOD flag */
		p_cell_ctx->si_mod_flag = 1;
        /* eMTC changes stop */
	}

	/*Allocating memory for Temp SI Scheduling Info*/
	p_cell_ctx->p_temp_si_schdl_info = rrc_mem_get(sizeof (si_schdl_info_t));

	if( PNULL == p_cell_ctx->p_temp_si_schdl_info )
	{
		RRC_CSC_TRACE(RRC_FATAL, "Unable to allocate Memory");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	memset_wrapper(p_cell_ctx->p_temp_si_schdl_info, 0, sizeof (si_schdl_info_t));
    /* eMTC changes stop */

	/* 1.Get Transaction ID and store in cell context*/
	p_cell_ctx->transaction_id = rrc_get_transaction_id(p_api);

	/* 2.Storing the Values of Cell Reconfiguration(ongoing) in csc cell context*/
	p_cell_ctx->recfg_cell_param = *p_reconfig_cell_req;

	/* --. set guard timer */
	if (!p_cell_ctx->si_mod_flag)
    /* eMTC changes stop */
	{
		guard_timer_data.auto_delete        = RRC_TRUE;
		guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
		guard_timer_data.cell_index         = p_cell_ctx->cell_index;
		guard_timer_data.cell_m_event       =
			CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED;

		RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_RECONFIG_TIMER] %u ms.",
				p_csc_gl_ctx->cell_recfg_duration);

		if(PNULL != p_cell_ctx->guard_timer)
		{
			rrc_stop_timer(p_cell_ctx->guard_timer);
			p_cell_ctx->guard_timer = PNULL;
		}

		p_cell_ctx->guard_timer = rrc_start_timer(
				p_csc_gl_ctx->cell_recfg_duration,
				&guard_timer_data, 
				sizeof (guard_timer_data), 
				RRC_FALSE);
	}
	/* 4.Initialise the RETRY COUNT to 0*/
	p_cell_ctx->retry_count = 0;

	/* 5. Fill and send CSC_LLIM_RECONFIG_CELL_REQ to LLIM*/
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_LLIM_MODULE_ID,
			CSC_LLIM_RECONFIG_CELL_REQ,
			sizeof(rrc_csc_llim_reconfig_cell_req_t));

	if (PNULL != p_intrl_msg)
	{
		if (RRC_FAILURE == fill_cell_reconfig_info_in_csc_context(
					p_cell_ctx,
					p_csc_gl_ctx,
					p_reconfig_cell_req,
					p_intrl_msg))
		{
			/* spr_11456_&_11429_fix_start */
			p_cell_ctx->si_mod_flag = 0;
			/* spr_11456_&_11429_fix_stop */
            /* eMTC changes stop */

			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
	}  /* In case of troubles rollback all changes */
	else
	{
		RRC_CSC_TRACE(RRC_FATAL, "UNABLE TO ALLOCATE MEMORY ");
		/* Stop guard timer */
		if(PNULL != p_cell_ctx->guard_timer)
		{
			rrc_stop_timer(p_cell_ctx->guard_timer);
			p_cell_ctx->guard_timer = PNULL;
		}    
	}
	RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_llim_reconfig_resp_pws_failure_rollback
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function Handles the CELL RECONFIGURATION Resp from LLIM
 *                  STATE: CELL_M_STATE_W_FOR_PWS_FAILURE_RESP;
 *                  EVENT: CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_llim_reconfig_resp_pws_failure_rollback(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    rrc_csc_llim_reconfig_cell_resp_t *p_reconfig_cell_resp =
        (rrc_csc_llim_reconfig_cell_resp_t *)p_msg;
    s1ap_pws_failure_resp_t *p_s1ap_pws_failure_resp = PNULL;
    rrc_return_et response = RRC_FAILURE;
    void *p_intrl_msg        = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_reconfig_cell_resp);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(p_reconfig_cell_resp->cell_index == p_cell_ctx->cell_index);

    /* Precondition - should be the same transaction */
    if (p_reconfig_cell_resp->transaction_id != p_cell_ctx->transaction_id)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /* 1. Stop guard timer */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_RECONFIGURE_TIMER]; Unused variable p_api=%p",p_api);/*SPR 17777 +-*/
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    /* 2. Handling the ResponseCode*/
    switch (p_reconfig_cell_resp->response)
    {
        case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS :

            RRC_CSC_TRACE(RRC_INFO,"ROll back due to PWS Failure has succeded.");
            response = RRC_SUCCESS;
            p_cell_ctx->retry_count = RRC_NULL; 
            
            /* State Transition */
            CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
            break;


        default:
            /* If Max Retries hasn't attempted for the rollback */
            if(p_cell_ctx->retry_count < p_csc_gl_ctx->
                    max_cell_reconfig_retry_count )
            {
                /* 2.1 Send Reconfig with Old Configuration */
                csc_send_llim_recfg_req_with_old_cfg_pws(
                        p_cell_ctx,
                        p_csc_gl_ctx,
                        MAC_ONLY);
            }
            else
            {
                RRC_CSC_TRACE(RRC_ERROR,"ROll back due to PWS Failure has failed and"
                     "RRM should send Cell Delete");  
                response = RRC_FAILURE;
                /*Roll back has failed*/
                CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
            }
            break;
    }
    if (CELL_M_STATE_CELL_IN_SERVICE == CELL_M_FSM_GET_CURRENT_STATE(p_cell_ctx))
    {
        p_intrl_msg = csc_alloc_intrl_msg(RRC_S1AP_MODULE_ID,
              S1AP_PWS_FAILURE_RESP,
              sizeof(s1ap_pws_failure_resp_t));
        
        if ( PNULL != p_intrl_msg )
        {
            p_s1ap_pws_failure_resp = (s1ap_pws_failure_resp_t *)
                ((U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

            p_s1ap_pws_failure_resp->transaction_id = p_cell_ctx->transaction_id;
            p_s1ap_pws_failure_resp->cell_index = p_cell_ctx->cell_index;
            p_s1ap_pws_failure_resp->response = response;
            RRC_CSC_TRACE(RRC_INFO, "Sending PWS Failure Resp to S1AP");
            rrc_send_message(p_intrl_msg, RRC_S1AP_MODULE_ID);
        }
    }

    RRC_CSC_UT_TRACE_EXIT();
}


/*
 *   CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_llim_reconfig_resp_rollback
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function Handles the CELL RECONFIGURATION Resp from LLIM
 *                  STATE: CELL_M_STATE_W_FOR_PWS_RESP;
 *                  EVENT: CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_llim_reconfig_resp_rollback(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    rrc_csc_llim_reconfig_cell_resp_t *p_reconfig_cell_resp =
        (rrc_csc_llim_reconfig_cell_resp_t *)p_msg;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_reconfig_cell_resp);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(p_reconfig_cell_resp->cell_index == p_cell_ctx->cell_index);

    /* Precondition - should be the same transaction */
    if (p_reconfig_cell_resp->transaction_id != p_cell_ctx->transaction_id)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /* 1. Stop guard timer */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_RECONFIGURE_TIMER]; Unused variable p_api=%p",p_api);/*SPR 17777 +-*/
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    /* 2. Handling the ResponseCode*/
    switch (p_reconfig_cell_resp->response)
    {
        case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS :

            RRC_CSC_TRACE(RRC_INFO,"ROll back due to PWS has succeded.");
            if ( PNULL != p_cell_ctx->p_enb_pws_req )
            { 
                cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                    RRM_RRC_INTERNAL_FAILURE, p_cell_ctx); 
            }

            p_cell_ctx->retry_count = RRC_NULL; 

            /* State Transition */
            CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
            break;


        default:
            /* If Max Retries hasn't attempted for the rollback */
            if(p_cell_ctx->retry_count < p_csc_gl_ctx->
                    max_cell_reconfig_retry_count )
            {
                /* 2.1 Send Reconfig with Old Configuration */
                csc_send_llim_recfg_req_with_old_cfg_pws(
                        p_cell_ctx,
                        p_csc_gl_ctx,
                        MAC_ONLY);
            }
            else
            {
                RRC_CSC_TRACE(RRC_ERROR,"ROll back due to PWS has failed and"
                     "RRM should send Cell Delete");  
                if ( PNULL != p_cell_ctx->p_enb_pws_req )
                { 
                    cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                        RRM_RRC_UNKNOWN_FAILURE, p_cell_ctx); 
                }
                /*Roll back has failed*/
                CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
            }
            break;
    }

    RRC_CSC_UT_TRACE_EXIT();
}


/*****************************************************************************
 *  FUNCTION NAME : csc_compose_sib_type10_info
 *  INPUT         : sib_type_10_Info_t          *p_sib10_info
 *                  s1ap_pws_req_t              *p_pws_info
 *  OUTPUT        : none
 *  DESCRIPTION: This function composes the system info type 10 and fills the
 *  internal structure sib_type_10_Info_t from contents provided by eNB Manager
 *  in s1ap_pws_req_t API
 *
 *  RETURNS: None
 * ****************************************************************************/
static rrc_return_et csc_compose_sib_type10_info
(
 sib_type_10_Info_t          *p_sib10_info,  /* destination*/
 s1ap_pws_req_t              *p_pws_info /* source*/
 )
{
    RRC_CSC_UT_TRACE_ENTER();

    if(!(PWS_WARNING_TYPE_PRESENT & p_pws_info->bitmask))
    {
        /*As warning type is mandatory for Broadcasting SIB 10*/
        RRC_CSC_TRACE(RRC_ERROR,"PWS: warning type missing for SIB 10");
        return RRC_FAILURE;
    }

    l3_memcpy_wrapper(p_sib10_info->msg_id, p_pws_info->message_identifier, 
            MSG_ID_OCTET_SIZE);
    l3_memcpy_wrapper(p_sib10_info->serial_number, p_pws_info->serial_number, 
            SERIAL_NUMBER_OCTET_SIZE);
    l3_memcpy_wrapper(p_sib10_info->warning_type, p_pws_info->warning_type, 
            WARNING_TYPE_OCTET_SIZE);

    if(PWS_WARNING_SECURITY_INFO_PRESENT& 
            p_pws_info->bitmask)
    { 
        p_sib10_info->presence_bitmask |= SIB_10_WARN_SEC_INFO_PRESENCE_FLAG;
        l3_memcpy_wrapper(p_sib10_info->warning_security_info, 
                p_pws_info->warning_security_info, 
                SECURITY_INFORMATION_OCTET_SIZE);
    }

    return RRC_SUCCESS;

    RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
 *  FUNCTION NAME : csc_compose_sib_type11_info
 *  INPUT         : sib_type_11_Info_t          *p_sib11_info
 *                  s1ap_pws_req_t              *p_pws_info
 *  OUTPUT        : none
 *  DESCRIPTION: This function composes the system info type 11 and fills the
 *  internal structure sib_type_11_Info_t from contents provided by eNB Manager
 *  in s1ap_pws_req_t API
 *
 *  RETURNS: None
 * ****************************************************************************/
static rrc_return_et csc_compose_sib_type11_info
(
  sib_type_11_Info_t          *p_sib11_info,  /* destination*/
  s1ap_pws_req_t              *p_pws_info /* source*/
)
{
    RRC_CSC_UT_TRACE_ENTER();

    if(!((PWS_WARNING_MESSAGE_SIZE_PRESENT & p_pws_info->bitmask) &&
         ( PWS_WARNING_MESSAGE_CONTENTS_PRESENT & p_pws_info->bitmask)))
    {
        RRC_CSC_TRACE(RRC_ERROR,"PWS: warning message contents missing for SIB 11");
        return RRC_FAILURE;
    }

    l3_memcpy_wrapper(p_sib11_info->msg_id, p_pws_info->message_identifier,
                   MSG_ID_OCTET_SIZE);
    l3_memcpy_wrapper(p_sib11_info->serial_number, p_pws_info->serial_number,
                   SERIAL_NUMBER_OCTET_SIZE);
    p_sib11_info->warning_msg_size = p_pws_info->warning_msg_size;

    l3_memcpy_wrapper(p_sib11_info->warning_msg, p_pws_info->wrw_message_contents,
                  p_pws_info->warning_msg_size);
    if (PWS_DATA_CODING_SCHEME_PRESENT & p_pws_info->bitmask)
    {
      l3_memcpy_wrapper(p_sib11_info->data_coding_scheme,
                 p_pws_info->data_coding_scheme,
                 DATA_CODING_SCHEME_OCTET_SIZE);
      p_sib11_info->presence_bitmask |= SIB_11_DATA_COD_SCHEME_PRESENCE_FLAG;
    }

    return RRC_SUCCESS;
    RRC_CSC_UT_TRACE_EXIT();
}


/*****************************************************************************
 *  FUNCTION NAME : csc_compose_sib_type12_info
 *  INPUT         : sib_type_12_Info_t          *p_sib12_info
 *                  s1ap_pws_req_t              *p_pws_info
 *  OUTPUT        : none                
 *  DESCRIPTION: This function composes the system info type 12 and fills the
 *  internal structure sib_type_12_Info_t from contents provided by eNB Manager
 *  in s1ap_pws_req_t API
 *
 *  RETURNS: None
 * ****************************************************************************/
static rrc_return_et csc_compose_sib_type12_info
(
 sib_type_12_Info_t          *p_sib12_info,  /* destination*/
 s1ap_pws_req_t              *p_pws_info /* source*/
 )
{
  RRC_CSC_UT_TRACE_ENTER();
  
  if(!((PWS_WARNING_MESSAGE_SIZE_PRESENT & p_pws_info->bitmask) && 
          ( PWS_WARNING_MESSAGE_CONTENTS_PRESENT & p_pws_info->bitmask)))
  {
      RRC_CSC_TRACE(RRC_ERROR,"PWS: warning message contents missing for SIB 12");
      return RRC_FAILURE;
  }

  memset_wrapper(p_sib12_info,0,sizeof(sib_type_12_Info_t));

  l3_memcpy_wrapper(p_sib12_info->msg_id, p_pws_info->message_identifier, 
      MSG_ID_OCTET_SIZE);
  l3_memcpy_wrapper(p_sib12_info->serial_number, p_pws_info->serial_number, 
      SERIAL_NUMBER_OCTET_SIZE);

  p_sib12_info->warning_msg_size = p_pws_info->warning_msg_size;

  l3_memcpy_wrapper(p_sib12_info->warning_msg, p_pws_info->wrw_message_contents, 
      p_pws_info->warning_msg_size);

  if (PWS_DATA_CODING_SCHEME_PRESENT & p_pws_info->bitmask)
  {
    l3_memcpy_wrapper(p_sib12_info->data_coding_scheme, 
        p_pws_info->data_coding_scheme, 
        DATA_CODING_SCHEME_OCTET_SIZE);
    p_sib12_info->presence_bitmask |= SIB_12_DATA_COD_SCHEME_PRESENCE_FLAG;
  }

  return RRC_SUCCESS;
  RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_cell_in_service_llim_reconf_cell_resp
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function Handles the CELL RECONFIGURATION Resp from LLIM
 *                  STATE: CELL_M_STATE_CELL_IN_SERVICE;
 *                  EVENT: CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP
 *                  This is a special case handling when only new SIB-8 segments are generated
 *                  and passed to MAC in MAC_RECONFIG_REQ via LLIM
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_cell_in_service_llim_reconf_cell_resp(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{

    rrc_csc_llim_reconfig_cell_resp_t *p_reconfig_cell_resp =
        (rrc_csc_llim_reconfig_cell_resp_t *)p_msg;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_reconfig_cell_resp);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(p_reconfig_cell_resp->cell_index == p_cell_ctx->cell_index);

    /* Precondition - should be the same transaction */
    if (p_reconfig_cell_resp->transaction_id != p_cell_ctx->transaction_id)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /* 2. Handling the ResponseCode*/
    switch (p_reconfig_cell_resp->response)
    {
        case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS :
             /* 2.1. Update the cell configuration with new Configuration */
             csc_update_cell_config(p_cell_ctx);
             /*reset the sib8_new_segments_generate_count count when
              * llim_reconig_cell_resp comes with a success*/
             p_cell_ctx->sfn_sf_timestamp_requested = RRC_FALSE;
             p_cell_ctx->rcfg_status_flag = RRC_FALSE;
             /* BUG 11440 Fix Start */
             RRC_CSC_TRACE(RRC_WARNING, "RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS %s, LN=%d", __FUNCTION__, __LINE__);
             /* BUG 11440 Fix Stop */
             break;

        case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_FAIL :
        case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_TIMER_EXP:
             /*In case failure is received, reduce the
              * sib8_new_segments_generate_count so that it meets the condition
              * to trigger generating new SIB-8 instances after some time*/
             p_cell_ctx->sib8_new_segments_generate_count -= W_F_SIB8_NEW_SEG_GENERATE;
             p_cell_ctx->rcfg_status_flag = RRC_FALSE;
             RRC_CSC_TRACE(RRC_WARNING, "RRC_FATAL: L2 & L3 are out of sync. Handling of this message is not present."
			 	"Unexpected behaviour will be observed in SI broadcasting");
            break;

        default:
            RRC_TRACE(RRC_WARNING,"Unknown Response code from LLIM"
                    " for Cell Reconfiguration : [%d]; Unused variable p_api=%p p_csc_gl_ctx=%p"
                    ,p_reconfig_cell_resp->response,p_api,p_csc_gl_ctx);/*SPR 17777 +-*/
            break;

    }
    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: update_llim_pws_info_in_cell_context 
 *      
 *      this function update scheduling info in cell context
 *      prepare pws request and sent to LLIM
 *
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
static rrc_return_et update_llim_pws_info_in_cell_context(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx,  
        csc_cell_m_timer_buf_t *guard_timer_data,
        rrc_si_transmission_info_list_t     *p_rrc_si_transmission_info_list,
        void *p_intrl_msg)
{
    if ( PNULL != p_cell_ctx->p_temp_si_schdl_info )
    {
        memset_wrapper ( p_cell_ctx->p_temp_si_schdl_info, RRC_NULL, 
                sizeof(si_schdl_info_t) );
    }

    if (RRC_FAILURE == fill_new_etws_and_cmas_scheduling_info (p_cell_ctx))
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
                "procedure has failed");
        cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
                RRC_RRM_UNKNOWN_FAILURE,
                p_cell_ctx);
        /* STATE TRANSITION */
        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
        return RRC_FAILURE;
    }

    /* Fill and send CSC_LLIM_PWS_REQ to LLIM*/
    p_intrl_msg = csc_alloc_intrl_msg( 
            RRC_LLIM_MODULE_ID,
            CSC_LLIM_PWS_REQ,
            sizeof(rrc_csc_llim_pws_req_t));
    if (PNULL != p_intrl_msg)
    {
        rrc_csc_llim_pws_req_t *p_llim_pws_req =
            (rrc_csc_llim_pws_req_t *)((U8 *)p_intrl_msg +
                    RRC_API_HEADER_SIZE);

        /*Storing Transaction Id in rrc_csc_llim_pws_req_t */
        p_llim_pws_req->transaction_id = p_cell_ctx->transaction_id;
        p_llim_pws_req->cell_index = p_cell_ctx->cell_index;
        /* Store the SFN ,using SFN = SFN(curr) + OFFSET */
        p_llim_pws_req->sfn = (rrc_sfn_t)(p_cell_ctx->mod_period_boud_counter
                - CSC_SFN_OFFSET);

        if (KILL_SI_TRANSMISSION_INFO_LIST & p_cell_ctx->p_enb_kill_req->bitmask)
        {
            p_rrc_si_transmission_info_list =
                &p_cell_ctx->p_enb_kill_req->si_transmission_info;
        }
        /* SPR 19838: coverity 10096 fix start */
        if ( PNULL != p_cell_ctx->p_temp_si_schdl_info )
        {
            /* SPR 19838: coverity 10096 fix end */
            /* Check and fill SIB1 and SIs info */
            if (RRC_SUCCESS == bch_m_sib1_sis_pws(
                        p_llim_pws_req,
                        &(p_cell_ctx->p_enb_kill_req->scheduling_info_list), 
                        p_cell_ctx,
                        RRC_TRUE,
                        p_rrc_si_transmission_info_list))
            {
                guard_timer_data->auto_delete        = RRC_TRUE;
                guard_timer_data->target_submodule   = CSC_SUBMODULE_CELL_M;
                guard_timer_data->cell_index         = p_cell_ctx->cell_index;
                guard_timer_data->cell_m_event       = CELL_M_EVENT_KILL_TIMER_EXPIRED;
                RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_KILL_TIMER] %u ms.",
                        p_csc_gl_ctx->kill_duration);

                if(PNULL != p_cell_ctx->guard_timer)
                {
                    rrc_stop_timer(p_cell_ctx->guard_timer);
                    p_cell_ctx->guard_timer = PNULL;
                }

                p_cell_ctx->guard_timer = rrc_start_timer(
                        p_csc_gl_ctx->kill_duration,
                        guard_timer_data, 
                        sizeof (csc_cell_m_timer_buf_t), 
                        RRC_FALSE);

                if(PNULL == p_cell_ctx->guard_timer)
                {
                    RRC_CSC_TRACE(RRC_ERROR,"CSC_KILL_TIMER start timer failed");
                    return RRC_FAILURE;
                }

                rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);

                /* STATE TRANSITION */
                CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_W_FOR_KILL_RESP);

            }
            else
            {
                /* Bad incoming info */
                RRC_CSC_TRACE(RRC_ERROR, "Bad message - UNABLE TO ENCODE SI/SIBs.");
                if (p_intrl_msg)
                {
                    rrc_msg_mem_free(p_intrl_msg);
                }
                /* Stop guard timer */
                if (PNULL != p_cell_ctx->guard_timer)
                {
                    rrc_stop_timer(p_cell_ctx->guard_timer);
                    p_cell_ctx->guard_timer = PNULL;
                }
                RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
                        "procedure has failed");
                cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
                        RRC_RRM_UNKNOWN_FAILURE,
                        p_cell_ctx);
                /* STATE TRANSITION */
                CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index);
            }
            /* SPR 19838: coverity 10096 fix start */
        }

        /* SPR 19838: coverity 10096 fix end */

        /* SPR 20007: coverity 110664 fix Start */
        else
        {
            if (p_intrl_msg)
            {
                rrc_msg_mem_free(p_intrl_msg);
            }
        }
        /* SPR 20007: coverity 110664 fix End */
    }
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: update_mib_sib_info_in_cell_context 
 *      
 *      this function updates the system information in cell context as
 *      recevied from RRM
 *
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
static rrc_return_et update_mib_sib_info_in_cell_context(
	csc_cell_ctx_t  *p_cell_ctx,
	csc_gl_ctx_t    *p_csc_gl_ctx, 
	rrc_csc_llim_pws_req_t *p_llim_pws_req, 
        csc_cell_m_timer_buf_t *guard_timer_data,
         rrc_si_transmission_info_list_t     *p_rrc_si_transmission_info_list,
        void *p_intrl_msg ,
        U32             *cmas_key, 
        U32             *counter_value
        )
{
    cmas_info_t     *cmas_info_node      = PNULL;
    /* Check and fill MIB, SIB1 and SIs info */
    if (RRC_SUCCESS == bch_m_sib1_sis_pws(
                p_llim_pws_req,
                &(p_cell_ctx->p_enb_pws_req->scheduling_info_list), 
                p_cell_ctx,
                RRC_FALSE,
                p_rrc_si_transmission_info_list))
    {
        guard_timer_data->auto_delete        = RRC_TRUE;
        guard_timer_data->target_submodule   = CSC_SUBMODULE_CELL_M;
        guard_timer_data->cell_index         = p_cell_ctx->cell_index;
        guard_timer_data->cell_m_event       = CELL_M_EVENT_PWS_TIMER_EXPIRED;
        RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_PWS_TIMER] %u ms.",
                p_csc_gl_ctx->pws_duration);

        if(PNULL != p_cell_ctx->guard_timer)
        {
            rrc_stop_timer(p_cell_ctx->guard_timer);
            p_cell_ctx->guard_timer = PNULL;
        }

        p_cell_ctx->guard_timer = rrc_start_timer(
                p_csc_gl_ctx->pws_duration,
                guard_timer_data, 
                sizeof (csc_cell_m_timer_buf_t), 
                RRC_FALSE);

        if(PNULL == p_cell_ctx->guard_timer)
        {
            RRC_CSC_TRACE(RRC_ERROR,"CSC_PWS_TIMER start timer failed");
            return RRC_FAILURE;
        }

        /* Setting the counter value */
        if (RRC_FALSE == p_cell_ctx->cmas_flag)
        {
            p_cell_ctx->etws_counter = (*counter_value);
            p_cell_ctx->etws_time_elapsed = 0;
        }
        else
        {
            (*cmas_key) = (U32) ( p_cell_ctx->p_enb_pws_req->
                    message_identifier[1] << 24 );
            (*cmas_key) |= (U32) ( p_cell_ctx->p_enb_pws_req->
                    message_identifier[0] << 16 );
            (*cmas_key) |= (U32) ( p_cell_ctx->p_enb_pws_req->
                    serial_number[1] << 8 );
            (*cmas_key) |= (U32) ( p_cell_ctx->p_enb_pws_req->serial_number[0]);
            cmas_info_node = (cmas_info_t *)(st_find_tree_node( 
                        (const search_tree_t *)&(p_cell_ctx->cmas_info_list),
                        (const void*)cmas_key));
            if (PNULL != cmas_info_node)
            {
                cmas_info_node->cmas_counter = (*counter_value);
                cmas_info_node->cmas_time_elapsed = 0;
            }
            else
            {
                RRC_CSC_TRACE(RRC_ERROR,"CMAS node not found");
                cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                        RRM_RRC_INTERNAL_FAILURE,
                        p_cell_ctx);
                /* STATE TRANSITION */
                CELL_M_FSM_SET_STATE(p_cell_ctx,
                        CELL_M_STATE_W_FOR_PWS_RESP);
                return RRC_FAILURE; 
            }
        }
        rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);

        /* STATE TRANSITION */
        CELL_M_FSM_SET_STATE(p_cell_ctx,
                CELL_M_STATE_W_FOR_PWS_RESP);
    }
    else
    {
        /* Bad incoming info */
        RRC_CSC_TRACE(RRC_ERROR, 
                "Bad message - UNABLE TO ENCODE SI/SIB AND MIBs.");

        if (p_intrl_msg)
        {
            rrc_msg_mem_free(p_intrl_msg);
        }

        /* Stop guard timer */
        if (PNULL != p_cell_ctx->guard_timer)
        {
            rrc_stop_timer(p_cell_ctx->guard_timer);
            p_cell_ctx->guard_timer = PNULL;
        }
        /*  Send PWS Failure to S1AP*/
        cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                RRM_RRC_INTERNAL_FAILURE,
                p_cell_ctx);

        /* STATE TRANSITION */
        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
    }
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: check_and_update_cms_info_in_cell_ctx
 *      
 *      this function check the cms info recevied from RRM
 *      and update them in cell context
 *
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/


static rrc_return_et check_and_update_cms_info_in_cell_ctx(
	    csc_cell_ctx_t  *p_cell_ctx,
	    csc_gl_ctx_t    *p_csc_gl_ctx   , 
        csc_cell_m_timer_buf_t *guard_timer_data,
        U32             *cmas_key, 
        U32             *counter_value)
{ 
    U8              cmas_count      = RRC_NULL;
    cmas_info_t     *temp_cmas_info_node = PNULL;
    
    if ( PNULL == p_cell_ctx->p_temp_si_schdl_info )
    {
        RRC_CSC_TRACE(RRC_FATAL, "Unable to allocate Memory");
        return RRC_FAILURE;
    }

    memset_wrapper ( p_cell_ctx->p_temp_si_schdl_info, RRC_NULL, 
            sizeof(si_schdl_info_t) );

    /* --. 1.set guard timer */
    guard_timer_data->auto_delete        = RRC_TRUE;
    guard_timer_data->target_submodule   = CSC_SUBMODULE_CELL_M;
    guard_timer_data->cell_index         = p_cell_ctx->cell_index;
    guard_timer_data->cell_m_event       = CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ;


    RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_PWS_TIMER] %u ms.",
            p_csc_gl_ctx->pws_duration);

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
    p_cell_ctx->guard_timer = rrc_start_timer(
            p_csc_gl_ctx->pws_duration,
            guard_timer_data, 
            sizeof (csc_cell_m_timer_buf_t), 
            RRC_FALSE);

    /*Update Transaction ID in cell context*/
    p_cell_ctx->transaction_id = p_cell_ctx->p_enb_pws_req->transaction_id;

    /* Check whether the recieved PWS request is for CMAS or ETWS */
    if (( PWS_ETWS_SCHEDULING_INFO & p_cell_ctx->p_enb_pws_req->bitmask ) &&
            ( 0 == memcmp_wrapper(&(p_cell_ctx->p_enb_pws_req->message_identifier),
                              &(p_cell_ctx->p_enb_pws_req->etws_scheduling_info.message_identifier),
                              sizeof(U16))) && ( 0 == memcmp_wrapper(&(p_cell_ctx->p_enb_pws_req->serial_number), 
                              &(p_cell_ctx->p_enb_pws_req->etws_scheduling_info.serial_number), sizeof(U16))))
    {
        p_cell_ctx->cmas_flag = RRC_FALSE;
        if (RRC_FAILURE == validate_and_fill_etws_warning (
                    p_cell_ctx, counter_value))
        {
            /* STATE TRANSITION */
            CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
            return RRC_FAILURE;
        }
    }
    else
    {
        if (PWS_CMAS_SCHEDULING_INFO_LIST & p_cell_ctx->p_enb_pws_req->bitmask)
        {
            p_cell_ctx->cmas_flag = RRC_TRUE;
            if (RRC_FAILURE == validate_and_fill_cmas_warning (
                        p_cell_ctx, counter_value))
            {
                /* STATE TRANSITION */
                CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
                return RRC_FAILURE;
            }
        }
        else
        {
            RRC_CSC_TRACE(RRC_INFO,"Received wrong message id and serial no."); 
            cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
                    RRM_RRC_INTERNAL_FAILURE,
                    p_cell_ctx);
            /* STATE TRANSITION */
            CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
            return RRC_FAILURE;
        }
    }
    if (RRC_NULL == ((PWS_ETWS_SCHEDULING_INFO & p_cell_ctx->p_enb_pws_req->bitmask) |
                (PWS_CMAS_SCHEDULING_INFO_LIST & p_cell_ctx->p_enb_pws_req->bitmask)))
    {
        RRC_CSC_TRACE(RRC_ERROR,
                "ETWS or CMAS scheduling Info must be present");
        cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
                RRM_RRC_INTERNAL_FAILURE,
                p_cell_ctx);
        /* STATE TRANSITION */
        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
        return RRC_FAILURE;
    }
    if (PWS_ETWS_SCHEDULING_INFO & p_cell_ctx->p_enb_pws_req->bitmask)
    {
        if (PNULL != p_cell_ctx->etws_info)
        {
            if ( RRC_RRM_ETWS_SCHEDULING_NUM_OF_SEGMENTS_PRESENT &
                    p_cell_ctx->p_enb_pws_req->etws_scheduling_info.bitmask )
            {
                p_cell_ctx->etws_info->sib11_info.num_of_segments =
                    p_cell_ctx->p_enb_pws_req->etws_scheduling_info.num_of_segments;
            }
            else
            {
                p_cell_ctx->etws_info->sib11_info.num_of_segments = 1;
            }
        }
    }
    if (PWS_CMAS_SCHEDULING_INFO_LIST & p_cell_ctx->p_enb_pws_req->bitmask)
    {
        /* Change the num of segments and num of repetitions according to the
           CMAS scheduling received from RRM through S1AP in S1AP_PWS_REQ */
        for (cmas_count = 0; cmas_count < p_cell_ctx->
                p_enb_pws_req->cmas_scheduling_info_list.cmas_warning_count; ++cmas_count)
        {
            (*cmas_key) = (U32) (p_cell_ctx->p_enb_pws_req->cmas_scheduling_info_list. \
                    cmas_scheduling_info[cmas_count].message_identifier[1] << 24 );
            (*cmas_key) |= (U32) (p_cell_ctx->p_enb_pws_req->cmas_scheduling_info_list. \
                    cmas_scheduling_info[cmas_count].message_identifier[0] << 16 );
            (*cmas_key) |= (U32) (p_cell_ctx->p_enb_pws_req->cmas_scheduling_info_list. \
                    cmas_scheduling_info[cmas_count].serial_number[1] << 8 );
            (*cmas_key) |= (U32) (p_cell_ctx->p_enb_pws_req->cmas_scheduling_info_list. \
                    cmas_scheduling_info[cmas_count].serial_number[0]);
            temp_cmas_info_node = (cmas_info_t *)(st_find_tree_node( 
                        (const search_tree_t *)&(p_cell_ctx->cmas_info_list),
                        (const void*)cmas_key));
            if (PNULL == temp_cmas_info_node)
            {
                RRC_CSC_TRACE(RRC_ERROR,"CMAS scheduling sent by RRM is wrong");  
                cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                        RRM_RRC_INTERNAL_FAILURE,
                        p_cell_ctx);
                return RRC_FAILURE;
            }
            temp_cmas_info_node->num_of_segments = p_cell_ctx->p_enb_pws_req->
                cmas_scheduling_info_list.cmas_scheduling_info[cmas_count].num_of_segments;
            temp_cmas_info_node->num_of_repetitions = p_cell_ctx->p_enb_pws_req->
                cmas_scheduling_info_list.cmas_scheduling_info[cmas_count].num_of_repetitions;
        }
    }
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME:process_cell_reconfig_ind 
 *
 *   Description: This function read information from cell reconfig indication and
 *       fill them in csc context
 *
 *   INPUT: csc cell context info and csc global context info
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
static rrc_return_et process_cell_reconfig_ind(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx    
        )
{
    /* coverity_fix_62128_start */
    rrc_rrm_cell_setup_req_t *p_cfg_cell_param = &p_cell_ctx->cfg_cell_param;
    void *p_intrl_msg = PNULL;
    s1ap_cell_area_info_update_ind_t    *p_s1ap_cell_area_info_update_ind = PNULL;
    rrc_si_transmission_info_list_t     *p_rrc_si_transmission_info_list = PNULL;
    rrc_rrm_cell_reconfig_resp_t        cell_recfg_resp;
    csc_cell_m_timer_buf_t              guard_timer_data;

    U32             cmas_key             = RRC_NULL;
    U32             counter_value   = RRC_NULL;

    csc_cell_m_timer_buf_t      rand_regenerate_timer_data;
    U32  rand_seed, rand_max_limit = 0, rand_min_limit = 0 ;

    RRC_CSC_UT_TRACE_ENTER();

    if( p_cell_ctx->sib_modify_source == RRM_TRIGGERED_SIB8_MODIFY)
    {
        p_cell_ctx->cell_reconfig_done = RRC_TRUE;
        RRC_CSC_TRACE(RRC_INFO, "RRC_LLIM_RECONFIG_PHY_MAC_MODE_SUCCESS in RRM Triggered SIB-8 modify");
    }


    if (RRC_TRUE == p_cell_ctx->mac_reconfig_with_sib8_segments)
    {
        /*Bug#10186 fix start*/
        /* spr 10508 & 10509 start */
        if(RRC_FALSE == p_cell_ctx->cell_start_flag)
        {
            /* spr 10508 & 10509 stop */
            /* Send RRC_RRM_CELL_SETUP_RESP */
            if (RRC_SUCCESS !=  csc_send_rrm_cell_setup_resp(
                        p_cell_ctx->transaction_id,
                        p_cell_ctx->cell_index,
                        RRC_SUCCESS,
                        RRM_RRC_NO_ERROR))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP TO RRM");
                return RRC_FAILURE;
            }
        }
        else
        {
            /* spr 10508 & 10509 start */
            p_cell_ctx->cell_start_flag = RRC_FALSE;
            /* spr 10508 & 10509 stop */
            if (RRC_SUCCESS != csc_send_rrm_cell_start_resp(p_cell_ctx->transaction_id,
                        p_cell_ctx->cell_index,
                        RRC_SUCCESS,
                        RRM_RRC_NO_ERROR))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP TO RRM");
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
        }
        /*Bug#10186 fix stop*/

        p_cell_ctx->mac_reconfig_with_sib8_segments = RRC_FALSE;
        p_cell_ctx->sfn_sf_timestamp_requested = RRC_FALSE;
        p_cell_ctx->rcfg_status_flag = RRC_FALSE;
        CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
        return RRC_FAILURE;
    }

    if(RRC_NULL == p_cell_ctx->retry_count)
    {
        /* 2.1. Update the cell configuration with new Configuration */
        csc_update_cell_config(p_cell_ctx);

        /****** Sending CSC_UECC_CELL_RECONFIG_IND ******/

            p_intrl_msg = csc_alloc_intrl_msg( 
                    RRC_UECC_MODULE_ID,
                    CSC_UECC_CELL_RECONFIG_IND,
                    sizeof (rrc_csc_uecc_cell_reconfig_ind_t));

            if (PNULL != p_intrl_msg)
            {
                /* Fill and send message */
                rrc_csc_uecc_cell_reconfig_ind_t *p_uecc_cell_reconfig_ind =
                    (rrc_csc_uecc_cell_reconfig_ind_t *)((U8 *)p_intrl_msg +
                            RRC_API_HEADER_SIZE);

                p_uecc_cell_reconfig_ind->bitmask = 0;
                p_uecc_cell_reconfig_ind->mib_info = 
                    p_cfg_cell_param->mib_info;

                p_uecc_cell_reconfig_ind->sib_type_1_Info = 
                    p_cfg_cell_param->sib_type_1_Info;

                p_uecc_cell_reconfig_ind->sib_type_2_Info = 
                    p_cfg_cell_param->sib_type_2_Info;

                p_uecc_cell_reconfig_ind->eutra_dl_carrier_freq = 
                    p_cfg_cell_param->cell_config_param.cell_parameters.dl_earfcn;

                p_uecc_cell_reconfig_ind->phys_cell_id = p_cfg_cell_param->
                    cell_config_param.cell_parameters.phys_cell_id;


                p_uecc_cell_reconfig_ind->cell_index   = p_cell_ctx->cell_index;
                p_uecc_cell_reconfig_ind->num_of_antennas = 
                    p_cfg_cell_param->cell_config_param.cell_parameters.
                    num_of_antennas;

		/*SPR_18384_START*/
		if (CELL_RECONFIG_REQ_API_CDMA2000_CELL_PARAM_PRESENT &
				p_cell_ctx->recfg_cell_param.presence_bitmask)
		{
                rand_min_limit = p_cell_ctx->cfg_cell_param.
                    cdma2000_cell_param.
                    cdma2000_rand.rand_min;
                rand_max_limit = p_cell_ctx->cfg_cell_param.
                    cdma2000_cell_param.
                    cdma2000_rand.rand_max  ;

                if(PNULL != p_cell_ctx->rand_regenerate_timer)
                {
                    rrc_stop_timer(p_cell_ctx->rand_regenerate_timer);
                    RRC_CSC_TRACE(RRC_BRIEF, "Timer [CSC_RAND_REGENERATE_TIMER] stopped");
                    p_cell_ctx->rand_regenerate_timer = PNULL;
                }

                if (rand_max_limit < rand_min_limit)
                {
                    RRC_CSC_TRACE(RRC_ERROR, "RAND Max = %u" 
                            "received from RRM is less than RAND min= %u "
                            , rand_max_limit,rand_min_limit);

                    RRC_CSC_TRACE(RRC_BRIEF, "Therefore made RAND Max =  RAND min ");
                    rand_max_limit = rand_min_limit;

                    p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
                        cdma2000_rand.rand_max	=
                        p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
                        cdma2000_rand.rand_min ;
                }  

                rand_seed = p_cell_ctx->cfg_cell_param.
                    cdma2000_cell_param.cdma2000_rand.rand_seed;

                RRC_CSC_TRACE(RRC_BRIEF,"CDMA: RAND seed = %u\n RAND min = "
                        "%u\n RAND max = %u\n", 
                        rand_seed,rand_min_limit,rand_max_limit);

                get_cdma2000_rand_val(p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
                        cdma2000_rand.rand_seed, 
                        rand_min_limit, 
                        rand_max_limit,
                        &(p_cell_ctx->cdma2000_rand));


                /*Start the RAND timer */
                rand_regenerate_timer_data.auto_delete = RRC_TRUE;
                rand_regenerate_timer_data.target_submodule = CSC_SUBMODULE_CELL_M;
                rand_regenerate_timer_data.cell_index    = p_cell_ctx->cell_index;
                rand_regenerate_timer_data.cell_m_event  =
                    CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED;

                RRC_CSC_TRACE(RRC_BRIEF,"CDMA: RAND value = %u ", 
                        p_cell_ctx->cdma2000_rand);

                /*Multiplying by 1000 to change the duration received in sec to
                 * msec*/
                RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_RAND_REGENERATE_TIMER] %u ms.",
                        (p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
                         cdma2000_rand.rand_regenerate_timer)*1000);

                p_cell_ctx->rand_regenerate_timer = rrc_start_timer(
                        (p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
                         cdma2000_rand.rand_regenerate_timer)*1000,
                        &rand_regenerate_timer_data, 
                        sizeof (rand_regenerate_timer_data), 
                        RRC_FALSE);

                p_uecc_cell_reconfig_ind->bitmask |=
                    RRC_CSC_UECC_CDMA2000_RAND_PRESENT;

                p_uecc_cell_reconfig_ind->cdma2000_rand = 
                    p_cell_ctx->cdma2000_rand;

                if(CDMA2000_PARAM_1XRTT_CELL_ID_PRESENCE_FLAG & 
                        p_cfg_cell_param->cdma2000_cell_param.bitmask)    
                {

                    p_uecc_cell_reconfig_ind->bitmask |=
                        RRC_CSC_UECC_CDMA2000_1XRTT_CELL_ID_PRESENT ;

                    RRC_ASSERT(sizeof(p_uecc_cell_reconfig_ind->cell_id_1xrtt) == 
                            sizeof (p_cfg_cell_param->cdma2000_cell_param.cell_id_1xrtt));

                    p_uecc_cell_reconfig_ind->cell_id_1xrtt = 
                        p_cfg_cell_param->cdma2000_cell_param.cell_id_1xrtt;
                }

                if(CDMA2000_PARAM_HRPD_SECTOR_ID_PRESENCE_FLAG & 
                        p_cfg_cell_param->cdma2000_cell_param.bitmask) 
                {
                    p_uecc_cell_reconfig_ind->bitmask |=
                        RRC_CSC_UECC_CDMA2000_HRPD_SECTOR_ID_PRESENT;

                    RRC_ASSERT(sizeof(p_uecc_cell_reconfig_ind->sector_id_hrpd) == 
                            sizeof (p_cfg_cell_param->cdma2000_cell_param.sector_id_hrpd));

                    p_uecc_cell_reconfig_ind->sector_id_hrpd =
                        p_cfg_cell_param->cdma2000_cell_param.sector_id_hrpd;
                }      

                /* CSR 00070260 Changes Start */
                if(CDMA2000_PARAM_1XRTT_REFERENCE_CELL_ID_PRESENCE_FLAG & 
                        p_cfg_cell_param->cdma2000_cell_param.bitmask) 
                {
                    p_uecc_cell_reconfig_ind->bitmask |=
                        RRC_CSC_UECC_CDMA2000_1XRTT_REF_CELL_ID_PRESENCE_FLAG;

                    RRC_ASSERT(sizeof(p_uecc_cell_reconfig_ind->ref_cell_id_1xrtt) == 
                            sizeof (p_cfg_cell_param->cdma2000_cell_param.ref_cell_id_1xrtt));

                    p_uecc_cell_reconfig_ind->ref_cell_id_1xrtt =
                        p_cfg_cell_param->cdma2000_cell_param.ref_cell_id_1xrtt;
                }

                if(CDMA2000_PARAM_HRPD_REFERENCE_CELL_ID_PRESENCE_FLAG & 
                        p_cfg_cell_param->cdma2000_cell_param.bitmask) 
                {
                    p_uecc_cell_reconfig_ind->bitmask |=
                        RRC_CSC_UECC_CDMA2000_HRPD_REF_CELL_ID_PRESENCE_FLAG;

                    RRC_ASSERT(sizeof(p_uecc_cell_reconfig_ind->ref_sector_id_hrpd) == 
                            sizeof (p_cfg_cell_param->cdma2000_cell_param.ref_sector_id_hrpd));

                    p_uecc_cell_reconfig_ind->ref_sector_id_hrpd =
                        p_cfg_cell_param->cdma2000_cell_param.ref_sector_id_hrpd;
                }
		}
		/*SPR_18384_END*/
                /* CSR 00070260 Changes Stop */
                /*Coverity CID 101178 Fix Start */      
                /* SPR 12289 Fix Start */
                rrc_send_message(p_intrl_msg, RRC_UECCMD_MODULE_ID);
            }
            /* SPR 12289 Fix Start */
            /*Coverity CID 101178 Fix End */      

        /* 2.2. Send RRC_RRM_CELL_RECONFIG_RESP */

        memset_wrapper ( &cell_recfg_resp, RRC_NULL, 
                sizeof(rrc_rrm_cell_reconfig_resp_t) );

        cell_recfg_resp.cell_index          = p_cell_ctx->cell_index;
        cell_recfg_resp.response            = RRC_SUCCESS;
        rrc_rrm_il_send_rrc_rrm_cell_reconfig_resp(
                &cell_recfg_resp,
                RRC_CSC_MODULE_ID, 
                RRC_RRM_MODULE_ID,
                p_cell_ctx->transaction_id,
                p_cell_ctx->cell_index);

        p_cell_ctx->rcfg_status_flag = RRC_FALSE;

        /* Updating the SI scheduling Info */
        update_si_scheduling_info(p_cell_ctx);

        /*Updating mod_period and sfn_gap*/
        p_cell_ctx->mod_period = csc_get_mod_period(
                &p_cell_ctx->recfg_cell_param.
                broadcast_info.sib_type_2_Info);

        p_cell_ctx->sfn_gap = csc_get_sfn_gap(
                &p_cell_ctx->recfg_cell_param.
                broadcast_info.sib_type_2_Info);

        /* eMTC changes stop */

        
        /* 2.3. Send S1AP_CELL_AREA_INFO_UPDATE_IND */

        p_intrl_msg = csc_alloc_intrl_msg( RRC_S1AP_MODULE_ID,
                S1AP_CELL_AREA_INFO_UPDATE_IND,
                sizeof (s1ap_cell_area_info_update_ind_t));
        if (PNULL != p_intrl_msg)
        {
            /* Fill and send message */
            p_s1ap_cell_area_info_update_ind = 
                (s1ap_cell_area_info_update_ind_t *)
                ((U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

            p_s1ap_cell_area_info_update_ind->cell_index = p_cell_ctx->cell_index;
            if(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG ||
                    p_cell_ctx->recfg_cell_param.presence_bitmask &
                    CELL_RECONFIG_REQ_API_EMERGENCY_AREA_ID_PRESENCE_FLAG)
            {
                if(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask &
                        CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG)
                {
                    p_s1ap_cell_area_info_update_ind->
                        cell_area_info.bitmask |=
                        S1AP_CELL_INFO_UPDATE_IND_CELL_ID_PRESENT;

                    l3_memcpy_wrapper(p_s1ap_cell_area_info_update_ind->
                            cell_area_info.
                            cell_id,
                            p_cell_ctx->recfg_cell_param.broadcast_info.
                            sib_type_1_Info.cell_access_related_info.cell_Id,
                            CELL_ID_OCTET_SIZE);

                    p_s1ap_cell_area_info_update_ind->
                        cell_area_info.bitmask |=
                        S1AP_CELL_INFO_UPDATE_IND_TAC_PRESENT;

                    l3_memcpy_wrapper(
                            p_s1ap_cell_area_info_update_ind->cell_area_info.tac,
                            p_cell_ctx->recfg_cell_param.broadcast_info.
                            sib_type_1_Info.cell_access_related_info.tac,
                            TAC_OCTET_SIZE);


                }

                if(p_cell_ctx->recfg_cell_param.presence_bitmask &
                        CELL_RECONFIG_REQ_API_EMERGENCY_AREA_ID_PRESENCE_FLAG)
                {
                    p_s1ap_cell_area_info_update_ind->cell_area_info.bitmask |=
                        S1AP_CELL_INFO_UPDATE_IND_EMERGENCY_ID_PRESENT;

                    l3_memcpy_wrapper(
                            p_s1ap_cell_area_info_update_ind->cell_area_info.
                            emergency_id,
                            p_cell_ctx->recfg_cell_param.
                            emergency_area_id,EMERGENCY_AREA_SIZE);


                }
                rrc_send_message(p_intrl_msg,RRC_S1AP_MODULE_ID);
                RRC_CSC_TRACE(RRC_INFO,
                        "S1AP_CELL_AREA_INFO_UPDATE_IND sent to S1AP");
            }
            /*SPR_18384_START*/
            else
            {
                /* coverity_fix_53637_start */
                rrc_msg_mem_free(p_intrl_msg);
                /* coverity_fix_53637_stop */
                p_intrl_msg = PNULL;
            }
            /*SPR_18384_END*/
        }
        else
        {
            RRC_CSC_TRACE(RRC_FATAL,
                    "Memory Allocation failed!");

        }
        /* STATE TRANSITION*/
        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
    }
    else
    {
        /* Rollback was going on and done successfully but reply 
           RRM with Reconfig Failure*/

        /* 2.1. Send RRM_RRC_LL_CONFIG_FAILURE */
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_LL_CONFIG_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                    "Unable to send RESP FAILURE TO RRM");
            return RRC_FAILURE;
        }
        p_cell_ctx->rcfg_status_flag = RRC_FALSE;
        p_cell_ctx->retry_count = RRC_NULL; 

        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;
        /* eMTC changes stop */

        /* STATE TRANSITION*/
        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 

        /*Check and start PWS procedure if pending*/
        if(PNULL != p_cell_ctx->p_enb_pws_req)
        {
            p_cell_ctx->p_temp_si_schdl_info = rrc_mem_get(sizeof(si_schdl_info_t));

            if ( RRC_FAILURE == check_and_update_cms_info_in_cell_ctx(
                        p_cell_ctx,
                        p_csc_gl_ctx, 
                        &guard_timer_data,
                        &cmas_key,
                        &counter_value))
            {
                return RRC_FAILURE;
            }

            if (PWS_SI_TRANSMISSION_INFO_LIST & p_cell_ctx->p_enb_pws_req->bitmask)
            {
                p_rrc_si_transmission_info_list =
                    &p_cell_ctx->p_enb_pws_req->si_transmission_info;
            }

            /* -- 2. Fill and send CSC_LLIM_PWS_REQ to LLIM*/
            p_intrl_msg = csc_alloc_intrl_msg( 
                    RRC_LLIM_MODULE_ID,
                    CSC_LLIM_PWS_REQ,
                    sizeof(rrc_csc_llim_pws_req_t));

            if (PNULL != p_intrl_msg)
            {
                rrc_csc_llim_pws_req_t *p_llim_pws_req =
                    (rrc_csc_llim_pws_req_t *)((U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

                /*Storing Transaction Id in rrc_csc_llim_pws_req_t */
                p_llim_pws_req->transaction_id = p_cell_ctx->transaction_id;

                p_llim_pws_req->cell_index = p_cell_ctx->cell_index;
                /* Store the SFN ,using SFN = SFN(curr) + OFFSET */
                p_llim_pws_req->sfn = (rrc_sfn_t)(p_cell_ctx->sfn +
                        CSC_PWS_SFN_OFFSET);

                p_llim_pws_req->sfn = (p_llim_pws_req->sfn % RRC_MAX_SFN_VALUE);
                /*-- 3. prepare llim pws request to send to csc*/
                if(RRC_FAILURE == update_mib_sib_info_in_cell_context(
                            p_cell_ctx,
                            p_csc_gl_ctx, 
                            p_llim_pws_req,
                            &guard_timer_data,
                            p_rrc_si_transmission_info_list,
                            p_intrl_msg ,
                            &cmas_key,
                            &counter_value))
                {
                    /* coverity_fix_53637_start */
                    rrc_msg_mem_free(p_intrl_msg);
                    /* coverity_fix_53637_stop */
                    return RRC_FAILURE;
                }
            }
        }
        else if ( PNULL != p_cell_ctx->p_enb_kill_req )
        {
            p_cell_ctx->p_temp_si_schdl_info = rrc_mem_get(sizeof(si_schdl_info_t));

            /*-- 4. prepare llim pws request to send to csc*/
            if(RRC_FAILURE == update_llim_pws_info_in_cell_context(
                        p_cell_ctx,
                        p_csc_gl_ctx,  
                        &guard_timer_data,
                        p_rrc_si_transmission_info_list,
                        p_intrl_msg))
            {
                return RRC_FAILURE;
            }
        }
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}
/* coverity_fix_62128_stop */

/******************************************************************************
 *   FUNCTION NAME: cell_reconfig_upon_phy_mode_timer_exp 
 *      
 *      this function update the cell information if cell reconfig fails 
 *      for  maximum retry count
 *
 *            
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
static rrc_return_et  cell_reconfig_upon_phy_mode_timer_exp(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx
        )
{
    RRC_CSC_UT_TRACE_ENTER();

    if (p_cell_ctx->retry_count < 
            p_csc_gl_ctx->max_cell_reconfig_retry_count)
    {
        /* Send RRM_RRC_LL_CONFIG_FAILURE to RRM */
        /* 2.1 Done with Maximum number of retry */
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_LL_CONFIG_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                    "Unable to send RESP FAILURE TO RRM");
            return RRC_FAILURE;
        }
    }
    else
    {
        /* 2.1 Done with Maximum number of retry */
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_UNKNOWN_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, 
                    "Unable to send RESP FAILURE TO RRM");
            return RRC_FAILURE;
        }
    }
    p_cell_ctx->rcfg_status_flag = RRC_FALSE;
    p_cell_ctx->retry_count = RRC_NULL; 

    rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
    p_cell_ctx->p_temp_si_schdl_info = PNULL;
    /* eMTC changes stop */
    /* SPR 22950 fix stop */

    if(PNULL != p_cell_ctx->p_enb_pws_req)
    {
        /*PWS was pending but cant start it now as 
         * Rollback has failed*/
        cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                RRM_RRC_UNKNOWN_FAILURE,
                p_cell_ctx);
    }
    else if(PNULL != p_cell_ctx->p_enb_kill_req)
    {
        /*KILL was pending but cant start it now as 
         * Rollback has failed*/
        RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
                "procedure has failed");
        cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
                RRC_RRM_UNKNOWN_FAILURE,
                p_cell_ctx);
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;

}

/******************************************************************************
 *   FUNCTION NAME: cell_reconfig_upon_phyMacMode_phy_timer_exp 
 *      
 *      this function update the cell information if cell reconfig fails 
 *      for  maximum retry count
 *
 *            
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
static rrc_return_et cell_reconfig_upon_phyMacMode_phy_timer_exp(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx
        )
{
    RRC_CSC_UT_TRACE_ENTER();
    if(p_cell_ctx->retry_count < 
            p_csc_gl_ctx->max_cell_reconfig_retry_count)
    {
        /* 2.1 Send Reconfig with Old Configuration */
        csc_send_llim_recfg_req_with_old_cfg(p_cell_ctx,p_csc_gl_ctx,
                PHY_ONLY);
    }
    else
    {
        /* 2.1 Done with Maximum number of retry */
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_UNKNOWN_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, 
                    "Unable to send RESP FAILURE TO RRM");
            return RRC_FAILURE;
        }
        p_cell_ctx->rcfg_status_flag = RRC_FALSE;
        p_cell_ctx->retry_count = RRC_NULL; 

        if(PNULL != p_cell_ctx->p_enb_pws_req)
        {
            /*PWS was pending but cant start it now as 
             * Rollback has failed*/
            cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                    RRM_RRC_UNKNOWN_FAILURE,
                    p_cell_ctx);
        }
        else if(PNULL != p_cell_ctx->p_enb_kill_req)
        {
            /*KILL was pending but cant start it now as 
             * Rollback has failed*/
            RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
                    "procedure has failed");
            cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
                    RRC_RRM_UNKNOWN_FAILURE,
                    p_cell_ctx);
        }

        /* eMTC changes start */
        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;
        /* eMTC changes stop */
        /* eMTC changes stop */

        /* STATE TRANSITION*/
        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: cell_reconfig_upon_mac_only_mode_timer_exp 
 *      
 *      this function update the cell information if cell reconfig fails 
 *      for  maximum retry count
 *
 *            
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
static rrc_return_et cell_reconfig_upon_mac_only_mode_timer_exp(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx
        )
{
    RRC_CSC_UT_TRACE_ENTER();

    if (RRC_TRUE == p_cell_ctx->mac_reconfig_with_sib8_segments) 
    {
        /* Send RRC_RRM_CELL_SETUP_RESP */
        if (RRC_SUCCESS !=  csc_send_rrm_cell_setup_resp( 
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_LL_CONFIG_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP TO RRM");
            return RRC_FAILURE;
        }
        p_cell_ctx->mac_reconfig_with_sib8_segments = RRC_FALSE;

        /* Delete from Cell DB and free */
        csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);
        return RRC_FAILURE;
    }

    if(p_cell_ctx->retry_count < p_csc_gl_ctx->
            max_cell_reconfig_retry_count )
    {
        /* 2.1 Send Reconfig with Old Configuration */
        csc_send_llim_recfg_req_with_old_cfg(
                p_cell_ctx,
                p_csc_gl_ctx,
                MAC_ONLY);
    }
    else
    {
        /* 2.1 Done with Maximum number of retry */
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_UNKNOWN_FAILURE))

        {
            RRC_CSC_TRACE(RRC_WARNING, 
                    "Unable to send RESP FAILURE TO RRM");
            return RRC_FAILURE;
        }

        p_cell_ctx->rcfg_status_flag = RRC_FALSE;
        p_cell_ctx->retry_count = RRC_NULL; 

        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;
        /* eMTC changes stop */
        /* SPR 22950 fix stop */

        if(PNULL != p_cell_ctx->p_enb_pws_req)
        {
            /*PWS was pending but cant start it now as 
             * Rollback has failed*/
            cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                    RRM_RRC_UNKNOWN_FAILURE,
                    p_cell_ctx);
        }
        else if(PNULL != p_cell_ctx->p_enb_kill_req)
        {
            /*KILL was pending but cant start it now as 
             * Rollback has failed*/
            RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
                    "procedure has failed");
            cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
                    RRC_RRM_UNKNOWN_FAILURE,
                    p_cell_ctx);
        }
        /* STATE TRANSITION*/
        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: cell_reconfig_upon_phyMacMode_mac_timer_exp 
 *      
 *      this function update the cell information if cell reconfig fails 
 *      for  maximum retry count
 *
 *            
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
static rrc_return_et cell_reconfig_upon_phyMacMode_mac_timer_exp(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx
        )
{   
    RRC_CSC_UT_TRACE_ENTER();

    if (p_cell_ctx->retry_count < 
            p_csc_gl_ctx->max_cell_reconfig_retry_count )
    {
        /* 2.1 Send Reconfig with Old Configuration */
        csc_send_llim_recfg_req_with_old_cfg(p_cell_ctx,p_csc_gl_ctx,
                NO_MODE);
    }
    else
    {
        /* 2.1 Done with Maximum number of retry */
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_UNKNOWN_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, 
                    "Unable to send RESP FAILURE TO RRM");
            return RRC_FAILURE;
        }

        p_cell_ctx->rcfg_status_flag = RRC_FALSE;
        p_cell_ctx->retry_count = RRC_NULL; 

        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;
        /* eMTC changes stop */
        /* SPR 22950 fix stop */

        if(PNULL != p_cell_ctx->p_enb_pws_req)
        {
            /*PWS was pending but cant start it now as 
             * Rollback has failed*/
            cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                    RRM_RRC_UNKNOWN_FAILURE,
                    p_cell_ctx);
        }
        else if(PNULL != p_cell_ctx->p_enb_kill_req)
        {
            /*KILL was pending but cant start it now as 
             * Rollback has failed*/
            RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
                    "procedure has failed");
            cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
                    RRC_RRM_UNKNOWN_FAILURE,
                    p_cell_ctx);
        }
        /* STATE TRANSITION*/
        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 

    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: cell_reconfig_phyMacMode_mac_reconfig_fail 
 *      
 *      this function update the cell information if cell reconfig fails 
 *      due to mac reconfig failure
 *            
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
static rrc_return_et cell_reconfig_phyMacMode_mac_reconfig_fail(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx
        )
{ 
    RRC_CSC_UT_TRACE_ENTER();

    if(p_cell_ctx->retry_count < MAX_RETRY_COUNT)
    {
        /* 2.1 Send Reconfig with Old Configuration */
        csc_send_llim_recfg_req_with_old_cfg(
                p_cell_ctx,
                p_csc_gl_ctx,
                PHY_ONLY);
    }
    else
    {
        /* 2.1 Done with maximum retry count */
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_UNKNOWN_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, 
                    "Unable to send RESP FAILURE TO RRM");
            return RRC_FAILURE;
        }
        p_cell_ctx->rcfg_status_flag = RRC_FALSE;
        p_cell_ctx->retry_count = RRC_NULL; 

        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;
        /* eMTC changes stop */
        /* SPR 22950 fix stop */

        if(PNULL != p_cell_ctx->p_enb_pws_req)
        {
            /*PWS was pending but cant start it now as 
             * Rollback has failed*/
            cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                    RRM_RRC_UNKNOWN_FAILURE,
                    p_cell_ctx);
        }
        else if(PNULL != p_cell_ctx->p_enb_kill_req)
        {
            /*KILL was pending but cant start it now as 
             * Rollback has failed*/
            RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
                    "procedure has failed");
            cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
                    RRC_RRM_UNKNOWN_FAILURE,
                    p_cell_ctx);
        }
        /* STATE TRANSITION*/
        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: cell_reconfig_phyMacMode_phy_reconfig_fail 
 *      
 *      this function update the cell information if cell reconfig fails 
 *      due to physical layer reconfig failure 
 *            
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/


static rrc_return_et cell_reconfig_phyMacMode_phy_reconfig_fail(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx
        )
{
    RRC_CSC_UT_TRACE_ENTER();

    if (RRC_TRUE == p_cell_ctx->mac_reconfig_with_sib8_segments) 
    {
        /* Send RRC_RRM_CELL_SETUP_RESP */
        if (RRC_SUCCESS !=  csc_send_rrm_cell_setup_resp( 
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_LL_CONFIG_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP TO RRM");
            return RRC_FAILURE;
        }
        p_cell_ctx->mac_reconfig_with_sib8_segments = RRC_FALSE;

        /* Delete from Cell DB and free */
        csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);
        return RRC_FAILURE;
    }

    if (p_cell_ctx->retry_count < MAX_RETRY_COUNT)
    {    /*2.1 Send RRM_RRC_LL_CONFIG_FAILURE to RRM */
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(  
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_LL_CONFIG_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, 
                    "Unable to send RESP FAILURE TO RRM");
            return RRC_FAILURE;
        }
        p_cell_ctx->rcfg_status_flag = RRC_FALSE;

        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;
        /* eMTC changes stop */
        /* SPR 22950 fix stop */

        if(PNULL != p_cell_ctx->p_enb_pws_req)
        {
            /*PWS was pending but cant start it now as 
             * Rollback has failed*/
            cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                    RRM_RRC_INTERNAL_FAILURE,
                    p_cell_ctx);
        }
        else if(PNULL != p_cell_ctx->p_enb_kill_req)
        {
            /*KILL was pending but cant start it now as 
             * Rollback has failed*/
            RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
                    "procedure has failed");
            cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
                    RRC_RRM_UNKNOWN_FAILURE,
                    p_cell_ctx);
        }
        /* STATE TRANSITION*/
        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
    }
    else
    {
        /* 2.1 Done with maximum retry count */
        if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index,
                    RRC_FAILURE,
                    RRM_RRC_UNKNOWN_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, 
                    "Unable to send RESP FAILURE TO RRM");
            return RRC_FAILURE;
        }
        p_cell_ctx->rcfg_status_flag = RRC_FALSE;
        p_cell_ctx->retry_count = RRC_NULL; 

        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;
        /* eMTC changes stop */
        /* SPR 22950 fix stop */

        if(PNULL != p_cell_ctx->p_enb_pws_req)
        {
            /*PWS was pending but cant start it now as 
             * Rollback has failed*/
            cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                    RRM_RRC_UNKNOWN_FAILURE,
                    p_cell_ctx);
        }
        else if(PNULL != p_cell_ctx->p_enb_kill_req)
        {
            /*KILL was pending but cant start it now as 
             * Rollback has failed*/
            RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
                    "procedure has failed");
            cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
                    RRC_RRM_UNKNOWN_FAILURE,
                    p_cell_ctx);
        }
        /* STATE TRANSITION*/
        CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index) ; 
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_reconf_cell_state_llim_resp
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function Handles the CELL RECONFIGURATION Resp from LLIM
 *                  STATE: CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP;
 *                  EVENT: CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_reconf_cell_state_llim_resp(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{

    rrc_csc_llim_reconfig_cell_resp_t *p_reconfig_cell_resp =
        (rrc_csc_llim_reconfig_cell_resp_t *)p_msg;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_reconfig_cell_resp);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(p_reconfig_cell_resp->cell_index == p_cell_ctx->cell_index);

    /* Precondition - should be the same transaction */
    if (p_reconfig_cell_resp->transaction_id != p_cell_ctx->transaction_id)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /* 1. Stop guard timer */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_RECONFIGURE_TIMER]; Unused variable p_api=%p",p_api);
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    /* 2. Handling the ResponseCode*/
    switch (p_reconfig_cell_resp->response)
    {
        case RRC_LLIM_RECONFIG_PHY_ONLY_MODE_SUCCESS :
        case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS :
        case RRC_LLIM_RECONFIG_PHY_MAC_MODE_SUCCESS :
        case RRC_LLIM_RECONFIG_MAC_RLC_MODE_SUCCESS :
            if(RRC_FAILURE == process_cell_reconfig_ind(p_cell_ctx,
                            p_csc_gl_ctx))
            {
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }
            break;

        case RRC_LLIM_RECONFIG_PHY_ONLY_MODE_TIMER_EXP:
            if(RRC_FAILURE == cell_reconfig_upon_phy_mode_timer_exp(p_cell_ctx,
                            p_csc_gl_ctx))
            {
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }
            /* STATE TRANSITION*/
            CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index); 
            break;

        case RRC_LLIM_RECONFIG_PHY_MAC_MODE_PHY_TIMER_EXP:
            if( RRC_FAILURE == cell_reconfig_upon_phyMacMode_phy_timer_exp(
                            p_cell_ctx, p_csc_gl_ctx))
            {
                RRC_CSC_UT_TRACE_EXIT();
                return; 
            }
            break;
        case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_TIMER_EXP:
            if( RRC_FAILURE == cell_reconfig_upon_mac_only_mode_timer_exp(
                        p_cell_ctx, p_csc_gl_ctx))
            {
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }
            break;
        case RRC_LLIM_RECONFIG_PHY_MAC_MODE_MAC_TIMER_EXP:
             if( RRC_FAILURE == cell_reconfig_upon_phyMacMode_mac_timer_exp(
                         p_cell_ctx, p_csc_gl_ctx))
             {
                RRC_CSC_UT_TRACE_EXIT();
                return;
             }
            break;
        case RRC_LLIM_RECONFIG_PHY_MAC_MODE_MAC_FAIL :
            if(RRC_FAILURE == cell_reconfig_phyMacMode_mac_reconfig_fail(
                        p_cell_ctx, p_csc_gl_ctx))
            {
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }
            break;
        case RRC_LLIM_RECONFIG_PHY_ONLY_MODE_FAIL :
        case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_FAIL :
        case RRC_LLIM_RECONFIG_PHY_MAC_MODE_PHY_FAIL:
            if (RRC_FAILURE == cell_reconfig_phyMacMode_phy_reconfig_fail(
                    p_cell_ctx, p_csc_gl_ctx))
            {
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }
            break;
        case RRC_LLIM_RECONFIG_MAC_RLC_MODE_RLC_FAIL :   
            if (RRC_FAILURE == cell_reconfig_mac_rlc_rlc_reconfig_fail(
                    p_cell_ctx, p_csc_gl_ctx))
            {
                RRC_CSC_UT_TRACE_EXIT();
                return;
            }
            break;

        default:
            RRC_TRACE(RRC_WARNING,"Unknown Response code from LLIM"
                    " for Cell Reconfiguration : [%d]",p_reconfig_cell_resp->response);
            break;
    }
    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_reconfig_resp_pws_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function handles write-replace warning request from S1AP.
 *                  STATE: CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP 
 *                  EVENT: CELL_M_EVENT_ENB_MGR_PWS_REQ
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_reconfig_resp_pws_req(
        void            *p_api,         /* p_api */
        void            *p_msg,         /* p_msg */
        csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
        csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
    s1ap_pws_req_t *p_enb_pws_req  =
        (s1ap_pws_req_t *)p_msg;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_CSC_TRACE(RRC_DETAILEDALL,
         "Unused variable in fsm p_api=%p", p_api);/*SPR 17777 +-*/

    RRC_ASSERT(PNULL != p_enb_pws_req);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);

    /*  Send Reconfig with Old Configuration */
    csc_send_llim_recfg_req_with_old_cfg(p_cell_ctx,p_csc_gl_ctx, NO_MODE);

    /* Check the scheduling info */
    if (SI_MAPPING_CHANGED == csc_check_pws_scheduling_info(
                p_cell_ctx,p_enb_pws_req))
    {
        cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
                RRM_RRC_INTERNAL_FAILURE,
                p_cell_ctx);
        return;
    }
    if ( PNULL != p_cell_ctx->p_temp_si_schdl_info)
    {
      p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = SI_MAPPING_NOT_CHANGED;
    }

    /*Store PWS req in cell context*/

    p_cell_ctx->p_enb_pws_req = rrc_mem_get(
            sizeof(s1ap_pws_req_t));

    if ( PNULL != p_cell_ctx->p_enb_pws_req)
    {
        l3_memcpy_wrapper(p_cell_ctx->p_enb_pws_req,
                p_enb_pws_req,
                sizeof(s1ap_pws_req_t));
    }

    RRC_CSC_UT_TRACE_EXIT();

}

/**   Cell manager FSM implementation **/

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_ignored_event
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : Handler of ignored events.
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_ignored_event(
        void                *param1,            /* p_api or timer_id */
        void                *param2,            /* p_msg or p_cell_m_timer_buf */
        csc_cell_ctx_t      *p_cell_ctx,        /* cell context */
        /* Pointer to the CSC global context data */
        csc_gl_ctx_t        *p_csc_gl_ctx)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);

    RRC_CSC_TRACE(RRC_DETAILED, "Unused variable in fsm param1=%p, param2=%p, "
            "p_csc_gl_ctx=%p", param1, param2, p_csc_gl_ctx);
    RRC_CSC_TRACE(RRC_WARNING, "[%s] ignore event received in state [%s]",
            RRC_CELL_M_FSM_NAME,
            CELL_M_FSM_STATES_NAMES[CELL_M_FSM_GET_CURRENT_STATE(p_cell_ctx)]);

    RRC_CSC_UT_TRACE_EXIT();
}
/*****************************************************************************
 *   FUNCTION NAME: csc_fill_llim_recfg_broad_nonbroad_cast_info_with_old_cfg
 *
 *       This function fills the cell Reconfiguration 
 *       broadcast and non broadcast info with old
 *       Configuration as received in CELL CONFIG from RRM
 *
 *   RETURNS:
 *       None
 *
 ***************************************************************************/
static void csc_fill_llim_recfg_broad_nonbroad_cast_info_with_old_cfg(
        csc_cell_ctx_t   *p_cell_ctx  /* csc cell context  */
)
{
    RRC_CSC_UT_TRACE_ENTER();
    /* update sib_type_1_Info_t with old values */
    p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_1_Info =
        p_cell_ctx->cfg_cell_param.sib_type_1_Info;
    p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
        CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG ;


    /* update sib_type_2_Info_t with old values */
    p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info =
        p_cell_ctx->cfg_cell_param.sib_type_2_Info;
    p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
        CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG ;

    /* update sib_type_3_Info_t with old values */
    if (p_cell_ctx->cfg_cell_param.presence_bitmask &
            CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG)
    {
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_3_Info =
            p_cell_ctx->cfg_cell_param.sib_type_3_Info;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
            CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG ;
    }

    /* update sib_type_4_Info_t with old values */
    if (p_cell_ctx->cfg_cell_param.presence_bitmask &
            CELL_SETUP_REQ_API_SIB_4_INFO_PRESENCE_FLAG)
    {
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_4_Info = 
            p_cell_ctx->cfg_cell_param.sib_type_4_Info;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
            CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG ;
    }

    /* update sib_type_5_Info_t with old values */
    if (p_cell_ctx->cfg_cell_param.presence_bitmask &
            CELL_SETUP_REQ_API_SIB_5_INFO_PRESENCE_FLAG)
    {
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_5_Info = 
            p_cell_ctx->cfg_cell_param.sib_type_5_Info;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
            CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG ;
    }

    /* update sib_type_6_Info_t with old values */
    if (p_cell_ctx->cfg_cell_param.presence_bitmask &
            CELL_SETUP_REQ_API_SIB_6_INFO_PRESENCE_FLAG)
    {
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_6_Info =
            p_cell_ctx->cfg_cell_param.sib_type_6_Info;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
            CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG ;
    }

    /* update sib_type_7_Info_t with old values */
    if (p_cell_ctx->cfg_cell_param.presence_bitmask &
            CELL_SETUP_REQ_API_SIB_7_INFO_PRESENCE_FLAG)
    {
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_7_Info = 
            p_cell_ctx->cfg_cell_param.sib_type_7_Info;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
            CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG ;
    }

    /* update sib_type_8_Info_t with old values */
    if (p_cell_ctx->cfg_cell_param.presence_bitmask &
            CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
    {
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_8_Info =
            p_cell_ctx->cfg_cell_param.sib_type_8_Info;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
            CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG ;
    }

    /* update sib_type_9_Info_t with old values */
    if (p_cell_ctx->cfg_cell_param.presence_bitmask &
            CELL_SETUP_REQ_API_SIB_9_INFO_PRESENCE_FLAG)
    {
        p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_9_Info = 
            p_cell_ctx->cfg_cell_param.sib_type_9_Info;
        p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
            CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG ;
    }

    /* update mib_Info_t with old values */
    p_cell_ctx->recfg_cell_param.broadcast_info.mib_info = 
        p_cell_ctx->cfg_cell_param.mib_info;
    p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
        CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG ;

    /* NON-BROADCAST INFO *
     * update dci_format_for_si_msgs with old values */
    p_cell_ctx->recfg_cell_param.non_broadcast_info.
    mac_reconfig_params.dci_format_for_si_msgs = 
                 p_cell_ctx->cfg_cell_param.cell_config_param.
                 mac_config.dci_format_for_si_msgs;
    p_cell_ctx->recfg_cell_param.presence_bitmask |=
                   CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG;
    p_cell_ctx->recfg_cell_param.non_broadcast_info.presence_bitmask |=
                    CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG;
    p_cell_ctx->recfg_cell_param.non_broadcast_info.
    mac_reconfig_params.bitmask |=
                    RRC_RRM_RECONFIG_DCI_FORMAT_FOR_SI_MESSAGES_PRESENT;

    /* update num_rbs_per_tbs_dci_1a with old values */
    if(p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask &
                        RRC_RRM_RBS_PER_TBS_DCI_1A_PRESENT)
    {
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.bitmask |=
                     RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1A_PRESENT;
        l3_memcpy_wrapper( &p_cell_ctx->recfg_cell_param.non_broadcast_info.
                         mac_reconfig_params.num_rbs_per_tbs_dci_1a ,
                    &p_cell_ctx->cfg_cell_param.cell_config_param.
                         mac_config.num_rbs_per_tbs_dci_1a,
                    RBS_FOR_DCI_1A_SIZE);
    }
    else if(p_cell_ctx->recfg_cell_param.non_broadcast_info.
           mac_reconfig_params.bitmask &
                RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1A_PRESENT)
    {
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.bitmask ^= 
                RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1A_PRESENT;
    }

    /* update num_rbs_per_tbs_dci_1c with old values */
    if(p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask &
                        RRC_RRM_RBS_PER_TBS_DCI_1C_PRESENT)
    {
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.bitmask |=
                     RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1C_PRESENT;
        l3_memcpy_wrapper( &p_cell_ctx->recfg_cell_param.non_broadcast_info.
                         mac_reconfig_params.num_rbs_per_tbs_dci_1c ,
                    &p_cell_ctx->cfg_cell_param.cell_config_param.
                         mac_config.num_rbs_per_tbs_dci_1c,
                    RBS_FOR_DCI_1C_SIZE);
    }
    else if(p_cell_ctx->recfg_cell_param.non_broadcast_info.
           mac_reconfig_params.bitmask &
                RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1C_PRESENT)
    {
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.bitmask ^= 
                RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1C_PRESENT;
    }

    /* update ul_sync_loss_timer with old values */
    if(p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask &
                        RRC_RRM_UL_SYNC_LOSS_TIMER_PRESENT)
    {
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.bitmask |=
                     RRC_RRM_RECONFIG_UL_SYNC_LOSS_TIMER_PRESENT;
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.ul_sync_loss_timer = 
                     p_cell_ctx->cfg_cell_param.cell_config_param.
                     mac_config.ul_sync_loss_timer;
    }
    else if(p_cell_ctx->recfg_cell_param.non_broadcast_info.
           mac_reconfig_params.bitmask &
                RRC_RRM_RECONFIG_UL_SYNC_LOSS_TIMER_PRESENT)
    {
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.bitmask ^= 
                RRC_RRM_RECONFIG_UL_SYNC_LOSS_TIMER_PRESENT;
    }

    /* update pucch_cqi_sinr_value with old values */
    if(p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask &
                       RRC_RRM_PUCCH_CQI_SINR_THRESHOLD_PRESENT)
    {
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.bitmask |=
                     RRC_RRM_RECONFIG_PUCCH_CQI_SINR_THRESHOLD_PRESENT;
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.pucch_cqi_sinr_value = 
                     p_cell_ctx->cfg_cell_param.cell_config_param.
                     mac_config.pucch_cqi_sinr_value;
    }
    else if(p_cell_ctx->recfg_cell_param.non_broadcast_info.
           mac_reconfig_params.bitmask &
                RRC_RRM_RECONFIG_PUCCH_CQI_SINR_THRESHOLD_PRESENT)
    {
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.bitmask ^= 
                RRC_RRM_RECONFIG_PUCCH_CQI_SINR_THRESHOLD_PRESENT;
    }

    /* update si_transmission_info_list with old values */
    if(p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask &
                        RRC_RRM_SI_TRANSMISSION_INFO_PRESENT)
    {
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.bitmask |=
                     RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT;
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
                         mac_reconfig_params.si_transmission_info =
                    p_cell_ctx->cfg_cell_param.cell_config_param.
                         mac_config.si_transmission_info;
    }
    else if(p_cell_ctx->recfg_cell_param.non_broadcast_info.
           mac_reconfig_params.bitmask &
                RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT)
    {
        p_cell_ctx->recfg_cell_param.non_broadcast_info.
        mac_reconfig_params.bitmask ^= 
                RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT;
    }
    RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
 *   FUNCTION NAME: csc_send_llim_recfg_req_with_old_cfg
 *   INPUT        : csc_cell_ctx_t   *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *                  rrc_reconfig_mode_et mode
 *   OUTPUT       : none
 *   DESCRIPTION  : This function sends the cell Reconfiguration 
 *                  ( PHY AND MAC ) with old
 *                  Configuration as received in CELL CONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_send_llim_recfg_req_with_old_cfg(
        csc_cell_ctx_t   *p_cell_ctx,  /* csc cell context  */
        csc_gl_ctx_t    *p_csc_gl_ctx,  /* Pointer to csc global context*/  
        rrc_reconfig_mode_et mode) /* reconfig mode */
{
	void                            *p_intrl_msg        = PNULL;
	csc_cell_m_timer_buf_t          guard_timer_data;

	RRC_CSC_UT_TRACE_ENTER();

	/* 1. Update the BROADCAST AND NON-BROADCAST INFO WITH OLD VALUES */
	csc_fill_llim_recfg_broad_nonbroad_cast_info_with_old_cfg(p_cell_ctx);


	/* PRS + */

	/* update prs_reconfig_info with old values */
	if(p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask &
			RRC_RRM_PRS_CONFIG_INFO_PRESENT)
	{
		p_cell_ctx->recfg_cell_param.non_broadcast_info.mac_reconfig_params.bitmask |=
			RRC_RRM_RECONFIG_PRS_INFO_PRESENT;
		p_cell_ctx->recfg_cell_param.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.request_type =
			p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.prs_config_info.request_type;
		if(p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.prs_config_info.bitmask & RRC_RRM_CONFIG_PRS_CONFIG_PRESENT &&
				(p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.prs_config_info.request_type == 1))
		{
			p_cell_ctx->recfg_cell_param.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.bitmask = RRC_RRM_RECONFIG_PRS_CONFIG_PRESENT;
			/* PRS_UPDATED + */
			p_cell_ctx->recfg_cell_param.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_bandwidth_info =
				p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.prs_config_info.prs_config.prs_bandwidth_info;
			/* PRS_UPDATED - */
			p_cell_ctx->recfg_cell_param.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_subframes_info =
				p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.prs_config_info.prs_config.prs_subframes_info;

			p_cell_ctx->recfg_cell_param.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_config_index_info =
				p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.prs_config_info.prs_config.prs_config_index_info;

			p_cell_ctx->recfg_cell_param.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_transmission_power_info =
				p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.prs_config_info.prs_config.prs_transmission_power_info;

			if(p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.prs_config_info.prs_config.bitmask &
					RRC_RRM_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT)
			{
				p_cell_ctx->recfg_cell_param.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_muting_config_info =
					p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.prs_config_info.prs_config.prs_muting_config_info;
			}
		}
	}
	else if(p_cell_ctx->recfg_cell_param.non_broadcast_info.mac_reconfig_params.bitmask &
			RRC_RRM_RECONFIG_PRS_INFO_PRESENT)
	{
		p_cell_ctx->recfg_cell_param.non_broadcast_info.mac_reconfig_params.bitmask ^=
			RRC_RRM_RECONFIG_PRS_INFO_PRESENT;
	}
	/* PRS - */
	/* 1. Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_RECONFIGURE_TIMER]");
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	/* 2. set guard timer */
	guard_timer_data.auto_delete        = RRC_TRUE;
	guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
	guard_timer_data.cell_index         = p_cell_ctx->cell_index;
	guard_timer_data.cell_m_event       =
		CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED;

	RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_RECONFIG_TIMER] %u ms.",
			p_csc_gl_ctx->cell_recfg_duration);

	p_cell_ctx->guard_timer = rrc_start_timer(p_csc_gl_ctx->cell_recfg_duration,
			&guard_timer_data, sizeof(guard_timer_data), RRC_FALSE);

	/* 3. send CSC_LLIM_RECONFIG_CELL_REQ */
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_LLIM_MODULE_ID,
			CSC_LLIM_RECONFIG_CELL_REQ,
			sizeof(rrc_csc_llim_reconfig_cell_req_t));
	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		rrc_csc_llim_reconfig_cell_req_t *p_llim_recfg_cell_req =
			(rrc_csc_llim_reconfig_cell_req_t *)((U8 *)p_intrl_msg +
					RRC_API_HEADER_SIZE);

		/*Storing Transaction Id and Cell index in 
		  rrc_csc_llim_reconfig_cell_req_t */
		p_llim_recfg_cell_req->transaction_id     = p_cell_ctx->transaction_id;
		p_llim_recfg_cell_req->cell_index         = p_cell_ctx->cell_index;

		/*Storing the cell Reconfig(ONGOING) parameter in 
		  "rrc_csc_llim_reconfig_cell_req_t"*/
		p_llim_recfg_cell_req->cell_reconfig.presence_bitmask = 0;
		if ( CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG & 
				p_cell_ctx->recfg_cell_param.presence_bitmask)
		{
			p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
				CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG;
			p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask =
				p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask;
			if ( CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG &
					p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
			{
				p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mib_info =
					p_cell_ctx->recfg_cell_param.broadcast_info.mib_info;
			}
			if ( CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG &
					p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
			{
				p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_1_Info =
					p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_1_Info;
			}
			if ( CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG &
					p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
			{
				p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_2_Info =
					p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info;
			}
		}
		if ( CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG & 
				p_cell_ctx->recfg_cell_param.presence_bitmask)
		{
			p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= 
				CELL_RECONFIG_REQ_NON_BROADCAST_INFO_PRESENCE_FLAG;
			p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info =
				p_cell_ctx->recfg_cell_param.non_broadcast_info;
		}
        /*SPR_19066_START*/
        /*Code Removed*/
        /*SPR_19066_END*/
		if ( CELL_RECONFIG_REQ_API_CONTENTION_FREE_RACH_TIMER_PRESENT & 
				p_cell_ctx->recfg_cell_param.presence_bitmask)
		{
			p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= 
				CELL_RECONFIG_REQ_CONTENTION_FREE_RACH_TIMER_PRESENCE_FLAG;
			p_llim_recfg_cell_req->cell_reconfig.contention_free_rach_timer =
				p_cell_ctx->recfg_cell_param.contention_free_rach_timer;
		}


		/*Storing the reconfig Mode*/
		p_llim_recfg_cell_req->mode = mode;

		/* ALERT : Send the Next SFN */
		p_llim_recfg_cell_req->sfn = (rrc_sfn_t)(p_cell_ctx->sfn + 
				p_csc_gl_ctx->sfn_offset);

		/* coverity_fix_64234&64235_start */
		if(PNULL != p_cell_ctx->p_temp_si_schdl_info)
		{
			l3_memcpy_wrapper(p_cell_ctx->p_temp_si_schdl_info, &p_cell_ctx->si_schdl_info,
					sizeof (si_schdl_info_t));
		}
		csc_get_new_encoded_sibs_info(p_cell_ctx); 

		if(PNULL != p_cell_ctx->p_temp_si_schdl_info)
		{
			/* Old configuration should go with SI_MAPPING_NOT_CHANGED*/
			p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = 
				SI_MAPPING_NOT_CHANGED;
		}
		/* coverity_fix_64234&64235_stop */
		/* Check and fill MIB, SIB1 and SIs info */
		if (RRC_SUCCESS == bch_m_mib_sib1_sis_recfg(p_llim_recfg_cell_req,
					&(p_cell_ctx->recfg_cell_param),p_cell_ctx))
		{

			rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);

			/* Increment the retry count */
			p_cell_ctx->retry_count++;
		}
		else
		{
			/* Bad incoming info */
			RRC_CSC_TRACE(RRC_ERROR, 
					"Bad message - UNABLE TO ENCODE SI/SIB AND MIBs.");
			if (p_intrl_msg)
			{
				/* CSC didn't send message - free memory */
				rrc_msg_mem_free(p_intrl_msg);
			}

			/* Stop guard timer */
			if (PNULL != p_cell_ctx->guard_timer)
			{
				rrc_stop_timer(p_cell_ctx->guard_timer);
				p_cell_ctx->guard_timer = PNULL;
			}
			/* . Send RRC_RRM_CELL_RECONFIG_RESP */
			if ( RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
						p_cell_ctx->transaction_id,
						p_cell_ctx->cell_index,
						RRC_FAILURE, 
						RRM_RRC_INTERNAL_FAILURE))
			{
				RRC_CSC_TRACE(RRC_WARNING, 
						"Unable to send RESP FAILURE TO RRM");
				RRC_CSC_UT_TRACE_EXIT();
				return;
			}
			p_cell_ctx->rcfg_status_flag = RRC_FALSE;
		}
	}
	/* In case of troubles rollback all changes */
	else
	{
		RRC_CSC_TRACE(RRC_FATAL, "UNABLE TO ALLOCATE MEMORY ");
		/* Stop guard timer */
		if (PNULL != p_cell_ctx->guard_timer)
		{
			rrc_stop_timer(p_cell_ctx->guard_timer);
			p_cell_ctx->guard_timer = PNULL;
		}    
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
 *   FUNCTION NAME: csc_send_llim_recfg_req_with_old_cfg_pws
 *   INPUT        : csc_cell_ctx_t   *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *                  rrc_reconfig_mode_et mode
 *   OUTPUT       : none
 *   DESCRIPTION  : This function sends the cell Reconfiguration 
 *                  for MAC with old
 *                  Configuration as received in CELL CONFIG from RRM 
 *                  as timer has expired while PWS was on going at CSC.
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_send_llim_recfg_req_with_old_cfg_pws(
		csc_cell_ctx_t   *p_cell_ctx,  /* csc cell context  */
		csc_gl_ctx_t    *p_csc_gl_ctx,  /* Pointer to csc global context*/  
		rrc_reconfig_mode_et mode) /* reconfig mode */
{
	void                            *p_intrl_msg        = PNULL;
	csc_cell_m_timer_buf_t          guard_timer_data;
	RRC_CSC_UT_TRACE_ENTER();

	/* 1. Update the BROADCAST AND NON-BROADCAST INFO WITH OLD VALUES */

	/* update sib_type_1_Info_t with old values */
	p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_1_Info =
		p_cell_ctx->cfg_cell_param.sib_type_1_Info;
	p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
		CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG ;


	/* update sib_type_2_Info_t with old values */
	p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info =
		p_cell_ctx->cfg_cell_param.sib_type_2_Info;
	p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
		CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG ;

	/* update sib_type_3_Info_t with old values */
	if (p_cell_ctx->cfg_cell_param.presence_bitmask &
			CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG)
	{
		p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_3_Info =
			p_cell_ctx->cfg_cell_param.sib_type_3_Info;
		p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
			CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG ;
	}

	/* update sib_type_4_Info_t with old values */
	if (p_cell_ctx->cfg_cell_param.presence_bitmask &
			CELL_SETUP_REQ_API_SIB_4_INFO_PRESENCE_FLAG)
	{
		p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_4_Info = 
			p_cell_ctx->cfg_cell_param.sib_type_4_Info;
		p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
			CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG ;
	}

	/* update sib_type_5_Info_t with old values */
	if (p_cell_ctx->cfg_cell_param.presence_bitmask &
			CELL_SETUP_REQ_API_SIB_5_INFO_PRESENCE_FLAG)
	{
		p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_5_Info = 
			p_cell_ctx->cfg_cell_param.sib_type_5_Info;
		p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
			CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG ;
	}

	/* update sib_type_6_Info_t with old values */
	if (p_cell_ctx->cfg_cell_param.presence_bitmask &
			CELL_SETUP_REQ_API_SIB_6_INFO_PRESENCE_FLAG)
	{
		p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_6_Info =
			p_cell_ctx->cfg_cell_param.sib_type_6_Info;
		p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
			CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG ;
	}

	/* update sib_type_7_Info_t with old values */
	if (p_cell_ctx->cfg_cell_param.presence_bitmask &
			CELL_SETUP_REQ_API_SIB_7_INFO_PRESENCE_FLAG)
	{
		p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_7_Info = 
			p_cell_ctx->cfg_cell_param.sib_type_7_Info;
		p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
			CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG ;
	}

	/* update sib_type_8_Info_t with old values */
	if (p_cell_ctx->cfg_cell_param.presence_bitmask &
			CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
	{
		p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_8_Info =
			p_cell_ctx->cfg_cell_param.sib_type_8_Info;
		p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
			CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG ;
	}

	/* update sib_type_9_Info_t with old values */
	if (p_cell_ctx->cfg_cell_param.presence_bitmask &
			CELL_SETUP_REQ_API_SIB_9_INFO_PRESENCE_FLAG)
	{
		p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_9_Info = 
			p_cell_ctx->cfg_cell_param.sib_type_9_Info;
		p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
			CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG ;
	}

	/* NON-BROADCAST INFO *
	 * update si_transmission_info_list with old values */
	if(p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask &
			RRC_RRM_SI_TRANSMISSION_INFO_PRESENT)
	{
		p_cell_ctx->recfg_cell_param.non_broadcast_info.
			mac_reconfig_params.bitmask |=
			RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT;
		p_cell_ctx->recfg_cell_param.non_broadcast_info.
			mac_reconfig_params.si_transmission_info =
			p_cell_ctx->cfg_cell_param.cell_config_param.
			mac_config.si_transmission_info;
	}
	else if(p_cell_ctx->recfg_cell_param.non_broadcast_info.
			mac_reconfig_params.bitmask &
			RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT)
	{
		p_cell_ctx->recfg_cell_param.non_broadcast_info.
			mac_reconfig_params.bitmask ^= 
			RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT;
	}
	/* 2. set guard timer */
	guard_timer_data.auto_delete        = RRC_TRUE;
	guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
	guard_timer_data.cell_index         = p_cell_ctx->cell_index;
	guard_timer_data.cell_m_event       =
		CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED;

	RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_RECONFIG_TIMER] %u ms.",
			p_csc_gl_ctx->cell_recfg_duration);

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
	p_cell_ctx->guard_timer = rrc_start_timer(p_csc_gl_ctx->cell_recfg_duration,
			&guard_timer_data, sizeof(guard_timer_data), RRC_FALSE);

	/* 3. send CSC_LLIM_RECONFIG_CELL_REQ */
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_LLIM_MODULE_ID,
			CSC_LLIM_RECONFIG_CELL_REQ,
			sizeof(rrc_csc_llim_reconfig_cell_req_t));
	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		rrc_csc_llim_reconfig_cell_req_t *p_llim_recfg_cell_req =
			(rrc_csc_llim_reconfig_cell_req_t *)((U8 *)p_intrl_msg +
					RRC_API_HEADER_SIZE);

		/*Storing Transaction Id and Cell index in 
		  rrc_csc_llim_reconfig_cell_req_t */
		p_llim_recfg_cell_req->transaction_id     = p_cell_ctx->transaction_id;
		p_llim_recfg_cell_req->cell_index         = p_cell_ctx->cell_index;

		/*Storing the cell Reconfig(ONGOING) parameter in 
		  "rrc_csc_llim_reconfig_cell_req_t"*/
		p_llim_recfg_cell_req->cell_reconfig.presence_bitmask = 0;
		if ( CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG & 
				p_cell_ctx->recfg_cell_param.presence_bitmask)
		{
			p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |=
				CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG;
			p_llim_recfg_cell_req->cell_reconfig.broadcast_info.presence_bitmask =
				p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask;
			if ( CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG &
					p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
			{
				p_llim_recfg_cell_req->cell_reconfig.broadcast_info.mib_info =
					p_cell_ctx->recfg_cell_param.broadcast_info.mib_info;
			}
			if ( CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG &
					p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
			{
				p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_1_Info =
					p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_1_Info;
			}
			if ( CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG &
					p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask)
			{
				p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib_type_2_Info =
					p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info;
			}
		}
		if ( CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG & 
				p_cell_ctx->recfg_cell_param.presence_bitmask)
		{
			p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= 
				CELL_RECONFIG_REQ_NON_BROADCAST_INFO_PRESENCE_FLAG;
			p_llim_recfg_cell_req->cell_reconfig.non_broadcast_info =
				p_cell_ctx->recfg_cell_param.non_broadcast_info;
		}
        /*SPR_19066_START*/
        /*Code Removed*/
        /*SPR_19066_END*/
		if ( CELL_RECONFIG_REQ_API_CONTENTION_FREE_RACH_TIMER_PRESENT & 
				p_cell_ctx->recfg_cell_param.presence_bitmask)
		{
			p_llim_recfg_cell_req->cell_reconfig.presence_bitmask |= 
				CELL_RECONFIG_REQ_CONTENTION_FREE_RACH_TIMER_PRESENCE_FLAG;
			p_llim_recfg_cell_req->cell_reconfig.contention_free_rach_timer =
				p_cell_ctx->recfg_cell_param.contention_free_rach_timer;
		}


		/*Storing the reconfig Mode*/
		p_llim_recfg_cell_req->mode = mode;

		/* ALERT : Send the Next SFN */
		p_llim_recfg_cell_req->sfn = (rrc_sfn_t)(p_cell_ctx->sfn + 
				p_csc_gl_ctx->sfn_offset);

		csc_get_new_encoded_sibs_info(p_cell_ctx); 

		/* Old configuration should go with SI_MAPPING_NOT_CHANGED*/
		p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = 
			SI_MAPPING_NOT_CHANGED;

		/* Check and fill MIB, SIB1 and SIs info */
		if (RRC_SUCCESS == bch_m_mib_sib1_sis_recfg(p_llim_recfg_cell_req,
					&(p_cell_ctx->recfg_cell_param),p_cell_ctx))
		{

			rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);

			/* Increment the retry count */
			p_cell_ctx->retry_count++;
		}
		else
		{
			/* Bad incoming info */
			RRC_CSC_TRACE(RRC_ERROR, 
					"Bad message - UNABLE TO ENCODE SI/SIB AND MIBs.");
			if (p_intrl_msg)
			{
				/* CSC didn't send message - free memory */
				rrc_msg_mem_free(p_intrl_msg);
			}

			/* Stop guard timer */
			if (PNULL != p_cell_ctx->guard_timer)
			{
				rrc_stop_timer(p_cell_ctx->guard_timer);
				p_cell_ctx->guard_timer = PNULL;
			}
			/*Send PWS failure to S1AP*/

			cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
					RRM_RRC_INTERNAL_FAILURE,
					p_cell_ctx);
		}
	}
	/* In case of troubles rollback all changes */
	else
	{
		RRC_CSC_TRACE(RRC_FATAL, "UNABLE TO ALLOCATE MEMORY ");
		/* Stop guard timer */
		if (PNULL != p_cell_ctx->guard_timer)
		{
			rrc_stop_timer(p_cell_ctx->guard_timer);
			p_cell_ctx->guard_timer = PNULL;
		}    
	}
	RRC_CSC_UT_TRACE_EXIT();
}



/*******************************************************************************
 *   FUNCTION NAME: csc_update_broadcast_info
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function updates(fills) the SIBs info according to new SI
 *                  MAPPING
 *                  --First check whether SIB is present in NEW SI Mapping
 *                  --if found, then check whether RRM has sent that SIB ?
 *                  --if sent , then OK
 *                  --if not sent by RRM then update(copy) with current 
 *                  value of that SIB
 *   RETURNS      : None
 ******************************************************************************/
static void csc_update_broadcast_info(
		csc_cell_ctx_t  *p_cell_ctx)    /*Cell context*/
{
	RRC_CSC_UT_TRACE_ENTER();
	/* Check whether SIB_2 is present in original Reconfig bitmask */
	if (!(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG))
	{
		/* Not sent by RRM,update required */   
		p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
			CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG;
		p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_2_Info = 
			p_cell_ctx->cfg_cell_param.sib_type_2_Info;
	}
	/* update sib_type_3_Info_t */
	/* Check whether SIB_3 is present in new SI_MAPPING */
	if (p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask 
			& CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG)
	{
		/* Check whether SIB_3 is present in original Reconfig bitmask */
		if (!(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
					& CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG))
		{
			/* Not sent by RRM,update required */   
			p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
				CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG;
			p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_3_Info =
				p_cell_ctx->cfg_cell_param.sib_type_3_Info;
		}
	}
	/* update sib_type_4_Info_t */
	/* Check whether SIB_4 is present in new SI_MAPPING */
	if (p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask 
			& CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG)
	{
		/* Check whether SIB_4 is present in original Reconfig bitmask */
		if (!(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
					& CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG))
		{
			/* Not sent by RRM,update required */   
			p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
				CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG;
			p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_4_Info = 
				p_cell_ctx->cfg_cell_param.sib_type_4_Info;
		}
	}
	/* update sib_type_5_Info_t */
	/* Check whether SIB_5 is present in new SI_MAPPING */
	if (p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask 
			& CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG)
	{
		/* Check whether SIB_1 is present in original Reconfig bitmask */
		if (!(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
					& CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG))
		{
			/* Not sent by RRM,update required */   
			p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
				CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG;
			p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_5_Info =
				p_cell_ctx->cfg_cell_param.sib_type_5_Info;
		}
	}
	/* update sib_type_6_Info_t */
	/* Check whether SIB_6 is present in new SI_MAPPING */
	if (p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask 
			& CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG)
	{
		/* Check whether SIB_6 is present in original Reconfig bitmask */
		if (!(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
					& CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG))
		{
			/* Not sent by RRM,update required */   
			p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
				CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG;
			p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_6_Info = 
				p_cell_ctx->cfg_cell_param.sib_type_6_Info;
		}
	}
	/* update sib_type_7_Info_t */
	/* Check whether SIB_7 is present in new SI_MAPPING */
	if (p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask 
			& CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG)
	{
		/* Check whether SIB_7 is present in original Reconfig bitmask */
		if (!(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
					& CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG))
		{
			/* Not sent by RRM,update required */   
			p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
				CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG;
			p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_7_Info = 
				p_cell_ctx->cfg_cell_param.sib_type_7_Info;
		}
	}
	/* update sib_type_8_Info_t */
	/* Check whether SIB_8 is present in new SI_MAPPING */
	if (p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask 
			& CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
	{
		/* Check whether SIB_8 is present in original Reconfig bitmask */
		if (!(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
					& CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG))
		{
			/* Not sent by RRM,update required */   
			p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
				CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG;
			p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_8_Info =
				p_cell_ctx->cfg_cell_param.sib_type_8_Info;
		}
	}
	/* update sib_type_9_Info_t */
	/* Check whether SIB_9 is present in new SI_MAPPING */
	if (p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask 
			& CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG)
	{
		/* Check whether SIB_9 is present in original Reconfig bitmask */
		if(!(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
					& CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG))
		{
			/* Not sent by RRM,update required */   
			p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
				CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG;
			p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_9_Info = 
				p_cell_ctx->cfg_cell_param.sib_type_9_Info;
		}
	}
#ifdef LTE_EMBMS_SUPPORTED
	/* update sib_type_13_Info_t */
	/* Check whether SIB_13 is present in new SI_MAPPING */
	if (p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask 
			& CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
	{
		/* Check whether SIB_13 is present in original Reconfig bitmask */
		if(!(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
					& CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG))
		{
			/* Not sent by RRM,update required */   
			p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask |=
				CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG;
			p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_13_info = 
				p_cell_ctx->cfg_cell_param.sib_type_13_Info;
		}
	}
#endif
	RRC_CSC_UT_TRACE_EXIT();
}
/*****************************************************************************
 * *   FUNCTION NAME: csc_update_cell_config
 * *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *     OUTPUT       : none
 * *   DESCRIPTION  : This function Update the cell configuration(PHY AND MAC ) with new
 * *                  Configuration as received in CELL RECONFIG from RRM
 * *   RETURNS      : None
 * *
 * ****************************************************************************/
static void csc_update_cell_config(   
		csc_cell_ctx_t  *p_cell_ctx) /* cell context*/
{
     /* SPR 18480 FIX Start */
#ifdef LTE_EMBMS_SUPPORTED
        U8    count = RRC_ZERO;
        U8    area_count = RRC_ZERO;
        U8    num_reserved_cell = RRC_ZERO;
        U8    area_id;
        U8    index = RRC_ZERO;
#endif
     /* SPR 18480 FIX End */
	RRC_CSC_UT_TRACE_ENTER();
	if (p_cell_ctx->recfg_cell_param.presence_bitmask & 
			CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG)
	{

		/* update sib_type_1_Info_t */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG)
		{
			csc_update_sib_type_1_info(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.broadcast_info);
		}

		/* update sib_type_2_Info_t */
		if(p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG)
		{
			csc_update_sib_type_2_info(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.broadcast_info);
		}

		/* update sib_type_3_Info_t */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG)
		{
			/* BZ_371 Fix Start */
			p_cell_ctx->cfg_cell_param.presence_bitmask |= 
				CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG;
			csc_update_sib_type_3_info(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.broadcast_info);
			/* BZ_371 Fix Stop */
		}

		/* update sib_type_4_Info_t */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG)
		{
			/* BZ_371 Fix Start */
			p_cell_ctx->cfg_cell_param.presence_bitmask |= 
				CELL_SETUP_REQ_API_SIB_4_INFO_PRESENCE_FLAG;
			/* BZ_371 Fix Stop */
			csc_update_sib_type_4_info(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.broadcast_info);
		}

		/* update sib_type_5_Info_t */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG)
		{
			/* BZ_371 Fix Start */
			p_cell_ctx->cfg_cell_param.presence_bitmask |= 
				CELL_SETUP_REQ_API_SIB_5_INFO_PRESENCE_FLAG;
			/* BZ_371 Fix Stop */
			csc_update_sib_type_5_info(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.broadcast_info);
		}                                                
		/* update sib_type_6_Info_t */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG)
		{
			/* BZ_371 Fix Start */
			p_cell_ctx->cfg_cell_param.presence_bitmask |= 
				CELL_SETUP_REQ_API_SIB_6_INFO_PRESENCE_FLAG;
			/* BZ_371 Fix Stop */
			csc_update_sib_type_6_info(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.broadcast_info);
		}

		/* update sib_type_7_Info_t */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG)
		{
			/* BZ_371 Fix Start */
			p_cell_ctx->cfg_cell_param.presence_bitmask |= 
				CELL_SETUP_REQ_API_SIB_7_INFO_PRESENCE_FLAG;
			/* BZ_371 Fix Stop */
			csc_update_sib_type_7_info(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.broadcast_info);
		}

		/* update sib_type_8_Info_t */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
		{
			/* BZ_371 Fix Start */
			p_cell_ctx->cfg_cell_param.presence_bitmask |= 
				CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG;
			/* BZ_371 Fix Stop */
			csc_update_sib_type_8_info(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.broadcast_info);
		}

		/* update sib_type_9_Info_t */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG)
		{
			/* BZ_371 Fix Start */
			p_cell_ctx->cfg_cell_param.presence_bitmask |= 
				CELL_SETUP_REQ_API_SIB_9_INFO_PRESENCE_FLAG;
			/* BZ_371 Fix Stop */
			csc_update_sib_type_9_info(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.broadcast_info);
		}
#ifdef LTE_EMBMS_SUPPORTED
		/* update sib_type_13_Info_t */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
		{
			/* BZ_371 Fix Start */
			p_cell_ctx->cfg_cell_param.presence_bitmask |= 
				CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG;
			/* BZ_371 Fix Stop */
			csc_update_sib_type_13_info(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.broadcast_info);
		}
        
		if (p_cell_ctx->recfg_cell_param.presence_bitmask 
				& CELL_RECONFIG_REQ_API_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG)
		{
			p_cell_ctx->cfg_cell_param.presence_bitmask |= 
				CELL_SETUP_REQ_API_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG;
            p_cell_ctx->cfg_cell_param.mbms_transmission_power = 
                p_cell_ctx->recfg_cell_param.mbms_transmission_power;
        }
        /* SPR 18672 Fix Start */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG)
		{
			p_cell_ctx->cfg_cell_param.presence_bitmask |= 
				CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG;
            p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list =
                 p_cell_ctx->recfg_cell_param.broadcast_info.
                 m2ap_reserved_mcch_related_bcch_info_list;
		}
        /* SPR 18672 Fix End */

        /* SPR 18446 Fix Start */
        else
		{
            if((p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask & 
                        CELL_RECONFIG_REQ_API_MBMS_AREA_INFO_LIST_FLAG) && (p_cell_ctx->cfg_cell_param.presence_bitmask &
                            CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG)) 
            {
                /* update reserved cell info in cell ctx */
                num_reserved_cell = p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list.count;
                for(index = 0; index < num_reserved_cell; index++)
                {
                    for(count = 0; count < p_cell_ctx->
                            recfg_cell_param.broadcast_info.mbms_updated_area_info_list.
                            num_valid_mbsfn_area_info; count++)
                    {
                        area_id = p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list.
                            m2ap_reserved_cell_mcch_info[index].area_id;
                        if(( area_id == p_cell_ctx->
                                    recfg_cell_param.broadcast_info.mbms_updated_area_info_list.
                                    mbms_updated_area_info[count].area_id) && (p_cell_ctx->
                                        recfg_cell_param.broadcast_info.mbms_updated_area_info_list.
                                        mbms_updated_area_info[count].flag == DELETED))
                        {
                            /* check if last entry */
                            if(area_id == p_cell_ctx->cfg_cell_param.
                                    m2ap_reserved_mcch_related_bcch_info_list.
                                    m2ap_reserved_cell_mcch_info[num_reserved_cell - 1].area_id)
                            {
                                p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list.
                                    m2ap_reserved_cell_mcch_info[num_reserved_cell - 1].area_id = 0;
                                p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list.count--;      
                            }
                            else
                            {
                                /* area id lies in middle so move last entry at that position */
                                p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list.
                                    m2ap_reserved_cell_mcch_info[index].area_id = 
                                    p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list.
                                    m2ap_reserved_cell_mcch_info[num_reserved_cell - 1].area_id;
                                p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list.
                                    m2ap_reserved_cell_mcch_info[num_reserved_cell - 1].area_id = 0;
                                p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list.count--;      

                            }
                            break;    
                        }
                    }

                }
                if(0 == p_cell_ctx->cfg_cell_param.m2ap_reserved_mcch_related_bcch_info_list.count)
                {
			p_cell_ctx->cfg_cell_param.presence_bitmask &= 
				~CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG;
		}
            }
        }
        /* SPR 18446 Fix End */
        /* SPR 18093 Fix Start */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_MBMS_AREA_INFO_LIST_FLAG)
		{
            p_cell_ctx->cfg_cell_param.mbms_updated_area_info_list.num_valid_mbsfn_area_info = 0;
			p_cell_ctx->cfg_cell_param.presence_bitmask |= 
				CELL_SETUP_REQ_API_EMBMS_AREA_INFO_LIST_PRESENCE_FLAG;
            /* SPR 18480 FIX Start */
            /* before storing mbms_updated_area_info_list, we will modify this list to exclude the delete areas,
             * so that delete req  for same not sent again */
            for(count = RRC_ZERO; count < p_cell_ctx->recfg_cell_param.broadcast_info.
                    mbms_updated_area_info_list.num_valid_mbsfn_area_info; count++)
            {
                if(p_cell_ctx->recfg_cell_param.broadcast_info.
                    mbms_updated_area_info_list.mbms_updated_area_info[count].flag != DELETED)
                {
                    p_cell_ctx->cfg_cell_param.mbms_updated_area_info_list.mbms_updated_area_info[area_count] = 
                 p_cell_ctx->recfg_cell_param.broadcast_info.
                        mbms_updated_area_info_list.mbms_updated_area_info[count];
                    area_count++;
                    p_cell_ctx->cfg_cell_param.mbms_updated_area_info_list.num_valid_mbsfn_area_info = area_count;
                }
            }
            /* SPR 18480 FIX End */
		}
        /* SPR 18093 Fix End */

#endif

		/* update mib info */
		if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask 
				& CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG )
		{
			csc_update_mib_info(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.broadcast_info);
		}
		if (p_cell_ctx->recfg_cell_param.presence_bitmask 
				& CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG)
		{
			/* Update cell configuration parameters */
			csc_update_phy_channel_config(
					&p_cell_ctx->cfg_cell_param, 
					&p_cell_ctx->recfg_cell_param.non_broadcast_info);
		}
	}
	if (p_cell_ctx->recfg_cell_param.presence_bitmask
			& CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG)
	{
		/* Update mac parameters */
		csc_update_mac_config(
				&p_cell_ctx->cfg_cell_param,
				&p_cell_ctx->recfg_cell_param.non_broadcast_info);
	}
	if (p_cell_ctx->recfg_cell_param.presence_bitmask 
			& CELL_RECONFIG_REQ_API_CDMA2000_CELL_PARAM_PRESENT )
	{
		/*UPDATE cdma2000 parameters */
		csc_update_cdma2000_cell_param(
				p_cell_ctx,
				&p_cell_ctx->recfg_cell_param.cdma2000_cell_param);
	}
#ifdef LTE_EMBMS_SUPPORTED
	if (p_cell_ctx->recfg_cell_param.presence_bitmask 
			& CELL_RECONFIG_REQ_API_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG )
    {
        p_cell_ctx->cfg_cell_param.mbms_transmission_power = 
            p_cell_ctx->recfg_cell_param.mbms_transmission_power;     
    }
#endif
}
/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_1_info
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_config_req
 *                  broadcast_config_info_t    *p_broadcast_info
 *   OUTPUT       : none
 *   DESCRIPTION  : This function Update the cell configuration(sib_type_1_info_t) with new
 *                  Configuration as received in CELL RECONFIG from RRM
 *    RETURNS     : None
 *
 *****************************************************************************/
static void csc_update_sib_type_1_info(
		rrc_rrm_cell_setup_req_t   *p_cell_config_req,
		broadcast_config_info_t    *p_broadcast_info)
{
	U8 list_count = 0;
	U8 mnc_count = 0;
	RRC_CSC_UT_TRACE_ENTER();

	/* 1. Fill cell access related info */
	/* 1.1. Fill Tracking area code info */
	l3_memcpy_wrapper(p_cell_config_req->sib_type_1_Info.cell_access_related_info.tac, 
			p_broadcast_info->sib_type_1_Info.cell_access_related_info.tac, 
			sizeof (p_cell_config_req->sib_type_1_Info.cell_access_related_info.
				tac));

	/* 1.2. Fill the cell id */
	l3_memcpy_wrapper(p_cell_config_req->sib_type_1_Info.cell_access_related_info.
			cell_Id, p_broadcast_info->sib_type_1_Info.cell_access_related_info.
			cell_Id, sizeof (p_cell_config_req->sib_type_1_Info.
				cell_access_related_info.cell_Id));

	/* 1.3.Fill Cell bared info */
	p_cell_config_req->sib_type_1_Info.cell_access_related_info.cell_barred = 
		p_broadcast_info->sib_type_1_Info.cell_access_related_info.cell_barred;

	/* 1.4. Fill inter frequency reselection */
	p_cell_config_req->sib_type_1_Info.cell_access_related_info.
		intra_freq_reselection = p_broadcast_info->sib_type_1_Info.
		cell_access_related_info.intra_freq_reselection;

	/* 1.5. Fill cell specific group indication */
	p_cell_config_req->sib_type_1_Info.cell_access_related_info.csg_indication =
		p_broadcast_info->sib_type_1_Info.cell_access_related_info.csg_indication;

	/* 1.6 Cell speficific identity*/
	if (p_broadcast_info->sib_type_1_Info.cell_access_related_info.
			presence_bitmask & CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG)
	{
		p_cell_config_req->sib_type_1_Info.cell_access_related_info.
			presence_bitmask |= 
			CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG;

		l3_memcpy_wrapper(p_cell_config_req->sib_type_1_Info.cell_access_related_info.
				csg_identity, p_broadcast_info->sib_type_1_Info.
				cell_access_related_info.csg_identity, sizeof (p_cell_config_req->
					sib_type_1_Info.cell_access_related_info.csg_identity));    
	}
	/* 1.7. Fill PLMN ID info List*/
	p_cell_config_req->sib_type_1_Info.cell_access_related_info.
		plmn_Id_info_list.count = 
		p_broadcast_info->sib_type_1_Info.cell_access_related_info.
		plmn_Id_info_list.count;

	for (list_count = 0; list_count < p_cell_config_req->
			sib_type_1_Info.cell_access_related_info.
			plmn_Id_info_list.count; list_count++)
	{
		if (p_broadcast_info->sib_type_1_Info.cell_access_related_info.
				plmn_Id_info_list.plmn_identity_info[list_count].
				plmn_identity.presence_bitmask & PLMN_IDENTITY_MCC_PRESENCE_FLAG)
		{       
			/*Set the bitmask for MCC in cell config */
			p_cell_config_req->sib_type_1_Info.cell_access_related_info.
				plmn_Id_info_list.plmn_identity_info[list_count].plmn_identity.
				presence_bitmask |= 
				PLMN_IDENTITY_MCC_PRESENCE_FLAG;

			l3_memcpy_wrapper(p_cell_config_req->sib_type_1_Info.cell_access_related_info.
					plmn_Id_info_list.plmn_identity_info[list_count].plmn_identity.mcc,
					p_broadcast_info->sib_type_1_Info.cell_access_related_info.
					plmn_Id_info_list.plmn_identity_info[list_count].plmn_identity.mcc,
					sizeof (p_cell_config_req->sib_type_1_Info.cell_access_related_info.
						plmn_Id_info_list.plmn_identity_info[list_count].
						plmn_identity.mcc));
		}
		/*Filling of MNC */
		p_cell_config_req->sib_type_1_Info.cell_access_related_info.
			plmn_Id_info_list.plmn_identity_info[list_count].
			plmn_identity.mnc.count = 
			p_broadcast_info->sib_type_1_Info.cell_access_related_info.
			plmn_Id_info_list.plmn_identity_info[list_count].plmn_identity.mnc.count;

		for (mnc_count = 0; mnc_count < p_cell_config_req->
				sib_type_1_Info.cell_access_related_info.
				plmn_Id_info_list.plmn_identity_info[list_count].
				plmn_identity.mnc.count; mnc_count++)
		{
			p_cell_config_req->sib_type_1_Info.cell_access_related_info.
				plmn_Id_info_list.plmn_identity_info[list_count].
				plmn_identity.mnc.mnc[mnc_count] = 
				p_broadcast_info->sib_type_1_Info.cell_access_related_info.
				plmn_Id_info_list.plmn_identity_info[list_count].
				plmn_identity.mnc.mnc[mnc_count];
		}
		/* Fill for_operator_use param */
		p_cell_config_req->sib_type_1_Info.cell_access_related_info.
			plmn_Id_info_list.plmn_identity_info[list_count].
			cell_res_for_operator_use = 
			p_broadcast_info->sib_type_1_Info.cell_access_related_info.
			plmn_Id_info_list.plmn_identity_info[list_count].
			cell_res_for_operator_use;
	}

	/* 2. Fill Cell selection info */
	p_cell_config_req->sib_type_1_Info.cell_selection_Info.q_rx_lev_min = 
		p_broadcast_info->sib_type_1_Info.cell_selection_Info.q_rx_lev_min;
	if (p_cell_config_req->sib_type_1_Info.cell_selection_Info.presence_bitmask 
			& CELL_SELECT_INFO_Q_RX_LEV_MIN_OFFSET_PRESENCE_FLAG)
	{
		p_cell_config_req->sib_type_1_Info.cell_selection_Info.presence_bitmask |= 
			CELL_SELECT_INFO_Q_RX_LEV_MIN_OFFSET_PRESENCE_FLAG;
		p_cell_config_req->sib_type_1_Info.cell_selection_Info.
			q_rx_lev_min_offset = 
			p_broadcast_info->sib_type_1_Info.cell_selection_Info.
			q_rx_lev_min_offset;
	}

	/* 3. Fill p_max */
	if (p_broadcast_info->sib_type_1_Info.presence_bitmask & 
			SIB_TYPE_1_P_MAX_PRESENCE_FLAG)
	{
		p_cell_config_req->sib_type_1_Info.presence_bitmask |=
			SIB_TYPE_1_P_MAX_PRESENCE_FLAG;
		p_cell_config_req->sib_type_1_Info.p_max = 
			p_broadcast_info->sib_type_1_Info.p_max;
	}

	/* 4. Fill frequency band indicator */
	p_cell_config_req->sib_type_1_Info.freq_band_indicator = 
		p_broadcast_info->sib_type_1_Info.freq_band_indicator;

	/* 5. Fill system information window length */
	p_cell_config_req->sib_type_1_Info.si_window_length = 
		p_broadcast_info->sib_type_1_Info.si_window_length;

	/* 6. Fill system information value tag */
	p_cell_config_req->sib_type_1_Info.si_value_tag = 
		p_broadcast_info->sib_type_1_Info.si_value_tag;

	/* 7. Fill scheduling info list */
	p_cell_config_req->sib_type_1_Info.scheduling_info_list.count = 
		p_broadcast_info->sib_type_1_Info.scheduling_info_list.count;
	for (list_count = 0; list_count < p_cell_config_req->sib_type_1_Info.
			scheduling_info_list.count; list_count++)
	{
		p_cell_config_req->sib_type_1_Info.scheduling_info_list.
			scheduling_info[list_count] =
			p_broadcast_info->sib_type_1_Info.scheduling_info_list.
			scheduling_info[list_count];
	}

	/* 8. TDD config */
	if (p_broadcast_info->sib_type_1_Info.presence_bitmask
			& SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG)
	{
		p_cell_config_req->sib_type_1_Info.presence_bitmask
			= SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG;

		p_cell_config_req->sib_type_1_Info.tdd_config =
			p_broadcast_info->sib_type_1_Info.tdd_config;
	}

	/* 9. Fill non critical extension */
	if ( p_broadcast_info->sib_type_1_Info.presence_bitmask &
			SIB_TYPE_1_NON_CRITICAL_EXTENSION_PRESENT_FLAG )
	{ 
		p_cell_config_req->sib_type_1_Info.presence_bitmask |= 
			SIB_TYPE_1_NON_CRITICAL_EXTENSION_PRESENT_FLAG;
		if ( p_broadcast_info->sib_type_1_Info.non_critical_extension.\
				presence_bitmask &
				SIB_TYPE_1_V890_IES_NON_CROTICAL_EXTENSION_PRESENT_FLAG )
		{
			p_cell_config_req->sib_type_1_Info.non_critical_extension.\
				presence_bitmask =
				SIB_TYPE_1_V890_IES_NON_CROTICAL_EXTENSION_PRESENT_FLAG;
			if ( p_broadcast_info->sib_type_1_Info.non_critical_extension.
					non_critical_extension.presence_bitmask &
					SIB_TYPE_1_V920_IES_IMS_EMERGENCY_SUPPORT_PRESENT_FLAG)
			{
				p_cell_config_req->sib_type_1_Info.non_critical_extension.
					non_critical_extension.presence_bitmask =
					SIB_TYPE_1_V920_IES_IMS_EMERGENCY_SUPPORT_PRESENT_FLAG;
				p_cell_config_req->sib_type_1_Info.non_critical_extension.
					non_critical_extension.ims_emergency_support = 
					p_broadcast_info->sib_type_1_Info.non_critical_extension.
					non_critical_extension.ims_emergency_support;
			}
		}
	}
	if (SIB_TYPE_1_V920_IES_CELL_SELECTION_INFO_V920_PRESENT_FLAG
			& p_broadcast_info->sib_type_1_Info.non_critical_extension.
			non_critical_extension.presence_bitmask)
	{

		p_cell_config_req->sib_type_1_Info.non_critical_extension.
			non_critical_extension.presence_bitmask |= 
			SIB_TYPE_1_V920_IES_CELL_SELECTION_INFO_V920_PRESENT_FLAG;

		p_cell_config_req->sib_type_1_Info.non_critical_extension.
			non_critical_extension.cell_selection_info_v920.q_qual_min_r9 =
			p_broadcast_info->sib_type_1_Info.non_critical_extension.
			non_critical_extension.cell_selection_info_v920.q_qual_min_r9; 

		if (p_broadcast_info->sib_type_1_Info.non_critical_extension.
				non_critical_extension.cell_selection_info_v920.presence_bitmask &
				CELL_SELECTION_INFO_Q_QUAL_MIN_OFFSET_R9_PRESENT_FLAG)
		{
			p_cell_config_req->sib_type_1_Info.non_critical_extension.
				non_critical_extension.cell_selection_info_v920.presence_bitmask |=
				CELL_SELECTION_INFO_Q_QUAL_MIN_OFFSET_R9_PRESENT_FLAG;

			p_cell_config_req->sib_type_1_Info.non_critical_extension.
				non_critical_extension.cell_selection_info_v920.
				q_qual_min_offset_r9 =
				p_broadcast_info->sib_type_1_Info.non_critical_extension.
				non_critical_extension.cell_selection_info_v920.
				q_qual_min_offset_r9;
		}
	}
	RRC_CSC_UT_TRACE_EXIT();
}
/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_2_info
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_config_req
 *                  broadcast_config_info_t    *p_broadcast_info
 *   OUTPUT       : none
 *   DESCRIPTION  : This function Update the cell configuration (sib_type_2_info_t) with new
 *                  Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 **************************************************************************/
static void csc_update_sib_type_2_info(
		rrc_rrm_cell_setup_req_t   *p_cell_config_req,
		broadcast_config_info_t    *p_broadcast_info)
{
	U8 mbsfn_count = 0;
	RRC_CSC_UT_TRACE_ENTER();

	/* 1. Fill access barring info */
	if (p_broadcast_info->sib_type_2_Info.presence_bitmask  
			& SIB2_ACCESS_BARR_INFO_PRESENCE_FLAG)
	{
		p_cell_config_req->sib_type_2_Info.presence_bitmask |= 
			SIB2_ACCESS_BARR_INFO_PRESENCE_FLAG;
		p_cell_config_req->sib_type_2_Info.access_barring_info.
			acBarringForEmergency = p_broadcast_info->sib_type_2_Info.
			access_barring_info.acBarringForEmergency;
		if (p_broadcast_info->sib_type_2_Info.access_barring_info.
				presence_bitmask & AC_BARR_INFO_MO_SIG_PRESENCE_FLAG)
		{
			p_cell_config_req->sib_type_2_Info.access_barring_info.
				presence_bitmask |= 
				AC_BARR_INFO_MO_SIG_PRESENCE_FLAG;
			p_cell_config_req->sib_type_2_Info.access_barring_info.
				ac_barring_for_mo_signalling = p_broadcast_info->sib_type_2_Info.
				access_barring_info.ac_barring_for_mo_signalling;
		}
		if (p_broadcast_info->sib_type_2_Info.access_barring_info.
				presence_bitmask & AC_BARR_INFO_MO_DATA_PRESENCE_FLAG)
		{
			p_cell_config_req->sib_type_2_Info.access_barring_info.
				presence_bitmask |= 
				AC_BARR_INFO_MO_DATA_PRESENCE_FLAG;
			p_cell_config_req->sib_type_2_Info.access_barring_info.
				ac_barring_for_mo_data = p_broadcast_info->sib_type_2_Info.
				access_barring_info.ac_barring_for_mo_data;
		}
	}
	/* 2. Fill ue timer and constant */
	p_cell_config_req->sib_type_2_Info.ue_timers_and_constants = 
		p_broadcast_info->sib_type_2_Info.ue_timers_and_constants;
	/* 3.Fill frequency info */
	p_cell_config_req->sib_type_2_Info.freq_info.ul_carrier_freq = 
		p_broadcast_info->sib_type_2_Info.freq_info.ul_carrier_freq;
	if (p_broadcast_info->sib_type_2_Info.freq_info.presence_bitmask  
			& FREQ_INFO_UL_BW_PRESENCE_FLAG)
	{
		p_cell_config_req->sib_type_2_Info.freq_info.presence_bitmask |= 
			FREQ_INFO_UL_BW_PRESENCE_FLAG;
		p_cell_config_req->sib_type_2_Info.freq_info.ul_bandwidth = 
			p_broadcast_info->sib_type_2_Info.freq_info.ul_bandwidth;
	}
	p_cell_config_req->sib_type_2_Info.freq_info.add_spectrum_emission = 
		p_broadcast_info->sib_type_2_Info.freq_info.add_spectrum_emission;
	/* 4. Fill MBSFN subframe configuration list */
	if (p_broadcast_info->sib_type_2_Info.presence_bitmask  
			& SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG)
	{
		p_cell_config_req->sib_type_2_Info.presence_bitmask |=
			SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG;    
		p_cell_config_req->sib_type_2_Info.mbsfn_subframe_config_list.count = 
			p_broadcast_info->sib_type_2_Info.mbsfn_subframe_config_list.count;
		for (mbsfn_count = 0; mbsfn_count < p_cell_config_req->
				sib_type_2_Info.mbsfn_subframe_config_list.count; mbsfn_count++)
		{
			p_cell_config_req->sib_type_2_Info.mbsfn_subframe_config_list.
				mbsfn_subframe_config[mbsfn_count].radio_frame_alloc_period = 
				p_broadcast_info->sib_type_2_Info.mbsfn_subframe_config_list.
				mbsfn_subframe_config[mbsfn_count].radio_frame_alloc_period;
			p_cell_config_req->sib_type_2_Info.mbsfn_subframe_config_list.
				mbsfn_subframe_config[mbsfn_count].radio_frame_alloc_offset =
				p_broadcast_info->sib_type_2_Info.mbsfn_subframe_config_list.
				mbsfn_subframe_config[mbsfn_count].radio_frame_alloc_offset;
			if (p_broadcast_info->sib_type_2_Info.mbsfn_subframe_config_list.
					mbsfn_subframe_config[mbsfn_count].subframe_allocation.
					presence_bitmask & SUBFRAME_ALLOCATION_ONE_FRAME)
			{
				/*Set the bitmask*/
				p_cell_config_req->sib_type_2_Info.mbsfn_subframe_config_list.
					mbsfn_subframe_config[mbsfn_count].subframe_allocation.
					presence_bitmask |= SUBFRAME_ALLOCATION_ONE_FRAME;

				l3_memcpy_wrapper(p_cell_config_req->sib_type_2_Info.
						mbsfn_subframe_config_list.mbsfn_subframe_config[mbsfn_count].
						subframe_allocation.one_frame, p_broadcast_info->
						sib_type_2_Info.mbsfn_subframe_config_list.
						mbsfn_subframe_config[mbsfn_count].subframe_allocation.
						one_frame, sizeof (p_cell_config_req->sib_type_2_Info.
							mbsfn_subframe_config_list.mbsfn_subframe_config[mbsfn_count].
							subframe_allocation.one_frame));
			}
			if (p_broadcast_info->sib_type_2_Info.mbsfn_subframe_config_list.
					mbsfn_subframe_config[mbsfn_count].subframe_allocation.
					presence_bitmask & SUBFRAME_ALLOCATION_FOUR_FRAMES)
			{
				/*Set the bitmask */
				p_cell_config_req->sib_type_2_Info.mbsfn_subframe_config_list.
					mbsfn_subframe_config[mbsfn_count].subframe_allocation.
					presence_bitmask |= SUBFRAME_ALLOCATION_FOUR_FRAMES;
				l3_memcpy_wrapper(p_cell_config_req->sib_type_2_Info.
						mbsfn_subframe_config_list.mbsfn_subframe_config[mbsfn_count].
						subframe_allocation.four_frames, p_broadcast_info->
						sib_type_2_Info.mbsfn_subframe_config_list.
						mbsfn_subframe_config[mbsfn_count].subframe_allocation.
						four_frames, sizeof(p_cell_config_req->sib_type_2_Info.
							mbsfn_subframe_config_list.mbsfn_subframe_config[mbsfn_count].
							subframe_allocation.four_frames));
			}
		}
	}
    else
    {
		p_cell_config_req->sib_type_2_Info.presence_bitmask &=
			~SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG;    
    }

	/* 5. Fill align timer */
	p_cell_config_req->sib_type_2_Info.time_align_timer =
		p_broadcast_info->sib_type_2_Info.time_align_timer;
	/* 6.Fill radio resource configuration */
	/* Fill Random Access Channel info */
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		rach_config_common.preamble_info.ra_preamble_count = 
		p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
		rach_config_common.preamble_info.ra_preamble_count;
	if (p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
			rach_config_common.preamble_info.presence_bitmask & 
			PREAMBLE_INFO_GROUP_A_CONFIG_PRESENCE_FLAG)
	{
		/*Set the bitmask */
		p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
			rach_config_common.preamble_info.presence_bitmask |= 
			PREAMBLE_INFO_GROUP_A_CONFIG_PRESENCE_FLAG;         
		p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
			rach_config_common.preamble_info.preambles_group_a_config = 
			p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
			rach_config_common.preamble_info.preambles_group_a_config;
	}
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		rach_config_common.power_ramping_params = 
		p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
		rach_config_common.power_ramping_params;
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		rach_config_common.ra_supervision_info = 
		p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
		rach_config_common.ra_supervision_info;
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		rach_config_common.max_harq_msg_3_tx = 
		p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
		rach_config_common.max_harq_msg_3_tx;
	/* Fill Broadcast Control Channel info */
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		bcch_config.mod_period_coeff = 
		p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
		bcch_config.mod_period_coeff;
	/* Fill Paging Control Channel info */
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		pcch_config = p_broadcast_info->sib_type_2_Info.
		radio_resource_config_common_sib.pcch_config;
	/* Fill PRACH information */
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		prach_config_sib = p_broadcast_info->sib_type_2_Info.
		radio_resource_config_common_sib.prach_config_sib;
	/* Fill ul cyclic prefix length */
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		ul_cyc_prefix_len = p_broadcast_info->sib_type_2_Info.
		radio_resource_config_common_sib.ul_cyc_prefix_len;
	/* Fill PDSCH info */
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		pdsch_config_common = p_broadcast_info->sib_type_2_Info.
		radio_resource_config_common_sib.pdsch_config_common;
	/* Fill PUSCH info */
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		pusch_config_common = p_broadcast_info->sib_type_2_Info.
		radio_resource_config_common_sib.pusch_config_common;
	/* Fill PUCCH information */
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		pusch_config_common = p_broadcast_info->sib_type_2_Info.
		radio_resource_config_common_sib.pusch_config_common;
	/* Fill sounding rs ul config common info */
	if (p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
			sounding_rs_ul_config_common.presence_bitmask &
			SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG)
	{
		/*Set the bitmask */
		p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
			sounding_rs_ul_config_common.presence_bitmask |=
			SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG;       
		p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
			sounding_rs_ul_config_common.setup.srs_bw_config =
			p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
			sounding_rs_ul_config_common.setup.srs_bw_config;
		p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
			sounding_rs_ul_config_common.setup.srs_subframe_config = 
			p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
			sounding_rs_ul_config_common.setup.srs_subframe_config;
		p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
			sounding_rs_ul_config_common.setup.ack_nack_srs_simul_trans =
			p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
			sounding_rs_ul_config_common.setup.ack_nack_srs_simul_trans;

		if (p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
				sounding_rs_ul_config_common.setup.presence_bitmask 
				& SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG)
		{
			/*Set the bitmask */    
			p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
				sounding_rs_ul_config_common.setup.presence_bitmask |=
				SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG;

			p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
				sounding_rs_ul_config_common.setup.srs_max_up_pts =
				p_broadcast_info->sib_type_2_Info.
				radio_resource_config_common_sib.sounding_rs_ul_config_common.
				setup.srs_max_up_pts;
		}
	}
	/* Fill uplink power control common information */
	p_cell_config_req->sib_type_2_Info.radio_resource_config_common_sib.
		uplink_power_control_common =
		p_broadcast_info->sib_type_2_Info.radio_resource_config_common_sib.
		uplink_power_control_common;

	/*cr_345 Fill SSAC Barring for MMTEL Voice r9 */
	if (p_broadcast_info->sib_type_2_Info.presence_bitmask 
			& SIB2_SSAC_BARRING_FOR_MMTEL_VOICE_PRESENCE_FLAG)
	{
		p_cell_config_req->sib_type_2_Info.presence_bitmask |=
			SIB2_SSAC_BARRING_FOR_MMTEL_VOICE_PRESENCE_FLAG;
		p_cell_config_req->sib_type_2_Info.ssac_barring_for_mmtel_voice_r9
			= p_broadcast_info->sib_type_2_Info.ssac_barring_for_mmtel_voice_r9;
	}
	/* 8.Fill SSAC Barring for MMTEL Video r9 */
	if (p_broadcast_info->sib_type_2_Info.presence_bitmask 
			& SIB2_SSAC_BARRING_FOR_MMTEL_VIDEO_PRESENCE_FLAG)
	{
		p_cell_config_req->sib_type_2_Info.presence_bitmask |=
			SIB2_SSAC_BARRING_FOR_MMTEL_VIDEO_PRESENCE_FLAG;
		p_cell_config_req->sib_type_2_Info.ssac_barring_for_mmtel_video_r9
			= p_broadcast_info->sib_type_2_Info.ssac_barring_for_mmtel_video_r9;
	}
	/* CR 477 changes Start*/
	if (p_broadcast_info->sib_type_2_Info.presence_bitmask
			& SIB2_AC_BARRING_FOR_CSFB_PRESENCE_FLAG )
	{
		p_cell_config_req->sib_type_2_Info.presence_bitmask |=
			SIB2_AC_BARRING_FOR_CSFB_PRESENCE_FLAG;
		p_cell_config_req->sib_type_2_Info.ac_barring_for_csfb_r10
			= p_broadcast_info->sib_type_2_Info.ac_barring_for_csfb_r10;

	}
	/* CR 477 changes End*/
	RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_3_info
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_set_req
 *                  broadcast_config_info_t    *p_broadcast_info
 *   OUTPUT       : none
 *   DESCRIPTION  : This function Update the cell configuration(sib_type_3_info_t) with new
 *                  Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_update_sib_type_3_info(
		rrc_rrm_cell_setup_req_t   *p_cell_set_req,
		broadcast_config_info_t    *p_broadcast_info)
{
	RRC_CSC_UT_TRACE_ENTER();
	/* 1. Fill cell reselect info */
	p_cell_set_req->sib_type_3_Info.timecell_reselect_info_comm.q_hyst = 
		p_broadcast_info->sib_type_3_Info.timecell_reselect_info_comm.q_hyst;

	if (p_broadcast_info->sib_type_3_Info.timecell_reselect_info_comm.
			presence_bitmask & CELL_RESELECT_SPEED_DEPEND_RESELECT_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_3_Info.timecell_reselect_info_comm.
			presence_bitmask |= 
			CELL_RESELECT_SPEED_DEPEND_RESELECT_PRESENCE_FLAG;

		/* Fill speed depend reselect info  */
		p_cell_set_req->sib_type_3_Info.timecell_reselect_info_comm.
			speed_depend_reselect = 
			p_broadcast_info->sib_type_3_Info.timecell_reselect_info_comm.
			speed_depend_reselect;
	}

	/* 2. Fill cell reselect serving frequency info */
	if (p_broadcast_info->sib_type_3_Info.cell_reselect_serv_freq_info.
			presence_bitmask & CELL_RESELECT_NON_INTRA_SEARCH_PRESENCE_FLAG )
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_3_Info.cell_reselect_serv_freq_info.
			presence_bitmask |= 
			CELL_RESELECT_NON_INTRA_SEARCH_PRESENCE_FLAG;

		p_cell_set_req->sib_type_3_Info.cell_reselect_serv_freq_info.
			s_non_intra_search = 
			p_broadcast_info->sib_type_3_Info.cell_reselect_serv_freq_info.
			s_non_intra_search;
	}

	p_cell_set_req->sib_type_3_Info.cell_reselect_serv_freq_info.
		thresh_serving_low = 
		p_broadcast_info->sib_type_3_Info.cell_reselect_serv_freq_info.
		thresh_serving_low;
	p_cell_set_req->sib_type_3_Info.cell_reselect_serv_freq_info.
		cell_reselect_priority = 
		p_broadcast_info->sib_type_3_Info.cell_reselect_serv_freq_info.
		cell_reselect_priority; 

	/* 3. Fill intra frequency cell reselect info */
	p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.
		q_rx_lev_min = 
		p_broadcast_info->sib_type_3_Info.intra_freq_cell_reselect_info.
		q_rx_lev_min;

	if (p_broadcast_info->sib_type_3_Info.intra_freq_cell_reselect_info.
			presence_bitmask & INTRA_FREQ_CELL_RESELECT_P_MAX_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.
			presence_bitmask |= 
			INTRA_FREQ_CELL_RESELECT_P_MAX_PRESENCE_FLAG;
		p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.p_max = 
			p_broadcast_info->sib_type_3_Info.intra_freq_cell_reselect_info.
			p_max;
	}
	if (p_broadcast_info->sib_type_3_Info.intra_freq_cell_reselect_info.
			presence_bitmask & INTRA_FREQ_CELL_RESELECT_INTRA_SEARCH_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.
			presence_bitmask |= 
			INTRA_FREQ_CELL_RESELECT_INTRA_SEARCH_PRESENCE_FLAG;

		p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.
			s_intra_search = 
			p_broadcast_info->sib_type_3_Info.
			intra_freq_cell_reselect_info.s_intra_search;
	}
	if (p_broadcast_info->sib_type_3_Info.intra_freq_cell_reselect_info.
			presence_bitmask & INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.
			presence_bitmask |= 
			INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRESENCE_FLAG;

		p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.
			measure_bw = 
			p_broadcast_info->sib_type_3_Info.
			intra_freq_cell_reselect_info.measure_bw;
	}
	p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.
		presence_antenna_port_1 = 
		p_broadcast_info->sib_type_3_Info.
		intra_freq_cell_reselect_info.presence_antenna_port_1;

	p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.
		neigh_cell_config = 
		p_broadcast_info->sib_type_3_Info.
		intra_freq_cell_reselect_info.neigh_cell_config;

	p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.
		t_reselec_eutra = 
		p_broadcast_info->sib_type_3_Info.
		intra_freq_cell_reselect_info.t_reselec_eutra;

	if (p_broadcast_info->sib_type_3_Info.intra_freq_cell_reselect_info.
			presence_bitmask & INTRA_FREQ_CELL_RESELECT_EUTRAN_SF_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.
			presence_bitmask |= 
			INTRA_FREQ_CELL_RESELECT_EUTRAN_SF_PRESENCE_FLAG;
		p_cell_set_req->sib_type_3_Info.intra_freq_cell_reselect_info.
			t_reselect_eutra_sf = 
			p_broadcast_info->sib_type_3_Info.
			intra_freq_cell_reselect_info.t_reselect_eutra_sf;
	}

	/* cr_398: Cell reselection enhancements*/
	/*Fill SystemInformationBlockType3_s_IntraSearch_v920*/
	if (p_broadcast_info->sib_type_3_Info.presence_bitmask &
			SIB_3_INTRA_SEARCH_V920_PRESENCE_FLAG)
	{
		p_cell_set_req->sib_type_3_Info.presence_bitmask |=
			SIB_3_INTRA_SEARCH_V920_PRESENCE_FLAG;

		p_cell_set_req->sib_type_3_Info.sib3_intra_search_v920.
			s_intra_search_p_r9 =
			p_broadcast_info->sib_type_3_Info.sib3_intra_search_v920.
			s_intra_search_p_r9;

		p_cell_set_req->sib_type_3_Info.sib3_intra_search_v920.
			s_intra_search_q_r9 =
			p_broadcast_info->sib_type_3_Info.sib3_intra_search_v920.
			s_intra_search_q_r9;
	}

	/*Fill SystemInformationBlockType3_s_NonIntraSearch_v920*/
	if (p_broadcast_info->sib_type_3_Info.presence_bitmask &
			SIB_3_NON_INTRA_SEARCH_V920_PRESENCE_FLAG)
	{
		p_cell_set_req->sib_type_3_Info.presence_bitmask |=
			SIB_3_NON_INTRA_SEARCH_V920_PRESENCE_FLAG;

		p_cell_set_req->sib_type_3_Info.sib3_non_intra_search_v920.
			s_non_intra_search_p_r9 =
			p_broadcast_info->sib_type_3_Info.sib3_non_intra_search_v920.
			s_non_intra_search_p_r9;

		p_cell_set_req->sib_type_3_Info.sib3_non_intra_search_v920.
			s_non_intra_search_q_r9 =
			p_broadcast_info->sib_type_3_Info.sib3_non_intra_search_v920.
			s_non_intra_search_q_r9;
	}
	/*Fill Q_QualMin_r9*/
	if (p_broadcast_info->sib_type_3_Info.presence_bitmask &
			SIB_3_Q_QUAL_MIN_R9_PRESENCE_FLAG)
	{
		p_cell_set_req->sib_type_3_Info.presence_bitmask |=
			SIB_3_Q_QUAL_MIN_R9_PRESENCE_FLAG;

		p_cell_set_req->sib_type_3_Info.q_qual_min_r9 =
			p_broadcast_info->sib_type_3_Info.q_qual_min_r9;
	}

	/*Fill threshServingLowQ_r9*/
	if (p_broadcast_info->sib_type_3_Info.presence_bitmask &
			SIB_3_THRESH_SERVING_LOW_Q_R9_PRESENCE_FLAG)
	{
		p_cell_set_req->sib_type_3_Info.presence_bitmask |=
			SIB_3_THRESH_SERVING_LOW_Q_R9_PRESENCE_FLAG;

		p_cell_set_req->sib_type_3_Info.thresh_serving_low_q_r9 =
			p_broadcast_info->sib_type_3_Info.thresh_serving_low_q_r9;
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_4_info
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_set_req
 *                  broadcast_config_info_t    *p_broadcast_info
 *   OUTPUT       : none
 *   DESCRIPTION  : This function Update the cell configuration (sib_type_4_info_t)
 *                  with new Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_update_sib_type_4_info(
		rrc_rrm_cell_setup_req_t   *p_cell_set_req,
		broadcast_config_info_t    *p_broadcast_info)
{
	U8 counter = 0;

	RRC_CSC_UT_TRACE_ENTER();
	/* 1. Fill intra frequency neigh cell list info */
	if (p_broadcast_info->sib_type_4_Info.presence_bitmask  
			& SIB_4_NEIGH_CELL_LIST_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_4_Info.presence_bitmask  |= 
			SIB_4_NEIGH_CELL_LIST_PRESENCE_FLAG;
		p_cell_set_req->sib_type_4_Info.intra_freq_neigh_cell_list.count = 
			p_broadcast_info->sib_type_4_Info.intra_freq_neigh_cell_list.count;

		for (counter = 0; counter < p_cell_set_req->sib_type_4_Info.
				intra_freq_neigh_cell_list.count; counter++)
		{
			p_cell_set_req->sib_type_4_Info.intra_freq_neigh_cell_list.
				neigh_cell[counter] = p_broadcast_info->sib_type_4_Info.
				intra_freq_neigh_cell_list.neigh_cell[counter];
		}
	}
    /*Bug_11959_fix_start*/
    else
    {
        if (p_cell_set_req->sib_type_4_Info.presence_bitmask &
                SIB_4_NEIGH_CELL_LIST_PRESENCE_FLAG)
        {
            p_cell_set_req->sib_type_4_Info.presence_bitmask ^=
                SIB_4_NEIGH_CELL_LIST_PRESENCE_FLAG;
        }
    }
    /*Bug_11959_fix_stop*/
	/* 2. Fill intra freqwquency black listed cell list information */
	if (p_broadcast_info->sib_type_4_Info.presence_bitmask & 
			SIB_4_BLACK_LIST_LIST_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_4_Info.presence_bitmask |= 
			SIB_4_BLACK_LIST_LIST_PRESENCE_FLAG;
		p_cell_set_req->sib_type_4_Info.intra_freq_black_listed_cell_list.
			count = p_broadcast_info->sib_type_4_Info.
			intra_freq_black_listed_cell_list.count;

		for (counter=0; counter < p_cell_set_req->sib_type_4_Info.
				intra_freq_black_listed_cell_list.count ; counter++)
		{
			p_cell_set_req->sib_type_4_Info.intra_freq_black_listed_cell_list.
				black_listed_cell[counter].start = p_broadcast_info->sib_type_4_Info.
				intra_freq_black_listed_cell_list.black_listed_cell[counter].start;

			if (p_broadcast_info->sib_type_4_Info.intra_freq_black_listed_cell_list.
					black_listed_cell[counter].presence_bitmask & PHY_CELL_ID_RANGE)
			{
				/* Set the bitmask */
				p_cell_set_req->sib_type_4_Info.intra_freq_black_listed_cell_list.
					black_listed_cell[counter].presence_bitmask |= PHY_CELL_ID_RANGE;

				p_cell_set_req->sib_type_4_Info.intra_freq_black_listed_cell_list.
					black_listed_cell[counter].range = p_broadcast_info->sib_type_4_Info.
					intra_freq_black_listed_cell_list.black_listed_cell[counter].range;
			}
		}
	}
	/* 3. Fill physical cell id range */
	if (p_broadcast_info->sib_type_4_Info.presence_bitmask & 
			SIB_4_PHY_CELL_ID_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_4_Info.presence_bitmask |= 
			SIB_4_PHY_CELL_ID_PRESENCE_FLAG;
		p_cell_set_req->sib_type_4_Info.csg_phy_cell_id_range.start = 
			p_broadcast_info->sib_type_4_Info.csg_phy_cell_id_range.start;

		if (p_broadcast_info->sib_type_4_Info.csg_phy_cell_id_range.presence_bitmask
				& PHY_CELL_ID_RANGE)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_4_Info.csg_phy_cell_id_range.
				presence_bitmask |= PHY_CELL_ID_RANGE;
			p_cell_set_req->sib_type_4_Info.csg_phy_cell_id_range.range = 
				p_broadcast_info->sib_type_4_Info.csg_phy_cell_id_range.range;
		}
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_5_info
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_set_req,
 *                  broadcast_config_info_t    *p_broadcast_info
 *   OUTPUT       : none
 *   DESCRIPTION  : This function Update the cell configuration (sib_type_5_info_t) 
 *                   with new Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_update_sib_type_5_info(
        rrc_rrm_cell_setup_req_t   *p_cell_set_req,
        broadcast_config_info_t    *p_broadcast_info)
{
	/* coverity_fix_54692_start */
	U8 counter1 = 0;
	U8 counter2 = 0;

	RRC_CSC_UT_TRACE_ENTER();
	/* Fill inter frequency carrier frequency list information */
	p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.count = 
		p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.count;

	for (counter1 = 0; counter1<p_cell_set_req->
			sib_type_5_Info.inter_freq_carrier_freq_list_list.
			count; counter1++)
	{
		/* SPR 11530 Fix Start */
		p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].presence_bitmask =
			p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].presence_bitmask;
		/* SPR 11530 Fix Stop */

		p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].eutra_dl_carrier_freq = 
			p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].eutra_dl_carrier_freq;

		p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].qrx_lev_min = 
			p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].qrx_lev_min;

		if (p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask & 
				INTER_FREQ_CARR_P_MAX_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask |= 
				INTER_FREQ_CARR_P_MAX_PRESENCE_FLAG;

			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].p_max = 
				p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].p_max;
		}
		p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].t_reselection_eutran = 
			p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].t_reselection_eutran;

		/* Fill speed depends scal parameters */
		if (p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask & 
				INTER_FREQ_CARR_SCALE_PARAM_PRESENCE_FLAG)
		{
			/* Set the bitmask */

		/* SPR 13759 Fix Start */
                        p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
                               inter_freq_carrier_freq_list[counter1].speed_depend_scal_params.sf_medium =
                        p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
                               inter_freq_carrier_freq_list[counter1].speed_depend_scal_params.sf_medium;
		/* SPR 13759 Fix Stop */
			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].
				speed_depend_scal_params.sf_high = 
				p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].speed_depend_scal_params.sf_high;
		}
		p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].thresh_x_high = 
			p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].thresh_x_high;

		p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].thresh_x_low = 
			p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].thresh_x_low;

		p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].measurement_bandwidth = 
			p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].measurement_bandwidth;

		p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].presence_antenna_port_1 = 
			p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].presence_antenna_port_1;

		if (p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask & 
				INTER_FREQ_CARR_CELL_RESELCT_PRIOR_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask |= 
				INTER_FREQ_CARR_CELL_RESELCT_PRIOR_PRESENCE_FLAG;

			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].cell_reselect_priority = 
				p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].cell_reselect_priority;
		}
		p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].neigh_cell_config = 
			p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].neigh_cell_config;

		p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].q_offset_freq = 
			p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
			inter_freq_carrier_freq_list[counter1].q_offset_freq;

		/* Fill inter frequency neighbour cell list */
		if (p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask & 
				INTER_FREQ_CARR_NEIGH_CELL_LIST_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask |= 
				INTER_FREQ_CARR_NEIGH_CELL_LIST_PRESENCE_FLAG;

			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].
				inter_freq_neigh_cell_list.count = 
				p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].inter_freq_neigh_cell_list.count;

			for (counter2 = 0; counter2 < p_cell_set_req->sib_type_5_Info.
					inter_freq_carrier_freq_list_list.
					inter_freq_carrier_freq_list[counter1].
					inter_freq_neigh_cell_list.count; counter2++)
			{
				p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
					inter_freq_carrier_freq_list[counter1].inter_freq_neigh_cell_list.
					neigh_cell[counter2].phys_cell_id = p_broadcast_info->sib_type_5_Info.
					inter_freq_carrier_freq_list_list.
					inter_freq_carrier_freq_list[counter1].
					inter_freq_neigh_cell_list.neigh_cell[counter2].phys_cell_id;

				p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
					inter_freq_carrier_freq_list[counter1].inter_freq_neigh_cell_list.
					neigh_cell[counter2].q_offset_cell = 
					p_broadcast_info->sib_type_5_Info.
					inter_freq_carrier_freq_list_list.
					inter_freq_carrier_freq_list[counter1].
					inter_freq_neigh_cell_list.neigh_cell[counter2].q_offset_cell;
			}
		}
		/* Fill inter frequency black listed cell list info */
		if (p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask & 
				INTER_FREQ_CARR_BLACK_LIST_CELL_LIST_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].
				inter_freq_black_listed_cell_list.
				count = 
				p_broadcast_info->sib_type_5_Info.
				inter_freq_carrier_freq_list_list.inter_freq_carrier_freq_list[counter1].
				inter_freq_black_listed_cell_list.count;

			for (counter2 = 0; counter2 < p_cell_set_req->sib_type_5_Info.
					inter_freq_carrier_freq_list_list.inter_freq_carrier_freq_list[counter1].
					inter_freq_black_listed_cell_list.count; counter2++)
			{
				p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
					inter_freq_carrier_freq_list[counter1].inter_freq_black_listed_cell_list.
					black_listed_cell[counter2].start = p_broadcast_info->sib_type_5_Info.
					inter_freq_carrier_freq_list_list.inter_freq_carrier_freq_list[counter1].
					inter_freq_black_listed_cell_list.black_listed_cell[counter2].start;

				if (p_broadcast_info->sib_type_5_Info.
						inter_freq_carrier_freq_list_list.inter_freq_carrier_freq_list[counter1].
						inter_freq_black_listed_cell_list.black_listed_cell[counter2].
						presence_bitmask & PHY_CELL_ID_RANGE)
				{
					/* Set the bitmask */
					p_cell_set_req->sib_type_5_Info.
						inter_freq_carrier_freq_list_list.
						inter_freq_carrier_freq_list[counter1].
						inter_freq_black_listed_cell_list.
						black_listed_cell[counter2].presence_bitmask |= PHY_CELL_ID_RANGE;

					p_cell_set_req->sib_type_5_Info.
						inter_freq_carrier_freq_list_list.
						inter_freq_carrier_freq_list[counter1].
						inter_freq_black_listed_cell_list.
						black_listed_cell[counter2].range = 
						p_broadcast_info->sib_type_5_Info.
						inter_freq_carrier_freq_list_list.
						inter_freq_carrier_freq_list[counter1].
						inter_freq_black_listed_cell_list.
						black_listed_cell[counter1].range;
				}
			}
		}
		/*cr_398:Cell reselection enhancements*/
		if (p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask & 
				INTER_FREQ_CARR_Q_QUAL_MIN_R9_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask |=
				INTER_FREQ_CARR_Q_QUAL_MIN_R9_PRESENCE_FLAG;

			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].q_qual_min_r9 =
				p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].q_qual_min_r9;

		}
		if (p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask & 
				INTER_FREQ_THRESH_X_Q_R9_PRESENCE_FLAG)
		{  
			/* Set the bitmask */
			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].presence_bitmask |=
				INTER_FREQ_THRESH_X_Q_R9_PRESENCE_FLAG;

			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].
				thresh_x_q_r9.thresh_x_high_q_r9 = 
				p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].
				thresh_x_q_r9.thresh_x_high_q_r9; 

			p_cell_set_req->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].
				thresh_x_q_r9.thresh_x_low_q_r9 =
				p_broadcast_info->sib_type_5_Info.inter_freq_carrier_freq_list_list.
				inter_freq_carrier_freq_list[counter1].
				thresh_x_q_r9.thresh_x_low_q_r9;
		}
	}
	RRC_CSC_UT_TRACE_EXIT();
}
/* coverity_fix_54692_stop */


/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_6_info
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_set_req,
 *                  broadcast_config_info_t    *p_broadcast_info
 *   OUTPUT       : none
 *   DESCRIPTION  : This function Update the cell configuration (sib_type_6_info_t) 
 *                  with new Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_update_sib_type_6_info(
		rrc_rrm_cell_setup_req_t   *p_cell_set_req,
		broadcast_config_info_t    *p_broadcast_info)
{
	U8 counter = 0;
	RRC_CSC_UT_TRACE_ENTER();

	/* Fill carrier freq list uta fdd */
	if (p_broadcast_info->sib_type_6_Info.presence_bitmask  
			& SIB_6_CARRIER_FREQ_LIST_UTRA_FDD_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_6_Info.presence_bitmask |= 
			SIB_6_CARRIER_FREQ_LIST_UTRA_FDD_PRESENCE_FLAG;
		p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.count = 
			p_broadcast_info->sib_type_6_Info.carrier_freq_list_utra_fdd.count;
		for (counter = 0; counter < p_cell_set_req->sib_type_6_Info.
				carrier_freq_list_utra_fdd.count; counter++)
		{
			p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
				carrier_freq = 
				p_broadcast_info->sib_type_6_Info.
				carrier_freq_list_utra_fdd.data[counter].carrier_freq;

			if (p_broadcast_info->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
					presence_bitmask & CR_FREQ_UTRA_FDD_CELL_RESEL_PRI_PRESENCE_FLAG)
			{
				/* Set the bitmask */
				p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
					presence_bitmask |= 
					p_broadcast_info->sib_type_6_Info.
					carrier_freq_list_utra_fdd.data[counter].presence_bitmask;

				p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.
					data[counter].cell_reselection_priority = 
					p_broadcast_info->sib_type_6_Info.
					carrier_freq_list_utra_fdd.data[counter].cell_reselection_priority;
			}
			p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
				thresh_x_high = 
				p_broadcast_info->sib_type_6_Info.
				carrier_freq_list_utra_fdd.data[counter].thresh_x_high;

			p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
				thresh_x_low = 
				p_broadcast_info->sib_type_6_Info.
				carrier_freq_list_utra_fdd.data[counter].thresh_x_low;

			p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
				q_rx_lev_min = 
				p_broadcast_info->sib_type_6_Info.
				carrier_freq_list_utra_fdd.data[counter].q_rx_lev_min;

			p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
				p_max_utra = 
				p_broadcast_info->sib_type_6_Info.
				carrier_freq_list_utra_fdd.data[counter].p_max_utra;

			p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
				q_qual_min = 
				p_broadcast_info->sib_type_6_Info.
				carrier_freq_list_utra_fdd.data[counter].q_qual_min;

			if (p_broadcast_info->sib_type_6_Info.
					carrier_freq_list_utra_fdd.data[counter].presence_bitmask & 
					CR_FREQ_UTRA_FDD_THRESH_X_Q_R9_PRESENCE_FLAG)
			{
				p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
					presence_bitmask |=  CR_FREQ_UTRA_FDD_THRESH_X_Q_R9_PRESENCE_FLAG;

				p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
					thresh_x_q_r9.thresh_x_high_q_r9 = 
					p_broadcast_info->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
					thresh_x_q_r9.thresh_x_high_q_r9; 

				p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
					thresh_x_q_r9.thresh_x_low_q_r9 =
					p_broadcast_info->sib_type_6_Info.carrier_freq_list_utra_fdd.data[counter].
					thresh_x_q_r9.thresh_x_low_q_r9; 
			}
		}
	}

	/* Fill carrier frequency list utra tdd */
	if (p_broadcast_info->sib_type_6_Info.presence_bitmask 
			& SIB_6_CARRIER_FREQ_LIST_UTRA_TDD_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_6_Info.presence_bitmask |= 
			SIB_6_CARRIER_FREQ_LIST_UTRA_TDD_PRESENCE_FLAG;
		p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_tdd.count = 
			p_broadcast_info->sib_type_6_Info.carrier_freq_list_utra_tdd.count;

		for (counter = 0; counter<p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_tdd.
				count; counter++)
		{
			p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_tdd.data[counter].
				carrier_freq = 
				p_broadcast_info->sib_type_6_Info.
				carrier_freq_list_utra_tdd.data[counter].carrier_freq;

			if (p_broadcast_info->sib_type_6_Info.carrier_freq_list_utra_tdd.data[counter].
					presence_bitmask & CR_FREQ_UTRA_TDD_CELL_RESEL_PRI_PRESENCE_FLAG)
			{
				/* Set the bitmask */
				p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_tdd.data[counter].
					presence_bitmask |= 
					p_broadcast_info->sib_type_6_Info.
					carrier_freq_list_utra_tdd.data[counter].presence_bitmask;

				p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_tdd.
					data[counter].cell_reselection_priority = 
					p_broadcast_info->sib_type_6_Info.carrier_freq_list_utra_tdd.
					data[counter].cell_reselection_priority;
			}
			p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_tdd.data[counter].
				thresh_x_high = 
				p_broadcast_info->sib_type_6_Info.
				carrier_freq_list_utra_tdd.data[counter].thresh_x_high;

			p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_tdd.data[counter].
				thresh_x_low = 
				p_broadcast_info->sib_type_6_Info.
				carrier_freq_list_utra_tdd.data[counter].thresh_x_low;

			p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_tdd.data[counter].
				q_rx_lev_min = 
				p_broadcast_info->sib_type_6_Info.
				carrier_freq_list_utra_tdd.data[counter].q_rx_lev_min;

			p_cell_set_req->sib_type_6_Info.carrier_freq_list_utra_tdd.data[counter].
				p_max_utra = 
				p_broadcast_info->sib_type_6_Info.
				carrier_freq_list_utra_tdd.data[counter].p_max_utra;

		}
	}
	p_cell_set_req->sib_type_6_Info.t_resel_utra = 
		p_broadcast_info->sib_type_6_Info.t_resel_utra;

	if (p_broadcast_info->sib_type_6_Info.presence_bitmask  
			& SIB_6_T_RESELECTION_UTRA_SF_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_6_Info.presence_bitmask |= 
			SIB_6_T_RESELECTION_UTRA_SF_PRESENCE_FLAG;
		p_cell_set_req->sib_type_6_Info.t_resel_utra_sf =
			p_broadcast_info->sib_type_6_Info.t_resel_utra_sf;
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_7_info
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_set_req,
 *                  broadcast_config_info_t    *p_broadcast_info
 *   OUTPUT       : none
 *   DESCRIPTION  : This function Update the cell configuration (sib_type_7_info_t) 
 *                  with new Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_update_sib_type_7_info(
		rrc_rrm_cell_setup_req_t   *p_cell_set_req,
		broadcast_config_info_t    *p_broadcast_info)
{
	U8 counter1 = 0;
	U8 counter2 = 0;
	RRC_CSC_UT_TRACE_ENTER();
	p_cell_set_req->sib_type_7_Info.t_reselection_geran = p_broadcast_info->
		sib_type_7_Info.t_reselection_geran;
	/* Fill speed state scale factors info */
	if (p_broadcast_info->sib_type_7_Info.presence_bitmask & 
			SIB_7_T_RESEL_GERAN_SF_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_7_Info.presence_bitmask |= 
			SIB_7_T_RESEL_GERAN_SF_PRESENCE_FLAG;
		p_cell_set_req->sib_type_7_Info.t_reselect_geran_sf = 
			p_broadcast_info->sib_type_7_Info.t_reselect_geran_sf;
	}
	/* Fill carrier frequency info list geran */
	if (p_broadcast_info->sib_type_7_Info.presence_bitmask  
			& SIB_7_CR_FREQS_INFO_LST_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_7_Info.presence_bitmask |= 
			SIB_7_CR_FREQS_INFO_LST_PRESENCE_FLAG;

		p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.count = 
			p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.count;
		for (counter1 = 0; counter1< p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
				count; counter1++)
		{
			p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.cr_freq_info[counter1].
				carrier_freqs.starting_arfcn = 
				p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
				cr_freq_info[counter1].carrier_freqs.starting_arfcn;

			p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.cr_freq_info[counter1].
				carrier_freqs.band_indicator = 
				p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
				cr_freq_info[counter1].carrier_freqs.band_indicator;

			/* Fill explicit list arfcns */
			if (p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.presence_bitmask
					& GERAN_EXPL_LIST_OF_ARFCNS_PRESENCE_FLAG )
			{
				/* Set the bitmask */
				p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.presence_bitmask |= 
					GERAN_EXPL_LIST_OF_ARFCNS_PRESENCE_FLAG;

				p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.
					explicit_list_of_arfcns.count = 
					p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.
					explicit_list_of_arfcns.count;

				for (counter2 = 0; counter2 < p_cell_set_req->sib_type_7_Info.
						carrier_freqs_info_lst.cr_freq_info[counter1].carrier_freqs.
						following_arfcns.explicit_list_of_arfcns.count; counter2++)
				{
					l3_memcpy_wrapper (&p_cell_set_req->sib_type_7_Info.
							carrier_freqs_info_lst.cr_freq_info[counter1].carrier_freqs.
							following_arfcns.explicit_list_of_arfcns.data[counter2], 
							&p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
							cr_freq_info[counter1].carrier_freqs.following_arfcns.
							explicit_list_of_arfcns.data[counter2], sizeof(p_cell_set_req->
								sib_type_7_Info.carrier_freqs_info_lst.
								cr_freq_info[counter1].carrier_freqs.following_arfcns.
								explicit_list_of_arfcns.data[counter2]));
				}
			}
			/* Fill equally spaced arfcns */
			if (p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.presence_bitmask 
					& GERAN_EQ_SPACED_ARFCNS_PRESENCE_FLAG)
			{
				/* Set the bitmask */
				p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.presence_bitmask |=
					GERAN_EQ_SPACED_ARFCNS_PRESENCE_FLAG;

				p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.
					equally_spaced_arfcns = 
					p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.
					equally_spaced_arfcns;
			}
			/* Fill variable bitmap of arfcns */
			if (p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.presence_bitmask 
					& GERAN_VAR_BITMAP_ARFCNS_PRESENCE_FLAG)
			{
				/* Set the bitmask */
				p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.presence_bitmask |= 
					GERAN_VAR_BITMAP_ARFCNS_PRESENCE_FLAG;

				p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.
					var_bitmap_of_arfcns.count = 
					p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].carrier_freqs.following_arfcns.
					var_bitmap_of_arfcns.count;

				for (counter2 = 0; counter2 < p_cell_set_req->sib_type_7_Info.
						carrier_freqs_info_lst.cr_freq_info[counter1].carrier_freqs.
						following_arfcns.var_bitmap_of_arfcns.count; counter2++)
				{
					l3_memcpy_wrapper (&p_cell_set_req->sib_type_7_Info.
							carrier_freqs_info_lst.cr_freq_info[counter1].carrier_freqs.
							following_arfcns.var_bitmap_of_arfcns.data[counter2],
							&p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
							cr_freq_info[counter1].carrier_freqs.following_arfcns.
							var_bitmap_of_arfcns.data[counter2], sizeof(p_cell_set_req->
								sib_type_7_Info.carrier_freqs_info_lst.cr_freq_info[counter1].
								carrier_freqs.following_arfcns.var_bitmap_of_arfcns.
								data[counter2]));
				}
			}
			/* Fill crfi geran commom information */
			if (p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].common_info.presence_bitmask 
					& CRFI_GERAN_CELL_RESEL_PRI_PRESENCE_FLAG)
			{
				/* Set the bitmask */
				p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].common_info.presence_bitmask |= 
					CRFI_GERAN_CELL_RESEL_PRI_PRESENCE_FLAG;

				p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].common_info.cell_reselection_priority = 
					p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].common_info.cell_reselection_priority;
			}
			l3_memcpy_wrapper (&p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].common_info.ncc_permitted[0],
					&p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].common_info.ncc_permitted[0],
					NCC_PERMITTED_OCTET_SIZE);

			p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
				cr_freq_info[counter1].common_info.q_rx_lev_min = 
				p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
				cr_freq_info[counter1].common_info.q_rx_lev_min;

			if (p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].common_info.presence_bitmask 
					& CRFI_GERAN_P_MAX_GERAN_PRESENCE_FLAG)
			{
				/* Set the bitmask */
				p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].common_info.p_max_geran = 
					p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
					cr_freq_info[counter1].common_info.p_max_geran;
			}

			p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
				cr_freq_info[counter1].common_info.thresh_x_high = 
				p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
				cr_freq_info[counter1].common_info.thresh_x_high;

			p_cell_set_req->sib_type_7_Info.carrier_freqs_info_lst.
				cr_freq_info[counter1].common_info.thresh_x_low = 
				p_broadcast_info->sib_type_7_Info.carrier_freqs_info_lst.
				cr_freq_info[counter1].common_info.thresh_x_low;
		}
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_8_1XRTT_params
 *
 *       Updates the cell configuration (1XRTT params insib_type_8_info_t) 
 *       with new Configuration as received in CELL RECONFIG from RRM
 *   INPUT  :
 *       cell setup request
 *       broadcast information
 *
 *   RETURNS:
 *       None
 *
 ***************************************************************************/
static void csc_update_sib_type_8_1XRTT_params(
		rrc_rrm_cell_setup_req_t   *p_cell_set_req,
		broadcast_config_info_t    *p_broadcast_info)
{
	U8 counter = 0;
	RRC_CSC_UT_TRACE_ENTER();
	if (p_broadcast_info->sib_type_8_Info.presence_bitmask 
			& SIB_8_PARAMS_1_XRTT_PRESENCE_FLAG)
	{
		/* Fill csfb_reg_param_1_xrtt */
		/* Set the bitmask */
		p_cell_set_req->sib_type_8_Info.presence_bitmask |= 
			SIB_8_PARAMS_1_XRTT_PRESENCE_FLAG;

		if (p_broadcast_info->sib_type_8_Info.params_1_xrtt.presence_bitmask 
				& CFSB_REG_PARAM_1_XRTT_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_8_Info.params_1_xrtt.presence_bitmask |= 
				CFSB_REG_PARAM_1_XRTT_PRESENCE_FLAG;
			p_cell_set_req->sib_type_8_Info.params_1_xrtt.csfb_reg_param_1_xrtt =
				p_broadcast_info->sib_type_8_Info.params_1_xrtt.csfb_reg_param_1_xrtt;
		}
		if (p_broadcast_info->sib_type_8_Info.params_1_xrtt.presence_bitmask  
				& LONG_CODE_STATE_1_XRTT_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_8_Info.params_1_xrtt.presence_bitmask |= 
				LONG_CODE_STATE_1_XRTT_PRESENCE_FLAG;

			l3_memcpy_wrapper (p_cell_set_req->sib_type_8_Info.params_1_xrtt.
					long_code_state_1_xrtt, 
					p_broadcast_info->sib_type_8_Info.params_1_xrtt.
					long_code_state_1_xrtt,
					sizeof(p_broadcast_info->sib_type_8_Info.params_1_xrtt.
						long_code_state_1_xrtt));
		}

		/* Fill cell reselection parameters of cdma2000 */
		if (p_broadcast_info->sib_type_8_Info.params_1_xrtt.presence_bitmask 
				& CELL_RESELECTION_PARAM_1_XRTT_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_8_Info.params_1_xrtt.presence_bitmask |= 
				CELL_RESELECTION_PARAM_1_XRTT_PRESENCE_FLAG;

			p_cell_set_req->sib_type_8_Info.params_1_xrtt.
				cell_reselection_params_1_xrtt.band_class_list.count = 
				p_broadcast_info->sib_type_8_Info.params_1_xrtt.
				cell_reselection_params_1_xrtt.band_class_list.count;

			for (counter = 0; counter < p_cell_set_req->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.band_class_list.count; counter++)
			{
				/* Fill band calss information cdma2000 */
				p_cell_set_req->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.band_class_list.
					band_class_info_cdma2000[counter].band_class = 
					p_broadcast_info->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.band_class_list.
					band_class_info_cdma2000[counter].band_class;

				if (p_broadcast_info->sib_type_8_Info.params_1_xrtt.
						cell_reselection_params_1_xrtt.band_class_list.
						band_class_info_cdma2000[counter].presence_bitmask &
						BAND_CLASS_INFO_CDMA2000_CELL_RESELECT_PRI_PRESENCE_FLAG)
				{
					/* Set the bitmask */
					p_cell_set_req->sib_type_8_Info.params_1_xrtt.
						cell_reselection_params_1_xrtt.band_class_list.
						band_class_info_cdma2000[counter].presence_bitmask |= 
						BAND_CLASS_INFO_CDMA2000_CELL_RESELECT_PRI_PRESENCE_FLAG;

					p_cell_set_req->sib_type_8_Info.params_1_xrtt.
						cell_reselection_params_1_xrtt.band_class_list.
						band_class_info_cdma2000[counter].cell_reselection_priority = 
						p_broadcast_info->sib_type_8_Info.params_1_xrtt.
						cell_reselection_params_1_xrtt.band_class_list.
						band_class_info_cdma2000[counter].cell_reselection_priority;
				}

				p_cell_set_req->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.band_class_list.
					band_class_info_cdma2000[counter].thresh_x_high = 
					p_broadcast_info->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.band_class_list.
					band_class_info_cdma2000[counter].thresh_x_high;

				p_cell_set_req->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.band_class_list.
					band_class_info_cdma2000[counter].thresh_x_low = 
					p_broadcast_info->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.band_class_list.
					band_class_info_cdma2000[counter].thresh_x_low;
			}

			/* Fill neighbour cell list cdma2000 information */
			p_cell_set_req->sib_type_8_Info.params_1_xrtt.
				cell_reselection_params_1_xrtt.neigh_cell_list.count = 
				p_broadcast_info->sib_type_8_Info.params_1_xrtt.
				cell_reselection_params_1_xrtt.neigh_cell_list.count;

			for (counter = 0; counter < p_cell_set_req->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.neigh_cell_list.count; counter++)
			{
				p_cell_set_req->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.neigh_cell_list.
					neigh_cell_cdma2000[counter] = 
					p_broadcast_info->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.neigh_cell_list.
					neigh_cell_cdma2000[counter];
			}

			p_cell_set_req->sib_type_8_Info.params_1_xrtt.
				cell_reselection_params_1_xrtt.t_reselection_cdma2000 = 
				p_broadcast_info->sib_type_8_Info.params_1_xrtt.
				cell_reselection_params_1_xrtt.t_reselection_cdma2000;

			/* Fill speed state scale factor info */
			if (p_broadcast_info->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.presence_bitmask &
					CELL_RESELECT_CDMA2000_SF_PRESENCE_FLAG)
			{
				/* Set the bitmask */
				p_cell_set_req->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.presence_bitmask |= 
					CELL_RESELECT_CDMA2000_SF_PRESENCE_FLAG;
				p_cell_set_req->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.t_reselection_cdma2000_sf = 
					p_broadcast_info->sib_type_8_Info.params_1_xrtt.
					cell_reselection_params_1_xrtt.t_reselection_cdma2000_sf;
			}
		}
	}
	RRC_CSC_UT_TRACE_EXIT();
}
/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_8_hrpd_params
 *
 *       Updates the cell configuration (HRPD params insib_type_8_info_t) 
 *       with new Configuration as received in CELL RECONFIG from RRM
 *   INPUT  :
 *       cell setup request
 *       broadcast information
 *
 *   RETURNS:
 *       None
 *
 ***************************************************************************/
static void csc_update_sib_type_8_hrpd_params(
		rrc_rrm_cell_setup_req_t   *p_cell_set_req,
		broadcast_config_info_t    *p_broadcast_info)
{
	U8 counter = 0;
	RRC_CSC_UT_TRACE_ENTER();
	if (p_broadcast_info->sib_type_8_Info.presence_bitmask  
			& SIB_8_PARAMS_HRPD_PRESENCE_FLAG)
	{
		/* fill pre regestration information */
		/* Set the bitmask */
		p_cell_set_req->sib_type_8_Info.presence_bitmask |= 
			SIB_8_PARAMS_HRPD_PRESENCE_FLAG;

		p_cell_set_req->sib_type_8_Info.params_hrpd.pre_reg_info_hrpd.
			pre_reg_allowed = p_broadcast_info->sib_type_8_Info.params_hrpd.
			pre_reg_info_hrpd.pre_reg_allowed;

		if (p_broadcast_info->sib_type_8_Info.params_hrpd.pre_reg_info_hrpd.
				presence_bitmask & PRE_REG_INFO_HRPD_ZONE_ID_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_8_Info.params_hrpd.pre_reg_info_hrpd.
				presence_bitmask |= PRE_REG_INFO_HRPD_ZONE_ID_PRESENCE_FLAG;

			p_cell_set_req->sib_type_8_Info.params_hrpd.pre_reg_info_hrpd.
				pre_reg_zone_id = 
				p_broadcast_info->sib_type_8_Info.params_hrpd.
				pre_reg_info_hrpd.pre_reg_zone_id;
		}

		/* Fill secondary pre regestration zone id list */
		if (p_broadcast_info->sib_type_8_Info.params_hrpd.pre_reg_info_hrpd.
				presence_bitmask & PRE_REG_INFO_HRPD_SECONDARY_LST_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_8_Info.params_hrpd.pre_reg_info_hrpd.
				presence_bitmask |= PRE_REG_INFO_HRPD_SECONDARY_LST_PRESENCE_FLAG;

			p_cell_set_req->sib_type_8_Info.params_hrpd.pre_reg_info_hrpd.
				secondary_list.count = 
				p_broadcast_info->sib_type_8_Info.params_hrpd.pre_reg_info_hrpd.
				secondary_list.count;
			for (counter = 0; counter < p_cell_set_req->sib_type_8_Info.
					params_hrpd.pre_reg_info_hrpd.secondary_list.count; counter++)
			{
				l3_memcpy_wrapper(&p_cell_set_req->sib_type_8_Info.params_hrpd.
						pre_reg_info_hrpd.secondary_list.pre_reg_zone_id[counter], 
						&p_broadcast_info->sib_type_8_Info.params_hrpd.pre_reg_info_hrpd.
						secondary_list.pre_reg_zone_id[counter], 
						sizeof(p_cell_set_req->sib_type_8_Info.params_hrpd.
							pre_reg_info_hrpd.secondary_list.pre_reg_zone_id[counter]));
			}
		}

		/* Fill cell reselection parameters of cdma2000 */
		if (p_broadcast_info->sib_type_8_Info.params_hrpd.presence_bitmask 
				& PARAMS_HRPD_CELL_RESELECTION_PARAMS_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_8_Info.params_hrpd.presence_bitmask |= 
				PARAMS_HRPD_CELL_RESELECTION_PARAMS_PRESENCE_FLAG;

			p_cell_set_req->sib_type_8_Info.params_hrpd.
				cell_reselection_params_hrpd.band_class_list.count = 
				p_broadcast_info->sib_type_8_Info.params_hrpd.
				cell_reselection_params_hrpd.band_class_list.count;

			for (counter = 0; counter < p_cell_set_req->sib_type_8_Info.
					params_hrpd.cell_reselection_params_hrpd.band_class_list.count; counter++)
			{
				/* Fill band calss information cdma2000 */
				p_cell_set_req->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.band_class_list.
					band_class_info_cdma2000[counter].band_class = 
					p_broadcast_info->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.band_class_list.
					band_class_info_cdma2000[counter].band_class;

				if (p_broadcast_info->sib_type_8_Info.params_hrpd.
						cell_reselection_params_hrpd.band_class_list.
						band_class_info_cdma2000[counter].presence_bitmask &
						BAND_CLASS_INFO_CDMA2000_CELL_RESELECT_PRI_PRESENCE_FLAG)
				{
					/* Set the bitmask */
					p_cell_set_req->sib_type_8_Info.params_hrpd.
						cell_reselection_params_hrpd.band_class_list.
						band_class_info_cdma2000[counter].presence_bitmask |= 
						BAND_CLASS_INFO_CDMA2000_CELL_RESELECT_PRI_PRESENCE_FLAG;

					p_cell_set_req->sib_type_8_Info.params_hrpd.
						cell_reselection_params_hrpd.band_class_list.
						band_class_info_cdma2000[counter].cell_reselection_priority = 
						p_broadcast_info->sib_type_8_Info.params_hrpd.
						cell_reselection_params_hrpd.band_class_list.
						band_class_info_cdma2000[counter].cell_reselection_priority;
				}

				p_cell_set_req->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.band_class_list.
					band_class_info_cdma2000[counter].thresh_x_high = 
					p_broadcast_info->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.band_class_list.
					band_class_info_cdma2000[counter].thresh_x_high;

				p_cell_set_req->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.band_class_list.
					band_class_info_cdma2000[counter].thresh_x_low = 
					p_broadcast_info->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.band_class_list.
					band_class_info_cdma2000[counter].thresh_x_low;
			}

			/* Fill neighbour cell list cdma2000 information */
			p_cell_set_req->sib_type_8_Info.params_hrpd.
				cell_reselection_params_hrpd.neigh_cell_list.count = 
				p_broadcast_info->sib_type_8_Info.params_hrpd.
				cell_reselection_params_hrpd.neigh_cell_list.count;

			for (counter = 0; counter < p_cell_set_req->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.neigh_cell_list.count; counter++)
			{
				p_cell_set_req->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.neigh_cell_list.
					neigh_cell_cdma2000[counter] = 
					p_broadcast_info->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.neigh_cell_list.
					neigh_cell_cdma2000[counter];
			}

			p_cell_set_req->sib_type_8_Info.params_hrpd.
				cell_reselection_params_hrpd.t_reselection_cdma2000 = 
				p_broadcast_info->sib_type_8_Info.params_hrpd.
				cell_reselection_params_hrpd.t_reselection_cdma2000;

			/* Fill speed state scale factor info */
			if (p_broadcast_info->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.presence_bitmask 
					& CELL_RESELECT_CDMA2000_SF_PRESENCE_FLAG)
			{
				/* Set the bitmask */
				p_cell_set_req->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.presence_bitmask |= 
					CELL_RESELECT_CDMA2000_SF_PRESENCE_FLAG;

				p_cell_set_req->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.t_reselection_cdma2000_sf = 
					p_broadcast_info->sib_type_8_Info.params_hrpd.
					cell_reselection_params_hrpd.t_reselection_cdma2000_sf;
			}
		}
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_8_info
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_set_req,
 *                  broadcast_config_info_t    *p_broadcast_info
 *   OUTPUT       : This function Update the cell configuration (sib_type_8_info_t) 
 *                  with new Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_update_sib_type_8_info(
		rrc_rrm_cell_setup_req_t   *p_cell_set_req,
		broadcast_config_info_t    *p_broadcast_info)
{
	U8 counter = 0;
	U8 bandclass_counter = 0;

	RRC_CSC_UT_TRACE_ENTER();
	/* 1. Fill system information cdma2000 */
	if (p_broadcast_info->sib_type_8_Info.presence_bitmask 
			& SIB_8_SYS_TIME_INFO_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_8_Info.presence_bitmask |= 
			SIB_8_SYS_TIME_INFO_PRESENCE_FLAG;
		p_cell_set_req->sib_type_8_Info.sys_time_info.cdma_eutra_sync = 
			p_broadcast_info->sib_type_8_Info.sys_time_info.cdma_eutra_sync;

		if (p_broadcast_info->sib_type_8_Info.sys_time_info.presence_bitmask & 
				SYS_TIME_INFO_CDMA2000_SYNC_SYS_TIME_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_8_Info.sys_time_info.presence_bitmask |= 
				SYS_TIME_INFO_CDMA2000_SYNC_SYS_TIME_PRESENCE_FLAG;
			l3_memcpy_wrapper(p_cell_set_req->sib_type_8_Info.sys_time_info.
					sync_sys_time,p_broadcast_info->sib_type_8_Info.sys_time_info.
					sync_sys_time, sizeof (p_cell_set_req->sib_type_8_Info.
						sys_time_info.sync_sys_time));
		}
		if (p_broadcast_info->sib_type_8_Info.sys_time_info.presence_bitmask & 
				SYS_TIME_INFO_CDMA2000_ASYNC_SYS_TIME_PRESENCE_FLAG)
		{
			/* Set the bitmask */
			p_cell_set_req->sib_type_8_Info.presence_bitmask |= 
				SYS_TIME_INFO_CDMA2000_ASYNC_SYS_TIME_PRESENCE_FLAG;
			l3_memcpy_wrapper(p_cell_set_req->sib_type_8_Info.sys_time_info.
					async_sys_time, p_broadcast_info->sib_type_8_Info.sys_time_info.
					async_sys_time, sizeof(p_cell_set_req->sib_type_8_Info.
						sys_time_info.async_sys_time));
		}
	}
	/* 2. Search Window size*/
	if (p_broadcast_info->sib_type_8_Info.presence_bitmask 
			& SIB_8_SEARCH_WIN_SIZE_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_8_Info.presence_bitmask |= 
			SIB_8_SEARCH_WIN_SIZE_PRESENCE_FLAG;
		p_cell_set_req->sib_type_8_Info.search_window_size = 
			p_broadcast_info->sib_type_8_Info.search_window_size;
	}

	/* 3. Fill High Rate Packet Data parameters */
	csc_update_sib_type_8_hrpd_params(p_cell_set_req, p_broadcast_info);

	/* 4. Fill 1x Radio Transmission Technology parameters */
	csc_update_sib_type_8_1XRTT_params(p_cell_set_req, p_broadcast_info);

	/*(5) Fill cell reselection parameters of cdma2000_v920 */
	if (p_broadcast_info->sib_type_8_Info.presence_bitmask 
			& SIB_8_CELL_RESEL_PARAMS_HRPD_V920_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_8_Info.presence_bitmask |= 
			SIB_8_CELL_RESEL_PARAMS_HRPD_V920_PRESENCE_FLAG;

		/* Fill neighbour cell list cdma2000 information */
		p_cell_set_req->sib_type_8_Info.
			cell_reselection_params_hrpd_v920.neigh_cell_list_v920.count = 
			p_broadcast_info->sib_type_8_Info.
			cell_reselection_params_hrpd_v920.neigh_cell_list_v920.count;

		for (counter = 0; counter < p_cell_set_req->sib_type_8_Info.
				cell_reselection_params_hrpd_v920.neigh_cell_list_v920.count; counter++)
		{
			p_cell_set_req->sib_type_8_Info.cell_reselection_params_hrpd_v920.
				neigh_cell_list_v920.neigh_cell_cdma2000_v920[counter].
				neigh_cells_per_freq_list_v920.count = 
				p_broadcast_info->sib_type_8_Info.cell_reselection_params_hrpd_v920.
				neigh_cell_list_v920.neigh_cell_cdma2000_v920[counter].
				neigh_cells_per_freq_list_v920.count;

			for(bandclass_counter = 0; bandclass_counter < p_cell_set_req->
					sib_type_8_Info.cell_reselection_params_hrpd_v920.
					neigh_cell_list_v920.neigh_cell_cdma2000_v920[counter].
					neigh_cells_per_freq_list_v920.count ; bandclass_counter++)
			{
				p_cell_set_req->sib_type_8_Info.cell_reselection_params_hrpd_v920.
					neigh_cell_list_v920.neigh_cell_cdma2000_v920[counter].
					neigh_cells_per_freq_list_v920.
					neigh_cells_per_bandclass_cdma2000_v920[bandclass_counter] =
					p_broadcast_info->sib_type_8_Info.cell_reselection_params_hrpd_v920.
					neigh_cell_list_v920.neigh_cell_cdma2000_v920[counter].
					neigh_cells_per_freq_list_v920.
					neigh_cells_per_bandclass_cdma2000_v920[bandclass_counter];
			}
		}
	}
	/* (6) Fill cell reselection parameters of 1_xrtt_v920 */
	if (p_broadcast_info->sib_type_8_Info.presence_bitmask 
			& SIB_8_CELL_RESEL_PARAMS_1_XRTT_V920_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_8_Info.presence_bitmask |= 
			CELL_RESELECTION_PARAM_1_XRTT_PRESENCE_FLAG;

		/* Fill neighbour cell list cdma2000 information */
		p_cell_set_req->sib_type_8_Info.
			cell_reselection_params_1_xrtt_v920.neigh_cell_list_v920.count = 
			p_broadcast_info->sib_type_8_Info.
			cell_reselection_params_1_xrtt_v920.neigh_cell_list_v920.count;

		for (counter = 0; counter < p_cell_set_req->sib_type_8_Info.
				cell_reselection_params_1_xrtt_v920.neigh_cell_list_v920.count; counter++)
		{

			p_cell_set_req->sib_type_8_Info.
				cell_reselection_params_1_xrtt_v920.neigh_cell_list_v920.
				neigh_cell_cdma2000_v920[counter] = 
				p_broadcast_info->sib_type_8_Info.
				cell_reselection_params_1_xrtt_v920.neigh_cell_list_v920.
				neigh_cell_cdma2000_v920[counter];

			for(bandclass_counter = 0; bandclass_counter < p_cell_set_req->
					sib_type_8_Info.cell_reselection_params_1_xrtt_v920.
					neigh_cell_list_v920.neigh_cell_cdma2000_v920[counter].
					neigh_cells_per_freq_list_v920.count ; bandclass_counter++)
			{
				p_cell_set_req->sib_type_8_Info.cell_reselection_params_1_xrtt_v920.
					neigh_cell_list_v920.neigh_cell_cdma2000_v920[counter].
					neigh_cells_per_freq_list_v920.
					neigh_cells_per_bandclass_cdma2000_v920[bandclass_counter] =
					p_broadcast_info->sib_type_8_Info.cell_reselection_params_1_xrtt_v920.
					neigh_cell_list_v920.neigh_cell_cdma2000_v920[counter].
					neigh_cells_per_freq_list_v920.
					neigh_cells_per_bandclass_cdma2000_v920[bandclass_counter];
			}
		}
	}
	/*cr_309 csfb_support_for_dual_rx_ues_r9*/
	if (p_broadcast_info->sib_type_8_Info.presence_bitmask 
			& SIB_8_CSFB_SUPPORT_FOR_DUAL_RX_UES_R9_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_8_Info.presence_bitmask |= 
			SIB_8_CSFB_SUPPORT_FOR_DUAL_RX_UES_R9_PRESENCE_FLAG;
		p_cell_set_req->sib_type_8_Info.csfb_support_for_dual_rx_ues_r9 = 
			p_broadcast_info->sib_type_8_Info.csfb_support_for_dual_rx_ues_r9;
	}
	/*cr_374 ac_barring_config_1_xrtt_r9 */
	if (p_broadcast_info->sib_type_8_Info.presence_bitmask 
			& SIB_8_AC_BARRING_CONFIG_1_XRTT_R9_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_8_Info.presence_bitmask |= 
			SIB_8_AC_BARRING_CONFIG_1_XRTT_R9_PRESENCE_FLAG;

		p_cell_set_req->sib_type_8_Info.ac_barring_config_1_xrtt_r9.
			ac_barring_0_to_9_r9 = p_broadcast_info->sib_type_8_Info.
			ac_barring_config_1_xrtt_r9.ac_barring_0_to_9_r9;

		p_cell_set_req->sib_type_8_Info.ac_barring_config_1_xrtt_r9.
			ac_barring_10_r9  = p_broadcast_info->sib_type_8_Info.
			ac_barring_config_1_xrtt_r9.ac_barring_10_r9;

		p_cell_set_req->sib_type_8_Info.ac_barring_config_1_xrtt_r9.
			ac_barring_11_r9  = p_broadcast_info->sib_type_8_Info.
			ac_barring_config_1_xrtt_r9.ac_barring_11_r9;

		p_cell_set_req->sib_type_8_Info.ac_barring_config_1_xrtt_r9.
			ac_barring_12_r9  = p_broadcast_info->sib_type_8_Info.
			ac_barring_config_1_xrtt_r9.ac_barring_12_r9;

		p_cell_set_req->sib_type_8_Info.ac_barring_config_1_xrtt_r9.
			ac_barring_13_r9  = p_broadcast_info->sib_type_8_Info.
			ac_barring_config_1_xrtt_r9.ac_barring_13_r9;

		p_cell_set_req->sib_type_8_Info.ac_barring_config_1_xrtt_r9.
			ac_barring_14_r9  = p_broadcast_info->sib_type_8_Info.
			ac_barring_config_1_xrtt_r9.ac_barring_14_r9;

		p_cell_set_req->sib_type_8_Info.ac_barring_config_1_xrtt_r9.
			ac_barring_15_r9  = p_broadcast_info->sib_type_8_Info.
			ac_barring_config_1_xrtt_r9.ac_barring_15_r9;

		p_cell_set_req->sib_type_8_Info.ac_barring_config_1_xrtt_r9.
			ac_barring_msg_r9  = p_broadcast_info->sib_type_8_Info.
			ac_barring_config_1_xrtt_r9.ac_barring_msg_r9;

		p_cell_set_req->sib_type_8_Info.ac_barring_config_1_xrtt_r9.
			ac_barring_reg_r9  = p_broadcast_info->sib_type_8_Info.
			ac_barring_config_1_xrtt_r9.ac_barring_reg_r9;

		p_cell_set_req->sib_type_8_Info.ac_barring_config_1_xrtt_r9.
			ac_barring_emg_r9  = p_broadcast_info->sib_type_8_Info.
			ac_barring_config_1_xrtt_r9.ac_barring_emg_r9;
	}
	/*CDMA2000_CSFB:HO-start*/
	/*csfb_registration_param_1xrtt_v920*/
	if (p_broadcast_info->sib_type_8_Info.presence_bitmask 
			& SIB_8_CSFB_REG_PARAM_1_XRTT_R9_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_8_Info.presence_bitmask |= 
			SIB_8_CSFB_REG_PARAM_1_XRTT_R9_PRESENCE_FLAG;
		p_cell_set_req->sib_type_8_Info.csfb_registration_param_1xrtt_v920 = 
			p_broadcast_info->sib_type_8_Info.csfb_registration_param_1xrtt_v920;
	}
	/*CDMA2000_CSFB:HO-end*/
	RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_9_info
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_set_req,
 *                  broadcast_config_info_t    *p_broadcast_info
 *   OUTPUT       : none
 *   DESCRIPTION  : This function Update the cell configuration (sib_type_9_info_t) 
 *                  with new Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_update_sib_type_9_info(
		rrc_rrm_cell_setup_req_t   *p_cell_set_req,
		broadcast_config_info_t    *p_broadcast_info)
{

	RRC_CSC_UT_TRACE_ENTER();
	if (p_broadcast_info->sib_type_9_Info.presence_bitmask  
			& SIB_9_HNB_ID_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_9_Info.presence_bitmask |= 
			SIB_9_HNB_ID_PRESENCE_FLAG;
		p_cell_set_req->sib_type_9_Info.size_of_hnb_id = 
			p_broadcast_info->sib_type_9_Info.size_of_hnb_id;
	}
	if (p_broadcast_info->sib_type_9_Info.presence_bitmask  
			& SIB_9_HNB_ID_PRESENCE_FLAG)
	{
		/* Set the bitmask */
		p_cell_set_req->sib_type_9_Info.presence_bitmask |= 
			SIB_9_HNB_ID_PRESENCE_FLAG;
		l3_memcpy_wrapper(p_cell_set_req->sib_type_9_Info.hnb_id,
				p_broadcast_info->sib_type_9_Info.hnb_id,
				sizeof (p_cell_set_req->sib_type_9_Info.hnb_id));
	}
	RRC_CSC_UT_TRACE_EXIT();
}

#ifdef LTE_EMBMS_SUPPORTED
/*****************************************************************************
 *   FUNCTION NAME: csc_update_sib_type_13_info
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_set_req,
 *                  broadcast_config_info_t    *p_broadcast_info
 *   OUTPUT       : none
 *   DESCRIPTION  : This function Update the cell configuration (sib_type_13_info_t) 
 *                  with new Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_update_sib_type_13_info(
		rrc_rrm_cell_setup_req_t   *p_cell_set_req,
		broadcast_config_info_t    *p_broadcast_info)
{
	RRC_CSC_UT_TRACE_ENTER();

    p_cell_set_req->sib_type_13_Info = p_broadcast_info->sib_type_13_info;

	RRC_CSC_UT_TRACE_EXIT();
}

#endif
/*****************************************************************************
 *   FUNCTION NAME: csc_update_mib_info
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_set_req,
 *                  broadcast_config_info_t    *p_broadcast_info
 *   OUTPUT       : none   
 *   DESCRIPTION  : This function Update the cell configuration (mib_info_t) with new
 *                  Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_update_mib_info
(
 rrc_rrm_cell_setup_req_t   *p_cell_set_req,
 broadcast_config_info_t    *p_broadcast_info
 )
{
	RRC_CSC_UT_TRACE_ENTER();

	/* Update mib infoprmation */
	p_cell_set_req->mib_info = p_broadcast_info->mib_info;

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME : csc_update_phy_channel_config
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_set_req,
 *                  non_broadcast_config_info_t    *p_non_broadcast_info
 *   OUTPUT       : none   
 *
 *   DESCRIPTION  : This function Update the cell configuration 
 *                 (rrc_rrm_phy_channel_config_t) with new
 *                 Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_update_phy_channel_config(
		rrc_rrm_cell_setup_req_t       *p_cell_set_req,
		non_broadcast_config_info_t    *p_non_broadcast_info)
{
	RRC_CSC_UT_TRACE_ENTER();

	/* Fill physical cell parameters */
	if (p_non_broadcast_info->presence_bitmask & 
			CELL_RECONFIG_REQ_API_CELL_PARAMETER_INFO_PRESENCE_FLAG)
	{
		if (p_non_broadcast_info->cell_parameters.bitmask & 
				RRC_RRM_RECONFIG_PHY_CELL_PARAMS_DL_EARFCN_PRESENT)
		{
			p_cell_set_req->cell_config_param.cell_parameters.dl_earfcn = 
				p_non_broadcast_info->cell_parameters.dl_earfcn;
		}
		if (p_non_broadcast_info->cell_parameters.bitmask & 
				RRC_RRM_RECONFIG_PHY_CELL_PARAMS_NUM_OF_ANTENNAS)
		{
			p_cell_set_req->cell_config_param.cell_parameters.num_of_antennas =
				p_non_broadcast_info->cell_parameters.num_of_antennas;
		}
		if (p_non_broadcast_info->cell_parameters.bitmask & 
				RRC_RRM_RECONFIG_PHY_CELL_PARAMS_DL_CYCLIC_PREFIX)
		{
			p_cell_set_req->cell_config_param.cell_parameters.dl_cyclic_prefix = 
				p_non_broadcast_info->cell_parameters.dl_cyclic_prefix;
		}
		if (p_non_broadcast_info->cell_parameters.bitmask & 
				RRC_RRM_RECONFIG_PHY_CELL_PARAMS_RB_SIZE)
		{
			p_cell_set_req->cell_config_param.cell_parameters.rb_size = 
				p_non_broadcast_info->cell_parameters.rb_size;
		}
		if (p_non_broadcast_info->cell_parameters.bitmask & 
				RRC_RRM_RECONFIG_PHY_CELL_ID)
		{
			p_cell_set_req->cell_config_param.cell_parameters.phys_cell_id = 
				p_non_broadcast_info->cell_parameters.phys_cell_id;
		}
		if (p_non_broadcast_info->cell_parameters.bitmask & 
				RRC_RRM_RECONFIG_PHY_CELL_MAX_RS_EPRE_PRESENT)
		{
			p_cell_set_req->cell_config_param.cell_parameters.max_rs_epre = 
				p_non_broadcast_info->cell_parameters.max_rs_epre;
		}
	}

	/* Fill physical sync signals */
	if (p_non_broadcast_info->presence_bitmask & 
			CELL_RECONFIG_REQ_API_SYNC_SIGNAL_INFO_PRESENCE_FLAG)
	{
		p_cell_set_req->cell_config_param.sync_signals = 
			p_non_broadcast_info->sync_signals;
	}

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME : csc_update_mac_config
 *   INPUT        : rrc_rrm_cell_setup_req_t   *p_cell_set_req,
 *                  non_broadcast_config_info_t    *p_non_broadcast_info
 *   OUTPUT       : none   
 *   DESCRIPTION  : This function Update the cell configuration 
 *                  (rrc_mac_config_t) with new
 *                  Configuration as received in CELL RECONFIG from RRM
 *   RETURNS      : None
 *
 ***************************************************************************/
static void csc_update_mac_config(
		rrc_rrm_cell_setup_req_t       *p_cell_set_req,
		non_broadcast_config_info_t    *p_non_broadcast_info)
{
	RRC_CSC_UT_TRACE_ENTER();

	/* Fill mac config parameters */
	if (p_non_broadcast_info->presence_bitmask & 
			CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG)
	{
		if (p_non_broadcast_info->mac_reconfig_params.bitmask &
				RRC_RRM_RECONFIG_DCI_FORMAT_FOR_SI_MESSAGES_PRESENT)
		{
			p_cell_set_req->cell_config_param.
				mac_config.dci_format_for_si_msgs = 
				p_non_broadcast_info->mac_reconfig_params.
				dci_format_for_si_msgs;
		}

		if (p_non_broadcast_info->mac_reconfig_params.bitmask &
				RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1A_PRESENT)
		{
			p_cell_set_req->cell_config_param.
				mac_config.bitmask |= 
				RRC_RRM_RBS_PER_TBS_DCI_1A_PRESENT;
			l3_memcpy_wrapper( &p_cell_set_req->cell_config_param.
					mac_config.num_rbs_per_tbs_dci_1a,
					&p_non_broadcast_info->mac_reconfig_params.
					num_rbs_per_tbs_dci_1a,
					RBS_FOR_DCI_1A_SIZE);
		}
		else if (p_cell_set_req->cell_config_param.mac_config.bitmask &
				RRC_RRM_RBS_PER_TBS_DCI_1A_PRESENT)
		{
			p_cell_set_req->cell_config_param.mac_config.bitmask ^=
				RRC_RRM_RBS_PER_TBS_DCI_1A_PRESENT;
		}

		if (p_non_broadcast_info->mac_reconfig_params.bitmask &
				RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1C_PRESENT)
		{
			p_cell_set_req->cell_config_param.
				mac_config.bitmask |=
				RRC_RRM_RBS_PER_TBS_DCI_1C_PRESENT;
			l3_memcpy_wrapper( &p_cell_set_req->cell_config_param.
					mac_config.num_rbs_per_tbs_dci_1c,
					&p_non_broadcast_info->mac_reconfig_params.
					num_rbs_per_tbs_dci_1c,
					RBS_FOR_DCI_1C_SIZE);
		}
		else if (p_cell_set_req->cell_config_param.mac_config.bitmask &
				RRC_RRM_RBS_PER_TBS_DCI_1C_PRESENT)
		{
			p_cell_set_req->cell_config_param.mac_config.bitmask ^=
				RRC_RRM_RBS_PER_TBS_DCI_1C_PRESENT;
		}

		if (p_non_broadcast_info->mac_reconfig_params.bitmask &
				RRC_RRM_RECONFIG_UL_SYNC_LOSS_TIMER_PRESENT)
		{
			p_cell_set_req->cell_config_param.
				mac_config.bitmask |=
				RRC_RRM_UL_SYNC_LOSS_TIMER_PRESENT;
			p_cell_set_req->cell_config_param.
				mac_config.ul_sync_loss_timer =
				p_non_broadcast_info->mac_reconfig_params.
				ul_sync_loss_timer;
		}
		else if (p_cell_set_req->cell_config_param.mac_config.bitmask &
				RRC_RRM_UL_SYNC_LOSS_TIMER_PRESENT)
		{
			p_cell_set_req->cell_config_param.mac_config.bitmask ^=
				RRC_RRM_UL_SYNC_LOSS_TIMER_PRESENT;
		}

		if (p_non_broadcast_info->mac_reconfig_params.bitmask &
				RRC_RRM_RECONFIG_PUCCH_CQI_SINR_THRESHOLD_PRESENT)
		{
			p_cell_set_req->cell_config_param.
				mac_config.bitmask |=
				RRC_RRM_PUCCH_CQI_SINR_THRESHOLD_PRESENT;
			p_cell_set_req->cell_config_param.
				mac_config.pucch_cqi_sinr_value =
				p_non_broadcast_info->mac_reconfig_params.
				pucch_cqi_sinr_value;
		}
		else if (p_cell_set_req->cell_config_param.mac_config.bitmask &
				RRC_RRM_PUCCH_CQI_SINR_THRESHOLD_PRESENT)
		{
			p_cell_set_req->cell_config_param.mac_config.bitmask ^=
				RRC_RRM_PUCCH_CQI_SINR_THRESHOLD_PRESENT;
		}

		if (p_non_broadcast_info->mac_reconfig_params.bitmask &
				RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT)
		{
			p_cell_set_req->cell_config_param.
				mac_config.bitmask |= 
				RRC_RRM_SI_TRANSMISSION_INFO_PRESENT;
			p_cell_set_req->cell_config_param.
				mac_config.si_transmission_info = 
				p_non_broadcast_info->mac_reconfig_params.
				si_transmission_info;
		}
		else if (p_cell_set_req->cell_config_param.mac_config.bitmask &
				RRC_RRM_SI_TRANSMISSION_INFO_PRESENT)
		{
			p_cell_set_req->cell_config_param.mac_config.bitmask ^=
				RRC_RRM_SI_TRANSMISSION_INFO_PRESENT;
		}

		/* PRS + */
		if (p_non_broadcast_info->mac_reconfig_params.bitmask &
				RRC_RRM_RECONFIG_PRS_INFO_PRESENT)
		{
			p_cell_set_req->cell_config_param.mac_config.bitmask |=
				RRC_RRM_PRS_CONFIG_INFO_PRESENT;
			p_cell_set_req->cell_config_param.mac_config.prs_config_info.request_type= 
				p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.request_type;

			if(p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.bitmask & RRC_RRM_RECONFIG_PRS_CONFIG_PRESENT)
			{
				p_cell_set_req->cell_config_param.mac_config.prs_config_info.bitmask = RRC_RRM_CONFIG_PRS_CONFIG_PRESENT;

				/* PRS_UPDATED + */
				if(p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.prs_reconfig.bitmask & 
						RRC_RRM_RECONFIG_PRS_BANDWIDTH_INFO_PRESENT)
				{
					p_cell_set_req->cell_config_param.mac_config.prs_config_info.prs_config.prs_bandwidth_info =
						p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_bandwidth_info;
				}	
				/* PRS_UPDATED - */

				if(p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.prs_reconfig.bitmask & RRC_RRM_RECONFIG_PRS_SUBFRAMES_INFO_PRESENT)
				{ 
					p_cell_set_req->cell_config_param.mac_config.prs_config_info.prs_config.prs_subframes_info =
						p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_subframes_info;
				}

				if(p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.prs_reconfig.bitmask & RRC_RRM_RECONFIG_PRS_CONFIG_INDEX_INFO_PRESENT)
				{   
					p_cell_set_req->cell_config_param.mac_config.prs_config_info.prs_config.prs_config_index_info =
						p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_config_index_info;
				}

				if(p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.prs_reconfig.bitmask & 
						RRC_RRM_RECONFIG_PRS_TRANSMISSION_POWER_INFO_PRESENT)
				{   
					p_cell_set_req->cell_config_param.mac_config.prs_config_info.prs_config.prs_transmission_power_info =
						p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_transmission_power_info;

				}

				if(p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.prs_reconfig.bitmask & RRC_RRM_RECONFIG_PRS_MUTING_CONFIG_INFO_PRESENT)
				{
					p_cell_set_req->cell_config_param.mac_config.prs_config_info.prs_config.bitmask |= RRC_RRM_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT;

					p_cell_set_req->cell_config_param.mac_config.prs_config_info.prs_config.prs_muting_config_info =
						p_non_broadcast_info->mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_muting_config_info;

				}
			}
		}
		else if (p_cell_set_req->cell_config_param.mac_config.bitmask &
				RRC_RRM_PRS_CONFIG_INFO_PRESENT)
		{
			p_cell_set_req->cell_config_param.mac_config.bitmask ^=
				RRC_RRM_PRS_CONFIG_INFO_PRESENT;
		}

		/* PRS - */
	}

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: csc_send_llim_pws_req
 *
 *       This function sends CSC_LLIM_PWS_REQ to LLIM
 *       on receiving write-replace warning request from S1AP.
 *       STATE: CELL_M_STATE_CELL_IN_SERVICE; 
 *       EVENT: CELL_M_EVENT_ENB_MGR_PWS_REQ
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
static void csc_send_llim_pws_req(
		csc_gl_ctx_t    *p_csc_gl_ctx,  /* Pointer to the CSC global context data */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list,
		s1ap_pws_req_t *p_enb_pws_req,
		U32 	counter_value  
		)
{
	csc_cell_m_timer_buf_t guard_timer_data;
	void            *p_intrl_msg         = PNULL;
	U32             cmas_key             = 0;
	cmas_info_t     *cmas_info_node      = PNULL;
	RRC_ASSERT(PNULL != p_enb_pws_req);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_rrc_si_transmission_info_list);
	RRC_CSC_UT_TRACE_ENTER();
	/* Fill and send CSC_LLIM_PWS_REQ to LLIM*/
	p_intrl_msg = csc_alloc_intrl_msg(RRC_LLIM_MODULE_ID,
			CSC_LLIM_PWS_REQ,
			sizeof(rrc_csc_llim_pws_req_t));

	if (PNULL != p_intrl_msg)
	{
		rrc_csc_llim_pws_req_t *p_llim_pws_req = (rrc_csc_llim_pws_req_t *)
			((U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

		/*Storing Transaction Id in rrc_csc_llim_pws_req_t */
		p_llim_pws_req->transaction_id = p_cell_ctx->transaction_id;
		p_llim_pws_req->cell_index = p_cell_ctx->cell_index;
		/* Store the SFN ,using SFN = SFN(curr) + OFFSET */
		p_llim_pws_req->sfn = (rrc_sfn_t)(p_cell_ctx->sfn +
				CSC_PWS_SFN_OFFSET) ;

		p_llim_pws_req->sfn = (p_llim_pws_req->sfn % RRC_MAX_SFN_VALUE);

		/* Check and fill SIB1 and SIs info */
		if (RRC_SUCCESS == bch_m_sib1_sis_pws(p_llim_pws_req,
					&(p_enb_pws_req->scheduling_info_list), 
					p_cell_ctx, RRC_FALSE,
					p_rrc_si_transmission_info_list))
		{
			if ((p_cell_ctx->cell_reconfig_counter < CELL_RECONFIG_OFFSET) &&
					p_cell_ctx->cell_reconfig_counter > 0)
			{
				RRC_CSC_TRACE(RRC_INFO,"Cell reconfiguration has just finished");
				/* Store the SFN ,using SFN = SFN(curr) + OFFSET */
				p_llim_pws_req->sfn = (rrc_sfn_t)(p_cell_ctx->sfn +
						CSC_PWS_SFN_OFFSET) ;
				p_llim_pws_req->sfn = (p_llim_pws_req->sfn % RRC_MAX_SFN_VALUE);
			}
			/*case 3 on going*/
			/* --. set guard timer */
			guard_timer_data.auto_delete      = RRC_TRUE;
			guard_timer_data.target_submodule = CSC_SUBMODULE_CELL_M;
			guard_timer_data.cell_index       = p_cell_ctx->cell_index;
			guard_timer_data.cell_m_event     = CELL_M_EVENT_PWS_TIMER_EXPIRED;

			RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_PWS_TIMER] %u ms.",
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
				RRC_CSC_TRACE(RRC_ERROR,"CSC_PWS_TIMER start timer failed");
				return;
			}

			/* Setting the counter value */
			if (RRC_FALSE == p_cell_ctx->cmas_flag)
			{
				p_cell_ctx->etws_counter = counter_value;
				p_cell_ctx->etws_time_elapsed = 0;
			}
			else
			{
				cmas_key = (U32) ( p_enb_pws_req->message_identifier[1] << 24 );
				cmas_key |= (U32) ( p_enb_pws_req->message_identifier[0] << 16 );
				cmas_key |= (U32) ( p_enb_pws_req->serial_number[1] << 8 );
				cmas_key |= (U32) ( p_enb_pws_req->serial_number[0]);
				cmas_info_node = (cmas_info_t *)(st_find_tree_node( 
							(const search_tree_t *)&(p_cell_ctx->cmas_info_list),
							(const void*)&cmas_key));
				if (PNULL != cmas_info_node)
				{
					cmas_info_node->cmas_counter = counter_value;
					cmas_info_node->cmas_time_elapsed = 0;
				}
				else
				{
					RRC_CSC_TRACE(RRC_ERROR,"CMAS node not found");
					cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
							RRM_RRC_INTERNAL_FAILURE,
							p_cell_ctx);
					return;
				}
			}
			RRC_CSC_UT_TRACE_EXIT();
			rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
			/* STATE TRANSITION */
			CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_W_FOR_PWS_RESP);

		}
		else
		{
			/* Bad incoming info */
			RRC_CSC_TRACE(RRC_ERROR, "Bad message - UNABLE TO ENCODE SI/SIBs.");
			if (p_intrl_msg)
			{
				rrc_msg_mem_free(p_intrl_msg);
			}
			/* Stop guard timer */
			if (PNULL != p_cell_ctx->guard_timer)
			{
				rrc_stop_timer(p_cell_ctx->guard_timer);
				p_cell_ctx->guard_timer = PNULL;
			}
			RRC_CSC_UT_TRACE_EXIT();
			cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
					RRM_RRC_INTERNAL_FAILURE,
					p_cell_ctx);
		}
	}
	else
	{
		/*Initialize the PWS Info in Cell Context */
		RRC_CSC_UT_TRACE_EXIT();
		init_pws_info (p_cell_ctx);   
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_pws_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *   DESCRIPTION  : This function handles write-replace warning request from S1AP.
 *                   STATE: CELL_M_STATE_CELL_IN_SERVICE; 
 *                  EVENT: CELL_M_EVENT_ENB_MGR_PWS_REQ
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_pws_req(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	s1ap_pws_req_t *p_enb_pws_req = (s1ap_pws_req_t *)p_msg;
	U32             cmas_key             = 0;
	U32             counter_value        = 0;
	U8              cmas_count           = 0;
	cmas_info_t     *temp_cmas_info_node = PNULL;
	rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_enb_pws_req);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	/*Update Transaction ID in cell context*/
	p_cell_ctx->transaction_id = p_enb_pws_req->transaction_id;

	/*Initialize the PWS Info in Cell Context */
	init_pws_info (p_cell_ctx);   

	/*Store PWS req in cell context*/
	p_cell_ctx->p_enb_pws_req = rrc_mem_get(
			sizeof(s1ap_pws_req_t));

	if( PNULL == p_cell_ctx->p_enb_pws_req )
	{
		RRC_CSC_TRACE(RRC_FATAL, "Unable to allocate Memory; Unused variable p_api=%p",p_api);/*SPR 17777 +-*/
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	l3_memcpy_wrapper(p_cell_ctx->p_enb_pws_req,
			p_enb_pws_req,
			sizeof(s1ap_pws_req_t));

	p_cell_ctx->p_temp_si_schdl_info = rrc_mem_get(sizeof(si_schdl_info_t));

	if( PNULL == p_cell_ctx->p_temp_si_schdl_info )
	{
		RRC_CSC_TRACE(RRC_FATAL, "Unable to allocate Memory");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	memset_wrapper ( p_cell_ctx->p_temp_si_schdl_info, RRC_NULL, 
			sizeof(si_schdl_info_t) );

	/*Check the Scheduling info recieved in the S1AP_PWS_REQ */
	if (SI_MAPPING_CHANGED == csc_check_pws_scheduling_info(p_cell_ctx,
				p_enb_pws_req))
	{
		RRC_CSC_TRACE(RRC_ERROR,"Scheduling Info is not correct in PWS req");  
		cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
				RRM_RRC_INTERNAL_FAILURE,
				p_cell_ctx);
		return;
	}
	p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = SI_MAPPING_NOT_CHANGED;
	RRC_CSC_TRACE(RRC_INFO,"Scheduling Info is correct in PWS req"); 
	/* CR 852 Changes Start*/
	if (RRC_FAILURE == check_duplicate_warning_message(p_cell_ctx))
	{
		cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
				RRM_RRC_DUPLICATE_WARNING_MESSAGE,
				p_cell_ctx);
		return;
	}
	/* CR 852 Changes End*/

	/* Check whether the received S1AP_PWS_REQ is for CMAS or for ETWS */ 
	if (( PWS_ETWS_SCHEDULING_INFO & p_enb_pws_req->bitmask ) &&
			(0 == memcmp_wrapper(&(p_enb_pws_req->message_identifier),
					     &(p_enb_pws_req->etws_scheduling_info.message_identifier),
					     sizeof(U16))) && (0 == memcmp_wrapper(&(p_enb_pws_req->serial_number), 
					     &(p_enb_pws_req->etws_scheduling_info.serial_number), sizeof(U16))))
	{
		p_cell_ctx->cmas_flag = RRC_FALSE;
		if (RRC_FAILURE == validate_and_fill_etws_warning (
					p_cell_ctx, &counter_value))
		{
			return;
		}
	}
	else
	{   
		if (PWS_CMAS_SCHEDULING_INFO_LIST & p_enb_pws_req->bitmask)
		{
			p_cell_ctx->cmas_flag = RRC_TRUE;
			if (RRC_FAILURE == validate_and_fill_cmas_warning (
						p_cell_ctx, &counter_value))
			{
				return;
			}
		}
		else
		{
			RRC_CSC_TRACE(RRC_INFO,"Received wrong message id and serial no."); 
			cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
					RRM_RRC_INTERNAL_FAILURE,
					p_cell_ctx);
			/* STATE TRANSITION */
			CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
			return;
		}   
	}
	if (RRC_NULL == ((PWS_ETWS_SCHEDULING_INFO & p_enb_pws_req->bitmask) ||
				(PWS_CMAS_SCHEDULING_INFO_LIST & p_enb_pws_req->bitmask)))
	{
		RRC_CSC_TRACE(RRC_ERROR,
				"ETWS or CMAS scheduling info must be present");
		cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
				RRM_RRC_INTERNAL_FAILURE,
				p_cell_ctx);
		return;
	}
	if (PWS_ETWS_SCHEDULING_INFO & p_enb_pws_req->bitmask)
	{
		if ((PNULL != p_cell_ctx->etws_info) && 
				( RRC_RRM_ETWS_SCHEDULING_NUM_OF_SEGMENTS_PRESENT &
				  p_cell_ctx->p_enb_pws_req->etws_scheduling_info.bitmask ))
		{
			p_cell_ctx->etws_info->sib11_info.num_of_segments =
				p_cell_ctx->p_enb_pws_req->etws_scheduling_info.num_of_segments;
		}
		else if (PNULL != p_cell_ctx->etws_info)
		{
			p_cell_ctx->etws_info->sib11_info.num_of_segments = 1;
		}
	}
	if (PWS_CMAS_SCHEDULING_INFO_LIST & p_enb_pws_req->bitmask)
	{
		/* Change the num of segments and num of repetitions according to the
		   CMAS scheduling received from RRM through S1AP in S1AP_PWS_REQ */
		for (cmas_count = 0; cmas_count < p_cell_ctx->
				p_enb_pws_req->cmas_scheduling_info_list.cmas_warning_count; ++cmas_count)
		{
			cmas_key = (U32) (p_cell_ctx->p_enb_pws_req->cmas_scheduling_info_list. \
					cmas_scheduling_info[cmas_count].message_identifier[1] << 24 );
			cmas_key |= (U32) (p_cell_ctx->p_enb_pws_req->cmas_scheduling_info_list. \
					cmas_scheduling_info[cmas_count].message_identifier[0] << 16 );
			cmas_key |= (U32) (p_cell_ctx->p_enb_pws_req->cmas_scheduling_info_list. \
					cmas_scheduling_info[cmas_count].serial_number[1] << 8 );
			cmas_key |= (U32) (p_cell_ctx->p_enb_pws_req->cmas_scheduling_info_list. \
					cmas_scheduling_info[cmas_count].serial_number[0]);
			temp_cmas_info_node = (cmas_info_t *)(st_find_tree_node( 
						(const search_tree_t *)&(p_cell_ctx->cmas_info_list),
						(const void*)&cmas_key));
			if (PNULL == temp_cmas_info_node)
			{
				RRC_CSC_TRACE(RRC_ERROR,"CMAS scheduling sent by RRM is wrong");  
				cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
						RRM_RRC_INTERNAL_FAILURE,
						p_cell_ctx);
				return;
			}
			temp_cmas_info_node->num_of_segments = p_cell_ctx->p_enb_pws_req->
				cmas_scheduling_info_list.cmas_scheduling_info[cmas_count].num_of_segments;                      
			temp_cmas_info_node->num_of_repetitions = p_cell_ctx->p_enb_pws_req->
				cmas_scheduling_info_list.cmas_scheduling_info[cmas_count].num_of_repetitions;                      
		}
	}
	if (PWS_SI_TRANSMISSION_INFO_LIST & p_enb_pws_req->bitmask)
	{
		p_rrc_si_transmission_info_list =
			&p_cell_ctx->p_enb_pws_req->si_transmission_info;
	}
	/*Check if Cell reconfiguration is on going or not*/
	/* -- There could be three cases
	 * 1) If cell reconfiguration has come and paging has not sent to MAC.
	 * 2) If Paging has been sent but new SIs are to be sent to MAC.
	 * 3) If new SIs has been sent MAC has not started broadcasting
	 *    them. */

	/*Check case 1*/

	if( (p_cell_ctx->mod_period_boud_counter > CSC_SFN_OFFSET) &&
			(RRC_TRUE == p_cell_ctx->si_mod_flag ))
	{
		/*case 1 going on*/
		/*In this case, cell reconfiguration procedure will be dropped*/
		p_cell_ctx->si_mod_flag = RRC_FALSE;

		if (PNULL != p_cell_ctx->cell_reconfig_api)
		{
			rrc_msg_mem_free(p_cell_ctx->cell_reconfig_api);
			p_cell_ctx->cell_reconfig_counter = RRC_NULL;
		}

		RRC_CSC_TRACE(RRC_INFO,"Dropping RRC RRM cell reconfiguration");
		if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(p_cell_ctx->transaction_id,
					p_cell_ctx->cell_index,
					RRC_FAILURE,
					RRM_RRC_INTERACTION_WITH_OTHER_PROCEDURE_FAILURE))
		{
			RRC_CSC_TRACE(RRC_WARNING, 
					"Unable to send RESP FAILURE TO RRM");
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
		p_cell_ctx->rcfg_status_flag = RRC_FALSE;
	}
	/*Check the Cell Reconfig counter */
	else if (p_cell_ctx->cell_reconfig_counter > CELL_RECONFIG_OFFSET)
	{
		/*case 2 on going*/
		/*In this case, cell reconfiguration procedure will be dropped*/
		p_cell_ctx->cell_reconfig_counter = RRC_NULL;

		if (PNULL != p_cell_ctx->cell_reconfig_api)
		{
			rrc_msg_mem_free(p_cell_ctx->cell_reconfig_api);
			p_cell_ctx->cell_reconfig_api = PNULL;
		}

		RRC_CSC_TRACE(RRC_INFO,"Dropping RRC RRM cell reconfiguration");

		if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(p_cell_ctx->transaction_id,
					p_cell_ctx->cell_index,
					RRC_FAILURE,
					RRM_RRC_INTERACTION_WITH_OTHER_PROCEDURE_FAILURE))
		{
			RRC_CSC_TRACE(RRC_WARNING, 
					"Unable to send RESP FAILURE TO RRM");
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
		p_cell_ctx->rcfg_status_flag = RRC_FALSE;
	}

	csc_send_llim_pws_req(p_csc_gl_ctx, p_cell_ctx, p_rrc_si_transmission_info_list,
			p_enb_pws_req, counter_value);
	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_pws_resp_rrm_reconfig_cell_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function start cell Reconfiguration.
 *                  STATE: CELL_M_STATE_W_FOR_PWS_RESP/CELL_M_STATE_W_FOR_KILL_RESP 
 *                  EVENT: CELL_M_EVENT_RRM_RECONFIG_CELL_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_pws_resp_rrm_reconfig_cell_req(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	rrc_rrm_cell_reconfig_req_t *p_reconfig_cell_req  =
		(rrc_rrm_cell_reconfig_req_t *)p_msg;
#endif

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	RRC_ASSERT(PNULL != p_reconfig_cell_req);
	RRC_ASSERT(p_reconfig_cell_req->cell_index == p_cell_ctx->cell_index);
#endif
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);


	RRC_CSC_TRACE(RRC_INFO,"Discarding RRC_RRM_CELL_RECONFIG_REQ"\
			" as PWS enabling process is on going");

	/*  Send RRC_RRM_CELL_RECONFIG_RESP */
	if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
				rrc_get_transaction_id(p_api),
				p_cell_ctx->cell_index,
				RRC_FAILURE, RRM_RRC_INTERACTION_WITH_OTHER_PROCEDURE_FAILURE))
	{
		RRC_CSC_TRACE(RRC_WARNING, 
				"Unable to send RESP FAILURE TO RRM; Unused variable p_msg=%p p_csc_gl_ctx=%p",p_msg,p_csc_gl_ctx);/*SPR 17777 +-*/
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}
	/* Do NOT reset the p_cell_ctx->rcfg_status_flag here. */

	RRC_CSC_UT_TRACE_EXIT();

}



/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_rrm_updated_pws_si_resp
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function start cell Reconfiguration.
 *                  STATE: CELL_M_STATE_CELL_IN_SERVICE; 
 *                  EVENT: CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_rrm_updated_pws_si_resp(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	void *p_intrl_msg = PNULL;
	csc_cell_m_timer_buf_t          guard_timer_data;
	rrc_rrm_updated_pws_si_list_resp_t 
		*p_updated_pws_si_list_resp  =
		(rrc_rrm_updated_pws_si_list_resp_t*)p_msg;
	U32             cmas_key             = 0;
	U8              cmas_count           = 0;
	cmas_info_t     *temp_cmas_info_node = PNULL;
	rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_updated_pws_si_list_resp);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
	RRC_ASSERT(p_updated_pws_si_list_resp->cell_index 
			== p_cell_ctx->cell_index);

	if(RRC_FAILURE == p_updated_pws_si_list_resp->response)
	{
		RRC_CSC_TRACE(RRC_ERROR, "RRC_RRM_UPDATED_PWS_SI_LIST_RESP: "
				"Failure received from RRM");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	if (PNULL != p_cell_ctx->p_temp_si_schdl_info)
	{
		rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
		p_cell_ctx->p_temp_si_schdl_info = PNULL;
	}

	/*Allocating memory for Temp SI Scheduling Info*/
	p_cell_ctx->p_temp_si_schdl_info = rrc_mem_get(sizeof (si_schdl_info_t));

	if( PNULL == p_cell_ctx->p_temp_si_schdl_info )
	{
		RRC_CSC_TRACE(RRC_FATAL, "Unable to allocate Memeory; Unused variable p_api=%p",p_api);/*SPR 17777 +-*/
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	memset_wrapper ( p_cell_ctx->p_temp_si_schdl_info, RRC_NULL, 
			sizeof(si_schdl_info_t) );
	memset_wrapper(p_cell_ctx->p_temp_si_schdl_info, 0, sizeof (si_schdl_info_t));

	if(SI_MAPPING_CHANGED == csc_check_si_mapping_pws(p_cell_ctx,
				&(p_updated_pws_si_list_resp->scheduling_info_list)))
	{
		RRC_CSC_TRACE(RRC_ERROR,"SI mapping has changed");
		csc_send_rrm_updated_pws_si_list_cnf(p_cell_ctx, RRC_FAILURE);
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = SI_MAPPING_NOT_CHANGED;

	p_cell_ctx->cfg_cell_param.sib_type_1_Info.scheduling_info_list =
		p_updated_pws_si_list_resp->scheduling_info_list;

	if ( UPDATED_PWS_SI_LIST_RESP_SI_TRANSMISSION_INFO_PRESENT & 
			p_updated_pws_si_list_resp->bitmask)
	{
		p_rrc_si_transmission_info_list = 
			&p_updated_pws_si_list_resp->si_transmission_info;
		p_cell_ctx->cfg_cell_param.cell_config_param.mac_config. 
			si_transmission_info = p_updated_pws_si_list_resp->
			si_transmission_info;
	}

	/* Update PWS related SIBs in the Cell Context */
	update_pws_broadcast_config (p_cell_ctx);


	if (( UPDATED_PWS_SI_LIST_RESP_ETWS_INFO_PRESENT & 
				p_updated_pws_si_list_resp->bitmask) &&
			( PNULL != p_cell_ctx->etws_info))
	{
		if (((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask) &&
				( 0 == memcmp_wrapper(&(p_updated_pws_si_list_resp->etws_scheduling_info.
							message_identifier), &(p_cell_ctx->etws_info->sib10_info.msg_id),
						      sizeof(U16))) &&
				( 0 == memcmp_wrapper(&(p_updated_pws_si_list_resp->etws_scheduling_info.
							serial_number), &(p_cell_ctx->etws_info->sib10_info.serial_number),
						      sizeof(U16))))
		{
                    /* Do nothing */
		}
		else if (((8 << RRM_RRC_SIB_TYPE_11) & p_cell_ctx->pws_sibs_bitmask) &&
				( 0 == memcmp_wrapper(&(p_updated_pws_si_list_resp->etws_scheduling_info.
							message_identifier), &(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.
								msg_id),sizeof(U16))) &&
				( 0 == memcmp_wrapper(&(p_updated_pws_si_list_resp->etws_scheduling_info.
							serial_number), &(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.
								serial_number),sizeof(U16))))
		{
			if ( RRC_RRM_ETWS_SCHEDULING_NUM_OF_SEGMENTS_PRESENT &
					p_updated_pws_si_list_resp->etws_scheduling_info.bitmask )
			{
				p_cell_ctx->etws_info->sib11_info.num_of_segments =
					p_updated_pws_si_list_resp->etws_scheduling_info.num_of_segments;
			}
			else
			{
				p_cell_ctx->etws_info->sib11_info.num_of_segments = 1;
			}
		}
		else
		{
			RRC_CSC_TRACE(RRC_ERROR,"Wrong configuration sent by RRM");
			csc_send_rrm_updated_pws_si_list_cnf(p_cell_ctx, RRC_FAILURE);
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
	}
	if ( UPDATED_PWS_SI_LIST_RESP_CMAS_INFO_PRESENT & 
			p_updated_pws_si_list_resp->bitmask)
	{
		if (((8 << RRM_RRC_SIB_TYPE_12) & p_cell_ctx->pws_sibs_bitmask) &&
				( p_updated_pws_si_list_resp->cmas_scheduling_info_list. \
				  cmas_warning_count == st_get_node_count ((const search_tree_t *)
					  &(p_cell_ctx->cmas_info_list))))
		{
			for (cmas_count = 0; cmas_count < p_updated_pws_si_list_resp-> 
					cmas_scheduling_info_list.cmas_warning_count; ++cmas_count)
			{
				cmas_key = (U32) (p_updated_pws_si_list_resp->cmas_scheduling_info_list. \
						cmas_scheduling_info[cmas_count].message_identifier[1] << 24 );
				cmas_key |= (U32) (p_updated_pws_si_list_resp->cmas_scheduling_info_list. \
						cmas_scheduling_info[cmas_count].message_identifier[0] << 16 );
				cmas_key |= (U32) (p_updated_pws_si_list_resp->cmas_scheduling_info_list. \
						cmas_scheduling_info[cmas_count].serial_number[1] << 8 );
				cmas_key |= (U32) (p_updated_pws_si_list_resp->cmas_scheduling_info_list. \
						cmas_scheduling_info[cmas_count].serial_number[0]);
				temp_cmas_info_node = (cmas_info_t *)(st_find_tree_node( (const search_tree_t *)
							&(p_cell_ctx->cmas_info_list), (const void*)&cmas_key));
				if (PNULL == temp_cmas_info_node)
				{
					RRC_CSC_TRACE(RRC_ERROR,"Wrong configuration sent by RRM");
					csc_send_rrm_updated_pws_si_list_cnf(p_cell_ctx, RRC_FAILURE);
					RRC_CSC_UT_TRACE_EXIT();
					return;
				}
				temp_cmas_info_node->num_of_segments = p_updated_pws_si_list_resp->
					cmas_scheduling_info_list.cmas_scheduling_info[cmas_count].num_of_segments;                      
				temp_cmas_info_node->num_of_repetitions = p_updated_pws_si_list_resp->
					cmas_scheduling_info_list.cmas_scheduling_info[cmas_count].num_of_repetitions;                      
			}
		}
		else
		{
			RRC_CSC_TRACE(RRC_ERROR,"Wrong configuration sent by RRM");
			csc_send_rrm_updated_pws_si_list_cnf(p_cell_ctx, RRC_FAILURE);
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
	}
	/* --. set guard timer */
	guard_timer_data.auto_delete        = RRC_TRUE;
	guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
	guard_timer_data.cell_index         = p_cell_ctx->cell_index;
	guard_timer_data.cell_m_event       =
		CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED;


	if(PNULL != p_cell_ctx->guard_timer)
	{
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	p_cell_ctx->guard_timer = rrc_start_timer(
			p_csc_gl_ctx->cell_recfg_duration,
			&guard_timer_data, 
			sizeof (guard_timer_data), 
			RRC_FALSE);

	RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_UPDATED_PWS_SI_TIMER] %u ms. " 
			"for updated PWS SI",
			p_csc_gl_ctx->cell_recfg_duration);

	/* 5. Fill and send CSC_LLIM_RECONFIG_CELL_REQ to LLIM*/
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_LLIM_MODULE_ID,
			CSC_LLIM_UPDATED_PWS_SI_REQ,
			sizeof(rrc_csc_llim_updated_pws_si_req_t));

	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		rrc_csc_llim_updated_pws_si_req_t
			*p_updated_pws_si_req =
			(rrc_csc_llim_updated_pws_si_req_t*)((U8 *)p_intrl_msg +
					RRC_API_HEADER_SIZE);

		/*Storing Transaction Id and Cell index in 
		 *           rrc_csc_llim_pws_req_t*/
		p_updated_pws_si_req->transaction_id = p_cell_ctx->transaction_id;

		/*Cell Index*/
		p_updated_pws_si_req->cell_index = p_cell_ctx->cell_index;

		/* Store the SFN for Non- Broadcast,using SFN = SFN(curr) + OFFSET */
		p_updated_pws_si_req->sfn = (rrc_sfn_t)(p_cell_ctx->sfn +
				CSC_PWS_SFN_OFFSET) ;

		p_updated_pws_si_req->sfn = (p_updated_pws_si_req->sfn % RRC_MAX_SFN_VALUE);


		if (RRC_SUCCESS == bch_m_sib1_sis_updated_pws(
					p_updated_pws_si_req,
					p_cell_ctx,
					p_rrc_si_transmission_info_list))
		{
			rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);

			/* STATE TRANSITION */
			CELL_M_FSM_SET_STATE(p_cell_ctx,
					CELL_M_STATE_W_FOR_PWS_RESP);
		}
		else
		{
			/* Bad incoming info */
			RRC_CSC_TRACE(RRC_ERROR, 
					"Bad message - UNABLE TO ENCODE SI/SIB AND MIBs.");

			if (p_intrl_msg)
			{
				/* CSC didn't send message - free memory */
				rrc_msg_mem_free(p_intrl_msg);
			}

			/* Stop guard timer */
			if (PNULL != p_cell_ctx->guard_timer)
			{
				rrc_stop_timer(p_cell_ctx->guard_timer);
				p_cell_ctx->guard_timer = PNULL;
			}

			rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
			p_cell_ctx->p_temp_si_schdl_info = PNULL;

			/*  Send RRC_RRM_UPDATED_PWS_SI_LIST_CNF failure*/
			csc_send_rrm_updated_pws_si_list_cnf(p_cell_ctx, RRC_FAILURE);
		}
	}
	else
	{
		RRC_CSC_TRACE(RRC_FATAL, "UNABLE TO ALLOCATE MEMORY ");
		/* Stop guard timer */
		if(PNULL != p_cell_ctx->guard_timer)
		{
			rrc_stop_timer(p_cell_ctx->guard_timer);
			p_cell_ctx->guard_timer = PNULL;
			rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
			p_cell_ctx->p_temp_si_schdl_info = PNULL;
		}    
	}
	RRC_CSC_UT_TRACE_EXIT();

}

/** CELL_M_STATE_W_FOR_PWS_RESP state handlers **/
/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_for_pws_resp_llim_pws_resp
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function Handles the PWS resp from LLIM
 *                  STATE: CELL_M_STATE_W_FOR_PWS_RESP
 *                  EVENT: CELL_M_EVENT_LLIM_PWS_RESP
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_for_pws_resp_llim_pws_resp(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	void *p_intrl_msg = PNULL;

	s1ap_pws_resp_t *p_s1ap_pws_resp;

	rrc_csc_llim_pws_resp_t *p_pws_resp =
		(rrc_csc_llim_pws_resp_t *)p_msg;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_pws_resp);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
	RRC_ASSERT(p_pws_resp->cell_index == p_cell_ctx->cell_index);

	/* Precondition - should be the same transaction */
	if (p_pws_resp->transaction_id != p_cell_ctx->transaction_id)
	{
		RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
				"ignored");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	/* 1. Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_PWS_TIMER];Unused variable p_api=%p",p_api);/*SPR 17777 +-*/
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	/* 2. Handling the ResponseCode*/
	switch(p_pws_resp->response)
	{
		case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS:

			RRC_CSC_TRACE(RRC_BRIEF, "Sending PWS Paging to LLIM");

			p_intrl_msg = csc_alloc_intrl_msg(RRC_S1AP_MODULE_ID,
					S1AP_PWS_RESP,
					sizeof(s1ap_pws_resp_t)); 

			if(PNULL == p_intrl_msg)
			{
				RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
				return;
			}

			p_s1ap_pws_resp = 
				(s1ap_pws_resp_t*)
				((U8 *)p_intrl_msg +
				 RRC_API_HEADER_SIZE);

			if(PNULL == p_s1ap_pws_resp)
			{
				RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
				return;
			}
			/* Send PWS resp to eNB manager */
			p_s1ap_pws_resp->cell_index = p_cell_ctx->cell_index;
			p_s1ap_pws_resp->transaction_id = p_cell_ctx->transaction_id;

			p_s1ap_pws_resp->pws_response = RRC_SUCCESS;

			if (RRC_FALSE == p_cell_ctx->cmas_flag)
			{    
				/*Set the paging counter for PWS*/
				p_cell_ctx->etws_paging_counter = p_cell_ctx->mod_period_boud_counter +
					p_cell_ctx->mod_period;
				/* If the PWS req is received less than PWS_OFFSET from the mod boundary */
				if (p_cell_ctx->etws_paging_counter < (p_cell_ctx->mod_period + PWS_OFFSET))
				{
					p_cell_ctx->etws_paging_counter += p_cell_ctx->mod_period;
				}
			}
			else
			{
				/*Set the paging counter for CMAS*/
				p_cell_ctx->cmas_paging_counter = p_cell_ctx->mod_period_boud_counter +
					p_cell_ctx->mod_period;
				/* If the PWS req is received less than PWS_OFFSET from the mod boundary */
				if (p_cell_ctx->cmas_paging_counter < (p_cell_ctx->mod_period + PWS_OFFSET))
				{
					p_cell_ctx->cmas_paging_counter += p_cell_ctx->mod_period;
				}
			}    

			/* Send PAGING API due to PWS*/
			if ((p_cell_ctx->etws_paging_counter > PWS_OFFSET) && 
					(p_cell_ctx->cmas_paging_counter > PWS_OFFSET))
			{    
				csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn, 
						PAGE_ETWS_CMAS, p_cell_ctx
                        );
			}
			else if (p_cell_ctx->cmas_paging_counter > PWS_OFFSET)
			{    
				csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn, 
						PAGE_CMAS, p_cell_ctx
                        );
			}
			else if (p_cell_ctx->etws_paging_counter > PWS_OFFSET)
			{    
				csc_paging_build_and_send_broadcast_info(p_cell_ctx->sfn, 
						PAGE_ETWS, p_cell_ctx
                        );
			}
			else
			{
				RRC_CSC_TRACE(RRC_WARNING, "Not sending Paging to MAC");
			}
			/* store the PWS scheduling info in the cell context */
			p_cell_ctx->cfg_cell_param.sib_type_1_Info.scheduling_info_list =
				p_cell_ctx->p_enb_pws_req->scheduling_info_list;

			rrc_send_message(p_intrl_msg,
					RRC_S1AP_MODULE_ID);

			/* Update the PWS related SIBs in Cell Context */
			update_pws_broadcast_config (p_cell_ctx);
			/* Change the state back to CELL_M_STATE_CELL_IN_SERVICE*/
			CELL_M_FSM_SET_STATE(p_cell_ctx,
					CELL_M_STATE_CELL_IN_SERVICE);
			RRC_CSC_TRACE(RRC_BRIEF, "Sending S1AP PWS resp success to S1AP");
			break;

		case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_TIMER_EXP: 

			/* 2.1 Send Reconfig with Old Configuration */
			csc_send_llim_recfg_req_with_old_cfg_pws(
					p_cell_ctx,
					p_csc_gl_ctx,
					MAC_ONLY); 
			RRC_CSC_UT_TRACE_EXIT();
			return;

		case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_FAIL:
		default:
			cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
					RRM_RRC_LL_CONFIG_FAILURE, p_cell_ctx); 
			/* Change the state back to CELL_M_STATE_CELL_IN_SERVICE*/
			CELL_M_FSM_SET_STATE(p_cell_ctx,
					CELL_M_STATE_CELL_IN_SERVICE);

	}
	if(PNULL != p_cell_ctx->p_enb_pws_req)
	{
		rrc_mem_free(p_cell_ctx->p_enb_pws_req);
		p_cell_ctx->p_enb_pws_req = PNULL;
	}

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_llim_updated_pws_si_resp
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function Handles the UPDATED PWS SI resp from LLIM
 *                  STATE: CELL_M_STATE_W_FOR_PWS_RESP
 *                  EVENT: CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_llim_updated_pws_si_resp(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	rrc_rrm_updated_pws_si_list_cnf_t 
		rrc_rrm_updated_pws_si_cnf;

	rrc_csc_llim_updated_pws_si_resp_t 
		*p_updated_pws_si_resp =
		(rrc_csc_llim_updated_pws_si_resp_t*)p_msg;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_updated_pws_si_resp);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
	RRC_ASSERT(p_updated_pws_si_resp->cell_index == p_cell_ctx->cell_index);

	memset_wrapper(&rrc_rrm_updated_pws_si_cnf,0,
			sizeof(rrc_rrm_updated_pws_si_cnf));


	/* Precondition - should be the same transaction */
	if (p_updated_pws_si_resp->transaction_id != p_cell_ctx->transaction_id)
	{
		RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
				"ignored");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	/* 1. Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_UPDATED_PWS_SI_TIMER];Unused variable p_api=%p p_csc_gl_ctx =%p"
                ,p_api,p_csc_gl_ctx);/*SPR 17777 +-*/
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	rrc_rrm_updated_pws_si_cnf.cell_index = p_cell_ctx->cell_index;
	rrc_rrm_updated_pws_si_cnf.warning_info_list = p_cell_ctx->warning_info_list;

	/* 2. Handling the ResponseCode*/
	switch(p_updated_pws_si_resp->response)
	{
		case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS:

			/* Send updated PWS SI resp to eNB manager */
			rrc_rrm_updated_pws_si_cnf.response = RRC_SUCCESS;

			break;
		default: 
			rrc_rrm_updated_pws_si_cnf.response = RRC_FAILURE;
			RRC_CSC_TRACE(RRC_ERROR,"Failure recvd from LLIM, "
					"Sending failure: RRC_RRM_UPDATED_PWS_SI_LIST_CNF");

	}
	if(RRC_SUCCESS != 
			rrc_rrm_il_send_rrc_rrm_updated_pws_si_list_cnf(
				&rrc_rrm_updated_pws_si_cnf,
				RRC_CSC_MODULE_ID,RRC_RRM_MODULE_ID,p_cell_ctx->transaction_id,
				p_cell_ctx->cell_index))
	{
		RRC_CSC_TRACE(RRC_ERROR,
				"Send failure: RRC_RRM_UPDATED_PWS_SI_LIST_CNF");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	/* Change the state back to CELL_M_STATE_CELL_IN_SERVICE*/
	CELL_M_FSM_SET_STATE(p_cell_ctx,
			CELL_M_STATE_CELL_IN_SERVICE);

	RRC_CSC_UT_TRACE_EXIT();
}



/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_reconfig_cell_timer_expired
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function Handles the reconfig timer expired when expecting a Resp 
 *                  from LLIM for PWS due to roll back. 
 *                  STATE: CELL_M_STATE_W_FOR_PWS_RESP
 *                  EVENT: CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_reconfig_cell_timer_expired(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	RRC_CSC_UT_TRACE_ENTER();

	RRC_CSC_TRACE(RRC_DETAILED,
        "Unused variable in fsm p_api=%p, p_msg=%p", p_api, p_msg);

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);


	if(p_cell_ctx->retry_count < p_csc_gl_ctx->max_cell_reconfig_retry_count )
	{
		/* 2.1 Send Reconfig with Old Configuration */
		csc_send_llim_recfg_req_with_old_cfg_pws(
				p_cell_ctx,
				p_csc_gl_ctx,
				MAC_ONLY);
	}
	else
	{ 
		RRC_CSC_TRACE(RRC_ERROR,"ROll back due to PWS has failed.");  
		cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
				RRM_RRC_UNKNOWN_FAILURE,p_cell_ctx);
		/*Roll back has failed*/
		CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
	}

	RRC_CSC_UT_TRACE_EXIT();

}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_reconfig_cell_pws_failure_timer_expired
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function Handles the reconfig timer expired when expecting a Resp 
 *                  from LLIM for PWS_FAILURE_REQ due to roll back. 
 *                  STATE: CELL_M_STATE_W_FOR_PWS_FAILURE_RESP
 *                  EVENT: CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_reconfig_cell_pws_failure_timer_expired(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	s1ap_pws_failure_resp_t *p_s1ap_pws_failure_resp = PNULL;
	void *p_intrl_msg        = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	RRC_CSC_TRACE(RRC_DETAILED,
        "Unused variable in fsm p_api=%p, p_msg=%p", p_api, p_msg);

	if(p_cell_ctx->retry_count < p_csc_gl_ctx->max_cell_reconfig_retry_count )
	{
		/* 2.1 Send Reconfig with Old Configuration */
		csc_send_llim_recfg_req_with_old_cfg_pws(
				p_cell_ctx,
				p_csc_gl_ctx,
				MAC_ONLY);
	}
	else
	{ 
		RRC_CSC_TRACE(RRC_ERROR,"ROll back due to PWS Failure Req has failed.");  
		p_intrl_msg = csc_alloc_intrl_msg(RRC_S1AP_MODULE_ID,
				S1AP_PWS_FAILURE_RESP,
				sizeof(s1ap_pws_failure_resp_t));
		if(PNULL == p_intrl_msg)
		{
			RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
			return;
		}

		if (PNULL == p_intrl_msg )
		{
			RRC_CSC_TRACE(RRC_WARNING, "Memory Allocation failed");
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}

		p_s1ap_pws_failure_resp = (s1ap_pws_failure_resp_t *)
			((U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

		if (PNULL == p_s1ap_pws_failure_resp )
		{
			RRC_CSC_TRACE(RRC_WARNING, " Invalid Response ");
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}

		p_s1ap_pws_failure_resp->transaction_id = p_cell_ctx->transaction_id;
		p_s1ap_pws_failure_resp->cell_index = p_cell_ctx->cell_index;
		p_s1ap_pws_failure_resp->response = RRC_FAILURE;
		RRC_CSC_TRACE(RRC_INFO, "Sending PWS Failure Resp to S1AP");
		rrc_send_message(p_intrl_msg, RRC_S1AP_MODULE_ID);
		/*Roll back has failed*/
		CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
	}

	RRC_CSC_UT_TRACE_EXIT();

}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_pws_timer_expired
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function Handles the PWS timer expired when expecting a Resp from 
 *                  LLIM
 *                  STATE: CELL_M_STATE_W_FOR_PWS_RESP
 *                  EVENT: CELL_M_EVENT_PWS_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_pws_timer_expired(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	RRC_CSC_TRACE(RRC_DETAILED,
         "Unused variable in fsm p_api=%p, p_msg=%p", p_api, p_msg);

	/* 2.1 Send Reconfig with Old Configuration */
	csc_send_llim_recfg_req_with_old_cfg_pws(
			p_cell_ctx,
			p_csc_gl_ctx,
			MAC_ONLY);

	/* Send S1AP_PWS_RESP as Failure to S1AP */
	cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
			RRM_RRC_INTERNAL_FAILURE, p_cell_ctx); 

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_updated_pws_timer_expired
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *  DESCRIPTION   : This function Handles the PWS timer expired when expecting a Resp from 
 *                  LLIM
 *                  STATE: CELL_M_STATE_W_FOR_PWS_RESP
 *                  EVENT: CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_updated_pws_timer_expired(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	rrc_rrm_updated_pws_si_list_cnf_t 
		rrc_rrm_updated_pws_si_cnf;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	RRC_CSC_TRACE(RRC_DETAILEDALL,
         "Unused variable in fsm p_api=%p, p_msg=%p p_csc_gl_ctx =%p", p_api,
         p_msg,p_csc_gl_ctx);/*SPR 17777 +-*/

	memset_wrapper(&rrc_rrm_updated_pws_si_cnf,0,
			sizeof(rrc_rrm_updated_pws_si_cnf));

	rrc_rrm_updated_pws_si_cnf.cell_index = p_cell_ctx->cell_index;
	rrc_rrm_updated_pws_si_cnf.response = RRC_FAILURE;
	rrc_rrm_updated_pws_si_cnf.warning_info_list = p_cell_ctx->warning_info_list;
	RRC_CSC_TRACE(RRC_ERROR,"Timer Expired, Cell delete expected from RRM"
			"Sending failure: RRC_RRM_UPDATED_PWS_SI_LIST_CNF");
	if(RRC_SUCCESS != 
			rrc_rrm_il_send_rrc_rrm_updated_pws_si_list_cnf(
				&rrc_rrm_updated_pws_si_cnf,
				RRC_CSC_MODULE_ID,RRC_RRM_MODULE_ID,p_cell_ctx->transaction_id,
				p_cell_ctx->cell_index))
	{
		RRC_CSC_TRACE(RRC_ERROR,
				"Send failure: RRC_RRM_UPDATED_PWS_SI_LIST_CNF");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	/* Change the state back to CELL_M_STATE_CELL_IN_SERVICE*/
	CELL_M_FSM_SET_STATE(p_cell_ctx,
			CELL_M_STATE_CELL_IN_SERVICE);

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_pws_resp_enbm_pws_failure_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *       This function Handles the ENBM timer expired at S1AP when expecting 
 *       a Resp from LLIM
 *       STATE: CELL_M_STATE_W_FOR_PWS_RESP
 *       EVENT: CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
static void cell_m_fsm_w_pws_resp_enbm_pws_failure_req(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	U32 cmas_key = RRC_NULL;
	cmas_info_t *cmas_info_node = PNULL;
	s1ap_pws_failure_req_t  *p_s1ap_pws_failure_req = 
		(s1ap_pws_failure_req_t *)p_msg;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_msg);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	/*Update Transaction ID in cell context*/
	p_cell_ctx->transaction_id = p_s1ap_pws_failure_req->transaction_id;


	/* 1. Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_PWS_TIMER]; Unused variable p_api=%p",p_api);/*SPR 17777 +-*/
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}


	/* 2.1 Send Reconfig with Old Configuration */
	csc_send_llim_recfg_req_with_old_cfg_pws(
			p_cell_ctx,
			p_csc_gl_ctx,
			MAC_ONLY);
	if ( PNULL != p_cell_ctx->p_enb_pws_req)
	{
		if ( RRC_FALSE == p_cell_ctx->cmas_flag )
		{
			p_cell_ctx->etws_counter = RRC_NULL;
			p_cell_ctx->etws_paging_counter = RRC_NULL;
			p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_10));
			p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_11));
			rrc_mem_free(p_cell_ctx->etws_info);
			p_cell_ctx->etws_info = PNULL;
		}
		else
		{
			p_cell_ctx->cmas_paging_counter = RRC_NULL;
			cmas_key = (U32) ( p_cell_ctx->p_enb_pws_req->message_identifier[1] << 24 );
			cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->message_identifier[0] << 16 );
			cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->serial_number[1] << 8 );
			cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->serial_number[0]);
			cmas_info_node = (cmas_info_t *)(st_find_tree_node( (const search_tree_t *)
						&(p_cell_ctx->cmas_info_list),
						(const void*)&cmas_key));
			if (PNULL != cmas_info_node)
			{
				st_delete_node (&(p_cell_ctx->cmas_info_list), &(cmas_info_node->anchor));
				rrc_mem_free(cmas_info_node);
			}
			if ( 0 == st_get_node_count ((const search_tree_t *)
						&(p_cell_ctx->cmas_info_list)))
			{
				/* If there is no CMAS warning ongoing, then unset SIB12 bitmask */
				p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_12));
			}
		}
		rrc_mem_free(p_cell_ctx->p_enb_pws_req);
		p_cell_ctx->p_enb_pws_req = PNULL;
		p_cell_ctx->cmas_flag = RRC_FALSE;
	}

	/* Change the state back to CELL_M_STATE_CELL_IN_SERVICE*/
	CELL_M_FSM_SET_STATE(p_cell_ctx,
			CELL_M_STATE_W_FOR_PWS_FAILURE_RESP);

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_pws_resp_state_clean_up_req 
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *       This function abort ongoing operation and clear Cell DB.
 *       state  CELL_M_STATE_W_FOR_PWS_RESP 
 *
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_pws_resp_state_clean_up_req(
		void            *p_api,
		void            *p_msg,
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	RRC_CSC_UT_TRACE_ENTER();

	/*coverity fix start*/
	/*coverity fix stop*/
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	/*coverity fix start*/
	RRC_CSC_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p, p_msg=%p", p_api, p_msg);
	/*coverity fix stop*/

	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_PWS_TIMER]");
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	if ( PNULL != p_cell_ctx->p_enb_kill_req)
	{
		rrc_mem_free(p_cell_ctx->p_enb_kill_req);
		p_cell_ctx->p_enb_kill_req = PNULL;
	}

	/* Delete from Cell DB and free */
	csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);


	RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_kill_timer_expired
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function Handles the KILL timer expired when expecting a Resp from 
 *                  LLIM
 *                  STATE: CELL_M_STATE_W_FOR_KILL_RESP
 *                  EVENT: CELL_M_EVENT_KILL_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_kill_timer_expired(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	RRC_CSC_TRACE(RRC_DETAILEDALL,
        "Unused variable in fsm p_api=%p, p_msg=%p p_csc_gl_ctx=%p", p_api,
        p_msg,p_csc_gl_ctx);/*SPR 17777 +-*/
	RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
			"procedure has failed");

	/*Send KILL resp failure to S1AP*/ 
	csc_send_s1ap_kill_resp_failure(p_cell_ctx->cell_index,
			p_cell_ctx->transaction_id,
			RRC_FAILURE,
			RRC_RRM_UNKNOWN_FAILURE);
	if ( PNULL != p_cell_ctx->p_enb_kill_req)
	{
		rrc_mem_free(p_cell_ctx->p_enb_kill_req);
		p_cell_ctx->p_enb_kill_req = PNULL;
	}
	/* 2.2 Switch state */
	CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_kill_resp_enbm_kill_failure_ind
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *   
 *   DESCRIPTION  : This function Handles the Failure Ind received from S1AP 
 *                  STATE: CELL_M_STATE_W_FOR_KILL_RESP
 *                  EVENT: CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_kill_resp_enbm_kill_failure_ind(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	RRC_CSC_UT_TRACE_ENTER();

	RRC_CSC_TRACE(RRC_DETAILEDALL,
        "Unused variable in fsm p_api=%p, p_msg=%p p_csc_gl_ctx=%p", p_api, p_msg,p_csc_gl_ctx);

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	/* 1. Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_KILL_TIMER]");
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
			"procedure has failed");

	if ( PNULL != p_cell_ctx->p_enb_kill_req)
	{
		rrc_mem_free(p_cell_ctx->p_enb_kill_req);
		p_cell_ctx->p_enb_kill_req = PNULL;
	}

	RRC_CSC_UT_TRACE_EXIT();

}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_kill_resp_state_clean_up_req 
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function abort ongoing operation and clear Cell DB.
 *                  state  CELL_M_STATE_W_FOR_KILL_RESP 
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_kill_resp_state_clean_up_req(
		void            *p_api,
		void            *p_msg,
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	RRC_CSC_UT_TRACE_ENTER();

	/*coverity fix start*/
	/*coverity fix stop*/
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	/*coverity fix start*/
	RRC_CSC_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p, p_msg=%p", p_api, p_msg);
	/*coverity fix stop*/

	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_KILL_TIMER]");
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	/* Delete from Cell DB and free */
	csc_cell_ctx_cleanup(p_csc_gl_ctx,p_cell_ctx);

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_for_pws_resp_rrm_cell_delete_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function abort ongoing operation and clear Cell DB.
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_for_pws_resp_rrm_cell_delete_req(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	rrc_rrm_cell_del_req_t *p_del_cell_req  =
		(rrc_rrm_cell_del_req_t *)p_msg;
#endif
	void *p_intrl_msg        = PNULL;
	s1ap_cell_dreg_req_t *p_s1ap_cell_dreg_req = PNULL;
	csc_cell_m_timer_buf_t guard_timer_data;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	RRC_ASSERT(PNULL != p_del_cell_req);
	RRC_ASSERT(p_del_cell_req->cell_index == p_cell_ctx->cell_index);
#endif
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);



	p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer     = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */

	/*Send PWS resp failure to S1AP*/ 
	if ( PNULL != p_cell_ctx->p_enb_pws_req )
	{
		rrc_mem_free(p_cell_ctx->p_enb_pws_req);
		p_cell_ctx->p_enb_pws_req = PNULL;
	}

	/* 1. set guard timer */
	guard_timer_data.auto_delete        = RRC_TRUE;
	guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
	guard_timer_data.cell_index         = p_cell_ctx->cell_index;
	guard_timer_data.cell_m_event       =
		CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED;

	RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_DEL_TIMER] %u ms.",
			p_csc_gl_ctx->cell_del_duration);
	p_cell_ctx->guard_timer = rrc_start_timer(p_csc_gl_ctx->cell_del_duration,
			&guard_timer_data, sizeof (guard_timer_data), RRC_FALSE);

	/* 5. Send S1AP_CELL_DEREG_REQ */
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_S1AP_MODULE_ID,
			S1AP_CELL_DEREG_REQ,
			sizeof (s1ap_cell_dreg_req_t));
	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		p_cell_ctx->rrc_rrm_del_cell_flag = 1;
		p_s1ap_cell_dreg_req = (s1ap_cell_dreg_req_t *)(
				(U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

		p_s1ap_cell_dreg_req->cell_index = p_cell_ctx->cell_index;
		rrc_send_message(p_intrl_msg, RRC_S1AP_MODULE_ID);
	}
	else
	{
		/* Bad incoming info */
		RRC_CSC_TRACE(RRC_WARNING, "unable to send S1AP_CELL_DEREG_REQ message");
		return;
	}

	/* 2. send CSC_UECC_DEL_CELL_REQ */
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_UECC_MODULE_ID,
			CSC_UECC_DELETE_ALL_UE_REQ,
			sizeof (rrc_csc_uecc_delete_all_ue_req_t));

	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		rrc_csc_uecc_delete_all_ue_req_t *p_uecc_del_cell_req = 
			(rrc_csc_uecc_delete_all_ue_req_t *)((U8 *)p_intrl_msg +
					RRC_API_HEADER_SIZE);
		p_uecc_del_cell_req->cell_index = p_cell_ctx->cell_index;
		p_uecc_del_cell_req->is_cell_stopped = RRC_FALSE;

		rrc_send_message(p_intrl_msg, RRC_UECCMD_MODULE_ID);

		CELL_M_FSM_SET_STATE(p_cell_ctx,
				CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP);
	}
	else
	{
		/* Bad incoming info */
		RRC_CSC_TRACE(RRC_WARNING, "unable to send CSC_UECC_DEL_CELL_REQ message; Unused variable p_msg=%p"
                ,p_msg);/*SPR 17777 +-*/
		return;
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_for_kill_resp_rrm_cell_delete_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *  DESCRIPTION   : This function abort ongoing operation and clear Cell DB.
 *       
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_for_kill_resp_rrm_cell_delete_req(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	rrc_rrm_cell_del_req_t *p_del_cell_req  =
		(rrc_rrm_cell_del_req_t *)p_msg;
#endif
	void *p_intrl_msg        = PNULL;
	s1ap_cell_dreg_req_t *p_s1ap_cell_dreg_req = PNULL;
	csc_cell_m_timer_buf_t guard_timer_data;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	RRC_ASSERT(PNULL != p_del_cell_req);
	RRC_ASSERT(p_del_cell_req->cell_index == p_cell_ctx->cell_index);
#endif
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);


	p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer     = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */

	if ( PNULL != p_cell_ctx->p_enb_kill_req )
	{
		rrc_mem_free(p_cell_ctx->p_enb_kill_req);
		p_cell_ctx->p_enb_kill_req = PNULL;
	}

	/* 1. set guard timer */
	guard_timer_data.auto_delete        = RRC_TRUE;
	guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
	guard_timer_data.cell_index         = p_cell_ctx->cell_index;
	guard_timer_data.cell_m_event       =
		CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED;

	RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_DEL_TIMER] %u ms.",
			p_csc_gl_ctx->cell_del_duration);
	p_cell_ctx->guard_timer = rrc_start_timer(p_csc_gl_ctx->cell_del_duration,
			&guard_timer_data, sizeof (guard_timer_data), RRC_FALSE);

	/* 5. Send S1AP_CELL_DEREG_REQ */
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_S1AP_MODULE_ID,
			S1AP_CELL_DEREG_REQ,
			sizeof (s1ap_cell_dreg_req_t));
	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		p_cell_ctx->rrc_rrm_del_cell_flag = 1;
		p_s1ap_cell_dreg_req = (s1ap_cell_dreg_req_t *)(
				(U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

		p_s1ap_cell_dreg_req->cell_index = p_cell_ctx->cell_index;
		rrc_send_message(p_intrl_msg, RRC_S1AP_MODULE_ID);
	}
	else
	{
		/* Bad incoming info */
		RRC_CSC_TRACE(RRC_WARNING, "unable to send S1AP_CELL_DEREG_REQ message");
		return;
	}

	/* 2. send CSC_UECC_DEL_CELL_REQ */
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_UECC_MODULE_ID,
			CSC_UECC_DELETE_ALL_UE_REQ,
			sizeof (rrc_csc_uecc_delete_all_ue_req_t));

	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		rrc_csc_uecc_delete_all_ue_req_t *p_uecc_del_cell_req = 
			(rrc_csc_uecc_delete_all_ue_req_t *)((U8 *)p_intrl_msg +
					RRC_API_HEADER_SIZE);
		p_uecc_del_cell_req->cell_index = p_cell_ctx->cell_index;
		p_uecc_del_cell_req->is_cell_stopped = RRC_FALSE;

		rrc_send_message(p_intrl_msg, RRC_UECCMD_MODULE_ID);

		CELL_M_FSM_SET_STATE(p_cell_ctx,
				CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP);
	}
	else
	{
		/* Bad incoming info */
		RRC_CSC_TRACE(RRC_WARNING, "unable to send CSC_UECC_DEL_CELL_REQ message; Unused variable p_msg=%p"
                ,p_msg);/*SPR 17777 +-*/
		return;
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset
 *   INPUT        : U8               fail_cause
 *                  csc_cell_ctx_t  *p_cell_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function sends S1AP PWS RESP failure from LLIM
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
		U8               fail_cause,
		csc_cell_ctx_t  *p_cell_ctx)    /* cell context */
{
	void *p_intrl_msg = PNULL;
	s1ap_pws_resp_t *p_s1ap_pws_resp;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);
	p_cell_ctx->cmas_flag = RRC_FALSE;

	p_intrl_msg = csc_alloc_intrl_msg(RRC_S1AP_MODULE_ID,
			S1AP_PWS_RESP,
			sizeof(s1ap_pws_resp_t)); 

	if(PNULL == p_intrl_msg)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
		return;
	}
	p_s1ap_pws_resp = 
		(s1ap_pws_resp_t*)
		((U8 *)p_intrl_msg +
		 RRC_API_HEADER_SIZE);
	if(PNULL == p_s1ap_pws_resp)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
		return;
	}

	/* Precondition - should be the same transaction */

	/* 1. Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_PWS_TIMER]");
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	if (PNULL == p_s1ap_pws_resp )
	{
		RRC_CSC_TRACE(RRC_WARNING, " Invalid Response ");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	p_s1ap_pws_resp->cell_index = p_cell_ctx->cell_index;

	p_s1ap_pws_resp->transaction_id = p_cell_ctx->transaction_id;

	p_s1ap_pws_resp->pws_response = RRC_FAILURE;
	p_s1ap_pws_resp->bitmask |= PWS_RESP_FAIL_CAUSE_PRESENT;
	p_s1ap_pws_resp->pws_fail_cause = 
		fail_cause;


	RRC_CSC_TRACE(RRC_INFO, "Sending PWS Failure to S1AP");
	rrc_send_message(p_intrl_msg,
			RRC_S1AP_MODULE_ID);

	if(PNULL != p_cell_ctx->p_temp_si_schdl_info )
	{
		rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
		p_cell_ctx->p_temp_si_schdl_info = PNULL;
	}
	if(PNULL != p_cell_ctx->p_enb_pws_req )
	{
		rrc_mem_free(p_cell_ctx->p_enb_pws_req);
		p_cell_ctx->p_enb_pws_req = PNULL;
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_build_and_send_s1ap_pws_resp_failure
 *   INPUT        : U8               fail_cause,
 *                  csc_cell_ctx_t  *p_cell_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function sends S1AP PWS RESP failure from LLIM and Reset the 
 *                  configuration stored for ETWS or CMAS in the cell context.
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_build_and_send_s1ap_pws_resp_failure(
		U8               fail_cause,
		csc_cell_ctx_t  *p_cell_ctx)    /* cell context */
{
	void *p_intrl_msg = PNULL;
	U32 cmas_key = RRC_NULL;
	cmas_info_t *cmas_info_node = PNULL;
	s1ap_pws_resp_t *p_s1ap_pws_resp = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);
	if ( PNULL != p_cell_ctx->p_enb_pws_req )
	{ 
		if ( RRC_FALSE == p_cell_ctx->cmas_flag )
		{
			if(PNULL != p_cell_ctx->etws_info )
			{
				rrc_mem_free(p_cell_ctx->etws_info);
				p_cell_ctx->etws_info = PNULL;
			}
			p_cell_ctx->etws_counter        = RRC_NULL;
			p_cell_ctx->etws_paging_counter = RRC_NULL;
			p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_10));
			p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_11));
		}
		else
		{   
			p_cell_ctx->cmas_paging_counter = RRC_NULL;
			cmas_key = (U32) ( p_cell_ctx->p_enb_pws_req->message_identifier[1] << 24 );
			cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->message_identifier[0] << 16 );
			cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->serial_number[1] << 8 );
			cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->serial_number[0]);
			cmas_info_node = (cmas_info_t *)(st_find_tree_node( (const search_tree_t *)
						&(p_cell_ctx->cmas_info_list),
						(const void*)&cmas_key));
			if (PNULL != cmas_info_node)
			{
				st_delete_node (&(p_cell_ctx->cmas_info_list), &(cmas_info_node->anchor));
				rrc_mem_free(cmas_info_node);
			}
			if ( 0 == st_get_node_count ((const search_tree_t *)
						&(p_cell_ctx->cmas_info_list)))
			{
				/* If there is no CMAS warning ongoing, then unset SIB12 bitmask */
				p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_12));
			}
		}
	}
	p_cell_ctx->cmas_flag = RRC_FALSE;
	p_intrl_msg = csc_alloc_intrl_msg(RRC_S1AP_MODULE_ID,
			S1AP_PWS_RESP,
			sizeof(s1ap_pws_resp_t)); 
	if(PNULL == p_intrl_msg)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
		return;
	}

	p_s1ap_pws_resp = 
		(s1ap_pws_resp_t*)
		((U8 *)p_intrl_msg +
		 RRC_API_HEADER_SIZE);
	if(PNULL == p_s1ap_pws_resp)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
		return;
	}

	/* Precondition - should be the same transaction */

	/* 1. Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_PWS_TIMER]");
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	if (PNULL == p_s1ap_pws_resp )
	{
		RRC_CSC_TRACE(RRC_WARNING, " Invalid Response ");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	p_s1ap_pws_resp->cell_index = p_cell_ctx->cell_index;

	p_s1ap_pws_resp->transaction_id = p_cell_ctx->transaction_id;

	p_s1ap_pws_resp->pws_response = RRC_FAILURE;
	p_s1ap_pws_resp->bitmask |= PWS_RESP_FAIL_CAUSE_PRESENT;
	p_s1ap_pws_resp->pws_fail_cause = 
		fail_cause;


	RRC_CSC_TRACE(RRC_INFO, "Sending PWS Failure to S1AP");
	rrc_send_message(p_intrl_msg,
			RRC_S1AP_MODULE_ID);

	if(PNULL != p_cell_ctx->p_temp_si_schdl_info )
	{
		rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
		p_cell_ctx->p_temp_si_schdl_info = PNULL;
	}
	if(PNULL != p_cell_ctx->p_enb_pws_req )
	{
		rrc_mem_free(p_cell_ctx->p_enb_pws_req);
		p_cell_ctx->p_enb_pws_req = PNULL;
	}
	RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_build_and_send_s1ap_kill_resp_failure
 *   INPUT        : U8               fail_cause
 *                  csc_cell_ctx_t  *p_cell_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function sends S1AP PWS RESP failure from LLIM
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
		U8               fail_cause,
		csc_cell_ctx_t  *p_cell_ctx)    /* cell context */
{
	void *p_intrl_msg = PNULL;

	s1ap_kill_resp_t *p_s1ap_kill_resp;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);

	p_intrl_msg = csc_alloc_intrl_msg(RRC_S1AP_MODULE_ID,
			S1AP_KILL_RESP,
			sizeof(s1ap_kill_resp_t)); 
	if(PNULL == p_intrl_msg)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
		return;
	}

	p_s1ap_kill_resp = 
		(s1ap_kill_resp_t*)
		((U8 *)p_intrl_msg +
		 RRC_API_HEADER_SIZE);
	if(PNULL == p_s1ap_kill_resp)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
		return;
	}

	/* Precondition - should be the same transaction */

	/* 1. Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_KILL_TIMER]");
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	if ( PNULL != p_cell_ctx->p_enb_kill_req )
	{
		rrc_mem_free (p_cell_ctx->p_enb_kill_req);
		p_cell_ctx->p_enb_kill_req = PNULL;
	}
	if(PNULL != p_cell_ctx->p_temp_si_schdl_info )
	{
		rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
		p_cell_ctx->p_temp_si_schdl_info = PNULL;
	}

	if (PNULL == p_s1ap_kill_resp )
	{
		RRC_CSC_TRACE(RRC_WARNING, " Invalid Response ");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	p_s1ap_kill_resp->cell_index = p_cell_ctx->cell_index;

	p_s1ap_kill_resp->transaction_id = p_cell_ctx->transaction_id;

	p_s1ap_kill_resp->kill_response = RRC_FAILURE;
	p_s1ap_kill_resp->bitmask |= KILL_RESP_FAIL_CAUSE_PRESENT;
	p_s1ap_kill_resp->kill_fail_cause = 
		fail_cause;


	RRC_CSC_TRACE(RRC_INFO, "Sending KILL Failure to S1AP");
	rrc_send_message(p_intrl_msg,
			RRC_S1AP_MODULE_ID);

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_kill_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function handles write-replace warning request from S1AP.
 *                  STATE: CELL_M_STATE_CELL_IN_SERVICE; 
 *                  EVENT: CELL_M_EVENT_ENB_MGR_KILL_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_kill_req(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	s1ap_kill_req_t *p_enb_kill_req  =
		(s1ap_kill_req_t *)p_msg;
	csc_cell_m_timer_buf_t guard_timer_data;
	void            *p_intrl_msg         = PNULL;
	U32             cmas_key             = 0;
	cmas_info_t     *cmas_info_node      = PNULL;
	rrc_bool_et     kill_warning_found = RRC_FALSE;
	rrc_bool_et     csc_send_failure = RRC_FALSE;
	rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_enb_kill_req);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	do
	{  
		/*Update Transaction ID in cell context*/
		p_cell_ctx->transaction_id = p_enb_kill_req->transaction_id;

		/*Reset number_of_broadcasts in cell context*/
		p_cell_ctx->number_of_broadcasts = 0;

		if(PNULL != p_cell_ctx->p_temp_si_schdl_info )
		{
			rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
			p_cell_ctx->p_temp_si_schdl_info = PNULL;
		}
		/* Check whether the Kill req is received for ETWS or CMAS
		   -- If recieved for ETWS then send the KILL Failure
		   -- If received for CMAS then process it */

		if ( PNULL != p_cell_ctx->etws_info )
		{
			if ((((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask) &&
						( 0 == memcmp_wrapper(&(p_enb_kill_req->message_id), 
								      &(p_cell_ctx->etws_info->sib10_info.msg_id),
								      sizeof(U16))) &&
						( 0 == memcmp_wrapper(&(p_enb_kill_req->serial_number), 
								      &(p_cell_ctx->etws_info->sib10_info.serial_number),
								      sizeof(U16)))))
			{
				RRC_CSC_TRACE(RRC_ERROR,"Kill received for ETWS req");
				csc_send_failure = RRC_TRUE;
				break;
			}
			else if (((8 << RRM_RRC_SIB_TYPE_11) & p_cell_ctx->pws_sibs_bitmask) &&
					( 0 == memcmp_wrapper(&(p_enb_kill_req->message_id), 
							      &(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.msg_id),
							      sizeof(U16))) &&
					( 0 == memcmp_wrapper(&(p_enb_kill_req->serial_number), 
							      &(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.serial_number),
							      sizeof(U16))))
			{
				RRC_CSC_TRACE(RRC_ERROR,"Kill received for ETWS req");
				csc_send_failure = RRC_TRUE;
				break;
			}
		}
		if ((8 << RRM_RRC_SIB_TYPE_12) & p_cell_ctx->pws_sibs_bitmask)
		{
			cmas_key = (U32) (p_enb_kill_req->message_id[1] << 24 );
			cmas_key |= (U32) (p_enb_kill_req->message_id[0] << 16 );
			cmas_key |= (U32) (p_enb_kill_req->serial_number[1] << 8 );
			cmas_key |= (U32) (p_enb_kill_req->serial_number[0]);
			cmas_info_node = (cmas_info_t *)(st_find_tree_node( (const search_tree_t *)
						&(p_cell_ctx->cmas_info_list), (const void*)&cmas_key));

			if (PNULL != cmas_info_node)
			{
				p_cell_ctx->number_of_broadcasts = (U16)((cmas_info_node->cmas_time_elapsed)/
						(U32)((cmas_info_node->num_of_segments) * (cmas_info_node->sib12_si_periodicity)));
				p_cell_ctx->number_of_broadcasts = (U16)(p_cell_ctx->number_of_broadcasts *
						cmas_info_node->num_of_repetitions);
				/* Deleting the node for which KILL request has been recieved */
				st_delete_node (&(p_cell_ctx->cmas_info_list),
						&(cmas_info_node->anchor));
				rrc_mem_free(cmas_info_node);
				if ( 0 == st_get_node_count ((const search_tree_t *)
							&(p_cell_ctx->cmas_info_list)))
				{
					/* If there is no CMAS warning ongoing, then unset SIB12 bitmask */
					p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_12));
				}
				kill_warning_found = RRC_TRUE;
			}
		}
		/* If kill id neither of ETWS or CMAS type then send the failure*/
		if ( RRC_FALSE == kill_warning_found )
		{
			RRC_CSC_TRACE(RRC_ERROR,"No broadcast ongoing for which Kill Req has been received");
			cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
					RRC_RRM_NO_BROADCAST_ONGOING,
					p_cell_ctx);
			break;
		}
		p_cell_ctx->p_temp_si_schdl_info = rrc_mem_get(sizeof(si_schdl_info_t));

		if (PNULL == p_cell_ctx->p_temp_si_schdl_info)
		{
			RRC_CSC_TRACE(RRC_FATAL,"Unable to allocate Memory");
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
		memset_wrapper ( p_cell_ctx->p_temp_si_schdl_info, RRC_NULL, 
				sizeof(si_schdl_info_t) );

		/* Check PWS related mapping */
		if(SI_MAPPING_CHANGED == csc_check_si_mapping_pws(p_cell_ctx,
					&(p_enb_kill_req->scheduling_info_list)))
		{
			RRC_CSC_TRACE(RRC_ERROR,"SI mapping has changed2");
			csc_send_failure = RRC_TRUE;
			break;
		}

		p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = SI_MAPPING_NOT_CHANGED;

		/* Store the new scheduling info in the cell context */ 
		p_cell_ctx->cfg_cell_param.sib_type_1_Info.scheduling_info_list =
			p_enb_kill_req->scheduling_info_list;

		/* Store the new si transmission info in the cell context */ 
		if (KILL_SI_TRANSMISSION_INFO_LIST & p_enb_kill_req->bitmask)
		{
			p_rrc_si_transmission_info_list =
				&p_enb_kill_req->si_transmission_info;

			p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.
				si_transmission_info = p_enb_kill_req->si_transmission_info;

			p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.
				bitmask |= RRC_RRM_SI_TRANSMISSION_INFO_PRESENT;
		}

		/* Update PWS related SIBs in the Cell Context */
		update_pws_broadcast_config (p_cell_ctx);

		/*Store KILL req in cell context*/

		p_cell_ctx->p_enb_kill_req = rrc_mem_get(
				sizeof(s1ap_kill_req_t));

		if(PNULL == p_cell_ctx->p_enb_kill_req)
		{
			RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed;Unused variable p_api=%p",__FUNCTION__,p_api);/*SPR 17777 +-*/
			return;
		}
		l3_memcpy_wrapper(p_cell_ctx->p_enb_kill_req,
				p_enb_kill_req,
				sizeof(s1ap_kill_req_t));

		if (RRC_FAILURE == fill_new_etws_and_cmas_scheduling_info (p_cell_ctx))
		{
			csc_send_failure = RRC_TRUE;
			break;
		}

		/*Check if Cell reconfiguration is on going or not*/
		/* -- There could be three cases
		 * 1) If cell reconfiguration has come and paging has not sent to MAC.
		 * 2) If Paging has been sent but new SIs are to be sent to MAC.
		 * 3) If new SIs has been sent MAC has not started broadcasting
		 *    them. */

		/*Check case 1*/

		if( (p_cell_ctx->mod_period_boud_counter > CSC_SFN_OFFSET) &&
				(RRC_TRUE == p_cell_ctx->si_mod_flag ))
		{
			/*case 1 going on*/
			/*In this case, cell reconfiguration procedure will be dropped*/
			p_cell_ctx->si_mod_flag = RRC_FALSE;

			if (PNULL != p_cell_ctx->cell_reconfig_api)
			{
				rrc_msg_mem_free(p_cell_ctx->cell_reconfig_api);
				p_cell_ctx->cell_reconfig_counter = RRC_NULL;
			}

			RRC_CSC_TRACE(RRC_INFO,"Dropping RRC RRM cell reconfiguration");
			if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(p_cell_ctx->transaction_id,
						p_cell_ctx->cell_index,
						RRC_FAILURE,
						RRM_RRC_INTERNAL_FAILURE))
			{
				RRC_CSC_TRACE(RRC_WARNING, 
						"Unable to send RESP FAILURE TO RRM");
				RRC_CSC_UT_TRACE_EXIT();
				return;
			}
		}
		/*Check the Cell Reconfig counter */
		else if (p_cell_ctx->cell_reconfig_counter > CELL_RECONFIG_OFFSET)
		{
			/*case 2 on going*/
			/*In this case, cell reconfiguration procedure will be dropped*/
			p_cell_ctx->cell_reconfig_counter = RRC_NULL;

			if (PNULL != p_cell_ctx->cell_reconfig_api)
			{
				rrc_msg_mem_free(p_cell_ctx->cell_reconfig_api);
				p_cell_ctx->cell_reconfig_api = PNULL;
			}

			RRC_CSC_TRACE(RRC_INFO,"Dropping RRC RRM cell reconfiguration");

			if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure(p_cell_ctx->transaction_id,
						p_cell_ctx->cell_index,
						RRC_FAILURE,
						RRM_RRC_INTERNAL_FAILURE))
			{
				RRC_CSC_TRACE(RRC_WARNING, 
						"Unable to send RESP FAILURE TO RRM");
				RRC_CSC_UT_TRACE_EXIT();
				return;
			}

		}

		/* --. set guard timer */
		guard_timer_data.auto_delete        = RRC_TRUE;
		guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
		guard_timer_data.cell_index         = p_cell_ctx->cell_index;
		guard_timer_data.cell_m_event       =
			CELL_M_EVENT_KILL_TIMER_EXPIRED;

		RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_KILL_TIMER] %u ms.",
				p_csc_gl_ctx->kill_duration);

		if(PNULL != p_cell_ctx->guard_timer)
		{
			rrc_stop_timer(p_cell_ctx->guard_timer);
			p_cell_ctx->guard_timer = PNULL;
		}

		p_cell_ctx->guard_timer = rrc_start_timer(
				p_csc_gl_ctx->kill_duration,
				&guard_timer_data, 
				sizeof (guard_timer_data), 
				RRC_FALSE);

		if(PNULL == p_cell_ctx->guard_timer)
		{
			RRC_CSC_TRACE(RRC_ERROR,"CSC_KILL_TIMER start timer failed");
			return;
		}

		/* Fill and send CSC_LLIM_PWS_REQ to LLIM*/
		p_intrl_msg = csc_alloc_intrl_msg( 
				RRC_LLIM_MODULE_ID,
				CSC_LLIM_PWS_REQ,
				sizeof(rrc_csc_llim_pws_req_t));
		if (PNULL != p_intrl_msg)
		{
			rrc_csc_llim_pws_req_t *p_llim_pws_req =
				(rrc_csc_llim_pws_req_t *)((U8 *)p_intrl_msg +
						RRC_API_HEADER_SIZE);

			/*Storing Transaction Id in 
			 *           rrc_csc_llim_pws_req_t */
			p_llim_pws_req->transaction_id = p_cell_ctx->transaction_id;
			p_llim_pws_req->cell_index = p_cell_ctx->cell_index;
			/* Store the SFN ,using SFN = SFN(curr) + OFFSET */
			p_llim_pws_req->sfn = (rrc_sfn_t)(p_cell_ctx->sfn +
					CSC_PWS_SFN_OFFSET) ;

			p_llim_pws_req->sfn = (p_llim_pws_req->sfn % RRC_MAX_SFN_VALUE);
			/* Check and fill SIB1 and SIs info */
			if (RRC_SUCCESS == bch_m_sib1_sis_pws(
						p_llim_pws_req,
						&(p_enb_kill_req->scheduling_info_list), 
						p_cell_ctx,
						RRC_TRUE,
						p_rrc_si_transmission_info_list))
			{
				if ((p_cell_ctx->cell_reconfig_counter < CELL_RECONFIG_OFFSET) &&
						p_cell_ctx->cell_reconfig_counter > 0)
				{
					RRC_CSC_TRACE(RRC_INFO,"Cell reconfiguration has just finished");
					/* Store the SFN ,using SFN = SFN(curr) + OFFSET */
					p_llim_pws_req->sfn = (rrc_sfn_t)(p_cell_ctx->sfn +
							CSC_PWS_SFN_OFFSET) ;
					p_llim_pws_req->sfn = (p_llim_pws_req->sfn % RRC_MAX_SFN_VALUE);
				}
				/*case 3 on going*/

				rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);

				/* STATE TRANSITION */
				CELL_M_FSM_SET_STATE(p_cell_ctx,
						CELL_M_STATE_W_FOR_KILL_RESP);

			}
			else
			{
				/* Bad incoming info */
				RRC_CSC_TRACE(RRC_ERROR, 
						"Bad message - UNABLE TO ENCODE SI/SIBs.");
				/* Stop guard timer */
				if (PNULL != p_cell_ctx->guard_timer)
				{
					rrc_stop_timer(p_cell_ctx->guard_timer);
					p_cell_ctx->guard_timer = PNULL;
				}
				if (p_intrl_msg)
				{
					rrc_msg_mem_free(p_intrl_msg);
				}
				csc_send_failure = RRC_TRUE;
				break;
			}
		}
	}while(0);
	/* Failure for kill response has to be sent */
	if ( RRC_TRUE == csc_send_failure)
	{
		RRC_CSC_TRACE(RRC_ERROR, 
				"Failure for kill is sent and expecting"
				" cell delete from RRM");
		cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
				RRC_RRM_UNKNOWN_FAILURE,
				p_cell_ctx);
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_for_pws_resp_llim_kill_resp
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 * DESCRIPTION    : This function Handles the PWS resp from LLIM
 *                  STATE: CELL_M_STATE_W_FOR_KILL_RESP
 *                  EVENT: CELL_M_EVENT_LLIM_PWS_RESP
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_for_pws_resp_llim_kill_resp(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	void *p_intrl_msg = PNULL;

	s1ap_kill_resp_t *p_s1ap_kill_resp;

	rrc_csc_llim_pws_resp_t *p_pws_resp =
		(rrc_csc_llim_pws_resp_t *)p_msg;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_pws_resp);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
	RRC_ASSERT(p_pws_resp->cell_index == p_cell_ctx->cell_index);

	p_intrl_msg = csc_alloc_intrl_msg(RRC_S1AP_MODULE_ID,
			S1AP_KILL_RESP,
			sizeof(s1ap_kill_resp_t)); 
	if(PNULL == p_intrl_msg)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed; Unused variable p_api=%p",__FUNCTION__,p_api);/*SPR 17777 +-*/
		return;
	}

	p_s1ap_kill_resp = 
		(s1ap_kill_resp_t*)
		((U8 *)p_intrl_msg +
		 RRC_API_HEADER_SIZE);
	if(PNULL == p_s1ap_kill_resp)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
		return;
	}

	/* Precondition - should be the same transaction */
	if (p_pws_resp->transaction_id != p_cell_ctx->transaction_id)
	{
		RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
				"ignored");
		RRC_CSC_UT_TRACE_EXIT();
		/* coverity_fix_48254_start */
		rrc_msg_mem_free(p_intrl_msg);
		/* coverity_fix_48254_stop */
		return;
	}

	/* 1. Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_KILL_TIMER]");
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	p_s1ap_kill_resp->bitmask = 0;
	p_s1ap_kill_resp->cell_index = p_cell_ctx->cell_index;
	p_s1ap_kill_resp->transaction_id = p_cell_ctx->transaction_id;

	/* 2. Handling the ResponseCode*/
	switch(p_pws_resp->response)
	{
		case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS:

			/* Send PWS resp to eNB manager */
			p_s1ap_kill_resp->kill_response = RRC_SUCCESS;

			/* Number of warnings broadcasted */
			p_s1ap_kill_resp->bitmask |= KILL_RESP_NUMBER_OF_BROADCASTS_PRESENT;
			p_s1ap_kill_resp->number_of_broadcasts = p_cell_ctx->number_of_broadcasts;

			/* Change the state back to CELL_M_STATE_CELL_IN_SERVICE*/
			CELL_M_FSM_SET_STATE(p_cell_ctx,
					CELL_M_STATE_CELL_IN_SERVICE);
			rrc_send_message(p_intrl_msg,
					RRC_S1AP_MODULE_ID);
			break;

		case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_TIMER_EXP: 

			/* 2.1 Sending Kill Request Again for Retries */
			csc_send_llim_recfg_req_with_old_cfg_pws(
					p_cell_ctx,
					p_csc_gl_ctx,
					MAC_ONLY); 
			if(PNULL != p_cell_ctx->p_enb_kill_req )
			{
				rrc_mem_free(p_cell_ctx->p_enb_kill_req);
				p_cell_ctx->p_enb_kill_req = PNULL;
			}
			/* coverity_fix_48254_start */
			rrc_msg_mem_free(p_intrl_msg);
			/* coverity_fix_48254_stop */
			RRC_CSC_UT_TRACE_EXIT();
			return;

		case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_FAIL:
		default: 
			p_s1ap_kill_resp->kill_response = RRC_FAILURE;
			p_s1ap_kill_resp->bitmask |= KILL_RESP_FAIL_CAUSE_PRESENT;
			p_s1ap_kill_resp->kill_fail_cause = 
				RRM_RRC_LL_CONFIG_FAILURE;

			RRC_CSC_TRACE(RRC_BRIEF, "Cell delete is expected from RRM as KILL"
					"procedure has failed");
			/* Change the state back to CELL_M_STATE_CELL_IN_SERVICE*/
			CELL_M_FSM_SET_STATE(p_cell_ctx,
					CELL_M_STATE_CELL_IN_SERVICE);
			rrc_send_message(p_intrl_msg,
					RRC_S1AP_MODULE_ID);

			RRC_CSC_TRACE(RRC_BRIEF, "Sending S1AP KILL resp failure to S1AP");
	}
	if(PNULL != p_cell_ctx->p_enb_kill_req )
	{
		rrc_mem_free(p_cell_ctx->p_enb_kill_req);
		p_cell_ctx->p_enb_kill_req = PNULL;
	}

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_reconfig_resp_kill_req
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function Handles the PWS resp from LLIM
 *                  STATE: CELL_M_STATE_W_FOR_KILL_RESP
 *                  EVENT: CELL_M_EVENT_LLIM_PWS_RESP
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_reconfig_resp_kill_req(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	s1ap_kill_req_t *p_enb_kill_req  =
		(s1ap_kill_req_t *)p_msg;
	U32 cmas_key = RRC_NULL;
	cmas_info_t    *cmas_info_node = PNULL;
	rrc_bool_et kill_warning_found = RRC_FALSE;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_enb_kill_req);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	/*Update Transaction ID in cell context*/
	p_cell_ctx->transaction_id = p_enb_kill_req->transaction_id;

	/*Reset number_of_broadcasts in cell context*/
	p_cell_ctx->number_of_broadcasts = 0;

	/*  Send Reconfig with Old Configuration */
	csc_send_llim_recfg_req_with_old_cfg(p_cell_ctx,p_csc_gl_ctx, NO_MODE);

	if(PNULL != p_cell_ctx->p_temp_si_schdl_info )
	{
		rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
		p_cell_ctx->p_temp_si_schdl_info = PNULL;
	}

	/* Check whether the kill is recieved for ETWS or CMAS */
	/* Acc to spec 36.300 v9.7, Section 19.2.2.17, Kill cannot be 
	   received for ETWS, If it is received, it is a failure */ 
	if ( PNULL != p_cell_ctx->etws_info )
	{
		/* Comparing Msg Id and Ser No. with SIB 10 Msg Id and Ser No. */
		if ((((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask) &&
					( 0 == memcmp_wrapper(&(p_enb_kill_req->message_id), 
							      &(p_cell_ctx->etws_info->sib10_info.msg_id),
							      sizeof(U16))) &&
					( 0 == memcmp_wrapper(&(p_enb_kill_req->serial_number), 
							      &(p_cell_ctx->etws_info->sib10_info.serial_number),
							      sizeof(U16)))))
		{
			kill_warning_found = RRC_TRUE;
		}
		/* Comparing Msg Id and Ser No. with SIB 11 Msg Id and Ser No. */
		else if (((8 << RRM_RRC_SIB_TYPE_11) & p_cell_ctx->pws_sibs_bitmask) &&
				( 0 == memcmp_wrapper(&(p_enb_kill_req->message_id), 
						      &(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.msg_id),
						      sizeof(U16))) &&
				( 0 == memcmp_wrapper(&(p_enb_kill_req->serial_number), 
						      &(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.serial_number),
						      sizeof(U16))))
		{
			kill_warning_found = RRC_TRUE;
		}
		/* If found, sending the failure */
		if ( RRC_TRUE == kill_warning_found )
		{
			RRC_CSC_TRACE(RRC_ERROR,"Kill received for ETWS req");
			cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
					RRC_RRM_UNKNOWN_FAILURE,
					p_cell_ctx);
			/* Switch state */
			CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
	}
	/* Check whether broadcast of any CMAS warning is ongoing for which
	   KILL has been recieved. If yes, then stop broadcast of that CMAS */
	if ((8 << RRM_RRC_SIB_TYPE_12) & p_cell_ctx->pws_sibs_bitmask)
	{
		cmas_key = (U32) (p_enb_kill_req->message_id[1] << 24 );
		cmas_key |= (U32) (p_enb_kill_req->message_id[0] << 16 );
		cmas_key |= (U32) (p_enb_kill_req->serial_number[1] << 8 );
		cmas_key |= (U32) (p_enb_kill_req->serial_number[0]);
		cmas_info_node = (cmas_info_t *)(st_find_tree_node( 
					(const search_tree_t *)&(p_cell_ctx->cmas_info_list), 
					(const void*)&cmas_key));

		if (PNULL != cmas_info_node)
		{
			p_cell_ctx->number_of_broadcasts = ((U16)((cmas_info_node->cmas_time_elapsed)/
						(U32)((cmas_info_node->num_of_segments) * (cmas_info_node->sib12_si_periodicity))));
			p_cell_ctx->number_of_broadcasts = (U16)(p_cell_ctx->number_of_broadcasts *
					cmas_info_node->num_of_repetitions);
			st_delete_node (&(p_cell_ctx->cmas_info_list),
					&(cmas_info_node->anchor));
			rrc_mem_free(cmas_info_node);
			if ( 0 == st_get_node_count ((const search_tree_t *)
						&(p_cell_ctx->cmas_info_list)))
			{
				p_cell_ctx->pws_sibs_bitmask &= (rrc_bitmask_t)(~(8 << RRM_RRC_SIB_TYPE_12));
			}
			kill_warning_found = RRC_TRUE;
		}
	}
	/* If warning not found, then send failure with cause NO_BROADCAST_ONGOING */
	if ( RRC_FALSE == kill_warning_found )
	{
		RRC_CSC_TRACE(RRC_ERROR,"No broadcast ongoing for which Kill Req"
				"has been received");
		cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
				RRC_RRM_NO_BROADCAST_ONGOING,
				p_cell_ctx);
		/* Switch state */
		CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	p_cell_ctx->p_temp_si_schdl_info = rrc_mem_get(sizeof(si_schdl_info_t));

	if (PNULL == p_cell_ctx->p_temp_si_schdl_info)
	{
		RRC_CSC_TRACE(RRC_FATAL,"Unable to allocate Memory");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}
	memset_wrapper ( p_cell_ctx->p_temp_si_schdl_info, RRC_NULL, 
			sizeof(si_schdl_info_t) );


	if(SI_MAPPING_CHANGED == csc_check_si_mapping_pws(p_cell_ctx,
				&(p_enb_kill_req->scheduling_info_list)))
	{
		RRC_CSC_TRACE(RRC_ERROR,"SI mapping has changed");
		RRC_CSC_TRACE(RRC_BRIEF, "Cell Delete is expected from RRM as Kill"
				"procedure has failed");
		cell_m_fsm_build_and_send_s1ap_kill_resp_failure(
				RRC_RRM_UNKNOWN_FAILURE,
				p_cell_ctx);
		/* Switch state */
		CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag = SI_MAPPING_NOT_CHANGED;

	p_cell_ctx->cfg_cell_param.sib_type_1_Info.scheduling_info_list =
		p_enb_kill_req->scheduling_info_list;

	if (KILL_SI_TRANSMISSION_INFO_LIST & p_enb_kill_req->bitmask)
	{
		p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.
			si_transmission_info = p_enb_kill_req->si_transmission_info;
		p_cell_ctx->cfg_cell_param.cell_config_param.mac_config.bitmask |=
			RRC_RRM_SI_TRANSMISSION_INFO_PRESENT; 
	}


	/* Update PWS related SIBs in the Cell Context */
	update_pws_broadcast_config (p_cell_ctx);

	/*Store KILL req in cell context*/

	p_cell_ctx->p_enb_kill_req = rrc_mem_get(
			sizeof(s1ap_kill_req_t));

	if(PNULL == p_cell_ctx->p_enb_kill_req)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed;Unused variable  p_api=%p",__FUNCTION__,p_api);/*SPR 17777 +-*/
		return;
	}
	l3_memcpy_wrapper(p_cell_ctx->p_enb_kill_req,
			p_enb_kill_req,
			sizeof(s1ap_kill_req_t));

	RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_for_kill_resp_llim_reconfig_resp_rollback
 *   INPUT        : void            *p_api
 *                  void            *p_msg
 *                  csc_cell_ctx_t  *p_cell_ctx
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : none               
 *
 *   DESCRIPTION  : This function Handles the Reconfig resp from LLIM
 *                  STATE: CELL_M_STATE_W_FOR_KILL_RESP
 *                  EVENT: CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_for_kill_resp_llim_reconfig_resp_rollback(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	rrc_csc_llim_reconfig_cell_resp_t *p_reconfig_cell_resp =
		(rrc_csc_llim_reconfig_cell_resp_t *)p_msg;
	void                            *p_intrl_msg        = PNULL;
	s1ap_kill_resp_t *p_s1ap_kill_resp = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_reconfig_cell_resp);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
	RRC_ASSERT(p_reconfig_cell_resp->cell_index == p_cell_ctx->cell_index);

	/* Precondition - should be the same transaction */
	if (p_reconfig_cell_resp->transaction_id != p_cell_ctx->transaction_id)
	{
		RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
				"ignored");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	/* 1. Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_RECONFIGURE_TIMER]");
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	p_intrl_msg = csc_alloc_intrl_msg(RRC_S1AP_MODULE_ID,
			S1AP_KILL_RESP,
			sizeof(s1ap_kill_resp_t)); 
	if(PNULL == p_intrl_msg)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed;Unused variable p_api=%p",__FUNCTION__,p_api);/*SPR 17777 +-*/
		return;
	}

	if (PNULL == p_intrl_msg )
	{
		RRC_CSC_TRACE(RRC_WARNING, "Memory Allocation failed");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}
	p_s1ap_kill_resp = (s1ap_kill_resp_t*)
		((U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);
	if(PNULL == p_s1ap_kill_resp)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
		return;
	}

	if (PNULL == p_s1ap_kill_resp )
	{
		RRC_CSC_TRACE(RRC_WARNING, " Invalid Response ");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	memset_wrapper (p_s1ap_kill_resp, RRC_NULL, sizeof(s1ap_kill_resp_t)); 

	p_s1ap_kill_resp->bitmask = 0;
	p_s1ap_kill_resp->cell_index = p_cell_ctx->cell_index;
	p_s1ap_kill_resp->transaction_id = p_cell_ctx->transaction_id;

	/* 2. Handling the ResponseCode*/
	switch (p_reconfig_cell_resp->response)
	{
		case RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS :

			RRC_CSC_TRACE(RRC_INFO,"Kill has succeded.");  
			/* Send KILL resp to eNB manager */
			p_s1ap_kill_resp->kill_response = RRC_SUCCESS;

			/* Number of warnings broadcasted */
			p_s1ap_kill_resp->bitmask |= KILL_RESP_NUMBER_OF_BROADCASTS_PRESENT;
			p_s1ap_kill_resp->number_of_broadcasts = p_cell_ctx->number_of_broadcasts;


			/* Change the state back to CELL_M_STATE_CELL_IN_SERVICE*/
			CELL_M_FSM_SET_STATE(p_cell_ctx,
					CELL_M_STATE_CELL_IN_SERVICE);
			rrc_send_message(p_intrl_msg,
					RRC_S1AP_MODULE_ID);
			RRC_CSC_TRACE(RRC_BRIEF, "Sending S1AP KILL resp success to S1AP");

			p_cell_ctx->retry_count = RRC_NULL; 

			rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
			p_cell_ctx->p_temp_si_schdl_info = PNULL;

			if(PNULL != p_cell_ctx->p_enb_kill_req )
			{
				rrc_mem_free(p_cell_ctx->p_enb_kill_req);
				p_cell_ctx->p_enb_kill_req = PNULL;
			}
			break;


		default:
			/* If Max Retries hasn't attempted for the rollback */
			if(p_cell_ctx->retry_count < p_csc_gl_ctx->
					max_cell_reconfig_retry_count )
			{
				/* 2.1 Send Reconfig with Old Configuration */
				csc_send_llim_recfg_req_with_old_cfg_pws(
						p_cell_ctx,
						p_csc_gl_ctx,
						MAC_ONLY);
			}
			else
			{
				RRC_CSC_TRACE(RRC_ERROR,"Cell delete is expected from RRM as" 
						"KILL has failed");  
				csc_send_s1ap_kill_resp_failure(p_cell_ctx->cell_index,
						p_cell_ctx->transaction_id,
						RRC_FAILURE,
						RRC_RRM_UNKNOWN_FAILURE); 

				rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
				p_cell_ctx->p_temp_si_schdl_info = PNULL;

				/*Roll back has failed*/
				/* State Transition : Waiting for cell delete req*/
				CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
			}

			/* coverity_fix_48253_start */
			if ( PNULL != p_intrl_msg )
			{
			    rrc_msg_mem_free(p_intrl_msg);
			    p_intrl_msg = PNULL;
			}
			/* coverity_fix_48253_stop */
			/*Coverity 86871 Fix Start*/
			break;
			/*Coverity 86871 Fix End*/
	}

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: validate_and_fill_etws_warning
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *                  U32             *p_counter_value
 *   OUTPUT       : none
 *   Description:  This function checks and fills the ETWS warning in 
 *                 Cell Context
 *   RETURNS:     1) etws_info in cell_ctx_t
 *                2) Value of counter to be set
 *
 ******************************************************************************/
rrc_return_et validate_and_fill_etws_warning(
		csc_cell_ctx_t  *p_cell_ctx,      /* cell context */
		U32             *p_counter_value) /* Value of counter to be set */
{
	sib_type_10_Info_t     sib10_info;
	sib_type_11_Info_t     sib11_info;
	etws_info_t            *p_etws_info_node = PNULL; 

	RRC_CSC_UT_TRACE_ENTER();

	memset_wrapper(&sib10_info, 0, sizeof(sib_type_10_Info_t));
	memset_wrapper(&sib11_info, 0, sizeof(sib_type_11_Info_t));

	if (PNULL != p_cell_ctx->etws_info)
	{
		/* Check whether the broadcast of ETWS warning with same message id and 
		   serial no. is already ongoing or not
		   - If already ongoing, then ignore it
		   - Else replace the ETWS warning with the recieved one */

		if (( 0 == memcmp_wrapper(&(p_cell_ctx->p_enb_pws_req->message_identifier),
						&(p_cell_ctx->etws_info->sib10_info.msg_id), sizeof(U16))) && 
				(0 == memcmp_wrapper(&(p_cell_ctx->p_enb_pws_req->serial_number), 
						     &(p_cell_ctx->etws_info->sib10_info.serial_number), sizeof(U16))))
		{
			RRC_CSC_TRACE(RRC_ERROR,"Broadcast of ETWS with same msg_id and"
					"serial no. is already ongoing");  
			cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
					RRM_RRC_INTERNAL_FAILURE, p_cell_ctx);
			return RRC_FAILURE;
		}
		else if (( 0 == memcmp_wrapper(&(p_cell_ctx->p_enb_pws_req->message_identifier),
						&(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.msg_id), sizeof(U16))) && 
				(0 == memcmp_wrapper(&(p_cell_ctx->p_enb_pws_req->serial_number), 
						     &(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.serial_number), sizeof(U16))))
		{
			RRC_CSC_TRACE(RRC_ERROR,"Broadcast of ETWS with same msg_id and"
					"serial no. is already ongoing");
			cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
					RRM_RRC_INTERNAL_FAILURE, p_cell_ctx);
			return RRC_FAILURE;
		}
	}
	if (PWS_CONCURRENT_WARNING_MSG_INDICATOR & p_cell_ctx->p_enb_pws_req->bitmask)
	{
		RRC_CSC_TRACE(RRC_ERROR,"Concurrent bit must not be present for ETWS warning");  
		cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
				RRM_RRC_INTERNAL_FAILURE,
				p_cell_ctx);
		return RRC_FAILURE;
	}
	/* If RRM hasn't sent the scheduling info for SIB 11 or SIB 12 */
	if (!(((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask) ||
				((8 << RRM_RRC_SIB_TYPE_11) & p_cell_ctx->pws_sibs_bitmask)))
	{
		RRC_CSC_TRACE(RRC_ERROR, "RRM hasn't sent scheduling info for" 
				"SIB10 and SIB11");
		cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
				RRM_RRC_INTERNAL_FAILURE, p_cell_ctx);
		return RRC_FAILURE;
	}
	/* If extended repetition period IE has been recieved the S1AP_PWS_REQ 
	   then ignore the repetition period IE */
	if ( p_cell_ctx->p_enb_pws_req->bitmask & PWS_EXTENDED_REPETITION_PERIOD_PRESENT )
	{
		*p_counter_value = (U32)(p_cell_ctx->p_enb_pws_req->extended_repetition_period *
				p_cell_ctx->p_enb_pws_req->number_of_broadcasts_requested * 100); 
	}
	else
	{
		if (((p_cell_ctx->p_enb_pws_req->repetition_period > 0) &&
					(p_cell_ctx->p_enb_pws_req->number_of_broadcasts_requested == 0)) ||
				((p_cell_ctx->p_enb_pws_req->repetition_period == 0) &&
				 (p_cell_ctx->p_enb_pws_req->number_of_broadcasts_requested > 1)))
		{
			if ((RRC_NULL == (PWS_WARNING_TYPE_PRESENT & p_cell_ctx->
							p_enb_pws_req->bitmask)) || 
					((8 << RRM_RRC_SIB_TYPE_11) & p_cell_ctx->pws_sibs_bitmask))
			{
				RRC_CSC_TRACE(RRC_ERROR,"Broadcast of SIB11 with 0 repetition period \
						or with 0 no. of broadcast is not successfull");  
					cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
							RRM_RRC_INTERNAL_FAILURE, p_cell_ctx);
				return RRC_FAILURE;
			}
		}

		*p_counter_value = (U32)(p_cell_ctx->p_enb_pws_req->repetition_period *
				p_cell_ctx->p_enb_pws_req->number_of_broadcasts_requested * 100);
	} 
	if ((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask)
	{
		/* Fill sib_type10_info from the info received from pws_req*/
		if (RRC_FAILURE == csc_compose_sib_type10_info(
					&sib10_info, p_cell_ctx->p_enb_pws_req))
		{
			RRC_CSC_TRACE(RRC_ERROR,"Unable to compose SIB10");  
			cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
					RRM_RRC_INTERNAL_FAILURE, p_cell_ctx);
			return RRC_FAILURE;
		}
		RRC_CSC_TRACE(RRC_INFO,"SIB10 info present");
	}
	if ((8 << RRM_RRC_SIB_TYPE_11) & p_cell_ctx->pws_sibs_bitmask)
	{
		/* Fill sib_type11_info from the info received from pws_req*/
		if (RRC_FAILURE == csc_compose_sib_type11_info(
					&sib11_info, p_cell_ctx->p_enb_pws_req))
		{
			RRC_CSC_TRACE(RRC_ERROR,"Unable to compose SIB11");  
			cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
					RRM_RRC_INTERNAL_FAILURE, p_cell_ctx);
			return RRC_FAILURE;
		}
		RRC_CSC_TRACE(RRC_INFO,"SIB11 info present");
	}
	if ( PNULL != p_cell_ctx->etws_info )
	{
		rrc_mem_free (p_cell_ctx->etws_info);
		p_cell_ctx->etws_info = PNULL;
	}
	p_etws_info_node = rrc_mem_get(sizeof(etws_info_t));
	if(PNULL == p_etws_info_node)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
		return RRC_FAILURE;
	}

	/* Broadcast for atleast one Modification Boundary is required if the value
	   of counter comes out to be 0 in above conditions of Number of Broadcasts
	   and Repetition Period */
	if ( RRC_NULL == *p_counter_value )
	{
		/* ETWS warning start*/  
		*p_counter_value = p_cell_ctx->mod_period_boud_counter +
			p_cell_ctx->mod_period;
		/* ETWS warning stop*/  
		if (*p_counter_value < (p_cell_ctx->mod_period + CSC_PWS_SFN_OFFSET))
		{
			*p_counter_value += p_cell_ctx->mod_period;
		}
	}
	*p_counter_value = *p_counter_value - CSC_PWS_SFN_OFFSET;
	p_etws_info_node->sib10_info = sib10_info;
	p_etws_info_node->sib11_info.sib_type_11_Info = sib11_info;
	p_cell_ctx->etws_info = p_etws_info_node;
	RRC_CSC_UT_TRACE_ENTER();
	return RRC_SUCCESS;
}


/******************************************************************************
 *   FUNCTION NAME: validate_and_fill_cmas_warning
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *                  U32             *p_counter_value
 *   OUTPUT       : none
 *
 *   Description:  This function checks and fills the CMAS warning in 
 *                  Cell Context
 *   RETURNS:     1) cmas_info_t in cell_ctx_t
 *                2) Value of counter to be set
 *
 ******************************************************************************/
rrc_return_et validate_and_fill_cmas_warning(
		csc_cell_ctx_t  *p_cell_ctx,      /* cell context */
		U32             *p_counter_value) /* Value of counter to be set */
{
	sib_type_12_Info_t  sib12_info;
	U32                 cmas_key = RRC_NULL;
	cmas_info_t         *cmas_info_node = PNULL;

	RRC_CSC_UT_TRACE_ENTER();
	/* Coverity Fix 83181 Start */
	memset_wrapper (&sib12_info, RRC_NULL, sizeof(sib_type_12_Info_t));
	/* Coverity Fix 83181 Stop */
	cmas_key = (U32) ( p_cell_ctx->p_enb_pws_req->message_identifier[1] << 24 );
	cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->message_identifier[0] << 16 );
	cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->serial_number[1] << 8 );
	cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->serial_number[0]);
	cmas_info_node = (cmas_info_t *)(st_find_tree_node( (const search_tree_t *)
				&(p_cell_ctx->cmas_info_list),
				(const void*)&cmas_key));

	/* Check whether the broadcast of CMAS warning with same message id and 
	   serial no. is already ongoing or not
	   1) If already ongoing, then ignore it
	   2) Else check the concurrent message indicator
	   2.1) If concurrent message indicator is not set then replace the 
	   CMAS warning with the recieved one.
	   2.2) If concurrent message indicator is set then schedule the CMAS 
	   warning with the CMAS warnings that are already broadcasted. */

	/* Case 2 is ongoing*/ 
	if (PNULL == cmas_info_node)
	{
		/* Case 2.1 is ongoing */
		if (0 == (PWS_CONCURRENT_WARNING_MSG_INDICATOR & p_cell_ctx->p_enb_pws_req->bitmask))
		{
			RRC_CSC_TRACE(RRC_ERROR,"Concurrent bit must be present for CMAS warning");  
			cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
					RRM_RRC_INTERNAL_FAILURE,
					p_cell_ctx);
			return RRC_FAILURE;
		}
		/* Case 2.2 with 2.1 is ongoing */
		cmas_info_node = (cmas_info_t *)rrc_mem_get(sizeof(cmas_info_t));

		if (PNULL == cmas_info_node)
		{
			RRC_CSC_TRACE(RRC_FATAL,"Unable to allocate Memory");
			RRC_CSC_UT_TRACE_EXIT();
			return RRC_FAILURE;
		}
		memset_wrapper (cmas_info_node, RRC_NULL, sizeof(cmas_info_t));

		cmas_info_node->cmas_key = cmas_key;
		cmas_info_node->indefinite_broadcast = RRC_FALSE;
		/* If extended repetition period IE has been recieved the S1AP_PWS_REQ 
		   then ignore the repetition period IE */
		if ( p_cell_ctx->p_enb_pws_req->bitmask & PWS_EXTENDED_REPETITION_PERIOD_PRESENT )
		{
			*p_counter_value = (U32)(p_cell_ctx->p_enb_pws_req->extended_repetition_period *
					p_cell_ctx->p_enb_pws_req->number_of_broadcasts_requested * 100); 
		}
		else
		{
			if ((p_cell_ctx->p_enb_pws_req->repetition_period == 0) &&
					((p_cell_ctx->p_enb_pws_req->number_of_broadcasts_requested == 0) ||
					 (p_cell_ctx->p_enb_pws_req->number_of_broadcasts_requested > 1)))
			{
				RRC_CSC_TRACE(RRC_ERROR,"Broadcast of CMAS warning is not successfull \
						for 0 repetition period");
				cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
						RRM_RRC_INTERNAL_FAILURE,
						p_cell_ctx);
				/* coverity_fix_63292_start */
				rrc_mem_free((void *)cmas_info_node);
				/* coverity_fix_63292_stop */
				return RRC_FAILURE;
			}
			/* If repetition period > 0  and Number of Broadcast is 0 then that 
			   CMAS is broadcasted indefinitely */
			if ((p_cell_ctx->p_enb_pws_req->repetition_period > 0) &&
					(p_cell_ctx->p_enb_pws_req->number_of_broadcasts_requested == 0))
			{
				cmas_info_node->indefinite_broadcast = RRC_TRUE;
			}
			if ((p_cell_ctx->p_enb_pws_req->repetition_period == 0) &&
					(p_cell_ctx->p_enb_pws_req->number_of_broadcasts_requested == 1))
			{
				/* Broadcast for atleast one Modification Boundary is required if the value
				   of counter comes out to be 0 in above conditions of Number of Broadcasts
				   and Repetition Period */
				/* ETWS warning start*/  
				*p_counter_value = p_cell_ctx->mod_period_boud_counter +
					p_cell_ctx->mod_period;
				/* ETWS warning stop*/  
				/* If the PWS req is received less than PWS_OFFSET from the mod boundary */
				if (*p_counter_value < (p_cell_ctx->mod_period + CSC_PWS_SFN_OFFSET))
				{
					*p_counter_value += p_cell_ctx->mod_period;
				}
			}
			else
			{ 
				*p_counter_value = (U32)(p_cell_ctx->p_enb_pws_req->repetition_period *
						p_cell_ctx->p_enb_pws_req->number_of_broadcasts_requested * 100); 
			}
		} 
		if ((8 << RRM_RRC_SIB_TYPE_12) & p_cell_ctx->pws_sibs_bitmask)
		{
			/* Fill sib_type12_info from the info received from pws_req*/
			if (RRC_FAILURE == csc_compose_sib_type12_info(&sib12_info, 
						p_cell_ctx->p_enb_pws_req))
			{
				RRC_CSC_TRACE(RRC_ERROR,"Unable to compose SIB12");  
				cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
						RRM_RRC_INTERNAL_FAILURE,
						p_cell_ctx);
				/* coverity_fix_63292_start */
				rrc_mem_free((void *)cmas_info_node);
				/* coverity_fix_63292_stop */
				return RRC_FAILURE;
			}
			RRC_CSC_TRACE(RRC_INFO,"SIB12 info present");
			cmas_info_node->sib12_info = sib12_info;
		}
		else
		{
			RRC_CSC_TRACE(RRC_ERROR,"RRM hasn't sent scheduling info of SIB12");  
			cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
					RRM_RRC_INTERNAL_FAILURE,
					p_cell_ctx);
			/* coverity_fix_63292_start */
			rrc_mem_free((void *)cmas_info_node);
			/* coverity_fix_63292_stop */
			return RRC_FAILURE;
		}
		*p_counter_value = *p_counter_value - CSC_PWS_SFN_OFFSET;
		st_insert_node (&(p_cell_ctx->cmas_info_list), &(cmas_info_node->anchor));

	}
	else /* Case 1 is ongoing */
	{
		RRC_CSC_TRACE(RRC_ERROR,
				"Broadcast of warning with same message id & serial no. is already ongoing");
		cell_m_fsm_build_and_send_s1ap_pws_resp_failure_without_reset(
				RRM_RRC_INTERNAL_FAILURE,
				p_cell_ctx);
		return RRC_FAILURE;
	}
	RRC_CSC_UT_TRACE_EXIT();
	return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: init_pws_info
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *   OUTPUT       : none
 *
 *   Description:  This function initializes the Cell Context for PWS Info
 *   Input:        p_cell_ctx
 *
 *   RETURNS:    void
 *
 ******************************************************************************/
void init_pws_info(
		csc_cell_ctx_t  *p_cell_ctx)      /* cell context */
{
	RRC_CSC_UT_TRACE_ENTER();
	if(PNULL != p_cell_ctx->p_temp_si_schdl_info )
	{
		rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
		p_cell_ctx->p_temp_si_schdl_info = PNULL;
	}
	if(PNULL != p_cell_ctx->p_enb_pws_req )
	{
		rrc_mem_free(p_cell_ctx->p_enb_pws_req);
		p_cell_ctx->p_enb_pws_req = PNULL;
	}
	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: fill_new_etws_and_cmas_scheduling_info 
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *                  U32             *p_counter_value
 *   OUTPUT       : none
 *
 *   Description:  This function checks and fills the CMAS warning in 
 *                  Cell Context
 *                
 *   RETURNS:     1) cmas_info_t in cell_ctx_t
 *                2) etws_info_t in cell_ctx_t
 *
 ******************************************************************************/
rrc_return_et fill_new_etws_and_cmas_scheduling_info (
		csc_cell_ctx_t               *p_cell_ctx)      /* cell context */
{
	U8  cmas_count = RRC_NULL;
	U32 cmas_key = RRC_NULL;
	cmas_info_t *temp_cmas_info_node = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	if (( KILL_ETWS_SCHEDULING_INFO & p_cell_ctx->p_enb_kill_req->bitmask) &&
			( PNULL != p_cell_ctx->etws_info))
	{
		if (((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask) &&
				( 0 == memcmp_wrapper(&(p_cell_ctx->p_enb_kill_req->etws_scheduling_info.
							message_identifier), &(p_cell_ctx->etws_info->sib10_info.msg_id),
						      sizeof(U16))) &&
				( 0 == memcmp_wrapper(&(p_cell_ctx->p_enb_kill_req->etws_scheduling_info.
							serial_number), &(p_cell_ctx->etws_info->sib10_info.serial_number),
						      sizeof(U16))))
		{
                    /* Do nothing */
		}
		else if (((8 << RRM_RRC_SIB_TYPE_11) & p_cell_ctx->pws_sibs_bitmask) &&
				( 0 == memcmp_wrapper(&(p_cell_ctx->p_enb_kill_req->etws_scheduling_info.
							message_identifier), &(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.
								msg_id),sizeof(U16))) &&
				( 0 == memcmp_wrapper(&(p_cell_ctx->p_enb_kill_req->etws_scheduling_info.
							serial_number), &(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.
								serial_number),sizeof(U16))))
		{
			if ( RRC_RRM_ETWS_SCHEDULING_NUM_OF_SEGMENTS_PRESENT &
					p_cell_ctx->p_enb_kill_req->etws_scheduling_info.bitmask )
			{
				p_cell_ctx->etws_info->sib11_info.num_of_segments =
					p_cell_ctx->p_enb_kill_req->etws_scheduling_info.num_of_segments;
			}
			else
			{
				p_cell_ctx->etws_info->sib11_info.num_of_segments = 1;
			}
		}
		else
		{
			RRC_CSC_TRACE(RRC_ERROR,"Wrong configuration sent by RRM");
			RRC_CSC_UT_TRACE_EXIT();
			return RRC_FAILURE;
		}
	}
	if ( KILL_CMAS_SCHEDULING_INFO_LIST & p_cell_ctx->p_enb_kill_req->bitmask)
	{
		if (((8 << RRM_RRC_SIB_TYPE_12) & p_cell_ctx->pws_sibs_bitmask) &&
				( p_cell_ctx->p_enb_kill_req->cmas_scheduling_info_list. \
				  cmas_warning_count == st_get_node_count ((const search_tree_t *)
					  &(p_cell_ctx->cmas_info_list))))
		{
			for (cmas_count = 0; cmas_count < p_cell_ctx->p_enb_kill_req-> 
					cmas_scheduling_info_list.cmas_warning_count; ++cmas_count)
			{
				cmas_key = (U32) (p_cell_ctx->p_enb_kill_req->cmas_scheduling_info_list. \
						cmas_scheduling_info[cmas_count].message_identifier[1] << 24 );
				cmas_key |= (U32) (p_cell_ctx->p_enb_kill_req->cmas_scheduling_info_list. \
						cmas_scheduling_info[cmas_count].message_identifier[0] << 16 );
				cmas_key |= (U32) (p_cell_ctx->p_enb_kill_req->cmas_scheduling_info_list. \
						cmas_scheduling_info[cmas_count].serial_number[1] << 8 );
				cmas_key |= (U32) (p_cell_ctx->p_enb_kill_req->cmas_scheduling_info_list. \
						cmas_scheduling_info[cmas_count].serial_number[0]);
				temp_cmas_info_node = (cmas_info_t *)(st_find_tree_node( (const search_tree_t *)
							&(p_cell_ctx->cmas_info_list), (const void*)&cmas_key));
				if (PNULL == temp_cmas_info_node)
				{
					RRC_CSC_TRACE(RRC_ERROR,"Wrong configuration sent by RRM");
					RRC_CSC_UT_TRACE_EXIT();
					return RRC_FAILURE;
				}
				temp_cmas_info_node->num_of_segments = p_cell_ctx->p_enb_kill_req->
					cmas_scheduling_info_list.cmas_scheduling_info[cmas_count].num_of_segments;                      
				temp_cmas_info_node->num_of_repetitions = p_cell_ctx->p_enb_kill_req->
					cmas_scheduling_info_list.cmas_scheduling_info[cmas_count].num_of_repetitions;                      
			}
		}
		else
		{
			RRC_CSC_TRACE(RRC_ERROR,"Wrong configuration sent by RRM");
			RRC_CSC_UT_TRACE_EXIT();
			return RRC_FAILURE;
		}
	}
	RRC_CSC_UT_TRACE_EXIT();
	return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: update_pws_broadcast_config 
 *
 *   Description:  This function fills the PWS related SIBs in Cell Context
 *
 *   Input:       1) p_cell_ctx
 *   Outputs    : None             
 *   RETURNS:     1) pws_broadcast_config_t in cell_ctx_t
 *
 ******************************************************************************/
void update_pws_broadcast_config (
		csc_cell_ctx_t               *p_cell_ctx)      /* cell context */
{
	U8  cmas_count = RRC_NULL;
	U8  cmas_warning_count = RRC_NULL;
	cmas_info_t *cmas_node = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);

	/* Allocating memory, if not already allocated */ 
	if ( PNULL == p_cell_ctx->pws_broadcast_config )
	{
		p_cell_ctx->pws_broadcast_config = rrc_mem_get (
				sizeof(pws_broadcast_config_t));

		if (PNULL == p_cell_ctx->pws_broadcast_config)
		{
			RRC_CSC_TRACE(RRC_FATAL,"Unable to allocate Memory");
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
	}
	if(PNULL == p_cell_ctx->pws_broadcast_config)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
		return;
	}
	memset_wrapper (p_cell_ctx->pws_broadcast_config, RRC_NULL,
			sizeof(pws_broadcast_config_t));

	/* Fill the bitmask */
	p_cell_ctx->pws_broadcast_config->bitmask = 
		p_cell_ctx->pws_sibs_bitmask;

	/* Fill SIB10 and SIB11 in the structure acc to their presence */    
	if ( PNULL != p_cell_ctx->etws_info)
	{
		if ((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask)
		{
			p_cell_ctx->pws_broadcast_config->sib_type_10_Info =
				p_cell_ctx->etws_info->sib10_info;
		}
		if ((8 << RRM_RRC_SIB_TYPE_11) & p_cell_ctx->pws_sibs_bitmask)
		{
			p_cell_ctx->pws_broadcast_config->sib11_info =
				p_cell_ctx->etws_info->sib11_info;
		}
	}

	/* Fill SIB12 in the structure acc to their presence */    
	if ((8 << RRM_RRC_SIB_TYPE_12) & p_cell_ctx->pws_sibs_bitmask)
	{
		/* Filling the count of CMAS warning */
		cmas_warning_count = (U8)(st_get_node_count(
					(const search_tree_t *)&(p_cell_ctx->cmas_info_list)));
		p_cell_ctx->pws_broadcast_config->sib_type_12_list.
			count = cmas_warning_count;

        /* SPR_20915 - Fix - Starts */
        for ( cmas_node = (cmas_info_t *)(st_get_first_node
                ((const search_tree_t *)&(p_cell_ctx->cmas_info_list)));
              (cmas_node && (MAX_NUM_CMAS_WARNING > cmas_count));
              cmas_node = (cmas_info_t *)(st_get_next_node
                ((const search_tree_node_t *)cmas_node)))
        /* SPR_20915 - Fix - Ends */
		{
			if ( cmas_count < cmas_warning_count )
			{
				/* Filling the No. of Segments of SIB12 */ 
				p_cell_ctx->pws_broadcast_config->sib_type_12_list.
					sib_type_12[cmas_count].num_of_segments = cmas_node->num_of_segments;

				/* Filling the contents of SIB12 */ 
				p_cell_ctx->pws_broadcast_config->sib_type_12_list.
					sib_type_12[cmas_count].sib_type_12_Info = cmas_node->sib12_info;
			}
			cmas_count++;
		}
	}
	RRC_CSC_UT_TRACE_EXIT();
}
/*********************************************************************************
 *   FUNCTION NAME : cell_m_fsm_rrm_cell_start_req
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *   DESCRIPTIN : This function handles the cell start request sent by the RRM
 *   to CSC
 *        STATE : CELL_M_STATE_CELL_OUT_OF_SERVICE
 *        EVENT : CELL_M_EVENT_RRM_CELL_START_REQ
 *
 *   RETURNS : RRC_SUCCESS/RRC_FAILURE
 ***********************************************************************************/
static void cell_m_fsm_rrm_cell_start_req(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx  /* Pointer to the CSC global context data
						*/
		)
{
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	rrc_rrm_cell_start_req_t *p_cell_start_req =
		(rrc_rrm_cell_start_req_t *)p_msg;
#endif
	csc_cell_m_timer_buf_t  guard_timer_data;

	void *p_intrl_msg    = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	RRC_ASSERT(PNULL != p_cell_start_req);
	RRC_ASSERT(p_cell_start_req->cell_index == p_cell_ctx->cell_index);
#endif
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);


	p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);

	p_intrl_msg = csc_alloc_intrl_msg(
			RRC_LLIM_MODULE_ID,
			CSC_LLIM_CELL_START_REQ,
			sizeof (rrc_csc_llim_cell_start_req_t));

#ifdef RRC_UT_FRAMEWORK
    pLlimCellStart =  rrc_mem_get(sizeof(rrc_csc_llim_cell_start_req_t) +
                                     RRC_API_HEADER_SIZE);

#endif

	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		rrc_csc_llim_cell_start_req_t *p_csc_llim_cell_start_req =
			(rrc_csc_llim_cell_start_req_t *)((U8 *)p_intrl_msg +
					RRC_API_HEADER_SIZE);

		p_csc_llim_cell_start_req->transaction_id  = p_cell_ctx->transaction_id;
		p_csc_llim_cell_start_req->cell_index = p_cell_ctx->cell_index;

        /* SPR 23608/SES-43 Fix Start */
        if (PNULL != p_cell_ctx->guard_timer)
        {
            RRC_CSC_TRACE(RRC_BRIEF,"Stopping Cell Context Guard Timer"); 
            /* Stop Timer */
            rrc_stop_timer(p_cell_ctx->guard_timer);
            p_cell_ctx->guard_timer             = PNULL;
        }
        /* SPR 23608/SES-43 Fix End */
		guard_timer_data.auto_delete        = RRC_TRUE;
		guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
		guard_timer_data.cell_index         = p_cell_ctx->cell_index;
		guard_timer_data.cell_m_event       =
			CELL_M_EVENT_CELL_START_TIMER_EXPIRED;
		RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_START_TIMER] %u ms.",
				p_csc_gl_ctx->cell_start_duration);

		p_cell_ctx->guard_timer = rrc_start_timer(
				p_csc_gl_ctx->cell_start_duration,
				&guard_timer_data,
				sizeof (guard_timer_data),
				RRC_FALSE);
#ifdef RRC_UT_FRAMEWORK
        l3_memcpy_wrapper(pLlimCellStart,p_intrl_msg,
                     (sizeof(rrc_csc_llim_cell_start_req_t) +
                                     RRC_API_HEADER_SIZE));
#endif

		rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);

		/*    p_cell_ctx->rrc_rrm_start_cell_flag = RRC_TRUE;*/

		CELL_M_FSM_SET_STATE(p_cell_ctx,
				CELL_M_STATE_W_FOR_LLIM_CELL_START_RESP);
	}
	else
	{
		/* Send failure response. */
		RRC_CSC_TRACE(RRC_WARNING, "Unable to send CSC_LLIM_CELL_START_REQ message"
				" as cell is already in CELL_IN_SERVICE state");

		if (RRC_SUCCESS != csc_send_rrm_cell_start_resp(p_cell_ctx->transaction_id,
					p_cell_ctx->cell_index,
					RRC_FAILURE,
					RRM_RRC_INTERNAL_FAILURE))
		{
			RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM; Unused variable p_msg=%p",p_msg);/*SPR 17777 +-*/
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
	}

	RRC_CSC_UT_TRACE_EXIT();
}
/*********************************************************************************
 *   FUNCTION NAME : cell_m_fsm_rrm_cell_start_req_failure
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *
 *   DESCRIPTIN : This function handles the cell start request sent by the RRM
 *   to CSC
 *                and sends the failre as a response to the RRM.
 *        STATE : CELL_M_STATE_CELL_IN_SERVICE
 *        EVENT : CELL_M_EVENT_RRM_CELL_START_REQ
 *
 *   RETURNS : RRC_SUCCESS/RRC_FAILURE
 ***********************************************************************************/
static void cell_m_fsm_rrm_cell_start_req_failure(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx  /* Pointer to the CSC global context data */
		)
{
#if RRC_DEBUG
	/* GDB Fix - compiler warnings */
	rrc_rrm_cell_start_req_t *p_cell_start_req =
		(rrc_rrm_cell_start_req_t *)p_msg;
#endif

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
#if RRC_DEBUG
	RRC_ASSERT(PNULL != p_cell_start_req);
	RRC_ASSERT(p_cell_start_req->cell_index == p_cell_ctx->cell_index);
#endif
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);


	if (RRC_SUCCESS != csc_send_rrm_cell_start_resp(
				rrc_get_transaction_id(p_api),
				p_cell_ctx->cell_index,
				RRC_FAILURE,
				RRM_RRC_INTERACTION_WITH_OTHER_PROCEDURE_FAILURE))
	{
		/* Send failure response. */
		RRC_CSC_TRACE(RRC_WARNING, "Unable to process" 
				"csc_send_rrm_cell_start_resp"
				"message; Unused variable p_msg=%p p_csc_gl_ctx =%p"
                ,p_msg,p_csc_gl_ctx);/*SPR 17777 +-*/
		return;
	}

	RRC_CSC_UT_TRACE_EXIT();
}
/*********************************************************************************
 *   FUNCTION NAME : cell_m_fsm_rrm_cell_stop_req_failure
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *
 *   DESCRIPTIN : This function handles the cell stop request sent by the RRM to
 *   CSC
 *                and sends the failre as a response to the RRM.
 *        STATE : CELL_M_STATE_W_FOR_PWS_RESP &
 *                CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP 
 *        EVENT : CELL_M_EVENT_RRM_CELL_STOP_REQ 
 *
 *   RETURNS : RRC_SUCCESS/RRC_FAILURE
 ***********************************************************************************/
static void cell_m_fsm_rrm_cell_stop_req_failure(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx  /* Pointer to the CSC global context data
						*/
		)
{
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	rrc_rrm_cell_stop_req_t *p_cell_stop = 
		(rrc_rrm_cell_stop_req_t *)p_msg;
#endif
	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	RRC_ASSERT(PNULL != p_cell_stop);
#endif

	if (RRC_SUCCESS != csc_send_rrm_cell_stop_resp(
				rrc_get_transaction_id(p_api),
				p_cell_ctx->cell_index,
				RRC_FAILURE,
				RRM_RRC_INTERACTION_WITH_OTHER_PROCEDURE_FAILURE))
	{
		/* Send failure response. */
        /*SPR 17777 +-*/
		RRC_CSC_TRACE(RRC_WARNING, "Unable to process"
                "csc_send_rrm_cell_stop_resp message; Unused variable m_msg=%p p_csc_gl_ctx=%p"
        /*SPR 17777 +-*/
                ,p_msg,p_csc_gl_ctx);/*SPR 17777 +-*/
		return;
	}

	RRC_CSC_UT_TRACE_EXIT();
}

/*********************************************************************************
 *   FUNCTION NAME : cell_m_fsm_rrm_cell_stop_req_send_to_llim
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *
 *   DESCRIPTIN : This function handles the cell stop request sent by the RRM to
 *                CSC and sends it to LLIM.
 *        STATE : CELL_M_STATE_CELL_OUT_OF_SERVICE
 *        EVENT : CELL_M_EVENT_RRM_CELL_STOP_REQ
 *
 *   RETURNS : RRC_SUCCESS/RRC_FAILURE
 ***********************************************************************************/

static void cell_m_fsm_rrm_cell_stop_req_send_to_llim(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx  /* Pointer to the CSC global context data
						*/
		)
{
	csc_cell_m_timer_buf_t  guard_timer_data;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);

	/* set guard timer */
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF,"Stopping Cell Context Guard Timer"); 
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer             = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
	guard_timer_data.auto_delete        = RRC_TRUE;
	guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
	guard_timer_data.cell_index         = p_cell_ctx->cell_index;
	guard_timer_data.cell_m_event       =
		CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED;
        /* NBIOT TIMER FIX */
        /* NBIOT TIMER FIX */
	RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_STOP_TIMER] %u ms.",
			p_csc_gl_ctx->cell_stop_duration);

	p_cell_ctx->guard_timer = rrc_start_timer(
			p_csc_gl_ctx->cell_stop_duration,
			&guard_timer_data,
			sizeof (guard_timer_data),
			RRC_FALSE);
        /* NBIOT TIMER FIX */
/* NBIOT TIMER FIX */
	/* send cell stop request to LLIM */
	cell_m_fsm_build_and_send_cell_stop_req_to_llim(p_api,
			p_msg,
			p_cell_ctx,
			p_csc_gl_ctx);

	RRC_CSC_UT_TRACE_EXIT();
}

/*****************************************************************************************
 *   FUNCTION NAME : cell_m_fsm_rrm_cell_stop_req
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *
 *   DESCRIPTION :This function handles the cell stop request send by the RRM to
 *   CSC
 *       STATE   : CELL_M_STATE_CELL_IN_SERVICE
 *       EVENT   : CELL_M_EVENT_RRM_CELL_STOP_REQ
 *
 *   RETURNS : RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************************/
static void cell_m_fsm_rrm_cell_stop_req(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx  /* Pointer to the CSC global context data */
		)
{

#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	rrc_rrm_cell_stop_req_t *p_cell_stop_req =
		(rrc_rrm_cell_stop_req_t *)p_msg;
#endif
	csc_cell_m_timer_buf_t  guard_timer_data;
	s1ap_cell_dreg_req_t    *p_s1ap_cell_dreg_req = PNULL;
	void                    *p_intrl_msg          = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	RRC_ASSERT(PNULL != p_cell_stop_req);
	RRC_ASSERT(p_cell_stop_req->cell_index == p_cell_ctx->cell_index);
#endif
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);


	p_cell_ctx->transaction_id  = rrc_get_transaction_id(p_api);

	/* set guard timer */
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF,"Stopping Cell Context Guard Timer"); 
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer             = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
	guard_timer_data.auto_delete        = RRC_TRUE;
	guard_timer_data.target_submodule   = CSC_SUBMODULE_CELL_M;
	guard_timer_data.cell_index         = p_cell_ctx->cell_index;
	guard_timer_data.cell_m_event       = CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED;
/* NBIOT TIMER FIX */
/* NBIOT TIMER FIX */
	RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_CELL_STOP_TIMER] %u ms.",
			p_csc_gl_ctx->cell_stop_duration);

	p_cell_ctx->guard_timer = rrc_start_timer(
			p_csc_gl_ctx->cell_stop_duration,
			&guard_timer_data,
			sizeof (guard_timer_data),
			RRC_FALSE);
        /* NBIOT TIMER FIX */
/* NBIOT TIMER FIX */
	p_cell_ctx->sib8_new_segments_generate_count = 0;
	p_cell_ctx->sib8_periodicity = 0;

	/* Send S1AP_CELL_DEREG_REQ */
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_S1AP_MODULE_ID,
			S1AP_CELL_DEREG_REQ,
			sizeof (s1ap_cell_dreg_req_t));

	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		p_s1ap_cell_dreg_req = (s1ap_cell_dreg_req_t *)(
				(U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

		p_s1ap_cell_dreg_req->cell_index = p_cell_ctx->cell_index;
		rrc_send_message(p_intrl_msg, RRC_S1AP_MODULE_ID);
	}
	else
	{
		/* Bad incoming info */
		RRC_CSC_TRACE(RRC_WARNING, "unable to send S1AP_CELL_DEREG_REQ message");
	}

    /*Send X2AP_CSC_CELL_DEL_IND*/
    p_intrl_msg = csc_alloc_intrl_msg( RRC_X2AP_MODULE_ID,
                X2AP_CSC_CELL_DEL_IND,
                sizeof (x2ap_csc_cell_del_t));

    if (PNULL != p_intrl_msg)
    {
        /* Fill and send message */
        x2ap_csc_cell_del_t *p_x2ap_csc_cell_del_ind = (x2ap_csc_cell_del_t *)(
                (U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

        p_x2ap_csc_cell_del_ind->cell_index = (U8)p_cell_ctx->cell_index;

        rrc_send_message(p_intrl_msg, RRC_X2AP_MODULE_ID);
    }
	else
	{
		RRC_CSC_TRACE(RRC_WARNING, "unable to send X2AP_CELL_STOP_REQ message");
	}

	/*  send CSC_UECC_DEL_ALL_UE_REQ */
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_UECC_MODULE_ID,
			CSC_UECC_DELETE_ALL_UE_REQ,
			sizeof (rrc_csc_uecc_delete_all_ue_req_t));

	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		rrc_csc_uecc_delete_all_ue_req_t *p_uecc_del_cell_req = 
			(rrc_csc_uecc_delete_all_ue_req_t *)((U8 *)p_intrl_msg +
					RRC_API_HEADER_SIZE);
		p_uecc_del_cell_req->cell_index = p_cell_ctx->cell_index;
		p_uecc_del_cell_req->is_cell_stopped = RRC_TRUE;

		rrc_send_message(p_intrl_msg, RRC_UECCMD_MODULE_ID);

		/* CELL_STOP GOING ON , HENCE SET THE FLAG */
		p_cell_ctx->rrc_rrm_stop_cell_flag = RRC_TRUE;

		/* S1AP DEREG AND UECC DEL ALL UE SENT , HENCE SET THE FLAG */
		p_cell_ctx->rrc_rrm_s1ap_dereg_flag = RRC_TRUE;

		CELL_M_FSM_SET_STATE(p_cell_ctx,
				CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP);
	}
	else
	{
	    /* Bad incoming info */
	    RRC_CSC_TRACE(RRC_WARNING, "unable to send CSC_UECC_DEL_ALL_UE_REQ message;Unused variable p_msg=%p"
                ,p_msg);/*SPR 17777 +-*/
	    /* coverity fix 81379 start */
	    //p_intrl_msg = PNULL;
	    /* coverity fix 81379 end */
	}

	RRC_CSC_UT_TRACE_EXIT();
}


/*****************************************************************************************
 *   FUNCTION NAME  : cell_m_fsm_build_and_send_cell_stop_req_to_llim
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *
 *   DESCRIPTION    :This function sends the cell stop request to LLIM
 *
 *   RETURNS        : None.
 *
 *******************************************************************************************/
static void cell_m_fsm_build_and_send_cell_stop_req_to_llim(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx  /* Pointer to the CSC global context data */
		)
{

	void *p_intrl_msg    = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	/* If the rrc_rrm_stop_cell_flag is true , incoming p_msg is of type
	 * rrc_csc_uecc_delete_all_ue_resp_t during state
	 * CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP else incoming p_msg is of type
	 * rrc_rrm_cell_stop_req_t while handling CELL_M_EVENT_RRM_CELL_STOP_REQ
	 * during state CELL_M_STATE_CELL_OUT_OF_SERVICE. 
	 * Depending upon the same, RRC_ASSERT is implemented.
	 * In both cases, rrc_csc_llim_cell_stop_req is sent to LLIM.*/

	if(p_cell_ctx->rrc_rrm_stop_cell_flag == RRC_TRUE)
	{
#ifdef RRC_DEBUG
		/* GDB Fix - compiler warning */
		rrc_csc_uecc_delete_all_ue_resp_t *p_delete_cell_resp =
			(rrc_csc_uecc_delete_all_ue_resp_t *)p_msg;
		RRC_ASSERT(PNULL != p_delete_cell_resp);
		RRC_ASSERT(p_delete_cell_resp->cell_index == p_cell_ctx->cell_index);
#endif
	}
	else
	{
#ifdef RRC_DEBUG
		/* GDB Fix - compiler warning */
		rrc_rrm_cell_stop_req_t *p_cell_stop_req = (rrc_rrm_cell_stop_req_t *)p_msg;
		RRC_ASSERT(PNULL != p_cell_stop_req);
		RRC_ASSERT(p_cell_stop_req->cell_index == p_cell_ctx->cell_index);
#endif
	}

	p_intrl_msg = csc_alloc_intrl_msg(
			RRC_LLIM_MODULE_ID,
			CSC_LLIM_CELL_STOP_REQ,
			sizeof (rrc_csc_llim_cell_stop_req_t));

#ifdef RRC_UT_FRAMEWORK
    pLlimCellStop =  rrc_mem_get(sizeof(rrc_csc_llim_cell_stop_req_t) +
                                     RRC_API_HEADER_SIZE);

#endif

	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		rrc_csc_llim_cell_stop_req_t *p_llim_cell_stop_req =
			(rrc_csc_llim_cell_stop_req_t *)((U8 *)p_intrl_msg +
					RRC_API_HEADER_SIZE);
		p_llim_cell_stop_req->transaction_id = p_cell_ctx->transaction_id;
		p_llim_cell_stop_req->cell_index = p_cell_ctx->cell_index;
#ifdef RRC_UT_FRAMEWORK
        l3_memcpy_wrapper(pLlimCellStop,p_intrl_msg,
                     (sizeof(rrc_csc_llim_cell_stop_req_t) +
                                     RRC_API_HEADER_SIZE));
#endif

		rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);

		CELL_M_FSM_SET_STATE(p_cell_ctx,
				CELL_M_STATE_W_FOR_LLIM_CELL_STOP_RESP);
	}
	else
	{
		/* Send the failure response to RRM */
		RRC_CSC_TRACE(RRC_WARNING, "Unable to send CSC_LLIM_CELL_STOP_REQ message ");
		if (RRC_SUCCESS != csc_send_rrm_cell_stop_resp(p_cell_ctx->transaction_id,
					p_cell_ctx->cell_index,
					RRC_FAILURE,
					RRM_RRC_INTERNAL_FAILURE))
		{
			RRC_CSC_TRACE(RRC_WARNING, "Unable to send Failure RESP TO RRM");
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
		else
		{
			RRC_CSC_TRACE(RRC_WARNING, "Sent Failure RESP TO RRM; Unused variable p_api=%p p_msg=%p p_csc_gl_ctx=%p"
                    ,p_api,p_msg,p_csc_gl_ctx);/*SPR 17777 +-*/
		}
		return;
	}
	RRC_CSC_UT_TRACE_EXIT();

}

static cell_m_fsm_act_func_ptr cell_m_fsm_tbl[N_CELL_M_STATES][N_CELL_M_EVENTS]
=
{
	/* CELL_M_STATE_IDLE */
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_idle_state_config_cell_req,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
	},
	/* CELL_M_STATE_W_FOR_LLIM_CONFIG_CELL_RESP */
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_w_llim_conf_cell_state_llim_resp,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_w_llim_conf_cell_state_timer_expired,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_w_llim_del_cell_state_llim_resp,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_w_llim_conf_cell_state_cleanup_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_w_llim_conf_cell_state_rrm_del_cell_req,
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_regenerate_rand_val,
	},

	/* CELL_M_STATE_CELL_IN_SERVICE */
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_cell_in_service_state_cleanup_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_cell_in_service_sfn_sync_timer_expired,
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_cell_in_service_state_rrm_delete_cell_req,      
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_cell_reconfig_cell_req,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_cell_in_service_llim_reconf_cell_resp, /*cell_m_fsm_ignored_event,*/
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_pws_req,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_rrm_updated_pws_si_resp,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_kill_req,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_rrm_cell_start_req_failure,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_rrm_cell_stop_req,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_regenerate_rand_val,
	},
	/* CELL_M_STATE_CELL_OUT_OF_SERVICE */
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_cell_out_of_service_state_cleanup_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_cell_xxx_state_rrm_delete_req,
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_cell_out_of_service_cell_reconfig,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_rrm_cell_start_req,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_rrm_cell_stop_req_send_to_llim,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_regenerate_rand_val,
	},
	/* CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP */
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_w_llim_del_cell_state_llim_resp,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_w_llim_del_cell_state_timer_expired,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_w_llim_del_cell_state_cleanup_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_ignored_event,            
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_rrm_cell_start_req_failure,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_rrm_cell_stop_req_failure,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
	},
	/* CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP */
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_w_uecc_del_ue_state_timer_exp,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_w_uecc_del_ue_state_clean_up_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,        
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_w_uecc_delete_ue_resp,
		/* CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_w_uecc_del_resp_cell_stop_timer_expired,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
	},
	/* CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP */
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_w_llim_reconfig_cell_resp_state_clean_up_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_w_for_llim_reconfig_cell_resp_sfn_sync_timer_expired, /*cell_m_fsm_ignored_event,*/
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_cell_in_w_llim_reconfig_state_rrm_delete_cell_req,
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_w_llim_resp_rrm_reconfig_cell_req,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_w_llim_reconf_cell_state_llim_resp,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_w_llim_reconf_cell_state_timer_expired,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_w_llim_reconfig_resp_pws_req,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_w_llim_reconfig_resp_kill_req,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_rrm_cell_start_req_failure,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_rrm_cell_stop_req_failure,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_regenerate_rand_val,
	},
	/* CELL_M_STATE_W_FOR_CELL_DELETE_REQ */
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_w_cell_del_state_clean_up_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_w_llim_conf_cell_state_rrm_del_cell_req,
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
	},
	/* CELL_M_STATE_W_FOR_PWS_RESP*/
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_w_llim_pws_resp_state_clean_up_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_w_for_pws_resp_rrm_cell_delete_req,
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_w_pws_resp_rrm_reconfig_cell_req,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_llim_reconfig_resp_rollback,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_reconfig_cell_timer_expired,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_w_for_pws_resp_llim_pws_resp,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_pws_timer_expired,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_w_pws_resp_enbm_pws_failure_req,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_llim_updated_pws_si_resp,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_updated_pws_timer_expired,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_rrm_cell_stop_req_failure,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_regenerate_rand_val,
	}, 
	/* CELL_M_STATE_W_FOR_KILL_RESP*/
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_w_llim_kill_resp_state_clean_up_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_w_for_kill_resp_rrm_cell_delete_req,
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_w_pws_resp_rrm_reconfig_cell_req,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_w_for_kill_resp_llim_reconfig_resp_rollback,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_reconfig_cell_timer_expired,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_w_for_pws_resp_llim_kill_resp,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_kill_timer_expired,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_w_kill_resp_enbm_kill_failure_ind,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_regenerate_rand_val,
	}, 
	/* CELL_M_STATE_W_FOR_PWS_FAILURE_RESP*/
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_w_llim_pws_resp_state_clean_up_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_w_for_pws_resp_rrm_cell_delete_req,
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_w_pws_resp_rrm_reconfig_cell_req,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_llim_reconfig_resp_pws_failure_rollback,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_reconfig_cell_pws_failure_timer_expired,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_regenerate_rand_val,
	},
	/* CELL_M_STATE_W_FOR_LLIM_CELL_START_RESP*/
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_cell_in_service_state_cleanup_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_cell_xxx_state_rrm_delete_req,
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_w_llim_cell_start_resp,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_csc_llim_cell_start_timer_expired,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_regenerate_rand_val,
	}, 
	/* CELL_M_STATE_W_FOR_LLIM_CELL_STOP_RESP */
	{
		/* CELL_M_EVENT_RRM_CONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_CONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_DELETE_CELL_RESP */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_OAMH_CLEANUP_REQ */
		cell_m_fsm_w_llim_pws_resp_state_clean_up_req,
		/* CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_DELETE_CELL_REQ */
		cell_m_fsm_cell_xxx_state_rrm_delete_req,
		/* CELL_M_EVENT_RRM_RECONFIG_CELL_REQ */
		cell_m_fsm_ignored_event,
		/* CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_PWS_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_REQ */
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_KILL_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_START_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_RRM_CELL_STOP_REQ*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_START_RESP*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_LLIM_CELL_STOP_RESP*/
		cell_m_fsm_w_llim_cell_stop_resp,
		/*CELL_M_EVENT_CELL_START_TIMER_EXPIRED*/
		cell_m_fsm_ignored_event,
		/*CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED*/
		cell_m_csc_llim_cell_stop_timer_expired,
		/*CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED*/
		cell_m_fsm_regenerate_rand_val,
	} 
};

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_process_event
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *
 *   DESCRIPTION   : This function call appropriate handler for incoming event
 *                   in current state.
 *
 *   RETURNS       : None
 *
 ******************************************************************************/
void cell_m_fsm_process_event(
		cell_m_fsm_event_et event,              /* Incoming event */
		void                *param1,            /* p_api or timer_id */
		void                *param2,            /* p_msg or p_cell_m_timer_buf */
		/* Cell context of current operation.
		 *  Here we store Cell manager FSM state too */
		csc_cell_ctx_t      *p_cell_ctx,
		/* Pointer to the CSC global context data */
		csc_gl_ctx_t        *p_csc_gl_ctx)
{
    cell_m_fsm_state_et new_state;
    /* Coverity Fix 96750 Start */
    rrc_cell_index_t cell_index;
    /* Coverity Fix 96750 End */
    /* Coverity 96893 Fix Start */
    if(PNULL != p_cell_ctx)
    {
        /* Coverity 96893 Fix End */
        cell_m_fsm_state_et current_state = CELL_M_FSM_GET_CURRENT_STATE
            (p_cell_ctx);
        /* Coverity Fix 96750 Start */
        cell_index = p_cell_ctx->cell_index;
        /* Coverity Fix 96750 Start */
        
        /* SPR 19273 Fix Start */
        SET_CELL_INDEX(cell_index);
        /* SPR 19273 Fix End */

        RRC_CSC_UT_TRACE_ENTER();

        if ((current_state < N_CELL_M_STATES) && (event < N_CELL_M_EVENTS))
        {
          RRC_CSC_TRACE(RRC_INFO, "### CellFSM Processing state: %s Event: %s ###",
              CELL_M_FSM_STATES_NAMES[current_state],
                    CELL_M_FSM_EVENTS_NAMES[event]);

            cell_m_fsm_tbl[current_state][event](param1, param2, p_cell_ctx,
                    p_csc_gl_ctx);
           
            if(PNULL != p_csc_gl_ctx->p_p_cell_ctx[cell_index])
            {
                new_state = CELL_M_FSM_GET_CURRENT_STATE(p_cell_ctx);

                if(current_state != new_state)
                {
                    RRC_CSC_TRACE(RRC_INFO, "### CellFSM Change state: %s -> %s Event: %s ###",
                            CELL_M_FSM_STATES_NAMES[current_state],
                            CELL_M_FSM_STATES_NAMES[new_state],
                            CELL_M_FSM_EVENTS_NAMES[event]);
                }
            }
            /*coverity 96750 Fix End*/

        }
        else
        {
            RRC_CSC_TRACE(RRC_ERROR, "cell_m_fsm_process_event: unexpected state "
                    "%u or event %u", current_state, event);
            RRC_ASSERT(0);
        }
        /* Coverity 96893 Fix Start */
    }
    else
    {
        RRC_CSC_TRACE(RRC_ERROR, "cell_m_fsm_process_event: p_cell_ctx is NULL");
    }
    /* Coverity 96893 Fix End */

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_resp_rrm_reconfig_cell_req
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *
 *   DESCRIPTION  : This function start cell Reconfiguration.
 *                  STATE: CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP 
 *                  EVENT: CELL_M_EVENT_RRM_RECONFIG_CELL_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cell_m_fsm_w_llim_resp_rrm_reconfig_cell_req(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	rrc_rrm_cell_reconfig_req_t *p_reconfig_cell_req  =
		(rrc_rrm_cell_reconfig_req_t *)p_msg;
#endif
	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
#ifdef RRC_DEBUG
	/* GDB Fix - compiler warning */
	RRC_ASSERT(PNULL != p_reconfig_cell_req);
	RRC_ASSERT(p_reconfig_cell_req->cell_index == p_cell_ctx->cell_index);
#endif
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);


	RRC_CSC_TRACE(RRC_INFO,"Discarding RRC_RRM_CELL_RECONFIG_REQ"\
			" as Cell Reconfiguration is already on going p_msg=%p p_csc_gl_ctx=%p"
            ,p_msg,p_csc_gl_ctx);/*SPR 17777 +-*/

	/*  Send RRC_RRM_CELL_RECONFIG_RESP */
	csc_send_rrm_cell_reconfig_resp_failure( 
			rrc_get_transaction_id(p_api),
			p_cell_ctx->cell_index,
			RRC_FAILURE, RRM_RRC_INTERACTION_WITH_OTHER_PROCEDURE_FAILURE);

	/* Do NOT reset the p_cell_ctx->rcfg_status_flag here. */
	RRC_CSC_UT_TRACE_EXIT();

}

/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_cell_start_resp
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *
 *   DESCRIPTION  : STATE: CELL_M_STATE_W_FOR_LLIM_CELL_START_RESP
 *                  EVENT:CELL_M_EVENT_LLIM_CELL_START_RESP 
 *
 *   RETURNS      : None
 *
 *****************************************************************************/
static void cell_m_fsm_w_llim_cell_start_resp(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	rrc_csc_llim_cell_start_resp_t *p_cell_start_resp  =
		(rrc_csc_llim_cell_start_resp_t *)p_msg;
#ifndef RRC_UNIT_TEST_FRAMEWORK
	csc_cell_m_timer_buf_t            sfn_sync_timer_data;
#endif
    /*SPR 21554 Start*/
    U8 count;
    /*SPR 21554 End*/

	rrc_csc_uecc_cell_start_ind_t *p_csc_uecc_start_ind = PNULL;

	rrc_csc_oamh_cell_start_ind_t *p_csc_oamh_start_ind = PNULL;

	void *p_intrl_msg = PNULL;

	rrm_fail_cause_et       fail_cause = RRM_RRC_NO_ERROR;
	rrc_return_et           response   = RRC_SUCCESS;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_cell_start_resp);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
	RRC_ASSERT(p_cell_start_resp->cell_index == p_cell_ctx->cell_index);

	RRC_CSC_TRACE(RRC_DETAILED,
        "Unused variable in fsm p_api=%p, p_msg=%p", p_api, p_msg);

	/* Precondition - should be the same transaction */
	if (p_cell_start_resp->transaction_id != p_cell_ctx->transaction_id)
	{
		RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
				"ignored");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_START_TIMER]");

	/* Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

	switch (p_cell_start_resp->response)
	{
		case RRC_LLIM_CELL_START_SUCCESS :
            /*SPR_19262_START*/
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
                g_rrc_stats.p_rrc_stats->rrc_csc_stats.
                    rrc_csc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_TRUE;
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[0].
                    rrc_uecc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_TRUE;
            }
            /*SPR_19262_END*/
			/* Init SFN */

#ifndef RRC_UNIT_TEST_FRAMEWORK
			sfn_m_init_sfn(p_cell_start_resp->sfn, p_cell_ctx,p_csc_gl_ctx);

			/* Start CELL_M_SFN_SYNC_TIMER timer */
			sfn_sync_timer_data.auto_delete      = CSC_SFN_SYNC_TIMER_AUTODELETE;
			sfn_sync_timer_data.target_submodule = CSC_SUBMODULE_CELL_M;
			sfn_sync_timer_data.cell_index       = p_cell_ctx->cell_index;
			sfn_sync_timer_data.cell_m_event     = CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED;

			RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CELL_M_SFN_SYNC_TIMER] %u ms.",
					RRC_CSC_SFN_SYNC_TIMER_DEFAULT_DURATION);

			p_cell_ctx->mac_sfn_sync_timer =  rrc_start_timer(
					RRC_CSC_SFN_SYNC_TIMER_DEFAULT_DURATION,
					&sfn_sync_timer_data,
					sizeof (sfn_sync_timer_data),
					CSC_SFN_SYNC_TIMER_REPEATED);
#endif

			/* Send indications to UECC/OAMH that the cell is now in service*/
			p_intrl_msg = csc_alloc_intrl_msg( 
					RRC_UECC_MODULE_ID,
					CSC_UECC_CELL_START_IND,
					sizeof (rrc_csc_uecc_cell_start_ind_t));

			/* Send CSC_UECC_CELL_START_IND to UECC */
			if (PNULL != p_intrl_msg)
			{
				/* Fill and send message */
				p_csc_uecc_start_ind = 
					(rrc_csc_uecc_cell_start_ind_t *)((U8 *)p_intrl_msg +
							RRC_API_HEADER_SIZE);
				p_csc_uecc_start_ind->cell_index = p_cell_ctx->cell_index;

				rrc_send_message(p_intrl_msg, RRC_UECCMD_MODULE_ID);
			}

			p_intrl_msg = csc_alloc_intrl_msg( 
					RRC_OAMH_MODULE_ID,
					CSC_OAMH_CELL_START_IND,
					sizeof (rrc_csc_oamh_cell_start_ind_t));

			/*   Send CSC_OAMH_START_IND to OAMH*/ 
			if (PNULL != p_intrl_msg)
			{
				/*Fill and send message */
				p_csc_oamh_start_ind = 
					(rrc_csc_oamh_cell_start_ind_t *)((U8 *)p_intrl_msg +
							RRC_API_HEADER_SIZE);
				p_csc_oamh_start_ind->cell_index = p_cell_ctx->cell_index;

				rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
			}

			/* Send CSC_S1AP_CELL_REG_REQ to S1AP */
			csc_send_s1ap_cell_register_info(
					p_cell_ctx,    
					p_csc_gl_ctx);
			/* Bug 8690 Fix Start */
			if ( (RRC_CSC_EXPLICIT_CELL_START == p_csc_gl_ctx->
						p_p_cell_ctx[p_cell_start_resp->cell_index]->explicit_start_required) &&
					(RRC_FALSE == p_cell_ctx->rrc_rrm_s1ap_dereg_flag) )
			{
				/*Send X2AP_CSC_CELL_ADD_IND*/ 
				p_intrl_msg = csc_alloc_intrl_msg( RRC_X2AP_MODULE_ID,
						X2AP_CSC_CELL_ADD_IND,
						sizeof (x2ap_csc_cell_add_t));

				if (PNULL != p_intrl_msg)
				{
					/* Fill and send message */
					x2ap_csc_cell_add_t *p_x2ap_csc_cell_add_ind = (x2ap_csc_cell_add_t *)(
							(U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

					p_x2ap_csc_cell_add_ind->cell_index = (U8)p_cell_ctx->cell_index;
					/*SPR_16723_START*/
					l3_memcpy_wrapper(p_x2ap_csc_cell_add_ind->eutran_cell_id,
							p_cell_ctx->cfg_cell_param.sib_type_1_Info.cell_access_related_info.
							cell_Id,
							CELL_ID_OCTET_SIZE);

					p_x2ap_csc_cell_add_ind->phys_cell_id =
						(U16)p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.phys_cell_id; 

					p_x2ap_csc_cell_add_ind->dl_earfcn = 
                    p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.dl_earfcn; 
                    /*SPR 21554 Fix Start*/
                    memset_wrapper (&p_x2ap_csc_cell_add_ind->broadcast_plmn_info, 
                            RRC_NULL, sizeof(x2ap_broadcast_plmn_t) );
                    p_x2ap_csc_cell_add_ind->broadcast_plmn_info.num_bcast_plmn_id=
                        p_cell_ctx->cfg_cell_param.sib_type_1_Info.cell_access_related_info.plmn_Id_info_list.count;                  
                    for(count=RRC_NULL;count<p_x2ap_csc_cell_add_ind->broadcast_plmn_info.num_bcast_plmn_id;count++)
                    {
                        csc_paging_plmn_identity_rrc_to_s1ap(p_x2ap_csc_cell_add_ind->broadcast_plmn_info.plmn_identity[count].plmn_id,
                               &p_cell_ctx->cfg_cell_param.sib_type_1_Info.cell_access_related_info.plmn_Id_info_list.plmn_identity_info[count].plmn_identity);

                    }
 
                            /*SPR 21554 Fix End*/
  
					/* ULARFCN will be present in FDD mode only */
                    if(0 == (p_cell_ctx->cfg_cell_param.sib_type_1_Info.presence_bitmask & SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG))
                    {
                     if (p_cell_ctx->cfg_cell_param.sib_type_2_Info.freq_info.presence_bitmask & FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG)
                     {
                       p_x2ap_csc_cell_add_ind->ul_earfcn = 
                              p_cell_ctx->cfg_cell_param.sib_type_2_Info.freq_info.ul_carrier_freq;
                     }
                     else 
                     {
                       p_x2ap_csc_cell_add_ind->ul_earfcn = UL_DL_ARFCN_DIFFRENCE +
                               p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.dl_earfcn;
                     }
                    }
                    /* ULARFCN is equal to DLEARFCN in TDD */
                    else
                    {
                      p_x2ap_csc_cell_add_ind->ul_earfcn = p_cell_ctx->cfg_cell_param.cell_config_param.cell_parameters.dl_earfcn;
                    }
                    /*SPR_16723_END*/
                    RRC_CSC_TRACE(RRC_INFO, \
                            "Sending X2AP_CSC_CELL_ADD_IND");
                    rrc_send_message(p_intrl_msg, RRC_X2AP_MODULE_ID);
				}
			}
			/* Bug 8690 Fix End */

			/* Move to CELL_IN_SERVICE state as MAC/PHY have started successfully. */ 
			p_csc_gl_ctx->p_p_cell_ctx[p_cell_start_resp->cell_index]->
				explicit_start_required = RRC_CSC_AUTOMATIC_CELL_START;
			/* Since S1AP REG REQ has been Sent , set the flag to false */
			p_cell_ctx->rrc_rrm_s1ap_dereg_flag = RRC_FALSE;

			/*cdma_fix bug 10186 start*/
			/*Perform following operations only when SIB-8 was recd in Cell Setup Req msg from RRM*/
			if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
			{
				/*Perform following operation only when CDMA system time came
				 * enabled in SIB8 parameters during cell setup*/
				if(SIB_8_SYS_TIME_INFO_PRESENCE_FLAG & p_cell_ctx->cfg_cell_param.sib_type_8_Info.presence_bitmask)
				{ 
					p_cell_ctx->sfn_sf_timestamp_requested = RRC_TRUE;
					if(RRC_SUCCESS != resume_high_priority_thread(p_cell_ctx, p_cell_start_resp->sfn,
								p_cell_start_resp->sf, p_cell_start_resp->timestamp))
					{
						RRC_CSC_TRACE(RRC_INFO, \
								"Couldn't generate the CDMA Base Time in %s",__FUNCTION__);
					}
					else
					{
						RRC_CSC_TRACE(RRC_INFO, \
								"CDMA Base Time generated successfully in %s",__FUNCTION__);
						if(RRC_SUCCESS != pack_and_send_new_sib8_segments(p_cell_ctx, p_csc_gl_ctx))
						{
							RRC_CSC_TRACE(RRC_INFO, \
									"pack_sib8_segments failed in %s",__FUNCTION__);
						}
						else
						{
							p_cell_ctx->mac_reconfig_with_sib8_segments = RRC_TRUE;
							/* STATE TRANSITION */
							CELL_M_FSM_SET_STATE(p_cell_ctx,
									CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP);
							/* spr 10508 & 10509 start */
							p_cell_ctx->cell_start_flag = RRC_TRUE;	
							/* spr 10508 & 10509 stop */
						}
					}
					/* BUG_15960 Fix Start */
					/*If some problem with packing sib8 instances, move state to Cell Out of Service*/
					if (RRC_TRUE != p_cell_ctx->mac_reconfig_with_sib8_segments)
					{
						CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_OUT_OF_SERVICE);
						fail_cause = RRM_RRC_INTERNAL_FAILURE;
						response   = RRC_FAILURE;
					}
				}
				/*If some problem with packing sib8 instances, move state to Cell Out of Service*/
				if (RRC_TRUE != p_cell_ctx->mac_reconfig_with_sib8_segments)
				{
					CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_OUT_OF_SERVICE);
					fail_cause = RRM_RRC_INTERNAL_FAILURE;
					response   = RRC_FAILURE;
				}
			}
			else
			{
				CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_IN_SERVICE);
			}

			/*cdma_fix bug 10186 stop*/
			break;

			/* Cell remains in the OUT_OF_SERVICE state as either MAC/PHY or both could not be started.*/ 
		case RRC_LLIM_CELL_START_PHY_FAIL :
			CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_OUT_OF_SERVICE);
			fail_cause = RRM_RRC_PHY_START_FAILURE;
			response   = RRC_FAILURE;
			break ;
		case RRC_LLIM_CELL_START_MAC_FAIL :
			CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_OUT_OF_SERVICE);
			fail_cause = RRM_RRC_MAC_START_FAILURE;
			response   = RRC_FAILURE;
			break ;

		default :
			CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_OUT_OF_SERVICE);
			RRC_CSC_TRACE(RRC_ERROR, " Invalid response received ");
			fail_cause = RRM_RRC_UNKNOWN_FAILURE;
			response   = RRC_FAILURE;
			break ;
	}

	/*Bug#10186 start*/
	/* spr 10508 & 10509 start */
	if(RRC_FALSE == p_cell_ctx->cell_start_flag)
	{
		/* spr 10508 & 10509 stop */
		/* Send response to the RRM */
		if (RRC_SUCCESS != csc_send_rrm_cell_start_resp(p_cell_ctx->transaction_id,
					p_cell_ctx->cell_index,
					response,
					fail_cause))
		{
			RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP TO RRM");
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
	}
	/*Bug#10186 stop*/
	RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cell_m_fsm_w_llim_cell_stop_resp
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *
 *   DESCRIPTION   : STATE: CELL_M_STATE_W_FOR_LLIM_CELL_STOP_RESP
 *                   EVENT:CELL_M_EVENT_LLIM_CELL_STOP_RESP 
 *
 *   RETURNS       : None
 *
 *****************************************************************************/
static void cell_m_fsm_w_llim_cell_stop_resp(
		void            *p_api,         /* p_api */
		void            *p_msg,         /* p_msg */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	rrc_csc_llim_cell_stop_resp_t *p_cell_stop_resp  =
		(rrc_csc_llim_cell_stop_resp_t *)p_msg;

	rrc_csc_uecc_cell_stop_ind_t *p_csc_uecc_stop_ind  = PNULL;

	rrc_csc_oamh_cell_stop_ind_t *p_csc_oamh_stop_ind = PNULL; 

	void *p_intrl_msg = PNULL;
	rrm_fail_cause_et       fail_cause = RRM_RRC_NO_ERROR;
	rrc_return_et           response   = RRC_SUCCESS;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_cell_stop_resp);
	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
	RRC_ASSERT(p_cell_stop_resp->cell_index == p_cell_ctx->cell_index);

	RRC_CSC_TRACE(RRC_DETAILEDALL,
        "Unused variable in fsm p_api=%p p_csc_gl_ctx=%p",
        p_api,p_csc_gl_ctx);/*SPR 17777 +-*/

	/* Precondition - should be the same transaction */
	if (p_cell_stop_resp->transaction_id != p_cell_ctx->transaction_id)
	{
		RRC_CSC_TRACE(RRC_WARNING, "Message with other transaction_id - "
				"ignored");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	RRC_CSC_TRACE(RRC_BRIEF, "Stop timer [CSC_CELL_STOP_TIMER]");

	/* Stop guard timer */
	if (PNULL != p_cell_ctx->guard_timer)
	{
		rrc_stop_timer(p_cell_ctx->guard_timer);
		p_cell_ctx->guard_timer = PNULL;
	}

    /*SPR_19262_START*/
    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
        g_rrc_stats.p_rrc_stats->rrc_csc_stats.
            rrc_csc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_FALSE;
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[
            0].rrc_uecc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_FALSE;
    }
    /*SPR_19262_END*/

	switch (p_cell_stop_resp->response)
	{
		case RRC_LLIM_CELL_STOP_SUCCESS :

			/* Send indications to UECC/OAMH that the cell is now out of service*/
			p_intrl_msg = csc_alloc_intrl_msg( 
					RRC_UECC_MODULE_ID,
					CSC_UECC_CELL_STOP_IND,
					sizeof (rrc_csc_uecc_cell_stop_ind_t));

			/* Send CSC_UECC_CELL_STOP_IND to UECC */
			if (PNULL != p_intrl_msg)
			{
				/* Fill and send message */
				p_csc_uecc_stop_ind = 
					(rrc_csc_uecc_cell_stop_ind_t *)((U8 *)p_intrl_msg +
							RRC_API_HEADER_SIZE);
				p_csc_uecc_stop_ind->cell_index = p_cell_ctx->cell_index;

				rrc_send_message(p_intrl_msg, RRC_UECCMD_MODULE_ID);
			}

			p_intrl_msg = csc_alloc_intrl_msg( 
					RRC_OAMH_MODULE_ID,
					CSC_OAMH_CELL_STOP_IND,
					sizeof (rrc_csc_oamh_cell_stop_ind_t));

			/*  Send CSC_OAMH_CELL_STOP_IND to OAMH */
			if (PNULL != p_intrl_msg)
			{
				/*Fill and send message */
				p_csc_oamh_stop_ind = 
					(rrc_csc_oamh_cell_stop_ind_t *)((U8 *)p_intrl_msg +
							RRC_API_HEADER_SIZE);
				p_csc_oamh_stop_ind->cell_index = p_cell_ctx->cell_index;

				rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
			}


			/* Stop activities like MAC SFN Sync*/

			if (PNULL != p_cell_ctx->mac_sfn_sync_timer)
			{
				RRC_CSC_TRACE(RRC_BRIEF,
						"[CELL:%u] Stop timer [CELL_M_SFN_SYNC_TIMER]", p_cell_ctx->cell_index);

				rrc_stop_timer(p_cell_ctx->mac_sfn_sync_timer);
				p_cell_ctx->mac_sfn_sync_timer = PNULL;
			}

			/* Stop SFN update process */
			sfn_m_stop_sfn(p_cell_ctx);

			if (PNULL != p_cell_ctx->sfn_timer)
			{
				RRC_CSC_TRACE(RRC_BRIEF,
						"[CELL:%u] Stop timer [CSC_SFN_TIMER]",  p_cell_ctx->cell_index);
				rrc_stop_timer(p_cell_ctx->sfn_timer);
				p_cell_ctx->sfn_timer = PNULL;
			}

			/* Free Paging records storage */
			csc_paging_storage_free(p_cell_ctx->p_paging_storage, RRC_TRUE);

			if (p_cell_ctx->p_temp_si_schdl_info)
			{
				rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
				p_cell_ctx->p_temp_si_schdl_info = PNULL;
			}

			/* SPR 5145 Start */
			p_cell_ctx->explicit_start_required = RRC_CSC_EXPLICIT_CELL_START;
			/* SPR 5145 End */

			/* Move to CELL_OUT_OF_SERVICE state as MAC/PHY has stopped successfully. */ 
			CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_OUT_OF_SERVICE);
			break;

		case RRC_LLIM_CELL_STOP_MAC_FAIL :
			CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_OUT_OF_SERVICE);
			fail_cause = RRM_RRC_MAC_STOP_FAILURE;
			response   = RRC_FAILURE;
			break;

		case RRC_LLIM_CELL_STOP_PHY_FAIL :
			/* Cell remains in the CELL_IN_SERVICE state as either MAC/PHY has not stopped.*/ 
			CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_OUT_OF_SERVICE);
			fail_cause = RRM_RRC_PHY_STOP_FAILURE;
			response   = RRC_FAILURE;
			break;

		default :
			RRC_CSC_TRACE(RRC_ERROR, "Invalid response received");
			fail_cause = RRM_RRC_UNKNOWN_FAILURE;
			response   = RRC_FAILURE;

	}



	/* Send the response to RRM */
	if (RRC_SUCCESS != csc_send_rrm_cell_stop_resp(p_cell_ctx->transaction_id,
				p_cell_ctx->cell_index,
				response,
				fail_cause))
	{
		RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP TO RRM");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	p_cell_ctx->rrc_rrm_stop_cell_flag = RRC_FALSE;

	RRC_CSC_UT_TRACE_EXIT();

}

/******************************************************************************
 *  FUNCTION NAME: cell_m_csc_llim_cell_start_timer_expired
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *  
 *  DESCRIPTION  : This function stop cell Reconfiguration 
 *                 state CELL_M_STATE_W_FOR_LLIM_CELL_START_RESP ;
 *                 event CELL_M_EVENT_CELL_START_TIMER_EXPIRED 
 *   RETURNS     :  None
 *          
 *******************************************************************************/
static void cell_m_csc_llim_cell_start_timer_expired(
		rrc_timer_t     timer_id,       /* timer_id - unused */
		void            *p_timer_buf,   /* p_csc_cell_m_timer_buf_t - unused */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	RRC_CSC_TRACE(RRC_DETAILEDALL,
            "Unused variable in fsm timer_id=%p, timer_buf=%p p_csc_gl_ctx=%p",
            timer_id, p_timer_buf,p_csc_gl_ctx);/*SPR 17777 +-*/
	/* Timer expired and is not valid any more */
	RRC_CSC_TRACE(RRC_BRIEF, "Timer [CSC_CELL_START_TIMER] expired");
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
	/* Send RRC_RRC_LL_CONFIG_FAILURE */
	if (RRC_SUCCESS != csc_send_rrm_cell_start_resp(   
				p_cell_ctx->transaction_id,
				p_cell_ctx->cell_index,
				RRC_FAILURE,
				RRM_RRC_CELL_START_TIMER_EXPIRED))
	{
		RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_OUT_OF_SERVICE);

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: cell_m_csc_llim_cell_stop_timer_expired
 *   INPUT         : void            *p_api
 *                   void            *p_msg
 *                   csc_cell_ctx_t  *p_cell_ctx
 *                   csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT        : none
 *  
 *   DESCRIPTION   : This function stop cell Reconfiguration 
 *                   state CELL_M_STATE_W_FOR_LLIM_CELL_STOP_RESP ;
 *                   event CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED 
 *       
 *   RETURNS       : None
 *          
 *******************************************************************************/
static void cell_m_csc_llim_cell_stop_timer_expired(
		rrc_timer_t     timer_id,       /* timer_id - unused */
		void            *p_timer_buf,   /* p_csc_cell_m_timer_buf_t - unused */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

	RRC_CSC_TRACE(RRC_DETAILEDALL,
            "Unused variable in fsm timer_id=%p, timer_buf=%p p_csc_gl_ctx=%p",
            timer_id, p_timer_buf,p_csc_gl_ctx);/*SPR 17777 +-*/
	/* Timer expired and is not valid any more */
    RRC_CSC_TRACE(RRC_INFO, "Timer [CSC_CELL_STOP_TIMER] expired");
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
    /*SPR_19262_START*/
    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
        g_rrc_stats.p_rrc_stats->rrc_csc_stats.
            rrc_csc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_FALSE;
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[
            0].rrc_uecc_cell_stats[p_cell_ctx->cell_index].is_cell_configured = RRC_FALSE;
    }
    /*SPR_19262_END*/

	/* Send RRC_RRC_LL_CONFIG_FAILURE */
	if (RRC_SUCCESS != csc_send_rrm_cell_stop_resp(   
				p_cell_ctx->transaction_id,
				p_cell_ctx->cell_index,
				RRC_FAILURE,
				RRM_RRC_CELL_STOP_TIMER_EXPIRED))
	{
		RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}
	else
	{
		RRC_CSC_TRACE(RRC_WARNING, "Sent RESP FAILURE TO RRM");
	}

	p_cell_ctx->rrc_rrm_stop_cell_flag = RRC_FALSE;
	CELL_M_FSM_SET_STATE(p_cell_ctx, CELL_M_STATE_CELL_OUT_OF_SERVICE);

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: csc_send_s1ap_cell_register_info
 *  INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *                 csc_gl_ctx_t    *p_csc_gl_ctx
 *  OUTPUT       : none
 *  DESCRIPTION  :  This function sends S1AP_CELL_REG_REQ to S1AP. 
 *  RETURNS      : None
 *          
 *******************************************************************************/
static void csc_send_s1ap_cell_register_info(
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{

	s1ap_cell_reg_req_t *p_s1ap_cell_reg_req = PNULL;
	void *p_intrl_msg = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);


	/* Send S1AP_CELL_REG_REQ */
	p_intrl_msg = csc_alloc_intrl_msg( RRC_S1AP_MODULE_ID,
			S1AP_CELL_REG_REQ,
			sizeof (s1ap_cell_reg_req_t));
	if(PNULL == p_intrl_msg)
	{
		RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
		return;
	}

	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		p_s1ap_cell_reg_req = (s1ap_cell_reg_req_t *)(
				(U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);

		p_s1ap_cell_reg_req->cell_index = p_cell_ctx->cell_index;

		p_s1ap_cell_reg_req->max_num_supported_ue = 
			p_csc_gl_ctx->max_num_supported_ue_per_cell[p_cell_ctx->cell_index];
		p_s1ap_cell_reg_req->cell_area_info.
			bitmask |=
			S1AP_CELL_INFO_UPDATE_IND_CELL_ID_PRESENT;

		l3_memcpy_wrapper(p_s1ap_cell_reg_req->cell_area_info.cell_id,
				p_cell_ctx->cfg_cell_param.sib_type_1_Info.
				cell_access_related_info.cell_Id,
				CELL_ID_OCTET_SIZE);

		p_s1ap_cell_reg_req->cell_area_info.bitmask |=
			S1AP_CELL_INFO_UPDATE_IND_TAC_PRESENT;

		l3_memcpy_wrapper(p_s1ap_cell_reg_req->cell_area_info.tac,
				p_cell_ctx->cfg_cell_param.sib_type_1_Info.
				cell_access_related_info.tac,
				TAC_OCTET_SIZE);

		p_s1ap_cell_reg_req->cell_area_info.bitmask |=
			S1AP_CELL_INFO_UPDATE_IND_EMERGENCY_ID_PRESENT;

		l3_memcpy_wrapper(p_s1ap_cell_reg_req->cell_area_info.emergency_id,
				p_cell_ctx->cfg_cell_param.emergency_area_id,
				EMERGENCY_AREA_SIZE);

		RRC_CSC_TRACE(RRC_INFO, "cell_index sent in S1AP_CELL_REG_REQ: %d", p_s1ap_cell_reg_req->cell_index);

                
		rrc_send_message(p_intrl_msg, RRC_S1AP_MODULE_ID);

	}        
	else
	{
		RRC_CSC_TRACE(RRC_WARNING, "Memory Allocation failed");
	}

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: csc_send_uecc_setup_ind
 *  INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *                 csc_gl_ctx_t    *p_csc_gl_ctx
 *  OUTPUT       : none
 *  
 *  Description  :  This function sends CSC_UECC_INIT_SETUP_IND to UECC. 
 *   RETURNS     : None
 *          
 *******************************************************************************/
static void csc_send_uecc_setup_ind(
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	rrc_rrm_cell_setup_req_t        *p_cfg_cell_param  = PNULL;

	void *p_intrl_msg = PNULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

    /* SPR 23209 FIXED START */
    U8 operator_count = RRC_NULL ;
    /* SPR 23209 FIXED END */

	/* Send CSC_UECC_INIT_SETUP_IND */
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_UECC_MODULE_ID,
			CSC_UECC_INIT_SETUP_IND,
			sizeof (rrc_csc_uecc_init_setup_ind_t));

#ifdef RRC_UT_FRAMEWORK
    pUeccInitSetupInd =  rrc_mem_get(sizeof(rrc_csc_uecc_init_setup_ind_t) +
                                     RRC_API_HEADER_SIZE);
#endif

	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		rrc_csc_uecc_init_setup_ind_t *p_uecc_init_setup_ind =
			(rrc_csc_uecc_init_setup_ind_t *)((U8 *)p_intrl_msg +
					RRC_API_HEADER_SIZE);
		p_uecc_init_setup_ind->bitmask = 0;
		p_uecc_init_setup_ind->cell_index   = p_cell_ctx->cell_index;
		/* Init 3 array IEs */

		p_cfg_cell_param  = &p_cell_ctx->cfg_cell_param;
		/*REL 1.2:HANDOVER PROCEDURE START*/

		RRC_ASSERT(sizeof(p_uecc_init_setup_ind->mib_info) == 
				sizeof (p_cfg_cell_param->mib_info));

		p_uecc_init_setup_ind->mib_info = 
			p_cfg_cell_param->mib_info;

		RRC_ASSERT(sizeof(p_uecc_init_setup_ind->sib_type_1_Info) == 
				sizeof (p_cfg_cell_param->sib_type_1_Info));
		p_uecc_init_setup_ind->sib_type_1_Info = 
			p_cfg_cell_param->sib_type_1_Info;

		RRC_ASSERT(sizeof(p_uecc_init_setup_ind->sib_type_2_Info) == 
				sizeof (p_cfg_cell_param->sib_type_2_Info));
		p_uecc_init_setup_ind->sib_type_2_Info = 
			p_cfg_cell_param->sib_type_2_Info;
		RRC_ASSERT(sizeof(p_uecc_init_setup_ind->eutra_dl_carrier_freq) == 
				sizeof (p_cfg_cell_param->cell_config_param.
					cell_parameters.dl_earfcn));
		p_uecc_init_setup_ind->eutra_dl_carrier_freq = 
			p_cfg_cell_param->cell_config_param.cell_parameters.dl_earfcn;
		p_uecc_init_setup_ind->phys_cell_id = p_cfg_cell_param->
			cell_config_param.cell_parameters.phys_cell_id;
		/*REL 1.2:HANDOVER PROCEDURE END*/

		p_uecc_init_setup_ind->num_of_antennas = 
			p_cfg_cell_param->cell_config_param.cell_parameters.
			num_of_antennas;

		/*** TDD changes start ***/
		if (SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG &
				p_cfg_cell_param->sib_type_1_Info.presence_bitmask)
		{
			p_uecc_init_setup_ind->duplexing_mode = DUPLEXING_MODE_TDD;
		}
		/* HD-FDD start */
		else
		{
			if(RRC_RRM_DUPLEXING_MODE_PRESENT & p_cfg_cell_param->cell_config_param.mac_config.bitmask)
			{    
				p_uecc_init_setup_ind->duplexing_mode = (duplexing_mode_et)p_cfg_cell_param->
					cell_config_param.mac_config.duplexing_mode;
			}
			else
			{
				p_uecc_init_setup_ind->duplexing_mode = DUPLEXING_MODE_FDD;
			}
		}
		/* HD-FDD stop */


		/*** TDD changes stop ***/

		/* In case, cell is configured but is not yet started,
		 * send an intimation to UECC. */
		if (p_csc_gl_ctx->p_p_cell_ctx[p_uecc_init_setup_ind->cell_index]->
				explicit_start_required == RRC_CSC_EXPLICIT_CELL_START)
		{
			p_uecc_init_setup_ind->cell_state = RRC_CSC_CELL_STOPPED; 
		}
		else
		{
			p_uecc_init_setup_ind->cell_state = RRC_CSC_CELL_ACTIVE; 
		}
		if (CELL_SETUP_REQ_API_CDMA2000_PARAM_PRESENCE_FLAG &
				p_cfg_cell_param->presence_bitmask)
		{
			p_uecc_init_setup_ind->bitmask |=
				RRC_CSC_UECC_CDMA2000_RAND_PRESENT;
			p_uecc_init_setup_ind->cdma2000_rand = 
				p_cell_ctx->cdma2000_rand;

			if(CDMA2000_PARAM_1XRTT_CELL_ID_PRESENCE_FLAG & 
					p_cfg_cell_param->cdma2000_cell_param.bitmask)    
			{
				p_uecc_init_setup_ind->bitmask |=
					RRC_CSC_UECC_CDMA2000_1XRTT_CELL_ID_PRESENT ;

				RRC_ASSERT(sizeof(p_uecc_init_setup_ind->cell_id_1xrtt) == 
						sizeof (p_cfg_cell_param->cdma2000_cell_param.cell_id_1xrtt));

				p_uecc_init_setup_ind->cell_id_1xrtt = 
					p_cfg_cell_param->cdma2000_cell_param.cell_id_1xrtt;
			}    

			if(CDMA2000_PARAM_HRPD_SECTOR_ID_PRESENCE_FLAG & 
					p_cfg_cell_param->cdma2000_cell_param.bitmask) 
			{
				p_uecc_init_setup_ind->bitmask |=
					RRC_CSC_UECC_CDMA2000_HRPD_SECTOR_ID_PRESENT;

				RRC_ASSERT(sizeof(p_uecc_init_setup_ind->sector_id_hrpd) == 
						sizeof (p_cfg_cell_param->cdma2000_cell_param.sector_id_hrpd));

				p_uecc_init_setup_ind->sector_id_hrpd =
					p_cfg_cell_param->cdma2000_cell_param.sector_id_hrpd;
			}    

			/* CSR 00070260 Changes Start */
			if(CDMA2000_PARAM_1XRTT_REFERENCE_CELL_ID_PRESENCE_FLAG & 
					p_cfg_cell_param->cdma2000_cell_param.bitmask) 
			{
				p_uecc_init_setup_ind->bitmask |=
					RRC_CSC_UECC_CDMA2000_1XRTT_REF_CELL_ID_PRESENCE_FLAG;

				RRC_ASSERT(sizeof(p_uecc_init_setup_ind->ref_cell_id_1xrtt) == 
						sizeof (p_cfg_cell_param->cdma2000_cell_param.ref_cell_id_1xrtt));

				p_uecc_init_setup_ind->ref_cell_id_1xrtt =
					p_cfg_cell_param->cdma2000_cell_param.ref_cell_id_1xrtt;
			}

			if(CDMA2000_PARAM_HRPD_REFERENCE_CELL_ID_PRESENCE_FLAG & 
					p_cfg_cell_param->cdma2000_cell_param.bitmask) 
			{
				p_uecc_init_setup_ind->bitmask |=
					RRC_CSC_UECC_CDMA2000_HRPD_REF_CELL_ID_PRESENCE_FLAG;

				RRC_ASSERT(sizeof(p_uecc_init_setup_ind->ref_sector_id_hrpd) == 
						sizeof (p_cfg_cell_param->cdma2000_cell_param.ref_sector_id_hrpd));

				p_uecc_init_setup_ind->ref_sector_id_hrpd =
					p_cfg_cell_param->cdma2000_cell_param.ref_sector_id_hrpd;
			}
			/* CSR 00070260 Changes Stop */

		}
        /* SPR 23209 FIXED START */
        if ( p_cfg_cell_param->cell_config_param.mac_config.bitmask &
                RRC_RRM_MOCN_OPERATOR_INFO_PRESENT)
        {
            RRC_CSC_TRACE(RRC_WARNING, "MOCN is enabled in system");
            p_uecc_init_setup_ind->bitmask |=
                RRC_CSC_UECC_MOCN_OPERATOR_PRESENCE_FLAG;
            for(operator_count = RRC_NULL;
                    (operator_count < p_cfg_cell_param->cell_config_param.
                     mac_config.mocn_operator_info.num_operator_info) &&
                    (operator_count < MAX_PLMN_IDS );
                    operator_count++)
            {
                p_uecc_init_setup_ind->operator_id[operator_count] =
                    p_cfg_cell_param->cell_config_param.
                    mac_config.mocn_operator_info.operator_id[operator_count];
            }
        }
        else
        {
            RRC_CSC_TRACE(RRC_WARNING, "MOCN is not enabled in system");
        }
       
        /* SPR 23209 FIXED END*/


#ifdef RRC_UT_FRAMEWORK
             l3_memcpy_wrapper(pUeccInitSetupInd,p_intrl_msg,
                        (sizeof(rrc_csc_uecc_init_setup_ind_t) +
                                     RRC_API_HEADER_SIZE));
#endif

		rrc_send_message(p_intrl_msg, RRC_UECCMD_MODULE_ID);
	}
	RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
 *  FUNCTION NAME: cell_m_fsm_w_uecc_del_resp_cell_stop_timer_expired
 *  INPUT        : rrc_timer_t     timer_id
 *                 void            *p_timer_buf
 *                 csc_cell_ctx_t  *p_cell_ctx
 *                 csc_gl_ctx_t    *p_csc_gl_ctx
 *  OUTPUT       : none
 *  DESCRIPTION  : This function stop cell Reconfiguration 
 *                 state CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP;
 *                 event CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED 
 *       
 *   RETURNS     : None
 *          
 *******************************************************************************/
static void cell_m_fsm_w_uecc_del_resp_cell_stop_timer_expired(
		rrc_timer_t     timer_id,       /* timer_id - unused */
		void            *p_timer_buf,   /* p_csc_cell_m_timer_buf_t - unused */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);

    RRC_CSC_TRACE(RRC_DETAILED, "Unused variable in fsm timer_id=%p, timer_buff=%p"
            " and csc_gl_ctx=%p", timer_id, p_timer_buf, p_csc_gl_ctx);
	/* Timer expired and is not valid any more */
    RRC_CSC_TRACE(RRC_INFO, "Timer [CSC_CELL_STOP_TIMER] expired");
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */

  /*SPR 13817 Fix Start*/ 
    if (RRC_SUCCESS != csc_send_rrm_cell_stop_resp(   
                p_cell_ctx->transaction_id,
                p_cell_ctx->cell_index,
                RRC_FAILURE,
                RRM_RRC_CELL_STOP_TIMER_EXPIRED))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }
    else
    {
        RRC_CSC_TRACE(RRC_WARNING, "Sent RESP FAILURE TO RRM");
    }
   /* SPR 20704 Fix Start */
    p_cell_ctx->rrc_rrm_stop_cell_flag = RRC_FALSE;
    CELL_M_FSM_SET_STATE(p_cell_ctx,CELL_M_STATE_CELL_IN_SERVICE);
  /*SPR 13817 Fix Stop*/ 
   /* SPR 20704 Fix End */
	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: cell_m_fsm_regenerate_rand
 *  INPUT        : rrc_timer_t     timer_id
 *                 void            *p_timer_buf
 *                 csc_cell_ctx_t  *p_cell_ctx
 *                 csc_gl_ctx_t    *p_csc_gl_ctx
 *  OUTPUT       : none
 *  
 *  DESCRIPTION  : This function regenerates the rand value 
 *                 state ;
 *                 event CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED 
 *   RETURNS     : None
 *          
 *******************************************************************************/
static void cell_m_fsm_regenerate_rand_val(
		rrc_timer_t     timer_id,       /* timer_id - unused */
		void            *p_timer_buf,   /* p_csc_cell_m_timer_buf_t - unused */
		csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
		csc_gl_ctx_t    *p_csc_gl_ctx)  /* Pointer to the CSC global context data */
{
	RRC_CSC_UT_TRACE_ENTER();
     /* BUG 13744 : begins*/

	rrc_rrm_cell_setup_req_t        *p_cfg_cell_param  = PNULL;
	void *p_intrl_msg = PNULL;
	csc_cell_m_timer_buf_t          rand_regenerate_timer_data;
	U32 rand_seed = 0,
	    rand_max_limit = 0,
	    rand_min_limit = 0 ;  

	RRC_ASSERT(PNULL != p_cell_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_CSC_TRACE(RRC_DETAILED, "Unused variable in fsm timer_id=%p,"
        " p_timer_buf=%p and p_csc_gl_ctx=%p", timer_id, p_timer_buf, p_csc_gl_ctx);
	/* Timer expired and is not valid any more */
	p_cell_ctx->rand_regenerate_timer = PNULL;
	RRC_CSC_TRACE(RRC_BRIEF, "Timer [CSC_RAND_REGENERATE_TIMER] expired");

	p_cfg_cell_param  = &p_cell_ctx->cfg_cell_param;

	rand_seed = p_cell_ctx->cdma2000_rand;
	rand_min_limit = 0 ;
	rand_max_limit = UINT_MAX;  

	if (CDMA2000_PARAM_RAND_MIN_PRESENCE_FLAG &
			p_cell_ctx->cfg_cell_param.cdma2000_cell_param.cdma2000_rand.bitmask)
	{
		rand_min_limit = p_cell_ctx->cfg_cell_param.cdma2000_cell_param. 
			cdma2000_rand.rand_min ;
	}

	if (CDMA2000_PARAM_RAND_MAX_PRESENCE_FLAG &
			p_cell_ctx->cfg_cell_param.cdma2000_cell_param.cdma2000_rand.bitmask)
	{
		rand_max_limit = p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
			cdma2000_rand.rand_max ;
	}

	RRC_CSC_TRACE(RRC_BRIEF,"CDMA: RAND seed = %u\n RAND min = %u\n RAND max = %u\n", 
			rand_seed,rand_min_limit,rand_max_limit);
	get_cdma2000_rand_val(rand_seed, 
			rand_min_limit, 
			rand_max_limit,
			&(p_cell_ctx->cdma2000_rand));


	/*Start the RAND timer */
	rand_regenerate_timer_data.auto_delete = RRC_TRUE;
	rand_regenerate_timer_data.target_submodule = CSC_SUBMODULE_CELL_M;
	rand_regenerate_timer_data.cell_index    = p_cell_ctx->cell_index;
	rand_regenerate_timer_data.cell_m_event  =
		CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED;

	RRC_CSC_TRACE(RRC_BRIEF,"CDMA: RAND value = %u ", 
			p_cell_ctx->cdma2000_rand);

	/*Multiplying by 1000 to change the duration received in sec to
	 * msec*/
	RRC_CSC_TRACE(RRC_BRIEF, "Start timer [CSC_RAND_REGENERATE_TIMER] %u ms.",
			(p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
			 cdma2000_rand.rand_regenerate_timer)*1000);

	p_cell_ctx->rand_regenerate_timer = rrc_start_timer(
			(p_cell_ctx->cfg_cell_param.cdma2000_cell_param.
			 cdma2000_rand.rand_regenerate_timer)*1000,
			&rand_regenerate_timer_data, 
			sizeof (rand_regenerate_timer_data), 
			RRC_FALSE);
	/* sending the updated value of rand  to UECC
	 * in CSC_UECC_CELL_RECONFIG_IND */

	/****** Sending CSC_UECC_CELL_RECONFIG_IND ******/
	p_intrl_msg = csc_alloc_intrl_msg( 
			RRC_UECC_MODULE_ID,
			CSC_UECC_CELL_RECONFIG_IND,
			sizeof (rrc_csc_uecc_cell_reconfig_ind_t));

	if (PNULL != p_intrl_msg)
	{
		/* Fill and send message */
		rrc_csc_uecc_cell_reconfig_ind_t *p_uecc_cell_reconfig_ind =
			(rrc_csc_uecc_cell_reconfig_ind_t *)((U8 *)p_intrl_msg +
					RRC_API_HEADER_SIZE);

		p_uecc_cell_reconfig_ind->bitmask = 0;

		p_uecc_cell_reconfig_ind->mib_info = 
			p_cfg_cell_param->mib_info;

		p_uecc_cell_reconfig_ind->sib_type_1_Info = 
			p_cfg_cell_param->sib_type_1_Info;

		p_uecc_cell_reconfig_ind->sib_type_2_Info = 
			p_cfg_cell_param->sib_type_2_Info;

		p_uecc_cell_reconfig_ind->eutra_dl_carrier_freq = 
			p_cfg_cell_param->cell_config_param.cell_parameters.dl_earfcn;

		p_uecc_cell_reconfig_ind->phys_cell_id = p_cfg_cell_param->
			cell_config_param.cell_parameters.phys_cell_id;

		p_uecc_cell_reconfig_ind->cell_index   = p_cell_ctx->cell_index;

		p_uecc_cell_reconfig_ind->num_of_antennas = 
			p_cfg_cell_param->cell_config_param.cell_parameters.
			num_of_antennas;


		p_uecc_cell_reconfig_ind->bitmask |=
			RRC_CSC_UECC_CDMA2000_RAND_PRESENT;

		p_uecc_cell_reconfig_ind->cdma2000_rand = 
			p_cell_ctx->cdma2000_rand;

		rrc_send_message(p_intrl_msg, RRC_UECCMD_MODULE_ID);
	}
    /* BUG 13744 : ends*/
	RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME : csc_update_cdma2000_cell_param
 *   INPUT         : csc_cell_ctx_t  *p_cell_ctx
 *                   cdma2000_cell_param_t    *p_cdma2000_param
 *   OUTPUT        : none
 *   DESCRIPTION   : This function Update the cell configuration 
 *                   (cdma2000_cell_param_t) with new
 *                   Configuration as received in CELL RECONFIG from RRM
 *   RETURNS       : None
 *
 ***************************************************************************/
static void csc_update_cdma2000_cell_param
(
 csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
 cdma2000_cell_param_t    *p_cdma2000_param
 )
{
	rrc_rrm_cell_setup_req_t  *p_cell_set_req = &(p_cell_ctx->cfg_cell_param);
	RRC_CSC_UT_TRACE_ENTER();

	p_cell_set_req->presence_bitmask |= 
		CELL_SETUP_REQ_API_CDMA2000_PARAM_PRESENCE_FLAG;

	p_cell_set_req->cdma2000_cell_param.bitmask = 0;
	p_cell_set_req->cdma2000_cell_param.cdma2000_rand.bitmask = 0;

	p_cell_set_req->cdma2000_cell_param.cdma2000_rand.rand_seed = 
		p_cdma2000_param->cdma2000_rand.rand_seed;


	if( CDMA2000_PARAM_RAND_MIN_PRESENCE_FLAG &
			p_cdma2000_param->cdma2000_rand.bitmask )
	{
		p_cell_set_req->cdma2000_cell_param.cdma2000_rand.bitmask |= 
			CDMA2000_PARAM_RAND_MIN_PRESENCE_FLAG;

		p_cell_set_req->cdma2000_cell_param.cdma2000_rand.rand_min =
			p_cdma2000_param->cdma2000_rand.rand_min;

	}

	if( CDMA2000_PARAM_RAND_MAX_PRESENCE_FLAG &
			p_cdma2000_param->cdma2000_rand.bitmask )
	{
		p_cell_set_req->cdma2000_cell_param.cdma2000_rand.bitmask |= 
			CDMA2000_PARAM_RAND_MAX_PRESENCE_FLAG;

		p_cell_set_req->cdma2000_cell_param.cdma2000_rand.rand_max =
			p_cdma2000_param->cdma2000_rand.rand_max;

	}
	p_cell_set_req->cdma2000_cell_param.cdma2000_rand.rand_regenerate_timer =
		p_cdma2000_param->cdma2000_rand.rand_regenerate_timer ;

	if (CDMA2000_PARAM_1XRTT_CELL_ID_PRESENCE_FLAG &
			p_cdma2000_param->bitmask )
	{
		p_cell_set_req->cdma2000_cell_param.bitmask |= 
			CDMA2000_PARAM_1XRTT_CELL_ID_PRESENCE_FLAG;    

		memset_wrapper( &(p_cell_set_req->cdma2000_cell_param.cell_id_1xrtt),
				0, sizeof(cdma2000_1xrtt_cell_identifier_t));

		l3_memcpy_wrapper(&(p_cell_set_req->cdma2000_cell_param.cell_id_1xrtt), 
				&(p_cdma2000_param->cell_id_1xrtt),CDMA2000_1XRTT_CELL_ID_LENGTH);
	}   

	if (CDMA2000_PARAM_HRPD_SECTOR_ID_PRESENCE_FLAG & 
			p_cdma2000_param->bitmask)
	{
		memset_wrapper(&(p_cell_set_req->cdma2000_cell_param.sector_id_hrpd),
				0,sizeof(cdma2000_hrpd_sector_identifier_t));

		p_cell_set_req->cdma2000_cell_param.bitmask |=
			CDMA2000_PARAM_HRPD_SECTOR_ID_PRESENCE_FLAG;

		p_cell_set_req->cdma2000_cell_param.sector_id_hrpd.sector_id_length = 
			p_cdma2000_param->sector_id_hrpd.sector_id_length;

		l3_memcpy_wrapper(&(p_cell_set_req->cdma2000_cell_param.sector_id_hrpd.sector_id), 
				&(p_cdma2000_param->sector_id_hrpd.sector_id),MAX_SECTOR_ID_LENGTH);
	}    

	RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME : get_cdma2000_rand_val
 *   INPUT  :
 *              seed : Value of the seed
 *              min  : Minimum value of the rand
 *              max  : Maximum Value of the rand
 *   OUTPUT   : none
 *   DESCRIPTION   : This function is using system time to get the random value instead of using
 *                   rand() function. It performs some arithmetic operation on the seed provided
 *                    and generates the random number within range specified through min & max variables
 *   RETURNS       : None
 *
 ***************************************************************************/
static void get_cdma2000_rand_val(
		U32 seed,/* Value of the seed */ 
		U32 min, /* Minimum value of the rand */ 
		U32 max, /* Maximum Value of the rand */
		U32 *rand_val /* rand value that will be generated*/
		) 
{
	U32 range = 0 ;

	if ( (max- min) == UINT_MAX )
	{
		range = max - min ;
	}
	else 
	{
		range = max - min + 1; 
	}
	do
	{
		*rand_val = get_rand_val_from_seed(seed);
		*rand_val = (*rand_val % range) + min ;

	}while(!(*rand_val <= max && *rand_val >= min));
}

/******************************************************************************
 *   FUNCTION NAME: check_duplicate_warning_message
 *   INPUT        : csc_cell_ctx_t  *p_cell_ctx
 *   OUTPUT       : none
 *   Description:  This function checks duplicate warning message in 
 *                 Cell Context
 *
 *   RETURNS:     1) etws_info in cell_ctx_t
 *
 ******************************************************************************/
rrc_return_et check_duplicate_warning_message(
		csc_cell_ctx_t  *p_cell_ctx)   /* cell context */
{
	RRC_ASSERT(PNULL != p_cell_ctx);
	U32                 cmas_key = RRC_NULL;
	cmas_info_t         *cmas_info_node = PNULL;
	rrc_return_et       result = RRC_SUCCESS; 
	RRC_CSC_UT_TRACE_ENTER();
	cmas_key = (U32) ( p_cell_ctx->p_enb_pws_req->message_identifier[1] << 24 );
	cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->message_identifier[0] << 16 );
	cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->serial_number[1] << 8 );
	cmas_key |= (U32) ( p_cell_ctx->p_enb_pws_req->serial_number[0]);
	cmas_info_node = (cmas_info_t *)(st_find_tree_node( (const search_tree_t *)
				&(p_cell_ctx->cmas_info_list),
				(const void*)&cmas_key));

	if (PNULL != p_cell_ctx->etws_info)
	{
		/* Check whether the broadcast of ETWS warning with same message id and 
		   serial no. is already ongoing or not
		   - If already ongoing, then ignore it
		   - Else replace the ETWS warning with the recieved one */

		if (( 0 == memcmp_wrapper(&(p_cell_ctx->p_enb_pws_req->message_identifier),
						&(p_cell_ctx->etws_info->sib10_info.msg_id), sizeof(U16))) && 
				(0 == memcmp_wrapper(&(p_cell_ctx->p_enb_pws_req->serial_number), 
						     &(p_cell_ctx->etws_info->sib10_info.serial_number), sizeof(U16))))
		{
			RRC_CSC_TRACE(RRC_ERROR,"Broadcast of ETWS with same msg_id and"
					" serial no. is already ongoing");  
			result = RRC_FAILURE;
		}
		else if (( 0 == memcmp_wrapper(&(p_cell_ctx->p_enb_pws_req->message_identifier),
						&(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.msg_id), sizeof(U16))) && 
				(0 == memcmp_wrapper(&(p_cell_ctx->p_enb_pws_req->serial_number), 
						     &(p_cell_ctx->etws_info->sib11_info.sib_type_11_Info.serial_number), sizeof(U16))))
		{
			RRC_CSC_TRACE(RRC_ERROR,"Broadcast of ETWS with same msg_id and"
					" serial no. is already ongoing");
			result = RRC_FAILURE;

		}
	}
	else if (PNULL != cmas_info_node)
	{
		RRC_CSC_TRACE(RRC_ERROR,
				"Broadcast of CMAS with same message id & serial no. is already ongoing");
		result = RRC_FAILURE;
	}
	else
	{
		RRC_CSC_TRACE(RRC_ERROR,
				"Broadcast of Message with new message id & serial no.");
		result = RRC_SUCCESS;

	}

	RRC_CSC_UT_TRACE_EXIT();	
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: cell_reconfig_mac_rlc_rlc_reconfig_fail 
 *      
 *      this function update the cell information if cell reconfig fails 
 *      due to rlc layer  reconfig failure  in case of embms
 *            
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/


static rrc_return_et cell_reconfig_mac_rlc_rlc_reconfig_fail(
        csc_cell_ctx_t  *p_cell_ctx,
        csc_gl_ctx_t    *p_csc_gl_ctx
        )
{
    RRC_CSC_UT_TRACE_ENTER();

    /* 2.1 Done with maximum retry count */
    if (RRC_SUCCESS != csc_send_rrm_cell_reconfig_resp_failure( 
                p_cell_ctx->transaction_id,
                p_cell_ctx->cell_index,
                RRC_FAILURE,
                RRM_RRC_UNKNOWN_FAILURE))
    {
        RRC_CSC_TRACE(RRC_WARNING, 
                "Unable to send RESP FAILURE TO RRM");
        return RRC_FAILURE;
    }
    p_cell_ctx->rcfg_status_flag = RRC_FALSE;
    p_cell_ctx->retry_count = RRC_NULL; 

    rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
    p_cell_ctx->p_temp_si_schdl_info = PNULL;

    /* STATE TRANSITION*/
    CSC_SET_FSM_NEW_STATE(p_csc_gl_ctx,p_cell_ctx->cell_index) ; 
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}
