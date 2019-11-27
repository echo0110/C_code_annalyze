/******************************************************************************
 *
 *   FILE NAME:
 *       rrc_llim_cemh_fsm.c
 *
 *   DESCRIPTION:
 *       This header file contains implementation of CEMH FSM,
 *       all CEMH states and handlers.
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   14 May 2009     VasylS      ---------       Initial
 *
 *   Copyright (c) 2009, Aricent Inc. All Rights Reserved
 *
 ******************************************************************************/
#include "rrc_cemh_fsm.h"
#include "rrc_defines.h"
#include "rrc_common_utils.h"
#include "rrc_cemh_timers.h"
#include "rrc_msg_mgmt.h"
#include "rrc_llim_utils.h"
#include "rrc_intrl_api.h"
#include "rrc_csc_llim_intf.h"
#include "rrc_mac_intf.h"
#include "rrc_mac_il_composer.h"
#include "rrc_phy_intf.h"
#include "rrc_phy_il_composer.h"
#include "rrc_asn_enc_dec.h"
#include "rrc_rrm_intf.h"
#include "rrc_common_md.h"
#include "rrc_ext_api.h"

#ifdef LTE_EMBMS_SUPPORTED
#include "rrc_rlc_intf.h"
#include "rrc_rlc_il_composer.h"
#endif


#define CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, cell_index)\
if(p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]->explicit_start_required == RRC_LLIM_AUTOMATIC_CELL_START)\
 {\
     CEMH_FSM_SET_STATE(p_cell_ctx,CEMH_STATE_CELL_IN_SERVICE);\
 }\
else\
{\
    CEMH_FSM_SET_STATE(p_cell_ctx,CEMH_STATE_CELL_OUT_OF_SERVICE);\
}\


/*
 *   Global definitions
 */
const S8 * CEMH_FSM_STATES_NAMES[] =
{
    (const S8*)"CEMH_STATE_IDLE",
	(const S8*)"CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF",
    (const S8*)"CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF",
    (const S8*)"CEMH_STATE_W_FOR_MAC_SFN_CNF",
    (const S8*)"CEMH_STATE_CELL_IN_SERVICE",
    (const S8*)"CEMH_STATE_CELL_OUT_OF_SERVICE",
    (const S8*)"CEMH_STATE_W_FOR_PHY_RECONFIG_CELL_CNF",
    (const S8*)"CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF",
    (const S8*)"CEMH_STATE_W_FOR_PHY_CELL_START_CNF",
    (const S8*)"CEMH_STATE_W_FOR_MAC_CELL_START_CNF",
    (const S8*)"CEMH_STATE_W_FOR_PHY_CELL_STOP_CNF",
    (const S8*)"CEMH_STATE_W_FOR_MAC_CELL_STOP_CNF",
    (const S8*)"CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF",
    (const S8*)"CEMH_STATE_W_FOR_MAC_CELL_DELETE_CNF",
    (const S8*)"CEMH_STATE_W_FOR_RLC_CREATE_AREA_ENTITY_CNF",
    (const S8*)"CEMH_STATE_W_FOR_RLC_DELETE_AREA_ENTITY_CNF"
};

const S8 * CEMH_FSM_EVENTS_NAMES[] =
{
    (const S8*)"CEMH_EVENT_CSC_CONFIG_CELL_REQ",
    (const S8*)"CEMH_EVENT_PHY_CONFIG_CELL_CNF",
    (const S8*)"CEMH_EVENT_MAC_CONFIG_CELL_CNF",
    (const S8*)"CEMH_EVENT_MAC_SFN_CNF",
    (const S8*)"CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED",
    (const S8*)"CEMH_EVENT_CSC_DELETE_CELL_REQ",
    (const S8*)"CEMH_EVENT_OAMH_CLEANUP_REQ",
    (const S8*)"CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ",
    (const S8*)"CEMH_EVENT_CSC_RECONFIG_CELL_REQ",
    (const S8*)"CEMH_EVENT_PHY_RECONFIG_CELL_CNF",
    (const S8*)"CEMH_EVENT_MAC_RECONFIG_CELL_CNF",
    (const S8*)"CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED",
    (const S8*)"CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ",
    (const S8*)"CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ",
    (const S8*)"CEMH_EVENT_PHY_CELL_START_REQ",
    (const S8*)"CEMH_EVENT_PHY_CELL_START_CNF",
    (const S8*)"CEMH_EVENT_MAC_CELL_START_CNF",
    (const S8*)"CEMH_EVENT_PHY_CELL_STOP_REQ",
    (const S8*)"CEMH_EVENT_PHY_CELL_STOP_CNF",
    (const S8*)"CEMH_EVENT_MAC_CELL_STOP_CNF",
    (const S8*)"CEMH_EVENT_PHY_DELETE_CELL_CNF",
    (const S8*)"CEMH_EVENT_MAC_DELETE_CELL_CNF",
    (const S8*)"CEMH_EVENT_CELL_START_TIMER_EXPIRED",
    (const S8*)"CEMH_EVENT_CELL_STOP_TIMER_EXPIRED",
    (const S8*)"CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED",
    (const S8*)"CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF",
    (const S8*)"CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF"
};

/*
 *   CEMH FSM types
 */

typedef void (*cemh_fsm_act_func_ptr)(void *, void *, rrc_llim_cell_ctx_t *,
        rrc_llim_gl_ctx_t *);


static void cemh_build_send_rrc_phy_reconfig_cell_req
(
    rrc_csc_llim_reconfig_cell_req_t    *p_recfg_cell_param,
    rrc_llim_cell_ctx_t                 *p_cell_ctx
);
static void cemh_fsm_phy_cell_start_req
(
 void                *p_api,             
 void                *p_msg,             
 rrc_llim_cell_ctx_t *p_cell_ctx,        
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 ); 
static void build_and_send_si_without_etws_resp_to_csc
(
 rrc_llim_cell_ctx_t *p_cell_ctx,
 rrc_mac_reconfig_cell_cnf_t       *p_mac_reconfig_cell_cnf
);
static void cemh_fsm_phy_cell_start_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);  /* Pointer to the LLIM
                                                     global context data */
static void cemh_fsm_mac_cell_start_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);  /* Pointer to the LLIMi*/

static void cemh_fsm_phy_cell_stop_req(
        void                *p_api,             
        void                *p_msg,             
        rrc_llim_cell_ctx_t *p_cell_ctx,    
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);  

static void cemh_build_and_send_mac_cell_start_req(
        rrc_transaction_id_t                 transaction_id,
        rrc_cell_index_t                     cell_index);

static rrc_return_et cemh_build_and_send_mac_cell_stop_req(
        rrc_transaction_id_t                 transaction_id,
        rrc_cell_index_t                     cell_index);

static void cemh_start_cell_delete_procedure_timer(
        rrc_llim_cell_ctx_t*    p_cell_ctx,
        rrc_timer_duration_t    timer_duration);

static void cemh_start_cell_start_procedure_timer(
        rrc_llim_cell_ctx_t*    p_cell_ctx,
        rrc_timer_duration_t    timer_duration);

static void cemh_start_cell_stop_procedure_timer(
        rrc_llim_cell_ctx_t*    p_cell_ctx,
        rrc_timer_duration_t    timer_duration);

static void cemh_fsm_mac_cell_stop_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);  /* Pointer to the LLIM
                                                     global context data */

static void cemh_fsm_phy_cell_stop_timer_expiry(    
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);  /* Pointer to the LLIM 
                                                    global context data */

static void cemh_fsm_mac_cell_stop_timer_expiry(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);  /* Pointer to the LLIM
                                                     global context data */

static void cemh_fsm_phy_cell_start_timer_expiry(
    void                *p_api,             /* p_api */
    void                *p_msg,             /* p_msg */
    rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);  /* Pointer to the LLIM
                                                global context data */

static void cemh_fsm_mac_cell_start_timer_expiry(
    void                *p_api,             /* p_api */
    void                *p_msg,             /* p_msg */
    rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);  /* Pointer to the LLIM
                                                global context data */

static rrc_bool_et cemh_fill_phy_recfg_cell_parametes
(
 rrc_csc_llim_reconfig_cell_req_t     *p_recfg_cell_param,
 rrc_phy_reconfig_cell_parameters_t   *p_phy_recfg_cell_parameters,
 rrc_llim_cell_ctx_t                 *p_cell_ctx
 );


static void cemh_fill_si_msg_req
(
   rrc_mac_si_msg_req_t  *p_mac_si_msg_req,
   rrc_si_msg_req_t      *p_si_msg_req
);
static void cemh_fsm_cell_out_of_service_cell_reconfig_cell_req(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);  /* Pointer to the LLIM 
                                                     global context data */
static void cemh_fsm_in_service_state_mac_sfn_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);

static void cemh_fsm_w_xxx_delete_cell_cnf_timer_expiry(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);

static rrc_llim_csc_response_et get_fail_response_config_te(
        cemh_rrc_current_proc_et            current_proc);

/* PRS + */
static U8 fill_prs_bandwidth_in_rb (U8  prs_bandwidth);
static U8 return_prs_subframes (U8  prs_subframe_val);
static U8 return_prs_muting_bits (U8  prs_muting_bits);
/* PRS - */
#ifdef LTE_EMBMS_SUPPORTED
static void cemh_build_send_rrc_rlc_create_area_req(
        rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param,
        rrc_llim_cell_ctx_t* p_cell_ctx);
static void cemh_build_send_rrc_rlc_delete_area_req(
        rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param,
        rrc_llim_cell_ctx_t* p_cell_ctx);
static void cemh_fsm_w_rlc_create_delete_area_cell_state_timer_expired(
        rrc_timer_t         timer_id,           /* timer_id - unused */
        void                *p_timer_buf,       /* p_cemh_timer_buf - unused */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx);
/* SA_FIX Start */
static rrc_bool_et check_if_area_index_reserved(
        rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param,
        U8   area_index);
/* SA_FIX End */
#endif

/*
 *   Helper functions used in different CEMH FSM states
 */
/******************************************************************************
 *   FUNCTION NAME: get_mac_ra_resp_win_size_from_enum
 *   INPUT        : U8 win_size
 *   OUTPUT       : none
 *   DESCRIPTION  : This function returns MAC response window size value from given enum
 *
 *   RETURNS      : MAC response window size value
 *
 ******************************************************************************/
static U16 get_mac_ra_resp_win_size_from_enum(
        U8 win_size /*mac_ra_resp_win_size_et*/)
{
    U16 win_size_val = 10;

    RRC_LLIM_UT_TRACE_ENTER();

    if (win_size < MAC_RA_RESP_WIN_SIZE_SF10)
    {
        win_size_val = (U16)(win_size + 2);
    }
    else if (win_size == MAC_RA_RESP_WIN_SIZE_SF10)
    {
        win_size_val = 10;
    }
    else
    {
        RRC_ASSERT(RRC_FALSE);
    }

    RRC_LLIM_UT_TRACE_EXIT();

    return win_size_val;
}

/******************************************************************************
 *   FUNCTION NAME: get_mac_cont_resol_timer_from_enum
 *   INPUT        : U8 resol_timer
 *   OUTPUT       : none
 *   DESCRIPTION  : This function returns MAC contention resolution timer
 *                  value from given enum
 *
 *   RETURNS      : MAC contention resolution timer value
 *
 ******************************************************************************/
static U16 get_mac_cont_resol_timer_from_enum(
        U8 resol_timer /*mac_cont_resol_timer_et*/)
{
    U16 resol_timer_val = (U16) ((resol_timer + 1) * 8) ;

    RRC_LLIM_UT_TRACE_ENTER();

    if (resol_timer > MAC_CONT_RES_TIMER_SF64)
    {
        RRC_ASSERT(RRC_FALSE);
    }

    RRC_LLIM_UT_TRACE_EXIT();

    return resol_timer_val;
}
/******************************************************************************
 *   FUNCTION NAME: get_mac_num_of_preambles_in_group_a
 *
 *   INPUT        : U8 num_of_preambles_grp_a_size
 *   OUTPUT       : none
 *   DESCRIPTION  : This function returns proper value for num_of_preambles_in_group_a
 *
 *   RETURNS      : message_size_group_a value
 *
 ******************************************************************************/
static U8 get_mac_num_of_preambles_in_group_a(
        U8 num_of_preambles_grp_a_size /*rrm_ra_preambles_group_a_size_et*/)
{
    U8 num_of_preambles_grp_a_size_val = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();
    if (num_of_preambles_grp_a_size <= RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N60)
    {
        num_of_preambles_grp_a_size_val = (U8)((num_of_preambles_grp_a_size + 1) * 4);
    }
    
    RRC_LLIM_UT_TRACE_EXIT();
    return num_of_preambles_grp_a_size_val;
}
/******************************************************************************
 *   FUNCTION NAME: get_mac_num_of_contention_based_preambles
 *
 *   INPUT        : U8 ra_preamble_count
 *   OUTPUT       : none
 *   DESCRIPTION  : This function returns proper value for num_of_preambles_in_group_a
 *
 *   RETURNS      : message_size_group_a value
 *
 ******************************************************************************/
static U8 get_mac_num_of_contention_based_preambles(
        U8 ra_preamble_count /*rrm_ra_preamble_count_et*/)
{
    U8 ra_preamble_count_val = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();
    if (ra_preamble_count <=  RRM_RRC_RA_PREAMBLE_COUNT_N64)
    {
        ra_preamble_count_val = (U8)((ra_preamble_count + 1) * 4);
    }
    
    RRC_LLIM_UT_TRACE_EXIT();
    return ra_preamble_count_val;
}

/******************************************************************************
 *   FUNCTION NAME: get_mac_message_size_group_a
 *
 *   INPUT        : U8 group_a_msg_size
 *   OUTPUT       : none
 *   DESCRIPTION  : This function returns proper value for message_size_group_a
 *
 *   RETURNS      : message_size_group_a value
 *
 ******************************************************************************/
static U16 get_mac_message_size_group_a(
        U8 group_a_msg_size /*rrm_group_a_msg_size_et*/)
{
    U16 msg_size_val = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    switch(group_a_msg_size)
    {
        case RRM_RRC_GROUP_A_MSG_SIZE_B56:
            msg_size_val = 56;
            break;

        case RRM_RRC_GROUP_A_MSG_SIZE_B144:
            msg_size_val = 144;
            break;

        case RRM_RRC_GROUP_A_MSG_SIZE_B208:
            msg_size_val = 208;
            break;

        case RRM_RRC_GROUP_A_MSG_SIZE_B256:
            msg_size_val = 256;
            break;

        default:
            RRC_LLIM_TRACE(RRC_WARNING,
                    "get_mac_message_size_group_a :: Wrong value for message_size_group_a");
            break;
    }
    RRC_LLIM_UT_TRACE_EXIT();
    return msg_size_val;
}

/******************************************************************************
 *   FUNCTION NAME: fill_ul_sync_loss_timer_value
 *   INPUT        : U8 ul_sync_loss_timer
 *   OUTPUT       : none
 *   DESCRIPTION  : thid function fills ul  syn loss timer value
 *
 *   RETURNS      : unsigned short int
 *
 ******************************************************************************/
static U16 fill_ul_sync_loss_timer_value (
    U8 ul_sync_loss_timer /*rrc_ul_sync_loss_timer_et*/ )
{
    U16 timer_val = RRC_NULL;
    RRC_LLIM_UT_TRACE_ENTER();
  
    switch (ul_sync_loss_timer)
    {
        case RRC_UL_SYNC_LOSS_TIMER_INFINITY:
        {
            timer_val = 0;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_100:
        {
            timer_val = 100;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_150:
        {
            timer_val = 150;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_200:
        {
            timer_val = 200;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_300:
        {
            timer_val = 300;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_400:
        {
            timer_val = 400;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_500:
        {
            timer_val = 500;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_750:
        {
            timer_val = 750;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_1280:
        {
            timer_val = 1280;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_1920:
        {
            timer_val = 1920;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_2560:
        {
            timer_val = 2560;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_5120:
        {
            timer_val = 5120;
            break;
        }
        case RRC_UL_SYNC_LOSS_TIMER_10240:
        {
            timer_val = 10240;
            break;
        }
        default:
        {
            RRC_LLIM_TRACE(RRC_WARNING,
                    " Wrong value for RC_UL_SYNC_LOSS_TIMER value");
            break;
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
    return timer_val;
}

/******************************************************************************
 *   FUNCTION NAME: cemh_e2v_mac_ul_cyclic_lenth_prefix
 *   INPUT        : U8 ul_cyclic_prefix
 *   OUTPUT       : none
 *   DESCRIPTION  : This function converts cyclic_prefix_et enum value into the
 *                  MAC format value
 *
 *   RETURNS      : UL cyclic prefix lenth value {1,2}
 *
 ******************************************************************************/
static U8 cemh_e2v_mac_ul_cyclic_lenth_prefix(
        U8 ul_cyclic_prefix /*cyclic_prefix_et*/ )
{
    U8 ul_cyclic_lenth_prefix = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    if (CYCLIC_PREFIX_NORMAL == ul_cyclic_prefix)
    {
        ul_cyclic_lenth_prefix = 1;
    }
    else if (CYCLIC_PREFIX_EXTENDED == ul_cyclic_prefix)
    {
        ul_cyclic_lenth_prefix = 2;
    }
    else
    {
        RRC_ASSERT(RRC_FALSE);
    }

    RRC_LLIM_UT_TRACE_EXIT();

    return ul_cyclic_lenth_prefix;
}

/******************************************************************************
 *   FUNCTION NAME: cemh_e2v_delta_pucch_shift
 *   INPUT        : U8 delta_pucch_shift
 *   OUTPUT       : none
 *   DESCRIPTION  : This function converts rrm_delta_pucch_shift_et enum
 *       value into the LL format value
 *
 *   RETURNS      : Delta PUCCH shift value {1,2,3}
 *
 ******************************************************************************/
static U8 cemh_e2v_delta_pucch_shift(
        U8 delta_pucch_shift /*rrm_delta_pucch_shift_et*/)
{
    U8 delta_pucch_shift_value = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    if (RRM_RRC_DS1 == delta_pucch_shift)
    {
        delta_pucch_shift_value = 1;
    }
    else if (RRM_RRC_DS2 == delta_pucch_shift)
    {
        delta_pucch_shift_value = 2;
    }
    else if (RRM_RRC_DS3 == delta_pucch_shift)
    {
        delta_pucch_shift_value = 3;
    }
    else
    {
        RRC_ASSERT(RRC_FALSE);
    }

    RRC_LLIM_UT_TRACE_EXIT();

    return delta_pucch_shift_value;
}
/* PRS + */
/******************************************************************************
 *     FUNCTION NAME :      fill_prs_bandwidth_in_rb
 *     DESCRIPTION   :      This function fills the prs bandwidth
 *                          in terms of number of RBs.
 *     Input         :      enum value of bandwidth
 *     Output        :      number of RBs
 *
*******************************************************************************/
static U8 fill_prs_bandwidth_in_rb (U8  prs_bandwidth)
{
    U8 prs_bandwidth_in_rb = 0;

    switch (prs_bandwidth)
    {
        case RRC_PRS_RB_N6:
           prs_bandwidth_in_rb = 6;
           break;
        
        case RRC_PRS_RB_N15:
           prs_bandwidth_in_rb = 15;
           break;

        case RRC_PRS_RB_N25:
           prs_bandwidth_in_rb = 25;
           break;

        case RRC_PRS_RB_N50:
           prs_bandwidth_in_rb = 50;
           break;

        case RRC_PRS_RB_N75:
           prs_bandwidth_in_rb = 75;
           break;

        case RRC_PRS_RB_N100:
           prs_bandwidth_in_rb = 100;
           break;

        default:
           RRC_TRACE (RRC_WARNING, "PRS Bandwidth is not correct\n");
           break;
    }

    return prs_bandwidth_in_rb;
}

/******************************************************************************
 *     FUNCTION NAME :      return_prs_subframes
 *     DESCRIPTION   :      This function returns the number of prs subframes.
 *     Input         :      enum value of prs subframes
 *     Output        :      nuber of continous subframes
 *
*******************************************************************************/
static U8 return_prs_subframes (U8  prs_subframe_val)
{
    U8 prs_subframes = 0;

    switch (prs_subframe_val)
    {
        case RRC_PRS_N_SF_1:
           prs_subframes = 1;
           break;
        
        case RRC_PRS_N_SF_2:
           prs_subframes = 2;
           break;

        case RRC_PRS_N_SF_4:
           prs_subframes = 4;
           break;

        case RRC_PRS_N_SF_6:
           prs_subframes = 6;
           break;

        default:
           RRC_TRACE (RRC_WARNING, "PRS subframe value is not correct\n");
           break;
    }

    return prs_subframes;
}

/******************************************************************************
 **     FUNCTION NAME :      return_prs_muting_bits
 **     DESCRIPTION   :      This function returns the number of bits to be
 **                          read starting from LSB.
 **     Input         :      enum value of prs muting bits
 **     Output        :      number of bits to be read
 **
 ********************************************************************************/
static U8 return_prs_muting_bits (U8  prs_muting_bits)
{
    U8 bits_to_be_read = 0;

    switch (prs_muting_bits)
    {
        case RRC_PRS_N2:
           bits_to_be_read = 2;
           break;

        case RRC_PRS_N4:
           bits_to_be_read = 4;
           break;

        case RRC_PRS_N8:
           bits_to_be_read = 8;
           break;

        case RRC_PRS_N16:
           bits_to_be_read = 16;
           break;

        default:
           RRC_TRACE (RRC_WARNING, "PRS muting bits is not correct\n");
           break;
    }

    return bits_to_be_read;
}


/* PRS -*/
/******************************************************************************
 *   FUNCTION NAME: cemh_build_and_send_rrc_mac_config_cell_req
 *   INPUT        : rrc_csc_llim_config_cell_req_t* p_cfg_cell_param
 *   OUTPUT       : none
 *   DESCRIPTION  : This function builds 'rrc_mac_config_cell_req_t'
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_build_and_send_rrc_mac_config_cell_req(
        rrc_csc_llim_config_cell_req_t* p_cfg_cell_param)
{
    rrc_mac_config_cell_req_t*  p_mac_config_cell_req = PNULL;
    rrc_counter_t               counter = RRC_NULL;
#ifdef LTE_EMBMS_SUPPORTED
    m2ap_notification_config_t      *m2ap_notification_config_p = PNULL;
    U8                               count;
    U8                               index;
    U8                               num_of_area;
    mbsfn_subframe_config_t         *mbsfn_subframe_config_p = PNULL;
    mbms_updated_area_info_t        *mbms_updated_area_info_p   = PNULL;
    m2ap_mcch_related_bcch_info_t   *mcch_related_bcch_info_p  = PNULL;
    /* SPR 18672 Fix Start */
    m2ap_reserved_mcch_related_bcch_info_t *m2ap_reserved_mcch_related_bcch_info_p = PNULL;
    /* SPR 18672 Fix End */
    /*SPR 18809 fix start*/
    U8                               temp_count;
    /*SPR 18809 fix end*/
#endif


    RRC_LLIM_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cfg_cell_param);

    p_mac_config_cell_req  = (rrc_mac_config_cell_req_t *)rrc_mem_get(
            sizeof (rrc_mac_config_cell_req_t));

    if (PNULL != p_mac_config_cell_req)
    {
        memset_wrapper(p_mac_config_cell_req, 0, sizeof (rrc_mac_config_cell_req_t));
        p_mac_config_cell_req->optional_elems_present = 0;

        p_mac_config_cell_req->dl_res_blocks = 
            p_cfg_cell_param->llim_cell_param.cell_config_param.
            mac_config.dl_res_blocks;

        p_mac_config_cell_req->ul_res_blocks = 
            p_cfg_cell_param->llim_cell_param.cell_config_param.
            mac_config.ul_res_blocks;

        p_mac_config_cell_req->max_harq_retrans = 
            p_cfg_cell_param->llim_cell_param.cell_config_param.
            mac_config.max_harq_retrans;

        p_mac_config_cell_req->start_ra_rnti_range = 
            p_cfg_cell_param->llim_cell_param.cell_config_param.
            mac_config.start_ra_rnti_range;

        p_mac_config_cell_req->end_ra_rnti_range = 
            p_cfg_cell_param->llim_cell_param.cell_config_param.
            mac_config.end_ra_rnti_range;

        p_mac_config_cell_req->num_of_tx_antennas = 
            p_cfg_cell_param->llim_cell_param.cell_config_param.
            cell_parameters.num_of_antennas;
        p_mac_config_cell_req->mod_period = p_cfg_cell_param->mod_period;

        p_mac_config_cell_req->sfn_gap = p_cfg_cell_param->sfn_gap;

        p_mac_config_cell_req->nb = p_cfg_cell_param->llim_cell_param.
            sib_type_2_Info.radio_resource_config_common_sib.pcch_config.nb;      

        p_mac_config_cell_req->dci_format_for_si_msgs = 
            p_cfg_cell_param->llim_cell_param.cell_config_param.
            mac_config.dci_format_for_si_msgs;

        /* SPR 19309 + */
        p_mac_config_cell_req->cfi_value = 
            p_cfg_cell_param->llim_cell_param.cell_config_param.
            mac_config.cfi_value;
        /* SPR 19309 - */
        /* SPR 19310 + */
        p_mac_config_cell_req->num_of_eul = 
            p_cfg_cell_param->llim_cell_param.cell_config_param.
            mac_config.num_of_eul;

        p_mac_config_cell_req->size_of_eul = 
            p_cfg_cell_param->llim_cell_param.cell_config_param.
            mac_config.size_of_eul;
        /* SPR 19310 - */
        

        /* Convert ul_cyclic_lenth_prefix to value */
        p_mac_config_cell_req->ul_cyclic_lenth_prefix =
            cemh_e2v_mac_ul_cyclic_lenth_prefix(p_cfg_cell_param->
                    llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
                    ul_cyc_prefix_len);
        p_mac_config_cell_req->optional_elems_present |=
            RRC_MAC_RACH_CONFIG_INFO_PRESENT;

        p_mac_config_cell_req->rach_config_info.ra_response_window_size =
            (U8) get_mac_ra_resp_win_size_from_enum(p_cfg_cell_param->
                                                    llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
                                                    rach_config_common.ra_supervision_info.ra_resp_win_size);

        p_mac_config_cell_req->rach_config_info.mac_contention_resolution_timer =
            (U8) get_mac_cont_resol_timer_from_enum(p_cfg_cell_param->
                                                    llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
                                                    rach_config_common.ra_supervision_info.mac_cont_resol_timer);

        p_mac_config_cell_req->rach_config_info.max_harq_msg3tx = 
            p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.rach_config_common.max_harq_msg_3_tx;


        /* Num of contention based preambles changes done for HO */
        p_mac_config_cell_req->rach_config_info.num_of_contention_based_preambles =
            get_mac_num_of_contention_based_preambles(
                    p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
                radio_resource_config_common_sib.rach_config_common.
                preamble_info.ra_preamble_count);

        /* CLPC Start */
        p_mac_config_cell_req->rach_config_info.preamble_initial_received_target_power =
            p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.rach_config_common.power_ramping_params.
            preamble_init_rec_target_pow;


        /* CLPC Stop */
        if (p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
                radio_resource_config_common_sib.rach_config_common.
                preamble_info.presence_bitmask &
                PREAMBLE_INFO_GROUP_A_CONFIG_PRESENCE_FLAG)
        {        
            /* assign num_of_preambles_in_group_a */ 
            p_mac_config_cell_req->rach_config_info.optional_elems_present |=
                RRC_MAC_PREAMBLE_GROUP_A_CONFIG_PRESENT;

            p_mac_config_cell_req->rach_config_info.preamble_group_a_config.
                num_of_preambles_in_group_a =
                get_mac_num_of_preambles_in_group_a(
                        p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
                        radio_resource_config_common_sib.rach_config_common.preamble_info.
                        preambles_group_a_config.ra_preambles_group_a_size);

            /* convert and assign message_size_group_a */ 
            p_mac_config_cell_req->rach_config_info.preamble_group_a_config.
                message_size_group_a =
                get_mac_message_size_group_a(
                        p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
                        radio_resource_config_common_sib.rach_config_common.
                        preamble_info.preambles_group_a_config.group_a_msg_size);
        }
 #ifdef LTE_EMBMS_SUPPORTED
        if(p_cfg_cell_param->llim_cell_param.sib_type_2_Info.presence_bitmask &
                SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG)
        {
            p_mac_config_cell_req->optional_elems_present |= RRC_CONFIG_EMBMS_SIB2_INFO_PRESENT;
            for(index = RRC_ZERO; index < p_cfg_cell_param->llim_cell_param.
                    sib_type_2_Info.mbsfn_subframe_config_list.count; index++)
            {
                mbsfn_subframe_config_p = &(p_cfg_cell_param->llim_cell_param.
                        sib_type_2_Info.mbsfn_subframe_config_list.mbsfn_subframe_config[index]);
                count = p_mac_config_cell_req->rrc_m2ap_embms_sib2_info.no_of_csa_pattern_counter;
                p_mac_config_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[
                    count].radio_frame_allocation_period = mbsfn_subframe_config_p->radio_frame_alloc_period;
                p_mac_config_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[
                    count].radio_frame_allocation_offset = mbsfn_subframe_config_p->radio_frame_alloc_offset;
                if(mbsfn_subframe_config_p->subframe_allocation.presence_bitmask & SUBFRAME_ALLOCATION_ONE_FRAME)
                {
                    p_mac_config_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[
                        count].choice = 0;
                    p_mac_config_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[count].subframe_allocation = 
                        mbsfn_subframe_config_p->subframe_allocation.one_frame[0];
                    /*SPR 18809 fix start*/
                    p_mac_config_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[count].subframe_allocation <<= 24;
                }
                else
                {
                    p_mac_config_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[
                        count].choice = 1;
                    for(temp_count = 0 ; temp_count < 3 ; temp_count++)
                    {
                        p_mac_config_cell_req->rrc_m2ap_embms_sib2_info.
                            csa_pattern_list[count].subframe_allocation <<= 8;
                        p_mac_config_cell_req->rrc_m2ap_embms_sib2_info.
                            csa_pattern_list[count].subframe_allocation |=
                            mbsfn_subframe_config_p->subframe_allocation.four_frames[temp_count];
                    }
                    p_mac_config_cell_req->rrc_m2ap_embms_sib2_info.
                        csa_pattern_list[count].subframe_allocation <<= 8;
                }
                p_mac_config_cell_req->rrc_m2ap_embms_sib2_info.no_of_csa_pattern_counter++;
            }
        }
#endif


        p_mac_config_cell_req->rach_config_info.prach_configuration_index =
            p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.prach_config_sib.prach_config_info.
            prach_config_index; 

        p_mac_config_cell_req->rach_config_info.prach_frequency_offset =
            p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.prach_config_sib.prach_config_info.
            prach_freq_offset;
        p_mac_config_cell_req->optional_elems_present |= 
            RRC_MAC_PHICH_CONFIG_INFO_PRESENT;

        p_mac_config_cell_req->phich_config_info.phich_duration =
            p_cfg_cell_param->llim_cell_param.mib_info.phich_config.phich_duration;

        p_mac_config_cell_req->phich_config_info.phich_resource =
            p_cfg_cell_param->llim_cell_param.mib_info.
            phich_config.phich_resource;

        p_mac_config_cell_req->optional_elems_present |=
            RRC_MAC_PUCCH_CONFIG_INFO_PRESENT;

        /* Convert delta_pucch_shift to value */
        p_mac_config_cell_req->pucch_config_info.delta_pucch_shift =
            cemh_e2v_delta_pucch_shift( 
                    p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
                    radio_resource_config_common_sib.pucch_config_common.
                    delta_pucch_shift);

        p_mac_config_cell_req->pucch_config_info.nrb_cqi = 
            p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.pucch_config_common.n_rb_cqi;

        p_mac_config_cell_req->pucch_config_info.ncs_an = 
            p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.pucch_config_common.n_cs_an;

        p_mac_config_cell_req->pucch_config_info.n1pucch_an = 
            p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.pucch_config_common.n_one_pucch_an;

        if (RRC_RRM_RBS_PER_TBS_DCI_1A_PRESENT & 
                        p_cfg_cell_param->llim_cell_param.
                        cell_config_param.mac_config.bitmask )
        {
            p_mac_config_cell_req->optional_elems_present |=
            RRC_MAC_RBS_PER_TBS_DCI_1A_PRESENT;

            l3_memcpy_wrapper(&p_mac_config_cell_req->num_rbs_per_tbs_dci_1a.num_rb_buffer,
                   &p_cfg_cell_param->llim_cell_param.cell_config_param.
                     mac_config.num_rbs_per_tbs_dci_1a,
                   RBS_FOR_DCI_1A_SIZE);
        }

        if (RRC_RRM_RBS_PER_TBS_DCI_1C_PRESENT & 
                        p_cfg_cell_param->llim_cell_param.
                        cell_config_param.mac_config.bitmask )
        {
            p_mac_config_cell_req->optional_elems_present |=
            RRC_MAC_RBS_PER_TBS_DCI_1C_PRESENT;

            l3_memcpy_wrapper(&p_mac_config_cell_req->num_rbs_per_tbs_dci_1c.num_rb_buffer,
                   &p_cfg_cell_param->llim_cell_param.cell_config_param.
                     mac_config.num_rbs_per_tbs_dci_1c,
                   RBS_FOR_DCI_1C_SIZE);
        }

        p_mac_config_cell_req->mib_msg_req_counter = 
            p_cfg_cell_param->mib_msg_req_counter;
        for (counter = 0; counter < p_cfg_cell_param->mib_msg_req_counter; 
                ++counter)
        {
            p_mac_config_cell_req->mib_msg_req[counter] = 
                p_cfg_cell_param->mib_msg_req[counter];
        }
        p_mac_config_cell_req->optional_elems_present |=
            RRC_MAC_SIBTYPE1_MSG_REQ_PRESENT;
        p_mac_config_cell_req->sib1_msg_req = p_cfg_cell_param->sib1_msg_req;

        p_mac_config_cell_req->optional_elems_present |= 
            RRC_MAC_SI_MSG_REQ_PRESENT;
        cemh_fill_si_msg_req (&p_mac_config_cell_req->si_msg_req,
                              &p_cfg_cell_param->si_msg_req);

        /******* TDD changes start *******/

        if(SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG &
                p_cfg_cell_param->llim_cell_param.sib_type_1_Info.
                presence_bitmask)
        {
            p_mac_config_cell_req->duplexing_mode = DUPLEXING_MODE_TDD;

            RRC_LLIM_TRACE(RRC_INFO,"TDD mode sent to MAC during Cell Setup");

            p_mac_config_cell_req->optional_elems_present |=
                RRC_MAC_TDD_CONFIG_PRESENT;

            p_mac_config_cell_req->tdd_config.
                sub_frame_assignment =
                p_cfg_cell_param->llim_cell_param.sib_type_1_Info.tdd_config.
                sub_frame_assignment;
            RRC_LLIM_TRACE(RRC_INFO,"TDD sub_frame_assignment [%u]",
                    p_mac_config_cell_req->tdd_config.sub_frame_assignment);

            p_mac_config_cell_req->tdd_config.
                special_sub_frame_pattern =
                p_cfg_cell_param->llim_cell_param.sib_type_1_Info.tdd_config.
                special_sub_frame_pattern;

        }
        /* HD-FDD start */
        else
        {
            if(RRC_RRM_DUPLEXING_MODE_PRESENT & p_cfg_cell_param->llim_cell_param.
                cell_config_param.mac_config.bitmask)
            {
                p_mac_config_cell_req->duplexing_mode = p_cfg_cell_param->llim_cell_param.
                   cell_config_param.mac_config.duplexing_mode;
                if(1 == p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.
                        duplexing_mode)
                {   
                         RRC_LLIM_TRACE(RRC_INFO,"FDD mode sent to MAC during Cell Setup");
                }   
                else
                {       
                        RRC_LLIM_TRACE(RRC_INFO,"HD-FDD mode sent to MAC during Cell Setup");
                }
            }
            else
            {
                p_mac_config_cell_req->duplexing_mode = DUPLEXING_MODE_FDD;
                RRC_LLIM_TRACE(RRC_INFO,"FDD mode sent to MAC during Cell Setup");
            }
        }
        /* HD-FDD stop */

        /******* TDD changes stop *******/


        /* SRS Changes Start */

        p_mac_config_cell_req->optional_elems_present |=
            RRC_MAC_SRS_COMMON_CONFIG_PRESENT;

        if (p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
                radio_resource_config_common_sib.sounding_rs_ul_config_common.presence_bitmask &
                SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG)
        {
            /* Set SRS Config Type 1 - Setup and 0 - Release */
            p_mac_config_cell_req->srs_common_config.config_type = 1;

            p_mac_config_cell_req->srs_common_config.optional_elems_present |= 
                RRC_MAC_SRS_COMMON_SETUP_INFO_PRESENT;

            p_mac_config_cell_req->srs_common_config.srs_common_setup_info.
                srs_bandwidth_config = 
                p_cfg_cell_param->llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
                sounding_rs_ul_config_common.setup.srs_bw_config;


            p_mac_config_cell_req->srs_common_config.srs_common_setup_info.
                srs_subframe_config = 
                p_cfg_cell_param->llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
                sounding_rs_ul_config_common.setup.srs_subframe_config;


            p_mac_config_cell_req->srs_common_config.srs_common_setup_info.
                ack_nack_srs_simultaneous_trans =
                p_cfg_cell_param->llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
                sounding_rs_ul_config_common.setup.ack_nack_srs_simul_trans;


            if (p_cfg_cell_param->llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
                    sounding_rs_ul_config_common.setup.presence_bitmask &
                    SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG)
            {
                p_mac_config_cell_req->srs_common_config.srs_common_setup_info.
                    optional_elems_present |= RRC_MAC_SRS_MAX_UPPTS_PRESENT;

                p_mac_config_cell_req->srs_common_config.srs_common_setup_info.
                    srs_max_uppts.max_uppts = 
                    p_cfg_cell_param->llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
                    sounding_rs_ul_config_common.setup.srs_max_up_pts;
            }


        }
        else
        {
            /* Set SRS Config Type 1 - Setup and 0 - Release */
            p_mac_config_cell_req->srs_common_config.config_type = 0;
        }
        /* SRS Changes Stop */

        /* Check presence for enable frequency scheduling */
        if (p_cfg_cell_param->llim_cell_param.cell_config_param.
                mac_config.bitmask & RRC_MAC_ENABLE_FREQUENCY_SELECTIVE_SCHEDULING_PRESENT)
        {
            /* Set bit in optional element */
            p_mac_config_cell_req->optional_elems_present |=
                RRC_MAC_FREQ_SELECTIVE_ENABLE_PRESENT;

            p_mac_config_cell_req->freq_selective_scheduling.ul_freq_selective_enable = 
                p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.
                enable_frequency_selective_scheduling.ul_freq_selective_enable;

            p_mac_config_cell_req->freq_selective_scheduling.dl_freq_selective_enable = 
                p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.
                enable_frequency_selective_scheduling.dl_freq_selective_enable;
        }
        /*SPR_19066_START*/
        /* Code Removed */
        /*SPR_19066_END*/
        /* CLPC Start */
        /* Fill PDSCH_CONFIG_COMMON_INFO : ReferenceSignalPower*/
        p_mac_config_cell_req->optional_elems_present |=
            RRC_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT;

        p_mac_config_cell_req->pdsch_config_common_info.reference_signal_power =
            p_cfg_cell_param->llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
            pdsch_config_common.ref_signal_power;


        p_mac_config_cell_req->pdsch_config_common_info.p_b =
            p_cfg_cell_param->llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
            pdsch_config_common.pb;

        /* Fill DOWNLINK_POWER_CONTROL_COMMON */
        p_mac_config_cell_req->optional_elems_present |=
            RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT;

        p_mac_config_cell_req->downlink_power_control_common =
            p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.
            downlink_power_control_common;

        /* Fill UPLINK_POWER_CONTROL_COMMON */
        p_mac_config_cell_req->optional_elems_present |=
            RRC_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT;

        p_mac_config_cell_req->uplink_power_control_common.p0_nominal_pusch =
            p_cfg_cell_param->llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
            uplink_power_control_common.p_zero_nominal_Pusch;

        p_mac_config_cell_req->uplink_power_control_common.alpha =
            p_cfg_cell_param->llim_cell_param.sib_type_2_Info.radio_resource_config_common_sib.
            uplink_power_control_common.alpha;

        /* Fill PUSCH_CONFIG_COMMON */  
        p_mac_config_cell_req->optional_elems_present |=
            RRC_MAC_PUSCH_CONFIG_COMMON_PRESENT;

        p_mac_config_cell_req->pusch_config_common_info.enable_64_qam =
               p_cfg_cell_param->llim_cell_param.sib_type_2_Info.
                 radio_resource_config_common_sib.pusch_config_common.
                   pusch_config_basic.enable_64_qam;
                     
        /* Fill TPC_RNTI_RANGE If present */
        if (RRC_MAC_TPC_RNTI_PRESENT &
                p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.bitmask)
        {
            p_mac_config_cell_req->optional_elems_present |=
                RRC_MAC_TPC_RNTI_RANGE_PRESENT;

            p_mac_config_cell_req->tpc_rnti_range =
                p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.tpc_rnti_range;
        }

        /* Fill POWER_CONTROL_ENABLE if present */
        if (RRC_MAC_POWER_CONTROL_PRESENT &
                p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.bitmask)
        {
            p_mac_config_cell_req->optional_elems_present |=
                RRC_MAC_POWER_CONTROL_ENABLE_PRESENT;

            p_mac_config_cell_req->power_control_enable =
                p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.power_control_enable;
        }

        /* CLPC Stop */

        /* Fill UE Inactive Time Value Stop */
  
        /* Fill Contention Free Rach Timer Info Start */
        if (RRC_RRM_CONTENTION_FREE_RACH_TIMER_PRESENT &
        p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.bitmask)
        {
            /* Set bit in optional parameter */
            p_mac_config_cell_req->optional_elems_present |=
                RRC_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT;

            p_mac_config_cell_req->contn_free_rach_timer_info.contn_free_rach_timer
                = p_cfg_cell_param->llim_cell_param.cell_config_param.
                  mac_config.contention_free_rach_timer;
        }
        /* Fill Contention Free Rach Timer Info Stop */


        /* Fill the explicit start required IE to start the Cell at MAC. */
        if (p_cfg_cell_param->llim_cell_setup_req.presence_bitmask & CELL_SETUP_REQ_API_EXPLICIT_START_REQUIRED_FLAG)
        {
            p_mac_config_cell_req->explicit_cell_start_required.explicit_start_required =
                p_cfg_cell_param->llim_cell_setup_req.explicit_start_required;

            p_mac_config_cell_req->optional_elems_present |= RRC_MAC_EXPLICIT_START_REQUIRED_PRESENT;
        }

        if (RRC_RRM_UL_SYNC_LOSS_TIMER_PRESENT & 
                        p_cfg_cell_param->llim_cell_param.
                        cell_config_param.mac_config.bitmask )
        {
            p_mac_config_cell_req->optional_elems_present |=
                RRC_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT;

            p_mac_config_cell_req->ul_sync_loss_timer_info.
            ul_sync_loss_timer_value =
                  fill_ul_sync_loss_timer_value(p_cfg_cell_param->llim_cell_param.
                     cell_config_param.mac_config.ul_sync_loss_timer);
        }

        if (RRC_RRM_PUCCH_CQI_SINR_THRESHOLD_PRESENT & 
                        p_cfg_cell_param->llim_cell_param.
                        cell_config_param.mac_config.bitmask )
        {
            p_mac_config_cell_req->optional_elems_present |=
                RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT;

            p_mac_config_cell_req->pucch_cqi_sinr_threshold_info.
              sinr_value =
                   p_cfg_cell_param->llim_cell_param.cell_config_param.
                     mac_config.pucch_cqi_sinr_value;
        }
        

        if (RRC_RRM_N_GAP_PRESENT & 
                        p_cfg_cell_param->llim_cell_param.
                        cell_config_param.mac_config.bitmask )
        {
            p_mac_config_cell_req->optional_elems_present |=
                RRC_MAC_N_GAP_PRESENT;

            p_mac_config_cell_req->n_gap.n_gap=
                   p_cfg_cell_param->llim_cell_param.cell_config_param.
                     mac_config.n_gap;
        }
        /* PDCCH start */
        if ((p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.bitmask &
                RRC_RRM_IS_DYNAMIC_PDCCH_DISABLED_PRESENT) && (p_cfg_cell_param->
                llim_cell_param.cell_config_param.mac_config.dynamic_pdcch == RRC_TRUE))
        {
            p_mac_config_cell_req->optional_elems_present |= RRC_MAC_DYNAMIC_PDCCH_INFO_PRESENT;
            
            p_mac_config_cell_req->dynamic_pdcch_info.isDynamicPdcchDisabled = p_cfg_cell_param->
                llim_cell_param.cell_config_param.mac_config.dynamic_pdcch;
        }
        /* PDCCH stop */

        /* SPS Start */
        if (p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.bitmask &
            RRC_RRM_SPS_CRNTI_RANGE_PRESENT)
        {
            p_mac_config_cell_req->optional_elems_present |= RRC_MAC_SPS_CRNTI_RANGE_PRESENT;

            p_mac_config_cell_req->sps_crnti_range.start_sps_crnti =
                p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.sps_crnti_range.start_sps_crnti;
            
            p_mac_config_cell_req->sps_crnti_range.end_sps_crnti = 
                p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.sps_crnti_range.end_sps_crnti;
        }
        /* SPS Stop */
	/* PRS + */
	if ((p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.bitmask &
				RRC_RRM_PRS_CONFIG_INFO_PRESENT))
	{
		p_mac_config_cell_req->optional_elems_present |= RRC_MAC_PRS_CONFIG_INFO_PRESENT;

		p_mac_config_cell_req->prs_config_info.request_type = 
			p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.prs_config_info.request_type;

		if(p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.prs_config_info.request_type == 1)
		{
			if ((p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.prs_config_info.bitmask &
						RRC_RRM_CONFIG_PRS_CONFIG_PRESENT))
			{				
				/* Set bitmask in optional element for all mandatory parameters at mac-L3 interface */
				p_mac_config_cell_req->prs_config_info.optional_elems_present |= RRC_MAC_CONFIG_PRS_BANDWIDTH_INFO_PRESENT;
		 		/* Store mapped value of prs_bandwidth recieved from rrm */
				p_mac_config_cell_req->prs_config_info.prs_bandwidth_info.prs_bandwidth = 
					fill_prs_bandwidth_in_rb(p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.
                                                prs_config_info.prs_config.prs_bandwidth_info.prs_bandwidth);

				p_mac_config_cell_req->prs_config_info.optional_elems_present |= RRC_MAC_CONFIG_PRS_SUBFRAMES_INFO_PRESENT;
				/* Store mapped value of prs_subframes recieved from rrm */
				p_mac_config_cell_req->prs_config_info.prs_subframes_info.prs_subframes =
					return_prs_subframes(p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.
                                                prs_config_info.prs_config.prs_subframes_info.prs_subframes);;
				
				p_mac_config_cell_req->prs_config_info.optional_elems_present |= RRC_MAC_CONFIG_PRS_CONFIG_INDEX_INFO_PRESENT;
				p_mac_config_cell_req->prs_config_info.prs_config_index_info.prs_config_index =     
					p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.prs_config_info.prs_config.
						prs_config_index_info.prs_config_index;

				p_mac_config_cell_req->prs_config_info.optional_elems_present |= RRC_MAC_CONFIG_PRS_TRANSMISSION_POWER_INFO_PRESENT;
				p_mac_config_cell_req->prs_config_info.prs_transmission_power_info.prs_transmission_power =     
					p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.prs_config_info.prs_config.
						prs_transmission_power_info.prs_transmission_power;

				if(p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.prs_config_info.prs_config.bitmask & 
						RRC_RRM_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT)
				{
					p_mac_config_cell_req->prs_config_info.optional_elems_present |= RRC_MAC_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT;
					/* Store mapped value of muting_bits recieved from rrm */
					p_mac_config_cell_req->prs_config_info.prs_muting_config_info.bits_to_be_read = 
							return_prs_muting_bits(p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.
                                                        prs_config_info.prs_config.prs_muting_config_info.bits_to_be_read);

					p_mac_config_cell_req->prs_config_info.prs_muting_config_info.prs_muting_config_index =
						p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.
						prs_config_info.prs_config.prs_muting_config_info.prs_muting_config_index;
				}
			}
		}
	}
	/* PRS - */
        RRC_LLIM_TRACE(RRC_INFO,"API:RRC->MAC:RRC_MAC_CONFIG_CELL_REQ:CellIndex:%u",
                         p_cfg_cell_param->cell_index);
#ifdef LTE_EMBMS_SUPPORTED
        if ((p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.bitmask &
                    RRC_RRM_SIB_13_INFO_PRESENCE_FLAG))
        {
            p_mac_config_cell_req->optional_elems_present |= RRC_CONFIG_SIB13_INFO_PRESENT;
            m2ap_notification_config_p = &(p_cfg_cell_param->llim_cell_param.
                    cell_config_param.mac_config.sib_type_13_info.m2ap_notification_config);
            for(index = RRC_ZERO; index < p_cfg_cell_param->llim_cell_param.
                    cell_config_param.mac_config.mbms_updated_area_info_list.num_valid_mbsfn_area_info;
                    index++)
            {
                mbms_updated_area_info_p = &(p_cfg_cell_param->llim_cell_param.
                        cell_config_param.mac_config.mbms_updated_area_info_list.mbms_updated_area_info[index]);
                mcch_related_bcch_info_p = get_m2ap_mcch_related_bcch_info(
                        &(p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.sib_type_13_info),
                        mbms_updated_area_info_p->area_id);
                if(((U8)ADDED == mbms_updated_area_info_p->flag) || ((U8)MODIFIED ==
                            mbms_updated_area_info_p->flag) || ((U8)UNCHANGE ==
                                mbms_updated_area_info_p->flag))
                {
                    num_of_area =
                        p_mac_config_cell_req->
                        rrc_m2ap_mac_create_embms_area_entity_info.num_of_area;
                    if(!(p_mac_config_cell_req->optional_elems_present &
                                RRC_M2AP_CONFIG_MBMS_AREA_CREATE_PRESENT))
                    {
                        p_mac_config_cell_req->optional_elems_present |= 
                            RRC_M2AP_CONFIG_MBMS_AREA_CREATE_PRESENT;
                    }
                    p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                        m2ap_mbms_create_area_info[num_of_area].area_id =
                        mbms_updated_area_info_p->area_id;
                    p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                        m2ap_mbms_create_area_info[num_of_area].area_index =
                        mbms_updated_area_info_p->area_index;
                    if((U8)ADDED == mbms_updated_area_info_p->flag)
                    {
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            NEW_AREA;
                    }
                    else if((U8)MODIFIED == mbms_updated_area_info_p->flag)
                    {
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            CHANGE_AREA;
                    }
                    else
                    {
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            NO_CHANGE;
                    }
                    if(mcch_related_bcch_info_p != PNULL)
                    {
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].notification_ind
                            = mcch_related_bcch_info_p->notification_indicator_r9;
                        /* SPR 18300 Fix Start */    
                        if(RRC_ZERO == mcch_related_bcch_info_p->pdcch_length)
                        {
                            p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_ONE;
                        }
                        else if(RRC_ONE == mcch_related_bcch_info_p->pdcch_length)
                        {
                            p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_TWO;
                        }
                        else
                        {
                            RRC_LLIM_TRACE(RRC_ERROR," Incorrect pdcch length recevied");
                            p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_ZERO;
                        }
                        /* SPR 18300 Fix End */    

                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].optional_elems_present
                            |= RRC_M2AP_MAC_MCCH_CONFIG_PRESENT;

                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_rep_period
                            = mcch_related_bcch_info_p->mcch_repetition_period;
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_offset
                            = mcch_related_bcch_info_p->mcch_offset;
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_mod_period
                            = mcch_related_bcch_info_p->mcch_modification_period;
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.sf_alloc_info
                            = mcch_related_bcch_info_p->sf_alloc_info;
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.signalling_mcs
                            = mcch_related_bcch_info_p->signalling_mcs;
                        /* SPR 18672 Fix Start */    
                        /* reserved cell changes start */
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].area_type
                            = RRC_ONE;
                        p_mac_config_cell_req->
                            rrc_m2ap_mac_create_embms_area_entity_info.num_of_area++;
                        p_mac_config_cell_req->
                            rrc_m2ap_mac_create_embms_area_entity_info.num_create_area_info_counter++;
                        /* SPR 18672 Fix End */    
                    }

                }
                else if((U8)DELETED == mbms_updated_area_info_p->flag)
                {
                    num_of_area =
                        p_mac_config_cell_req->
                        rrc_m2ap_mac_delete_embms_area_entity_info.num_area_delete_count;
                    if(!(p_mac_config_cell_req->optional_elems_present &
                                RRC_M2AP_MBMS_AREA_DELETE_PRESENT))
                    {
                        p_mac_config_cell_req->optional_elems_present |= 
                            RRC_M2AP_MBMS_AREA_DELETE_PRESENT;
                    }
                    p_mac_config_cell_req->rrc_m2ap_mac_delete_embms_area_entity_info.
                        area_index[num_of_area] =
                        mbms_updated_area_info_p->area_index;
                    p_mac_config_cell_req->
                        rrc_m2ap_mac_delete_embms_area_entity_info.num_area_delete_count++;
                }

            }
            /* fill sib13 scheduling info */
            /* SPR 18439 Fix Start */
            p_mac_config_cell_req->rrc_m2ap_mac_sib13_info.si_index =
                p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.
                sib13_scheduling_info.si_index;
            /* SPR 18439 Fix End */
            if(!(p_mac_config_cell_req->optional_elems_present &
                        RRC_M2AP_CONFIG_MBMS_AREA_CREATE_PRESENT))
            {
                p_mac_config_cell_req->optional_elems_present |= 
                    RRC_M2AP_CONFIG_MBMS_AREA_CREATE_PRESENT;
            }
            /* fill notification config */
            p_mac_config_cell_req->
                rrc_m2ap_mac_create_embms_area_entity_info.optional_elems_present
                |= RRC_M2AP_MAC_NOTIFICATION_CONFIG_PRESENT;
            if (m2ap_notification_config_p->notification_repetition_coeff == 0)
            {
                p_mac_config_cell_req->
                    rrc_m2ap_mac_create_embms_area_entity_info.
                    m2ap_embms_notification_config.notification_rept_coeff = 2;
            }
            else if (m2ap_notification_config_p->notification_repetition_coeff == 1)
            {
                p_mac_config_cell_req->
                    rrc_m2ap_mac_create_embms_area_entity_info.
                    m2ap_embms_notification_config.notification_rept_coeff = 4;
            }
            else
            {
                p_mac_config_cell_req->
                    rrc_m2ap_mac_create_embms_area_entity_info.
                    m2ap_embms_notification_config.notification_rept_coeff = 0;
            }
            p_mac_config_cell_req->
                rrc_m2ap_mac_create_embms_area_entity_info.
                m2ap_embms_notification_config.notification_offset =
                m2ap_notification_config_p->notification_offset;
            p_mac_config_cell_req->
                rrc_m2ap_mac_create_embms_area_entity_info.
                m2ap_embms_notification_config.notification_sf_index =
                m2ap_notification_config_p->notification_sf_Index;
        }
        /* SPR 18672 Fix Start */
        if ((p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.bitmask &
                    RRC_RRM_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG))
        {
            for(index = RRC_ZERO; index < p_cfg_cell_param->llim_cell_param.
                    cell_config_param.mac_config.mbms_updated_area_info_list.num_valid_mbsfn_area_info;
                    index++)
            {
                mbms_updated_area_info_p = &(p_cfg_cell_param->llim_cell_param.
                        cell_config_param.mac_config.mbms_updated_area_info_list.mbms_updated_area_info[index]);
                m2ap_reserved_mcch_related_bcch_info_p = get_m2ap_reserved_mcch_related_bcch_info(
                        &(p_cfg_cell_param->llim_cell_param.cell_config_param.
                            mac_config.m2ap_reserved_mcch_related_bcch_info_list),
                        mbms_updated_area_info_p->area_id);
                if(((U8)ADDED == mbms_updated_area_info_p->flag) || ((U8)MODIFIED ==
                            mbms_updated_area_info_p->flag) || ((U8)UNCHANGE ==
                                mbms_updated_area_info_p->flag))
                {
                    num_of_area =
                        p_mac_config_cell_req->
                        rrc_m2ap_mac_create_embms_area_entity_info.num_of_area;
                    if(!(p_mac_config_cell_req->optional_elems_present &
                                RRC_M2AP_CONFIG_MBMS_AREA_CREATE_PRESENT))
                    {
                        p_mac_config_cell_req->optional_elems_present |= 
                            RRC_M2AP_CONFIG_MBMS_AREA_CREATE_PRESENT;
                    }
                    p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                        m2ap_mbms_create_area_info[num_of_area].area_id =
                        mbms_updated_area_info_p->area_id;
                    p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                        m2ap_mbms_create_area_info[num_of_area].area_index =
                        mbms_updated_area_info_p->area_index;
                    if((U8)ADDED == mbms_updated_area_info_p->flag)
                    {
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            NEW_AREA;
                    }
                    else if((U8)MODIFIED == mbms_updated_area_info_p->flag)
                    {
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            CHANGE_AREA;
                    }
                    else
                    {
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            NO_CHANGE;
                    }
                    if(m2ap_reserved_mcch_related_bcch_info_p != PNULL)
                    {
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].notification_ind
                            = m2ap_reserved_mcch_related_bcch_info_p->notification_indicator_r9;
                        /* SPR 18300 Fix Start */    
                        if(RRC_ZERO == m2ap_reserved_mcch_related_bcch_info_p->pdcch_length)
                        {
                            p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_ONE;
                        }
                        else if(RRC_ONE == m2ap_reserved_mcch_related_bcch_info_p->pdcch_length)
                        {
                            p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_TWO;
                        }
                        else
                        {
                            RRC_LLIM_TRACE(RRC_ERROR," Incorrect pdcch length recevied");
                            p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_ZERO;
                        }
                        /* SPR 18300 Fix End */    

                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].optional_elems_present
                            |= RRC_M2AP_MAC_MCCH_CONFIG_PRESENT;

                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_rep_period
                            = m2ap_reserved_mcch_related_bcch_info_p->mcch_repetition_period;
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_offset
                            = m2ap_reserved_mcch_related_bcch_info_p->mcch_offset;
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_mod_period
                            = m2ap_reserved_mcch_related_bcch_info_p->mcch_modification_period;
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.sf_alloc_info
                            = m2ap_reserved_mcch_related_bcch_info_p->sf_alloc_info;
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.signalling_mcs
                            = m2ap_reserved_mcch_related_bcch_info_p->signalling_mcs;
                        /* reserved cell changes start */
                        p_mac_config_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].area_type
                            = RRC_TWO;
                        /* SPR 18672 Fix start */
                        p_mac_config_cell_req->
                            rrc_m2ap_mac_create_embms_area_entity_info.num_of_area++;
                        p_mac_config_cell_req->
                            rrc_m2ap_mac_create_embms_area_entity_info.num_create_area_info_counter++;
                        /* SPR 18672 Fix end */
                    }

                }

            }

        }
        /* SPR 18672 Fix End */
        if ((p_cfg_cell_param->llim_cell_param.cell_config_param.mac_config.bitmask &
                    RRC_RRM_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG))
        {
            p_mac_config_cell_req->optional_elems_present |=
                RRC_M2AP_CONFIG_EMBMS_CONFIG_INFO_PRESENT;
            p_mac_config_cell_req->rrc_m2ap_mac_embms_params_info.optional_elems_present
                |= RRC_M2AP_MBMS_TRANSMISSION_POWER_INFO_PRESENT;
            p_mac_config_cell_req->rrc_m2ap_mac_embms_params_info.
                m2ap_mac_embms_transmission_power_info.embms_transmission_power
                = p_cfg_cell_param->llim_cell_param.cell_config_param.
                mac_config.mbms_transmission_power;
        }

#endif
        rrc_mac_il_send_rrc_mac_config_cell_req(
            p_mac_config_cell_req,
            RRC_LLIM_MODULE_ID,
            RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_cfg_cell_param->cell_index),
		p_cfg_cell_param->transaction_id,
            	p_cfg_cell_param->cell_index);

        if (PNULL != p_mac_config_cell_req)
        {
            /* Free Memory allocated to p_mac_config_cell_req */
            rrc_mem_free(p_mac_config_cell_req);
        }

    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cemh_build_and_send_mac_cell_start_req
 *   INPUT        : rrc_transaction_id_t                 transaction_id
 *                  rrc_cell_index_t                     cell_index
 *   OUTPUT       : none
 *   DESCRIPTION  : This function builds 'rrc_mac_cell_start_req_t'
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_build_and_send_mac_cell_start_req(
        rrc_transaction_id_t                 transaction_id,
        rrc_cell_index_t                     cell_index)
{


    RRC_LLIM_UT_TRACE_ENTER();

    /* spr 10203 fix start */
    U16 msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_LLIM_TRACE(RRC_INFO,"API:RRC->MAC:RRC_MAC_CELL_START_REQ:CellIndex:%u",
		    cell_index);

    msg_api_length = RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        /* Not enough memory */
        return;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, RRC_LLIM_MODULE_ID,
        RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID, cell_index), 
     	RRC_MAC_CELL_START_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, RRC_LLIM_MODULE_ID,
        RRC_MAC_MODULE_ID , RRC_MAC_CELL_START_REQ, 0,cell_index);

    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;

    RRC_ASSERT((p_mac_msg - msg_api_length) == p_msg);

    /* Send message to */
    rrc_send_message(p_msg,
	RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID, cell_index));
    /* spr 10203 fix stop */

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_build_and_send_mac_cell_stop_req
 *   INPUT        : rrc_transaction_id_t                 transaction_id
 *                  rrc_cell_index_t                     cell_index
 *   OUTPUT       : none
 *   DESCRIPTION  : This function builds 'rrc_mac_cell_stop_req_t'
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static rrc_return_et cemh_build_and_send_mac_cell_stop_req(
        rrc_transaction_id_t                 transaction_id,
        rrc_cell_index_t                     cell_index)
{
    rrc_return_et   result = RRC_SUCCESS;
    RRC_LLIM_UT_TRACE_ENTER();

    /* spr 10203 fix start */
    U16 msg_api_length;
    U8 *p_msg, *p_mac_msg;

    RRC_LLIM_TRACE(RRC_INFO,"API:RRC->MAC:RRC_MAC_CELL_STOP_REQ:CellIndex:%u",
		    cell_index);
    msg_api_length = RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, RRC_VERSION_ID, RRC_LLIM_MODULE_ID,
        RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID, cell_index), 
	RRC_MAC_CELL_STOP_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, transaction_id, RRC_LLIM_MODULE_ID,
        RRC_MAC_MODULE_ID, RRC_MAC_CELL_STOP_REQ, 0, cell_index);

    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;

    RRC_ASSERT((p_mac_msg - msg_api_length) == p_msg);

    /* Send message to */
    rrc_send_message(p_msg,
	RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID, cell_index));
    /* spr 10203 fix stop */

    RRC_LLIM_UT_TRACE_EXIT();
    return result;
}

#ifdef LTE_EMBMS_SUPPORTED
/******************************************************************************
 *   FUNCTION NAME: get_m2ap_mcch_related_bcch_info
 *   INPUT        : rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param
 *                  area_id
 *   OUTPUT       : none
 *   DESCRIPTION  : This function returns mcch_related_bcch_info in sib13
 *
 *   RETURNS      : mcch_related_bcch_info_p
 *
 ******************************************************************************/
m2ap_mcch_related_bcch_info_t* get_m2ap_mcch_related_bcch_info(
        sib_type_13_info_t *sib_type_13_info_p,
        U8     area_id)
{
    U8    num_valid_mbsfn_area_info;
    U8    count;

    num_valid_mbsfn_area_info = sib_type_13_info_p->num_valid_mbsfn_area_info;

    for(count = 0; count < num_valid_mbsfn_area_info; count++)
    {
        if(sib_type_13_info_p->
                m2ap_mbsfn_area_info_list[count].area_id
                == area_id)
        {
            return
                &(sib_type_13_info_p->
                        m2ap_mbsfn_area_info_list[count]);\
        }
    }
    return PNULL;
}
/* SPR 18672 Fix Start */
/******************************************************************************
 *   FUNCTION NAME: get_m2ap_reserved_mcch_related_bcch_info
 *   INPUT        : rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param
 *                  area_id
 *   OUTPUT       : none
 *   DESCRIPTION  : This function returns mcch_related_bcch_info in sib13
 *
 *   RETURNS      : m2ap_reserved_mcch_related_bcch_info_p
 *
 ******************************************************************************/
m2ap_reserved_mcch_related_bcch_info_t* get_m2ap_reserved_mcch_related_bcch_info(
        m2ap_reserved_mcch_related_bcch_info_list_t *m2ap_reserved_mcch_related_bcch_info_list_p,
        U8     area_id)
{
    U8    num_valid_mbsfn_area_info;
    U8    count;

    num_valid_mbsfn_area_info = m2ap_reserved_mcch_related_bcch_info_list_p->count;

    for(count = 0; count < num_valid_mbsfn_area_info; count++)
    {
        if(m2ap_reserved_mcch_related_bcch_info_list_p->
                m2ap_reserved_cell_mcch_info[count].area_id
                == area_id)
        {
            return
                &(m2ap_reserved_mcch_related_bcch_info_list_p->
                        m2ap_reserved_cell_mcch_info[count]);\
        }
    }
    return PNULL;
}
/* SPR 18672 Fix End */

#endif


/******************************************************************************
 *   FUNCTION NAME: cemh_build_rrc_mac_reconfig_cell_req
 *   INPUT        : rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param
 *                  rrc_llim_cell_ctx_t* p_cell_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function builds 'rrc_mac_reconfig_cell_req_t'
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_build_send_rrc_mac_reconfig_cell_req(
        rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param,
        rrc_llim_cell_ctx_t* p_cell_ctx)
{
    rrc_mac_reconfig_cell_req_t *p_mac_reconfig_cell_req = PNULL;
    U16 mib_msg_count = 0;
#ifdef LTE_EMBMS_SUPPORTED
    m2ap_notification_config_t      *m2ap_notification_config_p = PNULL;
    m2ap_mcch_related_bcch_info_t   *mcch_related_bcch_info_p  = PNULL;
    U8                               index;
    mbms_updated_area_info_t        *mbms_updated_area_info_p   = PNULL;
    U8                               num_of_area                = RRC_ZERO;
    mbsfn_subframe_config_t         *mbsfn_subframe_config_p = PNULL;
    U8                               count;
    /* SPR 18672 Fix Start */
    m2ap_reserved_mcch_related_bcch_info_t *m2ap_reserved_mcch_related_bcch_info_p = PNULL;
    /* SPR 18672 Fix End */
    /*SPR 18809 fix start*/
    U8                               temp_count;
    /*SPR 18809 fix end*/
#endif

    RRC_LLIM_UT_TRACE_ENTER();
    p_mac_reconfig_cell_req = (rrc_mac_reconfig_cell_req_t*)rrc_mem_get(
            sizeof (rrc_mac_reconfig_cell_req_t));

    if(PNULL != p_mac_reconfig_cell_req)
    {

        memset_wrapper(p_mac_reconfig_cell_req, 0, sizeof (rrc_mac_reconfig_cell_req_t));

        /*Filling the SFN & SF */
        p_mac_reconfig_cell_req->sf = RRC_NULL;
        p_mac_reconfig_cell_req->sfn = (rrc_sfn_t)(p_recfg_cell_param->sfn);

        if (CELL_RECONFIG_REQ_NON_BROADCAST_INFO_PRESENCE_FLAG &
                p_recfg_cell_param->cell_reconfig.presence_bitmask)
        {
            if (CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG &
                p_recfg_cell_param->cell_reconfig.non_broadcast_info.presence_bitmask)
            {
                if (RRC_RRM_RECONFIG_DCI_FORMAT_FOR_SI_MESSAGES_PRESENT &
                        p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                        mac_reconfig_params.bitmask)
                {
                    p_mac_reconfig_cell_req->optional_elems_present |=
                        RRC_RECONFIG_MAC_DCI_FORMAT_FOR_SI_PRESENT;
                    p_mac_reconfig_cell_req->dci_format_for_si.dci_format_for_si_msgs =
                        p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                        mac_reconfig_params.dci_format_for_si_msgs;
                }

                if (RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1A_PRESENT &
                        p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                        mac_reconfig_params.bitmask)
                {
                    p_mac_reconfig_cell_req->optional_elems_present |=
                        RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1A_PRESENT;
                    l3_memcpy_wrapper(&p_mac_reconfig_cell_req->num_rbs_per_tbs_dci_1a.num_rb_buffer,
                           &p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                            mac_reconfig_params.num_rbs_per_tbs_dci_1a,
                           RBS_FOR_DCI_1A_SIZE);
                }

                if (RRC_RRM_RECONFIG_RBS_PER_TBS_DCI_1C_PRESENT &
                        p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                        mac_reconfig_params.bitmask)
                {
                    p_mac_reconfig_cell_req->optional_elems_present |=
                        RRC_RECONFIG_MAC_RBS_PER_TBS_DCI_1C_PRESENT;
                    l3_memcpy_wrapper(&p_mac_reconfig_cell_req->num_rbs_per_tbs_dci_1c.num_rb_buffer,
                           &p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                            mac_reconfig_params.num_rbs_per_tbs_dci_1c,
                           RBS_FOR_DCI_1C_SIZE);
                }

                if (RRC_RRM_RECONFIG_UL_SYNC_LOSS_TIMER_PRESENT &
                        p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                        mac_reconfig_params.bitmask)
                {
                    p_mac_reconfig_cell_req->optional_elems_present |=
                        RRC_RECONFIG_MAC_UL_SYNC_LOSS_TIMER_INFO_PRESENT;

                    p_mac_reconfig_cell_req->ul_sync_loss_timer_info.
                    ul_sync_loss_timer_value = 
                         fill_ul_sync_loss_timer_value (p_recfg_cell_param->
                            cell_reconfig.non_broadcast_info.
                            mac_reconfig_params.ul_sync_loss_timer);
                }

                if (RRC_RRM_RECONFIG_PUCCH_CQI_SINR_THRESHOLD_PRESENT &
                        p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                        mac_reconfig_params.bitmask)
                {
                    p_mac_reconfig_cell_req->optional_elems_present |=
                        RRC_RECONFIG_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO_PRESENT;

                    p_mac_reconfig_cell_req->pucch_cqi_sinr_threshold_info.
                     sinr_value = 
                           p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                            mac_reconfig_params.pucch_cqi_sinr_value;
                }
            }
        }

        if (p_recfg_cell_param->bitmask & RRC_CSC_MIB_MSG_REQ_PRESENT)
        {
            p_mac_reconfig_cell_req->optional_elems_present |=
                RRC_RECONFIG_MAC_MIB_MSG_REQ_PRESENT;

            p_mac_reconfig_cell_req->mib_msg_req_counter = 
                p_recfg_cell_param->mib_msg_req_counter;
            for (mib_msg_count = 0; mib_msg_count < p_recfg_cell_param->
                    mib_msg_req_counter; ++mib_msg_count)
            {
                p_mac_reconfig_cell_req->mib_msg_req[mib_msg_count] =
                    p_recfg_cell_param->mib_msg_req[mib_msg_count];
            }
        }
        if (p_recfg_cell_param->bitmask & RRC_CSC_SIB_TYPE_1_MSG_PRESENT)
        {
            p_mac_reconfig_cell_req->optional_elems_present |=
                RRC_RECONFIG_MAC_SIB_TYPE_1_MSG_PRESENT;
            p_mac_reconfig_cell_req->sib1_msg_req = p_recfg_cell_param->sib1_msg_req;

        }
        if (p_recfg_cell_param->bitmask & RRC_CSC_SI_MSG_REQ_PRESENT)
        {
            p_mac_reconfig_cell_req->optional_elems_present |=
                RRC_RECONFIG_MAC_SI_MSG_REQ_PRESENT;
            cemh_fill_si_msg_req (&p_mac_reconfig_cell_req->si_msg_req,
                              &p_recfg_cell_param->si_msg_req);
        }
        /* SPR 2751: IEs missing in MAC_RECONFIG_CELL_REQ API sent from L3.*/
        if (p_recfg_cell_param->bitmask & RRC_CSC_MOD_PERIOD_PRESENT)
        {
            p_mac_reconfig_cell_req->optional_elems_present |= 
                RRC_RECONFIG_MAC_MOD_PERIOD_PRESENT;
            p_mac_reconfig_cell_req->mod_period_info.mod_period = 
                p_recfg_cell_param->mod_period;
        }
        if (p_recfg_cell_param->bitmask & RRC_CSC_SFN_GAP_PRESENT)
        {
            p_mac_reconfig_cell_req->optional_elems_present |= 
                RRC_RECONFIG_MAC_SFN_GAP_PRESENT;
            p_mac_reconfig_cell_req->sfn_gap_info.sfn_gap = p_recfg_cell_param->sfn_gap;
        }

        /*SPR_19066_START*/
        /* Code Removed */
        /*SPR_19066_END*/

        /* CLPC Start */
        
        /* Fill PDSCH_CONFIG_COMMON_INFO & UPLINK_POWER_CONTROL_COMMON if present */
        if ((CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG &
                p_recfg_cell_param->cell_reconfig.presence_bitmask) &&
                (CELL_RECONFIG_REQ_SIB_2_INFO_PRESENCE_FLAG &
                p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask))
        {
            p_mac_reconfig_cell_req->optional_elems_present |=
                RRC_RECONFIG_MAC_PDSCH_CONFIG_COMMON_INFO_PRESENT;

            p_mac_reconfig_cell_req->pdsch_config_common_info.reference_signal_power =
                p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_2_Info.
                radio_resource_config_common_sib.pdsch_config_common.ref_signal_power;

                
            p_mac_reconfig_cell_req->pdsch_config_common_info.p_b =
                p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_2_Info.
                radio_resource_config_common_sib.pdsch_config_common.pb;

            p_mac_reconfig_cell_req->optional_elems_present |=
                RRC_RECONFIG_MAC_UPLINK_POWER_CONTROL_COMMON_PRESENT;

            p_mac_reconfig_cell_req->uplink_power_control_common_t.p0_nominal_pusch =
                p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_2_Info.
                radio_resource_config_common_sib.uplink_power_control_common.p_zero_nominal_Pusch;
           
            p_mac_reconfig_cell_req->uplink_power_control_common_t.alpha =
                p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_2_Info.
                radio_resource_config_common_sib.uplink_power_control_common.alpha;
            /* Fill PUSCH_CONFIG_COMMON_INFO */
            p_mac_reconfig_cell_req->optional_elems_present |=
                RRC_RECONFIG_MAC_PUSCH_CONFIG_COMMON_INFO_PRESENT;

            p_mac_reconfig_cell_req->pusch_config_common_info.enable_64_qam =
                p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_2_Info.
                radio_resource_config_common_sib.pusch_config_common.pusch_config_basic.
                enable_64_qam;
#ifdef LTE_EMBMS_SUPPORTED
            if( p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_2_Info.presence_bitmask &
                    SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG)
            {
                p_mac_reconfig_cell_req->optional_elems_present |= 
                    RRC_RECONFIG_EMBMS_SIB2_INFO_PRESENT ;
                for(index = RRC_ZERO; index < p_recfg_cell_param->cell_reconfig.broadcast_info.
                        sib_type_2_Info.mbsfn_subframe_config_list.count; index++)
                {
                    mbsfn_subframe_config_p = &(p_recfg_cell_param->cell_reconfig.broadcast_info.
                            sib_type_2_Info.mbsfn_subframe_config_list.mbsfn_subframe_config[index]);
                    count = p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.no_of_csa_pattern_counter;
                    p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[
                        count].radio_frame_allocation_period = mbsfn_subframe_config_p->radio_frame_alloc_period;
                    p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[
                        count].radio_frame_allocation_offset = mbsfn_subframe_config_p->radio_frame_alloc_offset;
                    if(mbsfn_subframe_config_p->subframe_allocation.presence_bitmask & SUBFRAME_ALLOCATION_ONE_FRAME)
                    {
                        p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[
                            count].choice = 0;
                        p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[count].subframe_allocation = 
                            mbsfn_subframe_config_p->subframe_allocation.one_frame[0];
                        /*SPR 18809 fix start*/
                        p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[count].subframe_allocation <<= 24;
                    }
                    else
                    {
                        p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.csa_pattern_list[
                            count].choice = 1;
                        for(temp_count = 0 ; temp_count < 3 ; temp_count++)
                        {
                            p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.
                                csa_pattern_list[count].subframe_allocation <<= 8;
                            p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.
                                csa_pattern_list[count].subframe_allocation |=
                                mbsfn_subframe_config_p->subframe_allocation.four_frames[temp_count];
                        }
                        p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.
                            csa_pattern_list[count].subframe_allocation <<= 8;

                    }
                    p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.no_of_csa_pattern_counter++;
                }
                /*SPR 18809 fix end*/
            }
            /* SIB_2_DISABLE Start*/
            if(p_recfg_cell_param->cell_reconfig.broadcast_info.is_mbsfn_disable_sib2)
            {
                /* send SIB2 TLV with no CSA PATTERN list */
                p_mac_reconfig_cell_req->optional_elems_present |= 
                    RRC_RECONFIG_EMBMS_SIB2_INFO_PRESENT ;
                p_mac_reconfig_cell_req->rrc_m2ap_embms_sib2_info.no_of_csa_pattern_counter = 0;
            }
            /* SIB_2_DISABLE End */
#endif
        }

        /* Fill DOWNLINK_POWER_CONTROL_COMMON if present */
        if (CELL_RECONFIG_REQ_MAC_DOWNLINK_POWER_CONTROL_PRESENCE_FLAG &
                p_recfg_cell_param->cell_reconfig.presence_bitmask)
        {
            p_mac_reconfig_cell_req->optional_elems_present |=
                RRC_RECONFIG_MAC_DOWNLINK_POWER_CONTROL_COMMON_PRESENT;

            p_mac_reconfig_cell_req->downlink_power_control_common =
                p_recfg_cell_param->cell_reconfig.downlink_power_control_common;
        }

        /* CLPC Stop */
        /* Fill Contention Free Rach Timer Info Start */
        if (p_recfg_cell_param->cell_reconfig.presence_bitmask &
            CELL_RECONFIG_REQ_CONTENTION_FREE_RACH_TIMER_PRESENCE_FLAG)
        {
            /* Set bit in optional parameter */
            p_mac_reconfig_cell_req->optional_elems_present |=
                RRC_RECONFIG_MAC_CONTN_FREE_RACH_TIMER_INFO_PRESENT;

            p_mac_reconfig_cell_req->contn_free_rach_timer_info.contn_free_rach_timer
                = p_recfg_cell_param->cell_reconfig.contention_free_rach_timer;        
        }
        /* Fill Contention Free Rach Timer Info Stop */

        if (RRC_RRM_RECONFIG_N_GAP_PRESENT & 
                        p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                        mac_reconfig_params.bitmask)
        {
            p_mac_reconfig_cell_req->optional_elems_present |=
                RRC_RECONFIG_MAC_N_GAP_PRESENT;

            p_mac_reconfig_cell_req->n_gap.n_gap=
                   p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                   mac_reconfig_params.n_gap;
        }

        /* SPS Start */
        if ((p_recfg_cell_param->cell_reconfig.presence_bitmask & 
                CELL_RECONFIG_REQ_NON_BROADCAST_INFO_PRESENCE_FLAG) &&
                (p_recfg_cell_param->cell_reconfig.non_broadcast_info.presence_bitmask & 
                 CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG) &&
                (p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.bitmask & 
                 RRC_RRM_RECONFIG_SPS_CRNTI_RANGE_PRESENT))
        {

            p_mac_reconfig_cell_req->optional_elems_present |=
                RRC_RECONFIG_MAC_SPS_CRNTI_RANGE_PRESENT;

            p_mac_reconfig_cell_req->sps_crnti_range.start_sps_crnti
                = p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.
                    sps_crnti_range.start_sps_crnti;        

            p_mac_reconfig_cell_req->sps_crnti_range.end_sps_crnti
                = p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.
                    sps_crnti_range.end_sps_crnti;
        }
	/* PRS + */
    /* SPR 7657 start */
	if ((p_recfg_cell_param->cell_reconfig.presence_bitmask & 
                CELL_RECONFIG_REQ_NON_BROADCAST_INFO_PRESENCE_FLAG) &&
                (p_recfg_cell_param->cell_reconfig.non_broadcast_info.presence_bitmask & 
                 CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG) &&
                 (p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.bitmask &
				RRC_RRM_RECONFIG_PRS_INFO_PRESENT))
	{
		p_mac_reconfig_cell_req->optional_elems_present |= RRC_RECONFIG_MAC_PRS_INFO_PRESENT;

		p_mac_reconfig_cell_req->prs_reconfig_info.request_type = 
			p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.request_type;

		if(p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.request_type == 1)
		{
			if ((p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.bitmask &
						RRC_RRM_RECONFIG_PRS_CONFIG_PRESENT))
			{

				/* PRS_UPDATED + */
				if((p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.
							prs_reconfig.bitmask & RRC_RRM_RECONFIG_PRS_BANDWIDTH_INFO_PRESENT))
                                {
                                        p_mac_reconfig_cell_req->prs_reconfig_info.optional_elems_present |= 
							RRC_MAC_CONFIG_PRS_BANDWIDTH_INFO_PRESENT;
                                        /* Store mapped value of prs_bandwidth recieved from rrm */
                                        p_mac_reconfig_cell_req->prs_reconfig_info.prs_bandwidth_info.prs_bandwidth =
                                                fill_prs_bandwidth_in_rb(p_recfg_cell_param->cell_reconfig.non_broadcast_info.
						mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_bandwidth_info.prs_bandwidth);
                                }	
				/* PRS_UPDATED - */
				if((p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.prs_reconfig.bitmask & 
							RRC_RRM_RECONFIG_PRS_SUBFRAMES_INFO_PRESENT))
				{
					p_mac_reconfig_cell_req->prs_reconfig_info.optional_elems_present |= RRC_MAC_CONFIG_PRS_SUBFRAMES_INFO_PRESENT;
					/* Store mapped value of prs_subframes recieved from rrm */
					p_mac_reconfig_cell_req->prs_reconfig_info.prs_subframes_info.prs_subframes = 
						return_prs_subframes(p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.
                                                prs_reconfig_info.prs_reconfig.prs_subframes_info.prs_subframes);
				}


				if((p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.prs_reconfig.bitmask & 
							RRC_RRM_RECONFIG_PRS_CONFIG_INDEX_INFO_PRESENT))
				{

					p_mac_reconfig_cell_req->prs_reconfig_info.optional_elems_present |= 
						RRC_MAC_CONFIG_PRS_CONFIG_INDEX_INFO_PRESENT;
					p_mac_reconfig_cell_req->prs_reconfig_info.prs_config_index_info.prs_config_index =
						p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.
						prs_reconfig_info.prs_reconfig.prs_config_index_info.prs_config_index;

				}

				if((p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.prs_reconfig.bitmask &
							RRC_RRM_RECONFIG_PRS_TRANSMISSION_POWER_INFO_PRESENT))
				{

					p_mac_reconfig_cell_req->prs_reconfig_info.optional_elems_present |= 
						RRC_MAC_CONFIG_PRS_TRANSMISSION_POWER_INFO_PRESENT;
					p_mac_reconfig_cell_req->prs_reconfig_info.prs_transmission_power_info.prs_transmission_power  =
						p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.
						prs_reconfig_info.prs_reconfig.prs_transmission_power_info.prs_transmission_power ;
				}

				if((p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.prs_reconfig.bitmask &
							RRC_RRM_RECONFIG_PRS_MUTING_CONFIG_INFO_PRESENT))
				{
					p_mac_reconfig_cell_req->prs_reconfig_info.optional_elems_present |=
						RRC_MAC_CONFIG_PRS_MUTING_CONFIG_INFO_PRESENT;
					/* Store mapped value of muting_bits recieved from rrm */
					p_mac_reconfig_cell_req->prs_reconfig_info.prs_muting_config_info.bits_to_be_read  = 
						return_prs_muting_bits(p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                                                mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_muting_config_info.bits_to_be_read);

					p_mac_reconfig_cell_req->prs_reconfig_info.prs_muting_config_info.prs_muting_config_index  =
						p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.
						prs_reconfig.prs_muting_config_info.prs_muting_config_index;
				}
        /* SPR 7657 end */
			}
		}
	}
        /* PRS - */
        /* SPS Stop */
#ifdef LTE_EMBMS_SUPPORTED
        if((p_recfg_cell_param->cell_reconfig.presence_bitmask &
                    CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG) && 
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask
                 & CELL_RECONFIG_REQ_SIB_13_INFO_PRESENCE_FLAG))
        {
            m2ap_notification_config_p = 
                &(p_recfg_cell_param->cell_reconfig.broadcast_info.
                        sib_type_13_info.m2ap_notification_config);
            for(index = RRC_ZERO; index <
                    p_recfg_cell_param->cell_reconfig.broadcast_info.
                    mbms_updated_area_info_list.num_valid_mbsfn_area_info;
                    index++)
            {
                mbms_updated_area_info_p =
                    p_recfg_cell_param->cell_reconfig.broadcast_info.
                    mbms_updated_area_info_list.mbms_updated_area_info + index;
                mcch_related_bcch_info_p = get_m2ap_mcch_related_bcch_info(
                        &(p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_13_info),
                        mbms_updated_area_info_p->area_id);
                if(((U8)ADDED == mbms_updated_area_info_p->flag) || ((U8)MODIFIED ==
                            mbms_updated_area_info_p->flag) || ((U8)UNCHANGE ==
                                mbms_updated_area_info_p->flag))
                {
                    num_of_area =
                        p_mac_reconfig_cell_req->
                        rrc_m2ap_mac_create_embms_area_entity_info.num_of_area;
                    if(!(p_mac_reconfig_cell_req->optional_elems_present &
                                RRC_M2AP_MBMS_AREA_CREATE_PRESENT))
                    {
                        p_mac_reconfig_cell_req->optional_elems_present |= 
                            RRC_M2AP_MBMS_AREA_CREATE_PRESENT;
                    }
                    p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                        m2ap_mbms_create_area_info[num_of_area].area_id =
                        mbms_updated_area_info_p->area_id;
                    p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                        m2ap_mbms_create_area_info[num_of_area].area_index =
                        mbms_updated_area_info_p->area_index;
                    if((U8)ADDED == mbms_updated_area_info_p->flag)
                    {
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            NEW_AREA;
                        /* Since new area has been added, send Reconfig to RLC
                         * also */
                        /*Change the mode in llim cell context */
                        p_cell_ctx->recfg_mode = RECONFIG_MAC_RLC;
                    }
                    else if((U8)MODIFIED == mbms_updated_area_info_p->flag)
                    {
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            CHANGE_AREA;
                    }
                    else
                    {
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            NO_CHANGE;
                    }
                    /* Coverity 94535 Fix Start */
                    if(mcch_related_bcch_info_p != PNULL)
                    {
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].notification_ind
                            = mcch_related_bcch_info_p->notification_indicator_r9;
                        /* SPR 18300 Fix Start */    
                        if(RRC_ZERO == mcch_related_bcch_info_p->pdcch_length)
                        {
                            p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_ONE;
                        }
                        else if(RRC_ONE == mcch_related_bcch_info_p->pdcch_length)
                        {
                            p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_TWO;
                        }
                        else
                        {
                            RRC_LLIM_TRACE(RRC_ERROR," Incorrect pdcch length recevied");
                            p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_ZERO;
                        }
                        /* SPR 18300 Fix End */    

                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].optional_elems_present
                            |= RRC_M2AP_MAC_MCCH_CONFIG_PRESENT;

                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_rep_period
                            = mcch_related_bcch_info_p->mcch_repetition_period;
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_offset
                            = mcch_related_bcch_info_p->mcch_offset;
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_mod_period
                            = mcch_related_bcch_info_p->mcch_modification_period;
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.sf_alloc_info
                            = mcch_related_bcch_info_p->sf_alloc_info;
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.signalling_mcs
                            = mcch_related_bcch_info_p->signalling_mcs;
                        /* SPR 18672 Fix Start */    
                        /* reserved cell changes start */
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].area_type
                            = RRC_ONE;
                        p_mac_reconfig_cell_req->
                            rrc_m2ap_mac_create_embms_area_entity_info.num_of_area++;
                        p_mac_reconfig_cell_req->
                            rrc_m2ap_mac_create_embms_area_entity_info.num_create_area_info_counter++;
                        /* SPR 18672 Fix End */   
                    }
                    /* Coverity 94535 Fix End */
                }
                else if((U8)DELETED == mbms_updated_area_info_p->flag)
                {
                    num_of_area =
                        p_mac_reconfig_cell_req->
                        rrc_m2ap_mac_delete_embms_area_entity_info.num_area_delete_count;
                    if(!(p_mac_reconfig_cell_req->optional_elems_present &
                                RRC_M2AP_MBMS_AREA_DELETE_PRESENT))
                    {
                        p_mac_reconfig_cell_req->optional_elems_present |= 
                            RRC_M2AP_MBMS_AREA_DELETE_PRESENT;
                    }
                    p_mac_reconfig_cell_req->rrc_m2ap_mac_delete_embms_area_entity_info.
                        area_index[num_of_area] =
                        mbms_updated_area_info_p->area_index;
                    p_mac_reconfig_cell_req->
                        rrc_m2ap_mac_delete_embms_area_entity_info.num_area_delete_count++;
                    /* Since new area has been deleted, send Reconfig to RLC
                     * also */
                    /*Change the mode in llim cell context */
                    p_cell_ctx->recfg_mode = RECONFIG_MAC_RLC;
                }
            }
            if(p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.num_of_area)
            {
                /* fill notification config */
                p_mac_reconfig_cell_req->
                    rrc_m2ap_mac_create_embms_area_entity_info.optional_elems_present
                    |= RRC_M2AP_MAC_NOTIFICATION_CONFIG_PRESENT;
                if (m2ap_notification_config_p->notification_repetition_coeff == 0)
                {
                    p_mac_reconfig_cell_req->
                        rrc_m2ap_mac_create_embms_area_entity_info.
                        m2ap_embms_notification_config.notification_rept_coeff = 2;
                }
                else if (m2ap_notification_config_p->notification_repetition_coeff == 1)
                {
                    p_mac_reconfig_cell_req->
                        rrc_m2ap_mac_create_embms_area_entity_info.
                        m2ap_embms_notification_config.notification_rept_coeff = 4;
                }
                else
                {
                    p_mac_reconfig_cell_req->
                        rrc_m2ap_mac_create_embms_area_entity_info.
                        m2ap_embms_notification_config.notification_rept_coeff = 0;
                }
                p_mac_reconfig_cell_req->
                    rrc_m2ap_mac_create_embms_area_entity_info.
                    m2ap_embms_notification_config.notification_offset =
                    m2ap_notification_config_p->notification_offset;
                p_mac_reconfig_cell_req->
                    rrc_m2ap_mac_create_embms_area_entity_info.
                    m2ap_embms_notification_config.notification_sf_index =
                    m2ap_notification_config_p->notification_sf_Index;
                /* SPR 18089:start */
                if(!(p_mac_reconfig_cell_req->optional_elems_present &
                            RRC_M2AP_MBMS_AREA_CREATE_PRESENT))
                {
                    p_mac_reconfig_cell_req->optional_elems_present |= 
                        RRC_M2AP_MBMS_AREA_CREATE_PRESENT;
                }
                /* fill sib13 scheduling info */
                p_mac_reconfig_cell_req->optional_elems_present|=
                    RRC_RECONFIG_SIB13_INFO_PRESENT;
                /* SPR 18439 Fix Start */    
                p_mac_reconfig_cell_req->rrc_m2ap_mac_sib13_info.si_index =
                    p_recfg_cell_param->cell_reconfig.broadcast_info.
                    sib13_scheduling_info.si_index;
                /* SPR 18439 Fix End */    
            }
        }
        /* SPR 18093 Fix Start */
        else if((p_recfg_cell_param->cell_reconfig.presence_bitmask &
                    CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG) && 
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask
                 & CELL_RECONFIG_REQ_MCCH_UPDATED_AREA_INFO_LIST))
        {
            RRC_LLIM_TRACE(RRC_BRIEF,
                    "SIB 13 info not sent by RRM: only updated area"
                    "info is sent for delete area");
            for(index = RRC_ZERO; index <
                    p_recfg_cell_param->cell_reconfig.broadcast_info.
                    mbms_updated_area_info_list.num_valid_mbsfn_area_info;
                    index++)
            {
                mbms_updated_area_info_p =
                    p_recfg_cell_param->cell_reconfig.broadcast_info.
                    mbms_updated_area_info_list.mbms_updated_area_info + index;
                if((U8)DELETED == mbms_updated_area_info_p->flag)
                {
                    num_of_area =
                        p_mac_reconfig_cell_req->
                        rrc_m2ap_mac_delete_embms_area_entity_info.num_area_delete_count;
                    if(!(p_mac_reconfig_cell_req->optional_elems_present &
                                RRC_M2AP_MBMS_AREA_DELETE_PRESENT))
                    {
                        p_mac_reconfig_cell_req->optional_elems_present |= 
                            RRC_M2AP_MBMS_AREA_DELETE_PRESENT;
                    }
                    p_mac_reconfig_cell_req->rrc_m2ap_mac_delete_embms_area_entity_info.
                        area_index[num_of_area] =
                        mbms_updated_area_info_p->area_index;
                    p_mac_reconfig_cell_req->
                        rrc_m2ap_mac_delete_embms_area_entity_info.num_area_delete_count++;
                    /* Since new area has been deleted, send Reconfig to RLC
                     * also */
                    /*Change the mode in llim cell context */
                    p_cell_ctx->recfg_mode = RECONFIG_MAC_RLC;
                }
            }
        }
        /* SPR 18672 Fix Start */
        if((p_recfg_cell_param->cell_reconfig.presence_bitmask &
                    CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG) && (p_recfg_cell_param->
                        cell_reconfig.broadcast_info.presence_bitmask &
                        CELL_RECONFIG_REQ_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG))
            /* SPR 18089 Fix End */
        {
            /* fill reserved cell info */
            for(index = RRC_ZERO; index <
                    p_recfg_cell_param->cell_reconfig.broadcast_info.
                    mbms_updated_area_info_list.num_valid_mbsfn_area_info;
                    index++)
            {
                mbms_updated_area_info_p =
                    p_recfg_cell_param->cell_reconfig.broadcast_info.
                    mbms_updated_area_info_list.mbms_updated_area_info + index;
                m2ap_reserved_mcch_related_bcch_info_p = get_m2ap_reserved_mcch_related_bcch_info(
                        &(p_recfg_cell_param->cell_reconfig.broadcast_info.m2ap_reserved_mcch_related_bcch_info_list),
                        mbms_updated_area_info_p->area_id);

                if(((U8)ADDED == mbms_updated_area_info_p->flag) || ((U8)MODIFIED ==
                            mbms_updated_area_info_p->flag) || ((U8)UNCHANGE ==
                                mbms_updated_area_info_p->flag))
                {
                    num_of_area =
                        p_mac_reconfig_cell_req->
                        rrc_m2ap_mac_create_embms_area_entity_info.num_of_area;
                    if(!(p_mac_reconfig_cell_req->optional_elems_present &
                                RRC_M2AP_MBMS_AREA_CREATE_PRESENT))
                    {
                        p_mac_reconfig_cell_req->optional_elems_present |= 
                            RRC_M2AP_MBMS_AREA_CREATE_PRESENT;
                    }
                    p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                        m2ap_mbms_create_area_info[num_of_area].area_id =
                        mbms_updated_area_info_p->area_id;
                    p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                        m2ap_mbms_create_area_info[num_of_area].area_index =
                        mbms_updated_area_info_p->area_index;
                    if((U8)ADDED == mbms_updated_area_info_p->flag)
                    {
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            NEW_AREA;
                    }
                    else if((U8)MODIFIED == mbms_updated_area_info_p->flag)
                    {
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            CHANGE_AREA;
                    }
                    else
                    {
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].request_type =
                            NO_CHANGE;
                    }
                    /* Coverity 94535 Fix Start */
                    if(m2ap_reserved_mcch_related_bcch_info_p != PNULL)
                    {
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].notification_ind
                            = m2ap_reserved_mcch_related_bcch_info_p->notification_indicator_r9;
                        /* SPR 18300 Fix Start */    
                        if(RRC_ZERO == m2ap_reserved_mcch_related_bcch_info_p->pdcch_length)
                        {
                            p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_ONE;
                        }
                        else if(RRC_ONE == m2ap_reserved_mcch_related_bcch_info_p->pdcch_length)
                        {
                            p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_TWO;
                        }
                        else
                        {
                            RRC_LLIM_TRACE(RRC_ERROR," Incorrect pdcch length recevied");
                            p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                                m2ap_mbms_create_area_info[num_of_area].non_mbsfn_region_len
                                = RRC_ZERO;
                        }
                        /* SPR 18300 Fix End */    

                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].optional_elems_present
                            |= RRC_M2AP_MAC_MCCH_CONFIG_PRESENT;

                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_rep_period
                            = m2ap_reserved_mcch_related_bcch_info_p->mcch_repetition_period;
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_offset
                            = m2ap_reserved_mcch_related_bcch_info_p->mcch_offset;
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.mcch_mod_period
                            = m2ap_reserved_mcch_related_bcch_info_p->mcch_modification_period;
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.sf_alloc_info
                            = m2ap_reserved_mcch_related_bcch_info_p->sf_alloc_info;
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].mcch_config.signalling_mcs
                            = m2ap_reserved_mcch_related_bcch_info_p->signalling_mcs;
                        /* reserved cell changes start */
                        p_mac_reconfig_cell_req->rrc_m2ap_mac_create_embms_area_entity_info.
                            m2ap_mbms_create_area_info[num_of_area].area_type
                            = RRC_TWO;
                        /* SPR 18672 Fix start */
                        p_mac_reconfig_cell_req->
                            rrc_m2ap_mac_create_embms_area_entity_info.num_of_area++;
                        p_mac_reconfig_cell_req->
                            rrc_m2ap_mac_create_embms_area_entity_info.num_create_area_info_counter++;
                        /* SPR 18672 Fix end */
                    }
                    /* Coverity 94535 Fix End */
                }
            }
        }
        /* SPR 18672 Fix End */
        /* SPR 18093 Fix End */
        if(p_recfg_cell_param->cell_reconfig.presence_bitmask &
                CELL_RECONFIG_REQ_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG)
        {
            p_mac_reconfig_cell_req->optional_elems_present |=
                RRC_M2AP_EMBMS_CONFIG_INFO_PRESENT;
            p_mac_reconfig_cell_req->rrc_m2ap_mac_embms_params_info.optional_elems_present
                |= RRC_M2AP_MBMS_TRANSMISSION_POWER_INFO_PRESENT;
            p_mac_reconfig_cell_req->rrc_m2ap_mac_embms_params_info.
                m2ap_mac_embms_transmission_power_info.embms_transmission_power
                = p_recfg_cell_param->cell_reconfig.mbms_transmission_power;
        }
#endif

        RRC_LLIM_TRACE(RRC_INFO,"API:RRC->MAC:RRC_MAC_RECONFIG_CELL_REQ:CellIndex:%u",
                         p_recfg_cell_param->cell_index);
        /* Send MAC RECONFIG */
        rrc_mac_il_send_rrc_mac_reconfig_cell_req(
            p_mac_reconfig_cell_req,
            RRC_LLIM_MODULE_ID,
            RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_recfg_cell_param->cell_index),
		p_recfg_cell_param->transaction_id,
            p_recfg_cell_param->cell_index);

        CEMH_FSM_SET_STATE(p_cell_ctx, 
                CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF);


        /* Free Memory allocated to p_mac_reconfig_cell_req */
        if (p_mac_reconfig_cell_req != PNULL)
        { 
            rrc_mem_free(p_mac_reconfig_cell_req);
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_build_and_send_rrc_mac_reconfig_cell_req_for_etws
 *   INPUT        : rrc_csc_llim_pws_req_t* p_wrw_req
 *   OUTPUT       : none
 *   DESCRIPTION  : This function builds 'rrc_mac_reconfig_cell_req_t'
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_build_and_send_rrc_mac_reconfig_cell_req_for_etws(
    rrc_csc_llim_pws_req_t* p_wrw_req)
{
    rrc_mac_reconfig_cell_req_t *p_mac_reconfig_cell_req = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();
    p_mac_reconfig_cell_req = (rrc_mac_reconfig_cell_req_t*)rrc_mem_get(
            sizeof (rrc_mac_reconfig_cell_req_t));

    if(PNULL != p_mac_reconfig_cell_req)
    {

        memset_wrapper(p_mac_reconfig_cell_req, 0, sizeof (rrc_mac_reconfig_cell_req_t));

        /*Filling the SFN */
        p_mac_reconfig_cell_req->sfn = (rrc_sfn_t)(p_wrw_req->sfn);

        p_mac_reconfig_cell_req->optional_elems_present |=
            RRC_RECONFIG_MAC_SIB_TYPE_1_MSG_PRESENT;
        p_mac_reconfig_cell_req->sib1_msg_req = p_wrw_req->sib1_msg_req;

        p_mac_reconfig_cell_req->optional_elems_present |=
            RRC_RECONFIG_MAC_SI_MSG_REQ_PRESENT;
        cemh_fill_si_msg_req (&p_mac_reconfig_cell_req->si_msg_req,
                              &p_wrw_req->si_msg_req);

        /* Send MAC RECONFIG */
        rrc_mac_il_send_rrc_mac_reconfig_cell_req(
            p_mac_reconfig_cell_req,
            RRC_LLIM_MODULE_ID,
            	RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_wrw_req->cell_index),
		p_wrw_req->transaction_id,
            p_wrw_req->cell_index);


        /* Free Memory allocated to p_mac_reconfig_cell_req */
        if (p_mac_reconfig_cell_req != PNULL)
        { 
            rrc_mem_free(p_mac_reconfig_cell_req);
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_stop_procedure_timer
 *   INPUT        : rrc_llim_cell_ctx_t* p_cell_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function stops procedure timer
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_stop_procedure_timer(
        rrc_llim_cell_ctx_t* p_cell_ctx)
{
    RRC_LLIM_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cell_ctx);

    /* SPR 19273 Fix Start */
    SET_CELL_INDEX(p_cell_ctx->cell_index);
    /* SPR 19273 Fix End */

    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stop timer [LLIM_TIMER]");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;

    }
    else
    {
        RRC_LLIM_TRACE(RRC_WARNING, "No LLIM Timer Running");
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_start_procedure_timer
 *   INPUT        : rrc_llim_cell_ctx_t*    p_cell_ctx
 *                  rrc_timer_duration_t    timer_duration
 *   OUTPUT       : none
 *   DESCRIPTION  : This function starts procedure timer
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_start_procedure_timer(
        rrc_llim_cell_ctx_t*    p_cell_ctx,
        rrc_timer_duration_t    timer_duration)
{
    rrc_cemh_timer_buf_t guard_timer_data;

    RRC_LLIM_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(0 < timer_duration);

    guard_timer_data.auto_delete = RRC_TRUE;
    guard_timer_data.target_submodule = LLIM_SUBMODULE_CEMH;
    guard_timer_data.cell_index = p_cell_ctx->cell_index;
    guard_timer_data.cemh_event = CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED;

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
    RRC_LLIM_TRACE(RRC_BRIEF,
            "Start timer [LLIM_CELL_SETUP_TIMER] %u ms.",
            timer_duration);

    p_cell_ctx->guard_timer = rrc_start_timer(
            timer_duration,
            &guard_timer_data,
            sizeof (guard_timer_data),
            RRC_FALSE);

    if (PNULL == p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_FATAL,
                "[LLIM_CELL_SETUP_TIMER] Starting error");
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cemh_start_cell_start_procedure_timer
 *   INPUT        : rrc_llim_cell_ctx_t*    p_cell_ctx
 *                  rrc_timer_duration_t    timer_duration
 *   OUTPUT       : none
 *   DESCRIPTION  : This function starts procedure timer for the cell start
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_start_cell_start_procedure_timer(
        rrc_llim_cell_ctx_t*    p_cell_ctx,
        rrc_timer_duration_t    timer_duration)
{
    rrc_cemh_timer_buf_t guard_timer_data;

    RRC_LLIM_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(0 < timer_duration);

    guard_timer_data.auto_delete = RRC_TRUE;
    guard_timer_data.target_submodule = LLIM_SUBMODULE_CEMH;
    guard_timer_data.cell_index = p_cell_ctx->cell_index;
    guard_timer_data.cemh_event = CEMH_EVENT_CELL_START_TIMER_EXPIRED;

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
    RRC_LLIM_TRACE(RRC_BRIEF,
            "Start timer [LLIM_CELL_START_TIMER] %u ms.",
            timer_duration);

    p_cell_ctx->guard_timer = rrc_start_timer(
            timer_duration,
            &guard_timer_data,
            sizeof (guard_timer_data),
            RRC_FALSE);

    if (PNULL == p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_FATAL,
                "[LLIM_CELL_START_TIMER] Starting error");
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cemh_start_cell_stop_procedure_timer
 *
 *   INPUT        : rrc_llim_cell_ctx_t*    p_cell_ctx
 *                  rrc_timer_duration_t    timer_duration
 *   OUTPUT       : none
 *   DESCRIPTION  : This function starts procedure timer for the cell stop
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_start_cell_stop_procedure_timer(
        rrc_llim_cell_ctx_t*    p_cell_ctx,
        rrc_timer_duration_t    timer_duration)
{
    rrc_cemh_timer_buf_t guard_timer_data;

    RRC_LLIM_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(0 < timer_duration);

    guard_timer_data.auto_delete = RRC_TRUE;
    guard_timer_data.target_submodule = LLIM_SUBMODULE_CEMH;
    guard_timer_data.cell_index = p_cell_ctx->cell_index;
    guard_timer_data.cemh_event = CEMH_EVENT_CELL_STOP_TIMER_EXPIRED;
        /*NBIOT TIMER FIX */

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
    RRC_LLIM_TRACE(RRC_BRIEF,
            "Start timer [LLIM_CELL_STOP_TIMER] %u ms.",
            timer_duration);

    p_cell_ctx->guard_timer = rrc_start_timer(
            timer_duration,
            &guard_timer_data,
            sizeof (guard_timer_data),
            RRC_FALSE);
        /*NBIOT TIMER FIX */
    /*NBIOT TIMER FIX */

    if (PNULL == p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_FATAL,
                "[LLIM_CELL_STOP_TIMER] Starting error");
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cemh_start_cell_delete_procedure_timer
 *   INPUT        : rrc_llim_cell_ctx_t*    p_cell_ctx
 *                  rrc_timer_duration_t    timer_duration
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function starts procedure timer for the cell delete
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_start_cell_delete_procedure_timer(
        rrc_llim_cell_ctx_t*    p_cell_ctx,
        rrc_timer_duration_t    timer_duration)
{
    rrc_cemh_timer_buf_t guard_timer_data;

    RRC_LLIM_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(0 < timer_duration);

    guard_timer_data.auto_delete = RRC_TRUE;
    guard_timer_data.target_submodule = LLIM_SUBMODULE_CEMH;
    guard_timer_data.cell_index = p_cell_ctx->cell_index;
    guard_timer_data.cemh_event = CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED;
/*NBIOT TIMER FIX */
        /*NBIOT TIMER FIX */
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
    RRC_LLIM_TRACE(RRC_BRIEF,
            "Start timer [LLIM_CELL_DELETE_TIMER] %u ms.",
            timer_duration);

    p_cell_ctx->guard_timer = rrc_start_timer(
            timer_duration,
            &guard_timer_data,
            sizeof (guard_timer_data),
            RRC_FALSE);
        /*NBIOT TIMER FIX */
    /*NBIOT TIMER FIX */

    if (PNULL == p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_FATAL,
                "[LLIM_CELL_DELETE_TIMER] Starting error");
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cemh_start_reconfig_procedure_timer
 *   INPUT        : rrc_llim_cell_ctx_t*    p_cell_ctx
 *                  rrc_timer_duration_t    timer_duration
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function starts procedure timer
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_start_reconfig_procedure_timer(
        rrc_llim_cell_ctx_t*    p_cell_ctx,
        rrc_timer_duration_t    timer_duration)
{
    rrc_cemh_timer_buf_t guard_timer_data;

    RRC_LLIM_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(0 < timer_duration);

    guard_timer_data.auto_delete = RRC_TRUE;
    guard_timer_data.target_submodule = LLIM_SUBMODULE_CEMH;
    guard_timer_data.cell_index = p_cell_ctx->cell_index;
    guard_timer_data.cemh_event = CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED;

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */
    RRC_LLIM_TRACE(RRC_BRIEF,
            "Start timer [LLIM_CELL_RECONFIG_TIMER] %u ms.",
            timer_duration);

    p_cell_ctx->guard_timer = rrc_start_timer(
            timer_duration,
            &guard_timer_data,
            sizeof (guard_timer_data),
            RRC_FALSE);

    if (PNULL == p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_FATAL,
                "[LLIM_CELL_RECONFIG_TIMER] Starting error");
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/*
 *   CEMH states
 */
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_del_cell
 *   INPUT        : cemh_fsm_state_et       to_be_del_layers
 *                  rrc_cell_index_t        cell_index
 *                  rrc_transaction_id_t    transaction_id
 *                  rrc_llim_gl_ctx_t       *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function delete cell on lower layers based on current state.
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static rrc_return_et cemh_fsm_del_cell(
        /* mark layers that should be deleted, usually current_state */
        cemh_fsm_state_et       to_be_del_layers,
        rrc_cell_index_t        cell_index,         /* identify cell */
        rrc_transaction_id_t    transaction_id,     /* identify transaction */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t       *p_rrc_llim_gl_ctx)
{
    rrc_return_et   result = RRC_SUCCESS;
    RRC_LLIM_UT_TRACE_ENTER();
/* SPR 20636 Changes Start*/
    RRC_LLIM_TRACE(RRC_DETAILED,
            "Unused variable in fsm llim_gl_ctx=%x",(U32_64_Bit) p_rrc_llim_gl_ctx);      

/* SPR 20636 Changes End*/

    switch (to_be_del_layers)
    {
        case CEMH_STATE_CELL_IN_SERVICE:
        case CEMH_STATE_CELL_OUT_OF_SERVICE:
        case CEMH_STATE_W_FOR_MAC_SFN_CNF:
        case CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF:
        case CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF:
        case CEMH_STATE_W_FOR_PHY_RECONFIG_CELL_CNF:
        case CEMH_STATE_W_FOR_PHY_CELL_START_CNF:
        case CEMH_STATE_W_FOR_MAC_CELL_START_CNF:
        case CEMH_STATE_W_FOR_PHY_CELL_STOP_CNF:
        case CEMH_STATE_W_FOR_MAC_CELL_STOP_CNF:
        case CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF:
            {

    /* spr 10203 fix start */
    U8 *p_msg, *p_mac_msg;
    U16 msg_api_length;

    RRC_LLIM_TRACE(RRC_INFO,"API:RRC->MAC:RRC_MAC_DELETE_CELL_REQ:CellIndex:%u",
		    cell_index);

    msg_api_length = RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;
    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        /* Not enough memory */
        RRC_LLIM_TRACE(RRC_WARNING, "Unable to send Cell Delete to MAC");
        /* coverity_fix_62461_start */
        RRC_LLIM_UT_TRACE_EXIT();
        return RRC_FAILURE;
        /* coverity_fix_62461_stop */
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_mac_msg = p_msg;


    /* Fill CSPL header */
    rrc_construct_api_header(p_mac_msg, 
                             RRC_VERSION_ID, 
                             RRC_LLIM_MODULE_ID, 
                             	RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID, cell_index), 
			   				RRC_MAC_DELETE_CELL_REQ, 
                             msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_mac_msg, 
                                       transaction_id, 
                                       RRC_LLIM_MODULE_ID,
                                       RRC_MAC_MODULE_ID, 
                                       RRC_MAC_DELETE_CELL_REQ,
                                       0,
                                       cell_index);

    p_mac_msg = p_mac_msg + RRC_INTERFACE_API_HEADER_SIZE;
    RRC_ASSERT((p_mac_msg - msg_api_length) == p_msg);

    /* Send message to */
    rrc_send_message(p_msg,
	RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID, cell_index));
    /* spr 10203 fix stop */

                break;
            }

        case CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF:
        case CEMH_STATE_W_FOR_MAC_CELL_DELETE_CNF:
            {
                RRC_LLIM_TRACE(RRC_WARNING, "Incorrect State for Cell Context deletion.");
            }
            break;

        default:
            RRC_ASSERT(0);
            RRC_LLIM_TRACE(RRC_WARNING, "cemh_fsm_del_cell: unexpected state!");
            result = RRC_FAILURE;
            break;
    }
    

    RRC_LLIM_UT_TRACE_EXIT();
    return result;
}


/******************************************************************************
 *   FUNCTION NAME: get_fail_response_config_te
 *   INPUT        : cemh_rrc_current_proc_et            current_proc
 *   OUTPUT       : none
 *   DESCRIPTION  : This function determine response on cell config timer expired
 *       based on current state
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static rrc_llim_csc_response_et get_fail_response_config_te(
        cemh_rrc_current_proc_et            current_proc)
{
    switch (current_proc)
    {
        case CEMH_CELL_SETUP_PHY_FAILED:
            return RRC_LLIM_CONFIG_CELL_TE_PHY;

        case CEMH_CELL_SETUP_MAC_FAILED:
            return RRC_LLIM_CONFIG_CELL_TE_MAC;

        case CEMH_CELL_SETUP_MAC_SFN_FAILED:
            return RRC_LLIM_SFN_TE;
			
        default:
            return RRC_LLIM_CSC_FAILURE;
            break;
    }
}

/*
 *   CEMH_STATE_IDLE state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_idle_state_config_cell_req
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function start cell configuration.
 *       state CEMH_STATE_IDLE; event CEMH_EVENT_CSC_CONFIG_CELL_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_idle_state_config_cell_req(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_csc_llim_config_cell_req_t  *p_config_cell_req =
        (rrc_csc_llim_config_cell_req_t *)p_msg;
   	 rrc_phy_config_cell_req_t*    p_phy_config_cell_req = PNULL;
	rrc_csc_llim_config_cell_req_t  *p_cfg_cell_param = PNULL;


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_config_cell_req);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(p_config_cell_req->cell_index == p_cell_ctx->cell_index);

    RRC_LLIM_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p", p_api);      

    /* Memory Optimization */
    p_cell_ctx->p_cfg_cell_param = (rrc_csc_llim_config_cell_req_t*)
        rrc_mem_get(sizeof(rrc_csc_llim_config_cell_req_t));
    
    if ( PNULL == p_cell_ctx->p_cfg_cell_param )
    {
        RRC_LLIM_TRACE(RRC_FATAL,"Unable to Allocate Memory");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* The malloc MUST not fail.*/
    if(PNULL == p_cell_ctx->p_cfg_cell_param)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return;
    }

    memset_wrapper(p_cell_ctx->p_cfg_cell_param, 0,
            sizeof(rrc_csc_llim_config_cell_req_t));

    p_cfg_cell_param = p_cell_ctx->p_cfg_cell_param;
    /* Special case on init operation */
    *p_cfg_cell_param = *p_config_cell_req;
    /* SPR 23608/SES-43 Fix +- */

    /* 1. start guard timer */
    cemh_start_procedure_timer(
            p_cell_ctx,
            p_rrc_llim_gl_ctx->cell_cfg_duration);





    /* 2. send RRC_PHY_CONFIG_CELL_REQ */
    p_phy_config_cell_req = (rrc_phy_config_cell_req_t *)rrc_mem_get(
            sizeof(rrc_phy_config_cell_req_t));

    if ( PNULL == p_phy_config_cell_req )
    {
        RRC_LLIM_TRACE(RRC_FATAL,"Unable to Allocate Memory");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_cell_ctx->transaction_id = p_cfg_cell_param->transaction_id;

    if(p_phy_config_cell_req!= PNULL)
    {
        /* MEMSET P_PHY_CONFIG_CELL_REQ*/
        memset_wrapper(p_phy_config_cell_req,0,sizeof(rrc_phy_config_cell_req_t));

        p_phy_config_cell_req->cell_index = p_cell_ctx->cell_index;

        if (p_config_cell_req->llim_cell_param.sib_type_1_Info.
                presence_bitmask & SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG)
        {
            /******* TDD changes start *******/

            p_phy_config_cell_req->cell_parameters.duplexing_mode = DUPLEXING_MODE_TDD;

            /* CSR#53377 Fix Start */
            RRC_LLIM_TRACE(RRC_INFO,"TDD mode sent to PHY during Cell Setup");
            /* CSR#53377 Fix Stop */

            p_phy_config_cell_req->cell_parameters.bitmask |=
                RRC_PHY_TDD_CONFIG_PRESENT;

            p_phy_config_cell_req->cell_parameters.tdd_config.
                sub_frame_assignment =
                p_cfg_cell_param->llim_cell_param.sib_type_1_Info.tdd_config.
                sub_frame_assignment;
            RRC_LLIM_TRACE(RRC_INFO,"TDD sub_frame_assignment [%u]",
                    p_phy_config_cell_req->cell_parameters.tdd_config.sub_frame_assignment);

            p_phy_config_cell_req->cell_parameters.tdd_config.
                special_sub_frame_pattern =
                p_cfg_cell_param->llim_cell_param.sib_type_1_Info.tdd_config.
                special_sub_frame_pattern;

            if (p_config_cell_req->llim_cell_param.sib_type_2_Info.freq_info.
                    presence_bitmask & FREQ_INFO_UL_BW_PRESENCE_FLAG)
            {
                RRC_LLIM_TRACE(RRC_INFO,"UL BANDWIDTH must be absent in case of TDD");
            }
            p_phy_config_cell_req->cell_parameters.ul_tx_bandwidth =
                p_config_cell_req->llim_cell_param.mib_info.dl_band_width;  

            if (p_config_cell_req->llim_cell_param.sib_type_2_Info.freq_info.
                    presence_bitmask & FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG)
            {
                RRC_LLIM_TRACE(RRC_INFO,"UL Carrier Freq must be absent in case of TDD");
            }
            p_phy_config_cell_req->cell_parameters.ul_earfcn = 
                (U16) (p_config_cell_req->llim_cell_param.cell_config_param.
                       cell_parameters.dl_earfcn);   
        }
        else
        {                 
        /* HD-FDD start */
            if(RRC_RRM_DUPLEXING_MODE_PRESENT & p_config_cell_req->llim_cell_param.
                cell_config_param.mac_config.bitmask)
            {
                p_phy_config_cell_req->cell_parameters.duplexing_mode = p_config_cell_req->
                    llim_cell_param.cell_config_param.mac_config.duplexing_mode; 
                
                if(1 == p_config_cell_req->llim_cell_param.cell_config_param.mac_config.
                        duplexing_mode)
                {   
                         RRC_LLIM_TRACE(RRC_INFO,"FDD mode sent to PHY during Cell Setup");
                }   
                else
                {       
                        RRC_LLIM_TRACE(RRC_INFO,"HD-FDD mode sent to PHY during Cell Setup");
                }
            }
            else
            {
                p_phy_config_cell_req->cell_parameters.duplexing_mode = DUPLEXING_MODE_FDD;
                RRC_LLIM_TRACE(RRC_INFO,"FDD mode sent to PHY during Cell Setup");
            }
        
        /* HD-FDD stop */

            if (p_config_cell_req->llim_cell_param.sib_type_2_Info.freq_info.
                    presence_bitmask & FREQ_INFO_UL_BW_PRESENCE_FLAG)
            {
                p_phy_config_cell_req->cell_parameters.ul_tx_bandwidth = 
                    p_config_cell_req->llim_cell_param.sib_type_2_Info.
                    freq_info.ul_bandwidth;
            }
            else
            {
                p_phy_config_cell_req->cell_parameters.ul_tx_bandwidth =
                    p_config_cell_req->llim_cell_param.mib_info.dl_band_width;  
            }

            if (p_config_cell_req->llim_cell_param.sib_type_2_Info.freq_info.
                    presence_bitmask & FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG)
            {
                p_phy_config_cell_req->cell_parameters.ul_earfcn = 
                    p_config_cell_req->llim_cell_param.sib_type_2_Info.
                    freq_info.ul_carrier_freq;
            }
            else
            {
                p_phy_config_cell_req->cell_parameters.ul_earfcn = 
                    (U16) (p_config_cell_req->llim_cell_param.cell_config_param.
                           cell_parameters.dl_earfcn + UL_DL_ARFCN_DIFFRENCE);   
	        }


        }
        /******* TDD changes stop *******/

        p_phy_config_cell_req->cell_parameters.freq_band_indicator = 
            p_config_cell_req->llim_cell_param.sib_type_1_Info.
            freq_band_indicator;
			
        p_phy_config_cell_req->cell_parameters.dl_earfcn = 
            p_config_cell_req->llim_cell_param.cell_config_param.
            cell_parameters.dl_earfcn;

        p_phy_config_cell_req->cell_parameters.num_of_antennas = 
            p_config_cell_req->llim_cell_param.cell_config_param.
            cell_parameters.num_of_antennas;


        p_phy_config_cell_req->cell_parameters.dl_tx_bandwidth = 
            p_config_cell_req->llim_cell_param.mib_info.dl_band_width;

        p_phy_config_cell_req->cell_parameters.subcarrier_spacing = 
            p_config_cell_req->llim_cell_param.cell_config_param.
            cell_parameters.subcarrier_spacing;

        p_phy_config_cell_req->cell_parameters.ul_cyclic_prefix = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.ul_cyc_prefix_len;

        p_phy_config_cell_req->cell_parameters.dl_cyclic_prefix = 
            p_config_cell_req->llim_cell_param.cell_config_param.
            cell_parameters.dl_cyclic_prefix; 

        if (p_config_cell_req->llim_cell_param.sib_type_2_Info.
                radio_resource_config_common_sib.sounding_rs_ul_config_common.
                presence_bitmask & SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG)
        {
            p_phy_config_cell_req->cell_parameters.srs_bandwidth_configuration = 
                p_config_cell_req->llim_cell_param.sib_type_2_Info.
                radio_resource_config_common_sib.sounding_rs_ul_config_common;
        }
        p_phy_config_cell_req->cell_parameters.phys_cell_id = 
            p_config_cell_req->llim_cell_param.cell_config_param.
            cell_parameters.phys_cell_id;

        /* CLPC Start */
        p_phy_config_cell_req->cell_parameters.pcfich_power_offset =
            p_config_cell_req->llim_cell_param.cell_config_param.mac_config.
            downlink_power_control_common.pcfichPowerOffset;

        p_phy_config_cell_req->cell_parameters.phich_power_offset =
            p_config_cell_req->llim_cell_param.cell_config_param.mac_config.
            downlink_power_control_common.phichPowerOffset;

        /* CLPC Stop */

        /* SPR 3802 Start */
        p_phy_config_cell_req->cell_parameters.max_rs_epre =
            p_config_cell_req->llim_cell_param.cell_config_param.cell_parameters.max_rs_epre;
        /* SPR 3802 Stop */
        /* PRS + */
        /* SPR 8212 Fix Start*/
        if ((p_config_cell_req->llim_cell_param.cell_config_param.mac_config.prs_config_info.bitmask &
                    RRC_RRM_CONFIG_PRS_CONFIG_PRESENT))
        {
            p_phy_config_cell_req->cell_parameters.bitmask |= RRC_PHY_PRS_CONFIG_INFO_PRESENT;
            /* Store mapped value of prs_bandwidth recieved from rrm in mac parameters*/
            p_phy_config_cell_req->cell_parameters.prs_config.prs_bandwidth = 
                fill_prs_bandwidth_in_rb(p_config_cell_req->llim_cell_param.cell_config_param.mac_config.
                        prs_config_info.prs_config.prs_bandwidth_info.prs_bandwidth);
            /* fill cyclic prefix type recieved from RRM */
            p_phy_config_cell_req->cell_parameters.prs_config.prs_cyclic_prefix_type =
                p_config_cell_req->llim_cell_param.cell_config_param.cell_parameters.dl_cyclic_prefix; 

        }


        /* SPR 8212 Fix Stop*/
        p_phy_config_cell_req->sync_signals = 
            p_config_cell_req->llim_cell_param.cell_config_param.sync_signals;

        p_phy_config_cell_req->prach_configuration.prach_config_sequence = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.prach_config_sib.
            prach_config_info.prach_config_index;

        p_phy_config_cell_req->prach_configuration.root_seq_index = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.prach_config_sib.root_seq_index;

        p_phy_config_cell_req->prach_configuration.prach_freq_offset = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.prach_config_sib.
            prach_config_info.prach_freq_offset;

        p_phy_config_cell_req->prach_configuration.zero_cor_zone_config = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.prach_config_sib.
            prach_config_info.zero_cor_zone_config;

        p_phy_config_cell_req->prach_configuration.high_speed_flag = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.prach_config_sib.
            prach_config_info.high_speed_flag;  

        p_phy_config_cell_req->pusch_configuration.ul_ref_signal =
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.pusch_config_common.
            ul_ref_signals_pusch;

        p_phy_config_cell_req->pusch_configuration.num_of_sub_bands = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.pusch_config_common.
            pusch_config_basic.nsb;

        p_phy_config_cell_req->pusch_configuration.pusch_hopping_mode = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.pusch_config_common.
            pusch_config_basic.hopping_mode;

        p_phy_config_cell_req->pusch_configuration.pusch_hopping_offset = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.pusch_config_common.
            pusch_config_basic.pusch_hopping_offset;

        /* Convert delta_pucch_shift to value */
        p_phy_config_cell_req->pucch_configuration.delta_pucch_shift = 
            cemh_e2v_delta_pucch_shift(p_config_cell_req->llim_cell_param.
                    sib_type_2_Info.radio_resource_config_common_sib.
                    pucch_config_common.delta_pucch_shift);

        p_phy_config_cell_req->pucch_configuration.nrb_cqi = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.pucch_config_common.n_rb_cqi;

        p_phy_config_cell_req->pucch_configuration.ncs_an = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.pucch_config_common.n_cs_an;

        p_phy_config_cell_req->pucch_configuration.n1pucch_an = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.pucch_config_common.n_one_pucch_an;

        p_phy_config_cell_req->phich_configuration.phich_resource = 
            p_config_cell_req->llim_cell_param.mib_info.
            phich_config.phich_resource;

        p_phy_config_cell_req->phich_configuration.phich_duration = 
            p_config_cell_req->llim_cell_param.mib_info.
            phich_config.phich_duration;

        p_phy_config_cell_req->pdsch_configuration = 
            p_config_cell_req->llim_cell_param.sib_type_2_Info.
            radio_resource_config_common_sib.pdsch_config_common;

        /* Fill the explicit start required IE to start the Cell at PHY. */
        if (p_config_cell_req->llim_cell_setup_req.presence_bitmask &
                CELL_SETUP_REQ_API_EXPLICIT_START_REQUIRED_FLAG)
        {
 p_phy_config_cell_req->bitmask |= RRC_PHY_EXPLICIT_START_REQUIRED_PRESENT;
           
            p_phy_config_cell_req->explicit_start_required =
                p_config_cell_req->llim_cell_setup_req.explicit_start_required;
        }

        
        rrc_phy_il_send_rrc_phy_config_cell_req(
                p_phy_config_cell_req,
                RRC_LLIM_MODULE_ID,
                RRC_GET_LL_MODULE_ID(RRC_PHY_1_MODULE_ID,p_phy_config_cell_req->cell_index),
                p_cfg_cell_param->transaction_id,
                p_cfg_cell_param->cell_index);
        CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF);

        if (PNULL != p_phy_config_cell_req)
        {
            /* Free memory allocated to p_phy_config_cell_req */
            rrc_mem_free(p_phy_config_cell_req);
        }
    }
	RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_xxx_conf_cell_state_timer_expired
 *   INPUT        : rrc_timer_t         timer_id
 *                  void                *p_timer_buf
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function stop cell configuration and delete cell.
 *                  state any of:
 *                  CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF,
 *                  CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF,
 *                  CEMH_STATE_W_FOR_MAC_SFN_CNF
 *                  event CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED
 *
 *   RETURNS       : None
 *
 ******************************************************************************/
static void cemh_fsm_w_xxx_conf_cell_state_timer_expired(
        rrc_timer_t         timer_id,           /* timer_id - unused */
        void                *p_timer_buf,       /* p_cemh_timer_buf - unused */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{

    rrc_phy_delete_cell_req_t       phy_delete_cell_req;
    rrc_csc_llim_config_cell_req_t  *p_cfg_cell_param   = PNULL; 

    RRC_LLIM_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_LLIM_TRACE(RRC_BRIEF, "Timer [LLIM_CELL_SETUP_TIMER] expired");

    RRC_LLIM_TRACE(RRC_DETAILED, "Unused variable in fsm timer_id=%p"
            " and p_timer_buf=%p", timer_id, p_timer_buf);      

    p_cfg_cell_param = p_cell_ctx->p_cfg_cell_param;

    /* Timer expired and is not valid any more */
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */

    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx != PNULL) &&
            (p_cell_ctx->p_cfg_cell_param != PNULL))
    {
        p_cfg_cell_param = 
            p_cell_ctx->p_cfg_cell_param;

        memset_wrapper(&phy_delete_cell_req,0,sizeof(rrc_phy_delete_cell_req_t));

        /* Delete on PHY level: send
         * RRC_PHY_DELETE_CELL_REQ */
        phy_delete_cell_req.cell_index = p_cell_ctx->cell_index;

        /* A new Guard timer is to be started here for Cell Deletion */
        cemh_start_cell_delete_procedure_timer(
                p_cell_ctx,
                p_rrc_llim_gl_ctx->cell_delete_duration);

        if (RRC_SUCCESS != rrc_phy_il_send_rrc_phy_delete_cell_req(
                    &phy_delete_cell_req,
                    RRC_LLIM_MODULE_ID,
                    RRC_GET_LL_MODULE_ID(RRC_PHY_1_MODULE_ID,phy_delete_cell_req.cell_index),
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index))
        {
            RRC_LLIM_TRACE(RRC_WARNING, "Unable to send Cell Delete to PHY");
        }

        if ((CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx)) ==
                CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF)
        {
            p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cfg_cell_param->cell_index]->
                is_mac_cell_delete_reqd = RRC_FALSE;
            p_cell_ctx->current_proc = CEMH_CELL_SETUP_PHY_FAILED;

        }
        else if((CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx)) ==
                CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF)
        {
            p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cfg_cell_param->cell_index]->
                is_mac_cell_delete_reqd = RRC_TRUE;
            p_cell_ctx->current_proc = CEMH_CELL_SETUP_MAC_FAILED;

        }
        else 
        {
            p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cfg_cell_param->cell_index]->
                is_mac_cell_delete_reqd = RRC_TRUE;
            p_cell_ctx->current_proc = CEMH_CELL_SETUP_MAC_SFN_FAILED;
        }
        /* 3. Change state */
         CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF);

    }
    else
    {
        RRC_LLIM_TRACE(RRC_WARNING, "p_cell_ctx->p_cfg_cell_param is NULL OR"
                "p_rrc_llim_gl_ctx->p_p_cell_ctx is NULL");
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_phy_delete_cell_cnf
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function waits for the cell delete cnf from the phy.
 *                  state:
 *                  CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF,
 *                  event CEMH_EVENT_PHY_DELETE_CELL_CNF
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_phy_delete_cell_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{

    rrc_csc_llim_delete_cell_resp_t delete_cell_resp;

    rrc_csc_llim_config_cell_resp_t config_cell_resp ;
   
    rrc_return_et   result = RRC_SUCCESS;
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    RRC_LLIM_TRACE(RRC_DETAILED, 
            "Unused variable in fsm p_api=%p and p_msg=%p", p_api, p_msg);  
    if (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "p_rrc_llim_gl_ctx->p_p_cell_ctx is NULL!");
    }
    else if (RRC_TRUE == p_rrc_llim_gl_ctx->
            p_p_cell_ctx[p_cell_ctx->cell_index]->is_mac_cell_delete_reqd)
    {
        result = cemh_fsm_del_cell(
                CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx),
                p_cell_ctx->cell_index,
                p_cell_ctx->transaction_id,
                p_rrc_llim_gl_ctx);

        if (RRC_SUCCESS == result)
        {
            /* Change state */
            CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_W_FOR_MAC_CELL_DELETE_CNF);
            /* Reset the FLAG in the global Context*/
            if(PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index])
            {
                p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index]->
                    is_mac_cell_delete_reqd = RRC_FALSE;
            }
        }
    }
    else
    {
        switch(p_cell_ctx->current_proc)
        {
            case  CEMH_CELL_SETUP_PHY_FAILED:
            case  CEMH_CELL_SETUP_MAC_FAILED:    
            case  CEMH_CELL_SETUP_MAC_SFN_FAILED: 

                /*  Stop guard timer */
                cemh_stop_procedure_timer(p_cell_ctx);
                
                memset_wrapper(&config_cell_resp, 0, 
                        sizeof (config_cell_resp));
                config_cell_resp.response =  
                    get_fail_response_config_te(p_cell_ctx->current_proc);
                
                /* Send CSC_LLIM_CELL_SETUP_RESP*/
                config_cell_resp.transaction_id = p_cell_ctx->transaction_id;
                config_cell_resp.cell_index     = p_cell_ctx->cell_index;
                rrc_llim_send_internal_msg(
                        RRC_CSC_MODULE_ID,
                        CSC_LLIM_CONFIG_CELL_RESP,
                        sizeof (config_cell_resp),
                        &config_cell_resp);

                break;

            case  CEMH_CELL_DELETE_ONGOING :  

                /*  Stop guard timer */
                cemh_stop_procedure_timer(p_cell_ctx);
                /* Confirm API is received from PHY, and no message is awaited from MAC
                 * so send success to CSC */
                delete_cell_resp.response = RRC_SUCCESS;
                /* Send CSC_LLIM_DELETE_CELL_RESP */ 
                delete_cell_resp.transaction_id = p_cell_ctx->transaction_id;
                delete_cell_resp.cell_index     = p_cell_ctx->cell_index;
                rrc_llim_send_internal_msg( 
                        RRC_CSC_MODULE_ID, 
                        CSC_LLIM_DELETE_CELL_RESP,
                        sizeof (delete_cell_resp), 
                        &delete_cell_resp);

                break;

            default:
                RRC_LLIM_TRACE(RRC_WARNING, "cemh_fsm_phy_delete_cell_cnf: unexpected procedure!");
                break;
        }

        /* Delete from Cell DB and free */
	/* Coverity Fix 83403 Start */
        p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index]= PNULL;
        rrc_mem_free(p_cell_ctx);
	/* Coverity Fix 83403 Stop */
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_delete_cell_req
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function stop cell configuration and delete cell.
 *                  state any of:
 *                  CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF,
 *                  CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF,
 *                  CEMH_STATE_W_FOR_MAC_SFN_CNF,
 *                  event CEMH_EVENT_CSC_DELETE_CELL_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_delete_cell_req(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_csc_llim_delete_cell_req_t  *p_delete_cell_req =
        (rrc_csc_llim_delete_cell_req_t *)p_msg;
    rrc_csc_llim_delete_cell_resp_t delete_cell_resp;
    rrc_phy_delete_cell_req_t phy_delete_cell_req;

    /* SPR 18440 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
    rrc_rlc_delete_mbms_area_entity_req_t *rrc_rlc_delete_mbms_area_entity_req_p = PNULL;
    mbms_updated_area_info_t              *mbms_updated_area_info_p   = PNULL;
    U8                                     index;
    U8                                     num_of_area;
    U8                                     rlc_area_delete_sent = RRC_FALSE;
    m2ap_reserved_mcch_related_bcch_info_t *m2ap_reserved_mcch_related_bcch_info_p = PNULL;
#endif
    /* SPR 18440 Fix End */


    rrc_return_et   result = RRC_SUCCESS;
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_delete_cell_req);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx);
    RRC_ASSERT(p_delete_cell_req->cell_index == p_cell_ctx->cell_index);
    RRC_LLIM_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p", p_api);      
    /* 1. Stop guard timer */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        cemh_stop_procedure_timer(p_cell_ctx);
    }

    /* SPR 13746 Fix Start */
    p_cell_ctx->current_proc = CEMH_CELL_DELETE_ONGOING; 
    /* SPR 13746 Fix Stop */
    /* SPR 18354 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
    p_cell_ctx->mbms_updated_area_info_list = p_delete_cell_req->mbms_updated_area_info_list; 
#endif
    /* SPR 18354 Fix End */

    /* A new Guard timer is to be started here for Cell Deletion */
    cemh_start_cell_delete_procedure_timer(
            p_cell_ctx,
            p_rrc_llim_gl_ctx->cell_delete_duration);

    /* SPR 18440 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
    if(CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF != CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx))
    {
        if(p_cell_ctx->mbms_updated_area_info_list.num_valid_mbsfn_area_info)
        {
            p_cell_ctx->transaction_id = p_delete_cell_req->transaction_id;
            rrc_rlc_delete_mbms_area_entity_req_p = 
                (rrc_rlc_delete_mbms_area_entity_req_t*)rrc_mem_get(
                        sizeof(rrc_rlc_delete_mbms_area_entity_req_t));

            if(PNULL != rrc_rlc_delete_mbms_area_entity_req_p)
            {
                memset_wrapper(rrc_rlc_delete_mbms_area_entity_req_p, 0, sizeof (rrc_rlc_delete_mbms_area_entity_req_t));
                for(index = RRC_ZERO; index <
                        p_cell_ctx->
                        mbms_updated_area_info_list.num_valid_mbsfn_area_info;
                        index++)
                {
                    mbms_updated_area_info_p =
                        &(p_cell_ctx->
                                mbms_updated_area_info_list.mbms_updated_area_info[index]);
                    if((U8)ADDED == mbms_updated_area_info_p->flag || 
                            (U8)MODIFIED == mbms_updated_area_info_p->flag ||
                            (U8)UNCHANGE == mbms_updated_area_info_p->flag) 
                    {
                        m2ap_reserved_mcch_related_bcch_info_p = get_m2ap_reserved_mcch_related_bcch_info(
                                &(p_delete_cell_req->m2ap_reserved_mcch_related_bcch_info_list),
                                mbms_updated_area_info_p->area_id);
                        if(PNULL == m2ap_reserved_mcch_related_bcch_info_p)
                        {

                        num_of_area = rrc_rlc_delete_mbms_area_entity_req_p->num_of_area;
                        rrc_rlc_delete_mbms_area_entity_req_p->area_id[num_of_area] = 
                            mbms_updated_area_info_p->area_index;
                        RRC_LLIM_TRACE(RRC_INFO,"API:RRC->RLC:RRC_RLC_DELETE_AREA_REQ:AreaIndex:%u",
                                mbms_updated_area_info_p->area_index);

                        rrc_rlc_delete_mbms_area_entity_req_p->num_of_area++;
                    }
                }
                }
                if(RRC_ZERO != rrc_rlc_delete_mbms_area_entity_req_p->num_of_area)
                {
                    if(RRC_SUCCESS != 
                            rrc_rlc_il_send_rrc_rlc_delete_mbms_area_entity_req(
                                rrc_rlc_delete_mbms_area_entity_req_p,
                                RRC_LLIM_MODULE_ID,
                                RRC_GET_LL_MODULE_ID(RRC_RLC_1_MODULE_ID,p_cell_ctx->cell_index),
                                p_cell_ctx->transaction_id,
                                p_cell_ctx->cell_index))
                    {
                        result = RRC_FAILURE;
                    }
                    else
                    {
                        rlc_area_delete_sent = RRC_TRUE;
                    }
                }
                /* Free Memory allocated to rrc_rlc_delete_mbms_area_entity_req_p */
                if (rrc_rlc_delete_mbms_area_entity_req_p != PNULL)
                {
                    rrc_mem_free(rrc_rlc_delete_mbms_area_entity_req_p);
                }
            }

        }
    }
    if(RRC_FALSE == rlc_area_delete_sent)
    {
#endif
        /* SPR 18440 Fix End */


    /* Delete on PHY level: send RRC_PHY_DELETE_CELL_REQ */
    phy_delete_cell_req.cell_index = 
        p_cell_ctx->cell_index;

    p_cell_ctx->transaction_id = p_delete_cell_req->transaction_id;


    if (RRC_SUCCESS != rrc_phy_il_send_rrc_phy_delete_cell_req(
                &phy_delete_cell_req,
                RRC_LLIM_MODULE_ID,
		RRC_GET_LL_MODULE_ID(RRC_PHY_1_MODULE_ID,phy_delete_cell_req.cell_index),
		  p_delete_cell_req->transaction_id,
                p_delete_cell_req->cell_index))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Unable to send Cell Delete to PHY");
        result = RRC_FAILURE;
    }
        /* SPR 18440 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
    }
#endif
    /* SPR 18440 Fix End */


    if(RRC_SUCCESS == result)
    {
        switch (CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx))
        {
            case CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF:
                /* As Cell is not configured at MAC, Cell Delete request
                 * should not be sent to MAC*/
                p_rrc_llim_gl_ctx->p_p_cell_ctx[phy_delete_cell_req.cell_index]->
                    is_mac_cell_delete_reqd = RRC_FALSE;
                /* 3. Change state */
                CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF);
                break;

            case CEMH_STATE_CELL_IN_SERVICE:
            case CEMH_STATE_CELL_OUT_OF_SERVICE:
            case CEMH_STATE_W_FOR_MAC_SFN_CNF:
            case CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF:
            case CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF:
            case CEMH_STATE_W_FOR_PHY_RECONFIG_CELL_CNF:
            case CEMH_STATE_W_FOR_PHY_CELL_START_CNF:
            case CEMH_STATE_W_FOR_MAC_CELL_START_CNF:
            case CEMH_STATE_W_FOR_PHY_CELL_STOP_CNF:
            case CEMH_STATE_W_FOR_MAC_CELL_STOP_CNF:

                p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index]->
                    is_mac_cell_delete_reqd = RRC_TRUE;
                /* 3. Change state */
#ifdef LTE_EMBMS_SUPPORTED
            /* SPR 18440 Fix Start */
            if(RRC_TRUE == rlc_area_delete_sent)
            {
                CEMH_FSM_SET_STATE(p_cell_ctx,CEMH_STATE_W_FOR_RLC_DELETE_AREA_ENTITY_CNF);

            }
            /* SPR 18440 Fix End */
            else
            {
#endif

                CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF);
#ifdef LTE_EMBMS_SUPPORTED
            }
#endif
                break;

	    default:
		RRC_ASSERT(0);
		RRC_LLIM_TRACE(RRC_WARNING, "cemh_fsm_del_cell: unexpected state!");
                break;
        }
    }
    else
    {
        delete_cell_resp.response   = RRC_FAILURE;
        /* Send CSC_LLIM_DELETE_CELL_RESP */ 
        delete_cell_resp.transaction_id = p_delete_cell_req->transaction_id;
        delete_cell_resp.cell_index     = p_cell_ctx->cell_index;
        rrc_llim_send_internal_msg( 
                RRC_CSC_MODULE_ID, 
                CSC_LLIM_DELETE_CELL_RESP,
                sizeof (delete_cell_resp), 
                &delete_cell_resp);

        /* Delete from Cell DB and free */
        p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index] = PNULL;
        rrc_mem_free(p_cell_ctx);
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: cemh_fsm_cleanup_req
*
*   OUTPUT       : none
*   DESCRIPTION  : This function cleanups CEMH FSM.
*                  state any of:
*                  CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF,
*                  CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF,
*                  CEMH_STATE_W_FOR_MAC_SFN_CNF,
*                  event CEMH_EVENT_OAMH_CLEANUP_REQ
*
*   RETURNS      : None
*
******************************************************************************/
static void cemh_fsm_cleanup_req(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    RRC_LLIM_UT_TRACE_ENTER();

    /*coverity fix start*/
    /*coverity fix stop*/
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    RRC_LLIM_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p and p_msg=%p", p_api, p_msg);

    if ((PNULL != p_cell_ctx) && (p_rrc_llim_gl_ctx->p_p_cell_ctx != PNULL))
    {
        /* 1. Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);

        /* 2. delete from Cell DB and free */
        p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index] = PNULL;

        rrc_mem_free(p_cell_ctx);
/* coverity_96718 start */
        p_cell_ctx = PNULL;
/* coverity_96718 stop */
}

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_mac_delete_cell_cnf
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function waits for the MAC CELL DELETE CNF.
 *                  state any of:
 *                  CEMH_STATE_W_FOR_MAC_CELL_DELETE_CNF,
 *                  event CEMH_EVENT_MAC_DELETE_CELL_CNF.
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_mac_delete_cell_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{

    rrc_csc_llim_delete_cell_resp_t delete_cell_resp;

    rrc_csc_llim_config_cell_resp_t config_cell_resp;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    /*coverity fix start*/
    /*coverity fix stop*/

    RRC_LLIM_TRACE(RRC_DETAILED,
            "Unused variable in fsm p_api=%p ,p_msg=%p", p_api, p_msg);     
    /*  Stop guard timer */
    cemh_stop_procedure_timer(p_cell_ctx);

    if ((p_cell_ctx->current_proc
             ==  CEMH_CELL_SETUP_PHY_FAILED) ||
         (p_cell_ctx->current_proc
             ==  CEMH_CELL_SETUP_MAC_FAILED) ||
         (p_cell_ctx->current_proc
             ==  CEMH_CELL_SETUP_MAC_SFN_FAILED)
 	)
    {

        memset_wrapper(&config_cell_resp, 0, 
                sizeof (config_cell_resp));

        config_cell_resp.response =  
            get_fail_response_config_te(p_cell_ctx->current_proc);
        /* Send CSC_LLIM_CELL_SETUP_RESP*/
        config_cell_resp.transaction_id = p_cell_ctx->transaction_id;
        config_cell_resp.cell_index     = p_cell_ctx->cell_index;
        rrc_llim_send_internal_msg(
                RRC_CSC_MODULE_ID,
                CSC_LLIM_CONFIG_CELL_RESP,
                sizeof (config_cell_resp),
                &config_cell_resp);

    }
    else
    { 

        /* Confirm API is received from PHY, and no message is awaited from MAC
         * so send success to CSC */
        delete_cell_resp.response = RRC_SUCCESS;
        /* Send CSC_LLIM_DELETE_CELL_RESP */ 
        delete_cell_resp.transaction_id = p_cell_ctx->transaction_id;
        delete_cell_resp.cell_index     = p_cell_ctx->cell_index;
        rrc_llim_send_internal_msg( 
                RRC_CSC_MODULE_ID, 
                CSC_LLIM_DELETE_CELL_RESP,
                sizeof (delete_cell_resp), 
                &delete_cell_resp);

    }

    /* 2. delete from Cell DB and free */
    if (p_rrc_llim_gl_ctx->p_p_cell_ctx != PNULL)
    {
        p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index] = PNULL;
    }

    /*valgrind fix start*/
    if ( PNULL != p_cell_ctx->p_cfg_cell_param )
    {
        rrc_mem_free(p_cell_ctx->p_cfg_cell_param);
    }
    /*valgrind fix stop*/


    rrc_mem_free(p_cell_ctx);


    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_mac_reconf_cell_state_timer_expired
 *   INPUT        : rrc_timer_t         timer_id
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function stop cell reconfiguration .
 *                  state any of:
 *                  CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF,
 *                  event CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_mac_reconf_cell_state_timer_expired(
        rrc_timer_t         timer_id,           /* timer_id - unused */
        void                *p_timer_buf,       /* p_cemh_timer_buf - unused */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_llim_csc_response_et    resp_code;

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */

    RRC_LLIM_UT_TRACE_ENTER();
    RRC_LLIM_TRACE(RRC_DETAILED, "Unused variable in fsm timer_id=%p"
            " and p_timer_buf=%p", timer_id, p_timer_buf);  

    if (CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ ==
            p_cell_ctx->msg_expected_against_event)
    {
        /*PWS going on*/
        rrc_csc_llim_pws_resp_t pws_resp;

        /* send CSC_LLIM_PWS_RESP (FAILURE) */

        memset_wrapper(&pws_resp, 0, 
                sizeof (pws_resp));

        pws_resp.transaction_id = p_cell_ctx->transaction_id;
        pws_resp.cell_index     = p_cell_ctx->cell_index;

        pws_resp.response       =
          RRC_LLIM_RECONFIG_MAC_ONLY_MODE_TIMER_EXP;

        if (RRC_SUCCESS == rrc_llim_send_internal_msg(
                    RRC_CSC_MODULE_ID, 
                    CSC_LLIM_PWS_RESP,
                    sizeof (pws_resp),
                    &pws_resp))
        {
            RRC_LLIM_TRACE(RRC_INFO,"CSC_LLIM_PWS_RESP sent");
        }

    }
    else if (CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ ==
            p_cell_ctx->msg_expected_against_event)
    {
        /*Updated PWS SI going on*/
        rrc_csc_llim_updated_pws_si_resp_t updated_pws_si_resp;
        
        /* send CSC_LLIM_PWS_RESP (SUCCESS) */
        memset_wrapper(&updated_pws_si_resp, 0, 
                sizeof (updated_pws_si_resp));

        updated_pws_si_resp.transaction_id = p_cell_ctx->transaction_id;
        updated_pws_si_resp.cell_index     = p_cell_ctx->cell_index;

        updated_pws_si_resp.response       =
          RRC_LLIM_RECONFIG_MAC_ONLY_MODE_TIMER_EXP;

        if (RRC_SUCCESS == rrc_llim_send_internal_msg(
                    RRC_CSC_MODULE_ID, 
                    CSC_LLIM_UPDATED_PWS_SI_RESP,
                    sizeof (updated_pws_si_resp),
                    &updated_pws_si_resp))
        {
            RRC_LLIM_TRACE(RRC_INFO,"CSC_LLIM_UPDATED_PWS_SI_RESP sent");
            

            
        }
    }

    else
    {
        /*Cell reconfiguration going on*/
        if (RECONFIG_MAC_ONLY == p_cell_ctx->recfg_mode)
        {
            resp_code = RRC_LLIM_RECONFIG_MAC_ONLY_MODE_TIMER_EXP;
        }
        else
        {   
            resp_code = RRC_LLIM_RECONFIG_PHY_MAC_MODE_MAC_TIMER_EXP;
        }
        if (RRC_SUCCESS == rrc_llim_send_csc_llim_reconfig_cell_resp_failure(
                    resp_code,
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index))
        {
            /*Change the CEMH(LLIM) state back to CEMH_STATE_CELL_IN_SERVICE */
            CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);  
            RRC_LLIM_UT_TRACE_EXIT();
            return;
        }
        else
        {
            RRC_LLIM_TRACE(RRC_WARNING, "UNABLE TO SEND RESPONSE TO CSC");
            RRC_LLIM_UT_TRACE_EXIT();
            return;
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_phy_reconf_cell_state_timer_expired
 *   INPUT        : rrc_timer_t         timer_id
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function stop cell reconfiguration .
 *                  state any of:
 *                  CEMH_STATE_W_FOR_PHY_RECONFIG_CELL_CNF,
 *                  CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF,
 *                  event CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_phy_reconf_cell_state_timer_expired(
        rrc_timer_t         timer_id,           /* timer_id - unused */
        void                *p_timer_buf,       /* p_cemh_timer_buf - unused */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_llim_csc_response_et    resp_code;

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */

    RRC_LLIM_UT_TRACE_ENTER();
    RRC_LLIM_TRACE(RRC_DETAILED, "Unused variable in fsm timer_id=%p"
            " and p_timer_buf=%p", timer_id, p_timer_buf);      
    if (RECONFIG_PHY_ONLY == p_cell_ctx->recfg_mode)
    {
        resp_code = RRC_LLIM_RECONFIG_PHY_ONLY_MODE_TIMER_EXP;
    }
    else
    {   
        resp_code = RRC_LLIM_RECONFIG_PHY_MAC_MODE_PHY_TIMER_EXP;
    }
    if (RRC_SUCCESS == rrc_llim_send_csc_llim_reconfig_cell_resp_failure(
                resp_code,
                p_cell_ctx->transaction_id,
                p_cell_ctx->cell_index))
    {
        /*Change the CEMH(LLIM) state */
        
        CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);  
    }
    else
    {
        RRC_LLIM_TRACE(RRC_WARNING, "UNABLE TO SEND RESPONSE TO CSC");
    }
    RRC_LLIM_UT_TRACE_EXIT();
    return;
}



/*
 *   CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_phy_conf_cell_state_phy_cnf
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function continue cell configuration if PHY is OK.
 *                  state CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF;
 *                  event CEMH_EVENT_PHY_CONFIG_CELL_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_phy_conf_cell_state_phy_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_phy_config_cell_cnf_t       *p_phy_config_cell_cnf =
        (rrc_phy_config_cell_cnf_t *)p_msg;

    rrc_csc_llim_config_cell_req_t  *p_cfg_cell_param = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_phy_config_cell_cnf);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(p_phy_config_cell_cnf->cell_index == p_cell_ctx->cell_index);

    if(PNULL != p_cell_ctx->p_cfg_cell_param)
    {
        p_cfg_cell_param = p_cell_ctx->p_cfg_cell_param;
    }
    if (PNULL == p_cfg_cell_param)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "p_cfg_cell_param pointer is null.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    /* Coverity 54815 and 55208 + */
    /* LAA Rel13 Code + */
    /* LAA Rel13 Code - */
    /* Coverity 54815 and 55208 - */

    /* Precondition - should be the same transaction */
    if (rrc_get_transaction_id(p_api) != p_cfg_cell_param->transaction_id)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    if (RRC_SUCCESS == p_phy_config_cell_cnf->response)
    {
        cemh_build_and_send_rrc_mac_config_cell_req(p_cfg_cell_param);
        /* eMTC changes stop */
        CEMH_FSM_SET_STATE(
                p_cell_ctx, 
                CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF);
    }
    else
    {
        /* 1. Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);

        /* 2. send CSC_LLIM_CONFIG_CELL_RESP */
        rrc_llim_send_csc_llim_config_cell_resp_failure(
                RRC_LLIM_CONFIG_CELL_FAIL_PHY,
                p_cfg_cell_param->transaction_id,
                p_cell_ctx->cell_index);

        if (p_cell_ctx->p_cfg_cell_param != PNULL)
        {
            rrc_mem_free(p_cell_ctx->p_cfg_cell_param);
            p_cell_ctx->p_cfg_cell_param = PNULL;
        }


        /* 4. delete from Cell DB and free */
        if (p_rrc_llim_gl_ctx->p_p_cell_ctx != PNULL)
        {
            p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index] = PNULL;
        }

        rrc_mem_free(p_cell_ctx);
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_cell_reconfig_cell_req 
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function start cell Reconfiguration.
 *                  state CEMH_STATE_CELL_IN_SERVICE; event CEMH_EVENT_CSC_RECONFIG_CELL_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_cell_reconfig_cell_req(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM 
                                                    global context data */
{
    rrc_csc_llim_reconfig_cell_req_t  *p_recfg_cell_param =
        (rrc_csc_llim_reconfig_cell_req_t *)p_msg;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_recfg_cell_param);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(p_recfg_cell_param->cell_index == p_cell_ctx->cell_index);
    RRC_LLIM_TRACE(RRC_DETAILEDALL,
                        "Unused variable in fsm p_api=%p", p_api);/*SPR 17777+-*/
    if (PNULL != p_cell_ctx->guard_timer) 
    {
        /* Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);
    }

    /* Setting the event in cell context */
    p_cell_ctx->msg_expected_against_event = CEMH_EVENT_CSC_RECONFIG_CELL_REQ;    

    /* Store the Transaction id in LLIM Cell context*/
    p_cell_ctx->transaction_id = p_recfg_cell_param->transaction_id;

    p_cell_ctx->p_recfg_req_param = (rrc_csc_llim_reconfig_cell_req_t*)
        rrc_mem_get(sizeof(rrc_csc_llim_reconfig_cell_req_t));

    if (PNULL != p_cell_ctx->p_recfg_req_param)
    {
        memset_wrapper(p_cell_ctx->p_recfg_req_param ,0,sizeof(rrc_csc_llim_reconfig_cell_req_t));

        /*Storing Reconfig Param in Cell Context*/
        l3_memcpy_wrapper((p_cell_ctx->p_recfg_req_param),
                p_recfg_cell_param,
                sizeof (rrc_csc_llim_reconfig_cell_req_t));  
    }

    /* 1. start guard timer */
    cemh_start_reconfig_procedure_timer(
            p_cell_ctx,
            p_rrc_llim_gl_ctx->cell_recfg_duration);

    if(PHY_ONLY == p_recfg_cell_param->mode)
    {
        /* Filling and Sending PHY Reconfig API */
        cemh_build_send_rrc_phy_reconfig_cell_req(
                p_recfg_cell_param,
                p_cell_ctx);

        /*Change the mode in llim cell context */
        p_cell_ctx->recfg_mode = RECONFIG_PHY_ONLY;
        return;
    }
    else if (MAC_ONLY == p_recfg_cell_param->mode)
    {
        /* Filling and Sending PHY Reconfig API */
        cemh_build_send_rrc_mac_reconfig_cell_req(
                p_recfg_cell_param,
                p_cell_ctx );

        /*Change the mode in llim cell context */
        p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY;
        return;
    }
    else /* Reconfig Request first time */
    {
        if ((p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG)||
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG)||
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG)||
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG)||
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG)||
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG)||
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG)||
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG)||
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG)
#ifdef LTE_EMBMS_SUPPORTED
                ||
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_SIB_13_INFO_PRESENCE_FLAG)||
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_MCCH_UPDATED_AREA_INFO_LIST)
#endif
                )
        {
            p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY;
        }
    }

    /* CHeck whether PHY Parameter has been changed/Modified or NOT */
    if(p_recfg_cell_param->cell_reconfig.presence_bitmask &
            CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG )
    {  
        if (p_recfg_cell_param->cell_reconfig.non_broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG)
        {
            p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY;
        }

        if ((p_recfg_cell_param->cell_reconfig.non_broadcast_info.presence_bitmask
                    & CELL_RECONFIG_REQ_API_SYNC_SIGNAL_INFO_PRESENCE_FLAG) ||
                (p_recfg_cell_param->cell_reconfig.non_broadcast_info.presence_bitmask & 
                 CELL_RECONFIG_REQ_API_CELL_PARAMETER_INFO_PRESENCE_FLAG)) 
        {
            if (NO_RECONFIG_REQ == p_cell_ctx->recfg_mode)
            {
                p_cell_ctx->recfg_mode = RECONFIG_PHY_ONLY;
            }   
            else
            {  
		/* SPR 13727 Fix Start */ 
                p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY;
		/* SPR 13727 Fix Stop */ 
            }
        }
    }   

    if(p_recfg_cell_param->cell_reconfig.presence_bitmask &
            CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG )
    {
        if ((p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG) || 
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG))
        {   
	   /* SPR 13727 Fix Start */ 
            p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY; 
	   /* SPR 13727 Fix Stop */ 
        }
    }

    /* CLPC Start */
    if(p_recfg_cell_param->cell_reconfig.presence_bitmask &
            CELL_RECONFIG_REQ_MAC_DOWNLINK_POWER_CONTROL_PRESENCE_FLAG)
    {
	/* SPR 13727 Fix Start */ 
        p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY;
	/* SPR 13727 Fix Stop */ 
    }
    /* CLPC Stop */


    if((RECONFIG_PHY_ONLY == p_cell_ctx->recfg_mode) || 
            (RECONFIG_PHY_MAC == p_cell_ctx->recfg_mode))
    {   
        /* Filling and Sending PHY Reconfig API */
        cemh_build_send_rrc_phy_reconfig_cell_req(
                p_recfg_cell_param, 
                p_cell_ctx);
    }
    else if (RECONFIG_MAC_ONLY == p_cell_ctx->recfg_mode)
    {   
        /* FILLING THE MAC API */
        cemh_build_send_rrc_mac_reconfig_cell_req( p_recfg_cell_param, p_cell_ctx);
    }
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_cell_out_of_service_cell_reconfig_cell_req 
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function start cell Reconfiguration.
 *                  state CEMH_STATE_CELL_IN_SERVICE; event CEMH_EVENT_CSC_RECONFIG_CELL_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_cell_out_of_service_cell_reconfig_cell_req(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM 
                                                    global context data */
{
    rrc_csc_llim_reconfig_cell_req_t  *p_recfg_cell_param =
        (rrc_csc_llim_reconfig_cell_req_t *)p_msg;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_recfg_cell_param);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(p_recfg_cell_param->cell_index == p_cell_ctx->cell_index);

    if (PNULL != p_cell_ctx->guard_timer) 
    {
        /* Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);
    }

    /* Setting the event in cell context */
    p_cell_ctx->msg_expected_against_event = CEMH_EVENT_CSC_RECONFIG_CELL_REQ;    

    /* Store the Transaction id in LLIM Cell context*/
    p_cell_ctx->transaction_id = p_recfg_cell_param->transaction_id;

    p_cell_ctx->p_recfg_req_param = (rrc_csc_llim_reconfig_cell_req_t*)
        rrc_mem_get(sizeof(rrc_csc_llim_reconfig_cell_req_t));

    if (PNULL != p_cell_ctx->p_recfg_req_param)
    {
        memset_wrapper(p_cell_ctx->p_recfg_req_param ,0,sizeof(rrc_csc_llim_reconfig_cell_req_t));

        /*Storing Reconfig Param in Cell Context*/
        l3_memcpy_wrapper((p_cell_ctx->p_recfg_req_param),
                p_recfg_cell_param,
                sizeof (rrc_csc_llim_reconfig_cell_req_t));  
    }

    if ( (PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx) && (PNULL != 
            p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index])&&
            (p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index]->
            explicit_start_required == RRC_LLIM_EXPLICIT_CELL_START))
    {
        /* 1. start guard timer */
        cemh_start_reconfig_procedure_timer(
                p_cell_ctx,
                p_rrc_llim_gl_ctx->cell_recfg_duration);

        if(PHY_ONLY == p_recfg_cell_param->mode)
        {
            /* Filling and Sending PHY Reconfig API */
            cemh_build_send_rrc_phy_reconfig_cell_req(
                    p_recfg_cell_param,
                    p_cell_ctx);

            /*Change the mode in llim cell context */
            p_cell_ctx->recfg_mode = RECONFIG_PHY_ONLY;
            return;
        }
        else if (MAC_ONLY == p_recfg_cell_param->mode)
        {
            /* Filling and Sending PHY Reconfig API */
            cemh_build_send_rrc_mac_reconfig_cell_req(
                    p_recfg_cell_param,
                    p_cell_ctx );

            /*Change the mode in llim cell context */
            p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY;
            return;
        }
        else /* Reconfig Request first time */
        {
            if ((p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                        CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG)||
                    (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                     CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG)||
                    (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                     CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG)||
                    (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                     CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG)||
                    (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                     CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG)||
                    (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                     CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG)||
                    (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                     CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG)||
                    (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                     CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG)||
                    (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                     CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG))
            {
                p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY;
            }
        }

        /* CHeck whether PHY Parameter has been changed/Modified or NOT */
        if(p_recfg_cell_param->cell_reconfig.presence_bitmask &
                CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG )
        {  
            if (p_recfg_cell_param->cell_reconfig.non_broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG)
            {
                p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY;
            }

            if ((p_recfg_cell_param->cell_reconfig.non_broadcast_info.presence_bitmask
                        & CELL_RECONFIG_REQ_API_SYNC_SIGNAL_INFO_PRESENCE_FLAG) ||
                    (p_recfg_cell_param->cell_reconfig.non_broadcast_info.presence_bitmask & 
                     CELL_RECONFIG_REQ_API_CELL_PARAMETER_INFO_PRESENCE_FLAG))
            {
                if (NO_RECONFIG_REQ == p_cell_ctx->recfg_mode)
                {
                    p_cell_ctx->recfg_mode = RECONFIG_PHY_ONLY;
                }   
                else
                { 
		    /* SPR 13727 Fix Start */ 
                    p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY;
		    /* SPR 13727 Fix Stop */ 
                }
            }
        }   

        if(p_recfg_cell_param->cell_reconfig.presence_bitmask &
                CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG )
        {
            if ((p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                        CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG) || 
                    (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                     CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG))
            {  
		/* SPR 13727 Fix Start */ 
                p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY;
		/* SPR 13727 Fix Stop */ 
            }
        }

/* SPR 5145 Start */
        if(p_recfg_cell_param->cell_reconfig.presence_bitmask &
                CELL_RECONFIG_REQ_MAC_DOWNLINK_POWER_CONTROL_PRESENCE_FLAG)
        {  
	   /* SPR 13727 Fix Start */ 
            p_cell_ctx->recfg_mode = RECONFIG_MAC_ONLY;
           /* SPR 13727 Fix Stop */ 
        }
/* SPR 5145 End */


        if((RECONFIG_PHY_ONLY == p_cell_ctx->recfg_mode) || 
                (RECONFIG_PHY_MAC == p_cell_ctx->recfg_mode))
        {   
            /* Filling and Sending PHY Reconfig API */
            cemh_build_send_rrc_phy_reconfig_cell_req(
                    p_recfg_cell_param, 
                    p_cell_ctx);
        }
        else if (RECONFIG_MAC_ONLY == p_cell_ctx->recfg_mode)
        {   
            /* FILLING THE MAC API */
            cemh_build_send_rrc_mac_reconfig_cell_req( p_recfg_cell_param, p_cell_ctx);
        }
    }
    else 
    {
        RRC_LLIM_TRACE(RRC_INFO,"CSC_LLIM_CELL_RECONFIG_REQ not sent; unused variable p_api = %p",p_api);/*SPR 17777 +-*/
    }
}

/*
 *   CEMH_STATE_CELL_IN_SERVICE state handlers
 */


/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_in_service_state_wrw_req
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function start cell Reconfiguration.
 *                  state CEMH_STATE_CELL_IN_SERVICE;
 *                  event CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ 
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_in_service_state_wrw_req(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM 
                                                    global context data */
{
    rrc_csc_llim_pws_req_t *p_wrw_req =
        (rrc_csc_llim_pws_req_t *)p_msg;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_wrw_req);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(p_wrw_req->cell_index == p_cell_ctx->cell_index);

    if (PNULL != p_cell_ctx->guard_timer) 
    {
        /* Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);
    }

    /* Store the Transaction id in LLIM Cell context*/
    p_cell_ctx->transaction_id = p_wrw_req->transaction_id;

    cemh_start_reconfig_procedure_timer(
            p_cell_ctx,
            p_rrc_llim_gl_ctx->cell_recfg_duration);

    /* Filling and Sending MAC Reconfig API for ETWS */
    cemh_build_and_send_rrc_mac_reconfig_cell_req_for_etws(
            p_wrw_req);

    p_cell_ctx->msg_expected_against_event =  
        CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ;

    CEMH_FSM_SET_STATE(p_cell_ctx, 
            CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF);
        /*SPR 17777 +-*/
    RRC_LLIM_TRACE(RRC_DETAILEDALL,
                        "Unused variable in fsm p_api=%p", p_api);/*SPR 17777+-*/

}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_in_service_csc_si_without_etws_req
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function start cell Reconfiguration.
 *                  state CEMH_STATE_CELL_IN_SERVICE;
 *                  event CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ 
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_in_service_csc_si_without_etws_req(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM 
                                                    global context data */
{
    /* cell index changes */
    rrc_csc_llim_updated_pws_si_req_t *p_updated_pws_si_req =
        (rrc_csc_llim_updated_pws_si_req_t *)p_msg;

    rrc_mac_reconfig_cell_req_t *p_mac_reconfig_cell_req = PNULL;
    RRC_LLIM_TRACE(RRC_DETAILEDALL,
                        "Unused variable in fsm p_api=%p", p_api);/*SPR 17777+-*/

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_updated_pws_si_req);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(p_updated_pws_si_req->cell_index == p_cell_ctx->cell_index);

    if (PNULL != p_cell_ctx->guard_timer) 
    {
        /* Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);
    }

    /* Store the Transaction id in LLIM Cell context*/
    p_cell_ctx->transaction_id = p_updated_pws_si_req->transaction_id;

    /* 1. start guard timer */
    cemh_start_reconfig_procedure_timer(
            p_cell_ctx,
            p_rrc_llim_gl_ctx->cell_recfg_duration);


    p_mac_reconfig_cell_req = (rrc_mac_reconfig_cell_req_t*)rrc_mem_get(
            sizeof (rrc_mac_reconfig_cell_req_t));

    if(PNULL != p_mac_reconfig_cell_req)
    {

        memset_wrapper(p_mac_reconfig_cell_req, 0, sizeof (rrc_mac_reconfig_cell_req_t));

        /*Filling the SFN */
        p_mac_reconfig_cell_req->sfn = (rrc_sfn_t)(p_updated_pws_si_req->sfn);

        p_mac_reconfig_cell_req->optional_elems_present |=
            RRC_RECONFIG_MAC_SIB_TYPE_1_MSG_PRESENT;
        p_mac_reconfig_cell_req->sib1_msg_req = p_updated_pws_si_req->sib1_msg_req;

        p_mac_reconfig_cell_req->optional_elems_present |=
            RRC_RECONFIG_MAC_SI_MSG_REQ_PRESENT;
        cemh_fill_si_msg_req (&p_mac_reconfig_cell_req->si_msg_req,
                              &p_updated_pws_si_req->si_msg_req);

        /* Send MAC RECONFIG */
        rrc_mac_il_send_rrc_mac_reconfig_cell_req(
            p_mac_reconfig_cell_req,
            RRC_LLIM_MODULE_ID,
		RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_updated_pws_si_req->cell_index),
		p_updated_pws_si_req->transaction_id,
            p_updated_pws_si_req->cell_index);


        /* Free Memory allocated to p_mac_reconfig_cell_req */
        if (p_mac_reconfig_cell_req != PNULL)
        { 
            rrc_mem_free(p_mac_reconfig_cell_req);
        }
    }

    p_cell_ctx->msg_expected_against_event =  
        CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ;

    CEMH_FSM_SET_STATE(p_cell_ctx, 
            CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF);
}

/******************************************************************************
 *   FUNCTION NAME: build_and_send_wrw_resp_to_csc
 *   INPUT        : rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_mac_reconfig_cell_cnf_t       *p_mac_reconfig_cell_cnf
 *   OUTPUT       : none
 *   DESCRIPTION  : This function sends WRW resp to CSC.
 *
 *   RETURNS      : None
 *
 ******************************************************************************/



static void build_and_send_wrw_resp_to_csc
(
 rrc_llim_cell_ctx_t *p_cell_ctx,
 rrc_mac_reconfig_cell_cnf_t       *p_mac_reconfig_cell_cnf
 )
{ 
    rrc_csc_llim_pws_resp_t pws_resp;
    /* send CSC_LLIM_PWS_RESP */
    memset_wrapper(&pws_resp, 0, 
        sizeof (pws_resp));

    pws_resp.transaction_id = p_cell_ctx->transaction_id;
    pws_resp.cell_index     = p_cell_ctx->cell_index;

    switch (p_mac_reconfig_cell_cnf->response)
    {
      case MAC_SUCCESS:
        pws_resp.response       =
          RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS;
        break;
      default:
        pws_resp.response       = 
          RRC_LLIM_RECONFIG_MAC_ONLY_MODE_FAIL;
        break;
    }

    if (RRC_SUCCESS == rrc_llim_send_internal_msg(
                RRC_CSC_MODULE_ID, 
                CSC_LLIM_PWS_RESP,
                sizeof (pws_resp),
                &pws_resp))
    {
        RRC_LLIM_TRACE(RRC_INFO,"CSC_LLIM_WRITE_REPLACE_WARNING_RESP sent");
    }

}

/******************************************************************************
 *   FUNCTION NAME: build_and_send_si_without_etws_resp_to_csc
 *   INPUT        : rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_mac_reconfig_cell_cnf_t       *p_mac_reconfig_cell_cn
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function sends SI without ETWS resp to CSC.
 *
 *   RETURNS      : None
 *
 ******************************************************************************/


static void build_and_send_si_without_etws_resp_to_csc
(
 rrc_llim_cell_ctx_t *p_cell_ctx,
 rrc_mac_reconfig_cell_cnf_t       *p_mac_reconfig_cell_cnf
 )
{ 
    rrc_csc_llim_updated_pws_si_resp_t updated_pws_si_resp;
    /* send CSC_LLIM_UPDATED_PWS_SI_RESP */
    memset_wrapper(&updated_pws_si_resp, 0, 
            sizeof (updated_pws_si_resp));

    updated_pws_si_resp.transaction_id = p_cell_ctx->transaction_id;
    updated_pws_si_resp.cell_index     = p_cell_ctx->cell_index;

    switch (p_mac_reconfig_cell_cnf->response)
    {
      case MAC_SUCCESS:
        updated_pws_si_resp.response       =
          RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS;
        break;
      default:
        updated_pws_si_resp.response       =
          RRC_LLIM_RECONFIG_MAC_ONLY_MODE_FAIL;
        break;
    }

    if (RRC_SUCCESS == rrc_llim_send_internal_msg(
                RRC_CSC_MODULE_ID, 
                CSC_LLIM_UPDATED_PWS_SI_RESP,
                sizeof (updated_pws_si_resp),
                &updated_pws_si_resp))
    {
        RRC_LLIM_TRACE(RRC_INFO,"CSC_LLIM_UPDATED_PWS_SI_RESP sent");
    }
}

/*******************************************************************************
 *   FUNCTION NAME: cemh_build_send_rrc_phy_reconfig_cell_req
 *   INPUT        : rrc_csc_llim_reconfig_cell_req_t    *p_recfg_cell_param,
 *                  rrc_llim_cell_ctx_t                 *p_cell_ctx
 *   DESCRIPTION  : This function  will Fill the PHY RECONFIG API with the 
 *                  R-Value as received in cell Reconfiguration request and 
 *                  SEND to PHY layer
 *
 *   RETURNS      : None
 *
 *******************************************************************************/
static void cemh_build_send_rrc_phy_reconfig_cell_req(
        rrc_csc_llim_reconfig_cell_req_t    *p_recfg_cell_param,
        rrc_llim_cell_ctx_t                 *p_cell_ctx)
{
	/* coverity_fix_54709_start */
	rrc_phy_reconfig_cell_req_t     *p_phy_reconfig_cell_req = PNULL;
	rrc_phy_reconfig_cell_req_t     phy_reconfig_cell_req;


	RRC_LLIM_UT_TRACE_ENTER();

	p_phy_reconfig_cell_req = &phy_reconfig_cell_req;

	memset_wrapper(p_phy_reconfig_cell_req, 0, sizeof (phy_reconfig_cell_req));

	p_phy_reconfig_cell_req->cell_index = p_recfg_cell_param->cell_index;

	/* It should always be zero */
	p_phy_reconfig_cell_req->sf = RRC_NULL;
	p_phy_reconfig_cell_req->sfn = p_recfg_cell_param->sfn;

	if (RRC_TRUE == cemh_fill_phy_recfg_cell_parametes(
				p_recfg_cell_param, 
				&p_phy_reconfig_cell_req->phy_recfg_cell_parameters,
				p_cell_ctx))
	{
		p_phy_reconfig_cell_req->bitmask |= 
			RRC_RECONFIG_PHY_CELL_PARAMETERS_PRESENT;
	}

	if (p_recfg_cell_param->cell_reconfig.presence_bitmask &
			CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG ) 
	{
		/* Filling SYN Signals parameters*/
		if (p_recfg_cell_param->cell_reconfig.non_broadcast_info.
				presence_bitmask & CELL_RECONFIG_REQ_API_SYNC_SIGNAL_INFO_PRESENCE_FLAG)
		{
			p_phy_reconfig_cell_req->sync_signals = 
				p_recfg_cell_param->cell_reconfig.non_broadcast_info.sync_signals;
			/* Setting Bitmask*/
			p_phy_reconfig_cell_req->bitmask |= RRC_RECONFIG_PHY_SYNC_SIGNAL_PRESENT;
		}
	}

	/* PREPARE  RRC_PHY_RECONFIG_CELL_REQ */
	if ((p_recfg_cell_param->cell_reconfig.presence_bitmask & 
				CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG))
	{
		if (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask & 
				CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG)
		{
			/* Filling the PRACH Configuration */
			/* ALERT : In cell Setup / RECONFIG Request all IEs of RadioResource 
			   are Mandotory and here we are assuming optional */   

			p_phy_reconfig_cell_req->bitmask |= 
				RRC_RECONFIG_PHY_PRACH_CONFIGURATION_PRESENT;

			p_phy_reconfig_cell_req->prach_configuration.prach_config_sequence = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				prach_config_sib.prach_config_info.prach_config_index;

			p_phy_reconfig_cell_req->prach_configuration.root_seq_index = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				prach_config_sib.root_seq_index;

			p_phy_reconfig_cell_req->prach_configuration.prach_freq_offset = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				prach_config_sib.prach_config_info.prach_freq_offset;

			p_phy_reconfig_cell_req->prach_configuration.zero_cor_zone_config = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				prach_config_sib.prach_config_info.zero_cor_zone_config;

			p_phy_reconfig_cell_req->prach_configuration.high_speed_flag = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				prach_config_sib.prach_config_info.high_speed_flag;

			/* Filling of PUSCH Configuration */    

			p_phy_reconfig_cell_req->bitmask |= 
				RRC_RECONFIG_PHY_PUSCH_CONFIGURATION_PRESENT;

			p_phy_reconfig_cell_req->pusch_configuration.num_of_sub_bands = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				pusch_config_common.pusch_config_basic.nsb;

			p_phy_reconfig_cell_req->pusch_configuration.pusch_hopping_mode = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				pusch_config_common.pusch_config_basic.hopping_mode;

			p_phy_reconfig_cell_req->pusch_configuration.ul_ref_signal =
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				pusch_config_common.ul_ref_signals_pusch;

			p_phy_reconfig_cell_req->pusch_configuration.pusch_hopping_offset =
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				pusch_config_common.pusch_config_basic.pusch_hopping_offset;

			/* Filling of PUCCH COnfiguration */
			p_phy_reconfig_cell_req->bitmask |= 
				RRC_RECONFIG_PHY_PUCCH_CONFIGURATION_PRESENT;

			p_phy_reconfig_cell_req->pucch_configuration.delta_pucch_shift = 
				cemh_e2v_delta_pucch_shift(
						p_recfg_cell_param->cell_reconfig.broadcast_info.
						sib_type_2_Info.radio_resource_config_common_sib.
						pucch_config_common.delta_pucch_shift);

			p_phy_reconfig_cell_req->pucch_configuration.nrb_cqi = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				pucch_config_common.n_rb_cqi;

			p_phy_reconfig_cell_req->pucch_configuration.ncs_an = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				pucch_config_common.n_cs_an;

			p_phy_reconfig_cell_req->pucch_configuration.n1pucch_an = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.
				sib_type_2_Info.radio_resource_config_common_sib.
				pucch_config_common.n_one_pucch_an;

			/* Filling of PDSCH configuration */
			p_phy_reconfig_cell_req->pdsch_configuration = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_2_Info.
				radio_resource_config_common_sib.pdsch_config_common;

			p_phy_reconfig_cell_req->bitmask |= 
				RRC_RECONFIG_PHY_PDSCH_CONFIGURATION_PRESENT;
		}
		/* Filling of PHICH COnfiguration */

		if(p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask & 
				CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG)
		{
			p_phy_reconfig_cell_req->bitmask |= 
				RRC_RECONFIG_PHY_PHICH_CONFIGURATION_PRESENT;

			p_phy_reconfig_cell_req->phich_configuration.phich_resource = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.mib_info.
				phich_config.phich_resource;

			p_phy_reconfig_cell_req->phich_configuration.phich_duration = 
				p_recfg_cell_param->cell_reconfig.broadcast_info.mib_info.
				phich_config.phich_duration;
		}



	}

	/* CLPC Start */
	if (p_recfg_cell_param->cell_reconfig.presence_bitmask &
			CELL_RECONFIG_REQ_MAC_DOWNLINK_POWER_CONTROL_PRESENCE_FLAG)
	{
		p_phy_reconfig_cell_req->bitmask |= 
			RRC_RECONFIG_PHY_CELL_PARAMETERS_PRESENT;

		p_phy_reconfig_cell_req->phy_recfg_cell_parameters.bitmask |=
			RRC_PCFICH_POWER_OFFSET_PRESENT;

		p_phy_reconfig_cell_req->phy_recfg_cell_parameters.pcfich_power_offset =
			p_recfg_cell_param->cell_reconfig.downlink_power_control_common.pcfichPowerOffset;

		p_phy_reconfig_cell_req->phy_recfg_cell_parameters.bitmask |=
			RRC_PHICH_POWER_OFFSET_PRESENT;

		p_phy_reconfig_cell_req->phy_recfg_cell_parameters.phich_power_offset =
			p_recfg_cell_param->cell_reconfig.downlink_power_control_common.phichPowerOffset;

	}
	/* CLPC Stop */

	/* PRS + */
	/*SPR 8212 Fix start*/ 
	if ((p_recfg_cell_param->cell_reconfig.presence_bitmask &
				CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG) &&
			(p_recfg_cell_param->cell_reconfig.non_broadcast_info.presence_bitmask &
			 CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG
			))
	{
		if ((p_recfg_cell_param->cell_reconfig.non_broadcast_info.mac_reconfig_params.prs_reconfig_info.bitmask &
					RRC_RRM_RECONFIG_PRS_CONFIG_PRESENT))
		{
			p_phy_reconfig_cell_req->phy_recfg_cell_parameters.bitmask |= RRC_RECONFIG_PHY_PRS_PRESENT;

			/* Store mapped value of prs_bandwidth recieved from rrm */
			p_phy_reconfig_cell_req->phy_recfg_cell_parameters.prs_reconfig.prs_bandwidth = 
				fill_prs_bandwidth_in_rb(p_recfg_cell_param->cell_reconfig.non_broadcast_info.
						mac_reconfig_params.prs_reconfig_info.prs_reconfig.prs_bandwidth_info.prs_bandwidth);

			if (p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.
					bitmask & RRC_RRM_RECONFIG_PHY_CELL_PARAMS_DL_CYCLIC_PREFIX)
			{ 
				p_phy_reconfig_cell_req->phy_recfg_cell_parameters.prs_reconfig.prs_cyclic_prefix_type =
					p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.dl_cyclic_prefix;
			}
			else
			{
				p_phy_reconfig_cell_req->phy_recfg_cell_parameters.prs_reconfig.prs_cyclic_prefix_type =
					p_cell_ctx->p_cfg_cell_param->llim_cell_param.cell_config_param.cell_parameters.dl_cyclic_prefix;
			} 
		}
	}
	/*SPR 8212 Fix stop*/ 
	/* PRS - */


	/* Send PHY RECONFIG */
	rrc_phy_il_send_rrc_phy_reconfig_cell_req(
			p_phy_reconfig_cell_req,
			RRC_LLIM_MODULE_ID,
			RRC_GET_LL_MODULE_ID(RRC_PHY_1_MODULE_ID,p_phy_reconfig_cell_req->cell_index),
			p_recfg_cell_param->transaction_id,
			p_recfg_cell_param->cell_index);

	CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_W_FOR_PHY_RECONFIG_CELL_CNF);
	RRC_LLIM_UT_TRACE_EXIT();
}
/* coverity_fix_54709_stop */

/*******************************************************************************
 *   FUNCTION NAME: cemh_fill_phy_recfg_cell_parameters
 *   INPUT        : rrc_csc_llim_reconfig_cell_req_t*      p_recfg_cell_param,
 *                  rrc_phy_reconfig_cell_parameters_t*    p_phy_recfg_cell_parameters,
 *                  rrc_llim_cell_ctx_t                 *p_cell_ctx
 *   OUTPUT       : none                
 *   DESCRIPTION  : This function  will Fill the Phy reconfig cell parameters values
 *
 *   RETURNS      : None
 *
 *******************************************************************************/
static rrc_bool_et cemh_fill_phy_recfg_cell_parametes(
        rrc_csc_llim_reconfig_cell_req_t*      p_recfg_cell_param,
        rrc_phy_reconfig_cell_parameters_t*    p_phy_recfg_cell_parameters,
        rrc_llim_cell_ctx_t                 *p_cell_ctx)
{
    rrc_bool_et ret_val = RRC_FALSE;
    RRC_LLIM_UT_TRACE_ENTER();

    p_phy_recfg_cell_parameters->bitmask = 0;

    if (p_recfg_cell_param->cell_reconfig.presence_bitmask &
            CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG ) 
    {
        if (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG)
        {
            if (p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_1_Info.
                    presence_bitmask & SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG)
            {
                /* Check whether TDD got changed or not */
                if (!(p_cell_ctx->p_cfg_cell_param->llim_cell_param.sib_type_1_Info.
                            presence_bitmask & SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG))
                {
                    /* Setting the bitmask for Duplexing Mode */
                    p_phy_recfg_cell_parameters->bitmask |=
                        RRC_RECONFIG_PHY_DUPLEX_MODE;
                    p_phy_recfg_cell_parameters->duplexing_mode  = DUPLEXING_MODE_TDD;
                }

            }
            ret_val = RRC_TRUE;
        }
        if (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG)
        {
            if (p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_2_Info.
                    freq_info.presence_bitmask &
                    FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG)
            {
                p_phy_recfg_cell_parameters->ul_earfcn = 
                    p_recfg_cell_param->cell_reconfig.broadcast_info.
                    sib_type_2_Info.freq_info.ul_carrier_freq;

                /* Setting Bitmask*/
                p_phy_recfg_cell_parameters->bitmask |= 
                    RRC_RECONFIG_PHY_UL_EARFCN;
            }

            if (p_recfg_cell_param->cell_reconfig.broadcast_info.
                    sib_type_2_Info.freq_info.presence_bitmask & 
                    FREQ_INFO_UL_BW_PRESENCE_FLAG)
            {
                p_phy_recfg_cell_parameters->ul_tx_bandwidth = 
                   p_recfg_cell_param->cell_reconfig.broadcast_info.
                   sib_type_2_Info.freq_info.ul_bandwidth;

            /* Setting Bitmask */
                p_phy_recfg_cell_parameters->bitmask |= 
                   RRC_RECONFIG_PHY_UL_TX_BANDWIDTH_PRESENT;
            }

            p_phy_recfg_cell_parameters->ul_cyclic_prefix = 
                p_recfg_cell_param->cell_reconfig.broadcast_info.
                sib_type_2_Info.radio_resource_config_common_sib.
                ul_cyc_prefix_len;

            /* Setting Bitmask*/
            p_phy_recfg_cell_parameters->bitmask |= 
                RRC_RECONFIG_PHY_UL_CYCLIC_PREFIX;


            if (p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_2_Info.
                    radio_resource_config_common_sib.
                    sounding_rs_ul_config_common.presence_bitmask & 
                    SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG)
            {
                p_phy_recfg_cell_parameters->srs_bandwidth_configuration = 
                    p_recfg_cell_param->cell_reconfig.broadcast_info.
                    sib_type_2_Info.radio_resource_config_common_sib.
                    sounding_rs_ul_config_common;

                /* Setting Bitmask*/
                p_phy_recfg_cell_parameters->bitmask |= 
                    RRC_RECONFIG_PHY_SRS_BANDWIDTH_CONFIGURATION_PRESENT;
            }
            ret_val = RRC_TRUE;
        }
        if (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask &
                CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG)
        { 
            p_phy_recfg_cell_parameters->dl_tx_bandwidth = 
                p_recfg_cell_param->cell_reconfig.broadcast_info.
                mib_info.dl_band_width;

            /* Setting Bitmask*/
            p_phy_recfg_cell_parameters->bitmask |= 
                RRC_RECONFIG_PHY_DL_TX_BANDWIDTH_PRESENT;
            ret_val = RRC_TRUE;
        }
    }
    if (p_recfg_cell_param->cell_reconfig.presence_bitmask &
            CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG) 
    {
        if (p_recfg_cell_param->cell_reconfig.non_broadcast_info.presence_bitmask & 
                CELL_RECONFIG_REQ_API_CELL_PARAMETER_INFO_PRESENCE_FLAG)
        {
            if(p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.bitmask
                    & RRC_RRM_RECONFIG_PHY_CELL_PARAMS_DL_EARFCN_PRESENT)
            {
                p_phy_recfg_cell_parameters->dl_earfcn = 
                    p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                    cell_parameters.dl_earfcn;

                /* Setting Bitmask*/
                p_phy_recfg_cell_parameters->bitmask |= 
                    RRC_RECONFIG_PHY_DL_EARFCN;
                ret_val = RRC_TRUE;
            }
            if(p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.
                    bitmask & RRC_RRM_RECONFIG_PHY_CELL_PARAMS_NUM_OF_ANTENNAS)
            {
                p_phy_recfg_cell_parameters->num_of_antennas = 
                    p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                    cell_parameters.num_of_antennas;

                /* Setting Bitmask*/
                p_phy_recfg_cell_parameters->bitmask |= 
                    RRC_RECONFIG_PHY_NUM_OF_ANTENNAS;
                ret_val = RRC_TRUE;
            }

            if (p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.
                    bitmask & RRC_RRM_RECONFIG_PHY_CELL_PARAMS_DL_CYCLIC_PREFIX)
            {
                p_phy_recfg_cell_parameters->dl_cyclic_prefix = 
                    p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                    cell_parameters.dl_cyclic_prefix;

                /* Setting Bitmask*/
                p_phy_recfg_cell_parameters->bitmask |= 
                    RRC_RECONFIG_PHY_DL_CYCLIC_PREFIX;
                ret_val = RRC_TRUE;
            }
            if (p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.
                    bitmask & RRC_RRM_RECONFIG_PHY_CELL_ID)
            {
                p_phy_recfg_cell_parameters->phys_cell_id =
                    p_recfg_cell_param->cell_reconfig.non_broadcast_info.
                    cell_parameters.phys_cell_id;

                /* Setting Bitmask*/
                p_phy_recfg_cell_parameters->bitmask |=
                    RRC_RECONFIG_PHY_CELL_ID;
                ret_val = RRC_TRUE;
            }

            /* CLPC Start */
            
            if (p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.
                            bitmask & RRC_RRM_PRIMARY_SYNC_SIGNAL_EPRE_EPRERS)
            {
                p_phy_recfg_cell_parameters->bitmask |=
                    RRC_PRIMARY_SYNC_SIGNAL_EPRE_EPRERS;
                p_phy_recfg_cell_parameters->pri_sync_sig_epre_eprers =
                    p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.
                    pri_sync_sig_epre_eprers;
            }   
            if (p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.
                            bitmask & RRC_RRM_SECONDARY_SYNC_SIGNAL_EPRE_EPRERS)
            {
                p_phy_recfg_cell_parameters->bitmask |=
                    RRC_SECONDARY_SYNC_SIGNAL_EPRE_EPRERS;
                p_phy_recfg_cell_parameters->sec_sync_sig_epre_eprers =
                    p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.
                    sec_sync_sig_epre_eprers;
            }

            /* CLPC Stop */

            /* SPR 3802 Start */
            if (p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.
                            bitmask & RRC_RRM_RECONFIG_PHY_CELL_MAX_RS_EPRE_PRESENT)
            {
                p_phy_recfg_cell_parameters->bitmask |=
                    RRC_RECONFIG_PHY_MAX_RS_EPRE_PRESENT;
                p_phy_recfg_cell_parameters->max_rs_epre = 
                    p_recfg_cell_param->cell_reconfig.non_broadcast_info.cell_parameters.
                    max_rs_epre;
                ret_val = RRC_TRUE;
            }
            /* SPR 3802 Stop */
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
    return ret_val;
}

/*
 *   CEMH_STATE_W_FOR_PHY_RECONFIG_CELL_CNF state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_phy_reconf_cell_state_phy_cnf
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function continue cell reconfiguration if PHY is OK.
 *                  state CEMH_STATE_W_FOR_PHY_RECONFIG_CELL_CNF;
 *                  event CEMH_EVENT_PHY_RECONFIG_CELL_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_phy_reconf_cell_state_phy_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_phy_reconfig_cell_cnf_t       *p_phy_reconfig_cell_cnf =
        (rrc_phy_reconfig_cell_cnf_t *)p_msg;

    rrc_csc_llim_reconfig_cell_resp_t reconfig_cell_resp;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_phy_reconfig_cell_cnf);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(p_phy_reconfig_cell_cnf->cell_index == p_cell_ctx->cell_index);

    /* Precondition - should be the same transaction */
    if (rrc_get_transaction_id(p_api) != p_cell_ctx->transaction_id)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    if ((RRC_SUCCESS == p_phy_reconfig_cell_cnf->response) && 
            (RECONFIG_PHY_MAC == p_cell_ctx->recfg_mode))
    {
        /* 2. build and send to Mac*/
        cemh_build_send_rrc_mac_reconfig_cell_req(
                p_cell_ctx->p_recfg_req_param, 
                p_cell_ctx);

    }

    else if ((RRC_SUCCESS == p_phy_reconfig_cell_cnf->response) &&
            (RECONFIG_PHY_ONLY == p_cell_ctx->recfg_mode))
    {
        /* 1. Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);
        /* 2. Send CSC_LLIM_RECONFIG_CELL_RESP (SUCCESS) */
        memset_wrapper(&reconfig_cell_resp, 0, 
                sizeof (rrc_csc_llim_reconfig_cell_resp_t));
        reconfig_cell_resp.transaction_id = p_cell_ctx->transaction_id;
        reconfig_cell_resp.cell_index     = p_cell_ctx->cell_index;
        reconfig_cell_resp.response       = RRC_LLIM_RECONFIG_PHY_ONLY_MODE_SUCCESS;
        if (RRC_SUCCESS == rrc_llim_send_internal_msg( 
                    RRC_CSC_MODULE_ID, 
                    CSC_LLIM_RECONFIG_CELL_RESP,
                    sizeof (reconfig_cell_resp),
                    &reconfig_cell_resp))
        {
            /* STATE TRANSITION*/
            CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);
        }
        else
        {
            RRC_LLIM_TRACE(RRC_WARNING, "UNABLE TO SEND RESPONSE TO CSC");
        }
        if (p_cell_ctx->p_recfg_req_param != PNULL)
        {
            rrc_mem_free(p_cell_ctx->p_recfg_req_param);
            p_cell_ctx->p_recfg_req_param = PNULL;
        }
    }
    /*RECONFIG PHY FAILS*/
    else if ((RRC_SUCCESS != p_phy_reconfig_cell_cnf->response) &&
            (RECONFIG_PHY_ONLY == p_cell_ctx->recfg_mode))
    {
        /* 1. Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);
        /* 2. send CSC_LLIM_RECONFIG_CELL_RESP */
        if (RRC_SUCCESS == rrc_llim_send_csc_llim_reconfig_cell_resp_failure(
                    RRC_LLIM_RECONFIG_PHY_ONLY_MODE_FAIL,
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index))
        {
            /* STATE TRANSITION*/
            CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);
        }
        else
        {
            RRC_LLIM_TRACE(RRC_WARNING, "UNABLE TO SEND RESPONSE TO CSC");
        }
        if (p_cell_ctx->p_recfg_req_param != PNULL)
        {
            rrc_mem_free(p_cell_ctx->p_recfg_req_param);
            p_cell_ctx->p_recfg_req_param = PNULL;
        }
    }
    /*RECONFIG PHY FAILS*/
    else if ((RRC_SUCCESS != p_phy_reconfig_cell_cnf->response)&&
            (RECONFIG_PHY_MAC == p_cell_ctx->recfg_mode))
    {
        /*  Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);
        
        /*  send CSC_LLIM_RECONFIG_CELL_RESP */
        if (RRC_SUCCESS == rrc_llim_send_csc_llim_reconfig_cell_resp_failure(
                               RRC_LLIM_RECONFIG_PHY_MAC_MODE_PHY_FAIL,
                               p_cell_ctx->transaction_id,
                               p_cell_ctx->cell_index))
        {
            /* STATE TRANSITION*/
            CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);
        }
        else
        {
            RRC_LLIM_TRACE(RRC_WARNING, "UNABLE TO SEND RESPONSE TO CSC");
        }
        if (p_cell_ctx->p_recfg_req_param != PNULL)
        {
            rrc_mem_free(p_cell_ctx->p_recfg_req_param);
            p_cell_ctx->p_recfg_req_param = PNULL;
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/*
 *   CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_mac_conf_cell_state_mac_cnf
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function continue cell configuration if MAC is OK.
 *                  state CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF;
 *                  event CEMH_EVENT_MAC_CONFIG_CELL_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_mac_conf_cell_state_mac_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_mac_sfn_req_t   *p_mac_sfn_req = PNULL;
    rrc_mac_config_cell_cnf_t       *p_mac_config_cell_cnf =
        (rrc_mac_config_cell_cnf_t *)p_msg;
    rrc_csc_llim_config_cell_req_t  *p_cfg_cell_param = PNULL;
    rrc_phy_delete_cell_req_t phy_delete_cell_req;
    rrc_csc_llim_config_cell_resp_t config_cell_resp;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_mac_config_cell_cnf);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    if (PNULL != p_cell_ctx->p_cfg_cell_param)
    {
        p_cfg_cell_param = p_cell_ctx->p_cfg_cell_param;
    }


    p_mac_sfn_req = (rrc_mac_sfn_req_t*)rrc_mem_get(sizeof(rrc_mac_sfn_req_t));
    if(PNULL == p_mac_sfn_req)
    {
        RRC_LLIM_TRACE(RRC_FATAL,"\nMemeory allocation failed ");
        return;
    }
    memset_wrapper(p_mac_sfn_req, 0, sizeof(rrc_mac_sfn_req_t));

    p_mac_sfn_req->optional_elems_present |= RRC_MAC_TIMESTAMP_REQD_INFO_PRESENT;
    p_mac_sfn_req->timestamp_reqd.timestamp_reqd = RRC_TRUE;

    /* Precondition - should be the same transaction */
    if ((PNULL != p_cfg_cell_param) && 
        (rrc_get_transaction_id(p_api) != p_cfg_cell_param->transaction_id))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        /* coverity_fix_63300_start */
        rrc_mem_free((void *)p_mac_sfn_req);
        /* coverity_fix_63300_stop */
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    else if (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "p_rrc_llim_gl_ctx->p_p_cell_ctx is NULL");
        /* coverity_fix_63300_start */
        rrc_mem_free((void *)p_mac_sfn_req);
        /* coverity_fix_63300_stop */
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    switch (p_mac_config_cell_cnf->response)
    {
        case MAC_SUCCESS:
            p_cell_ctx->mac_retries_left = RRC_LLIM_CELL_NUM_MAX_MAC_TRIES;
            /* As the allowed value of explicit_start_required is 'zero' only,
             * we need to check for its presence in the global context 
             * by comparing it with RRC_NULL. This indicates that cell is to be
             * started EXPLICITLY. */
            if ((PNULL != p_cell_ctx->p_cfg_cell_param) && 
                (p_cell_ctx->p_cfg_cell_param->llim_cell_setup_req.presence_bitmask & 
                    CELL_SETUP_REQ_API_EXPLICIT_START_REQUIRED_FLAG))
            {
               if(PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index])
               {
                   /* As explicit Cell is required, Move to OUT OF SERVICE state.*/
                   p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index]->
                       explicit_start_required = RRC_LLIM_EXPLICIT_CELL_START;
               }

                /* Send CSC_LLIM_CONFIG_CELL_RESP (SUCCESS) */
                memset_wrapper(&config_cell_resp, 0, sizeof (config_cell_resp));

                if(PNULL != p_cfg_cell_param)
                {
                   config_cell_resp.transaction_id = p_cfg_cell_param->transaction_id;
                }
                config_cell_resp.bitmask        = 0;
                config_cell_resp.cell_index     = p_cell_ctx->cell_index;
                config_cell_resp.response       = RRC_SUCCESS;

                /* Do NOT set RRC_CSC_LLIM_SFN_SF_INFO_PRESENT bit as cell is out of service. */
                rrc_llim_send_internal_msg(
                        RRC_CSC_MODULE_ID, 
                        CSC_LLIM_CONFIG_CELL_RESP,
                        sizeof (config_cell_resp),
                        &config_cell_resp);

                /* Stop guard timer */
                cemh_stop_procedure_timer(p_cell_ctx);

                CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_CELL_OUT_OF_SERVICE);
            }
            else
            {
                /* As explicit Cell is NOT required, start the SFN sync with MAC.*/
                /* 1. send RRC_MAC_SFN_REQ */
                if(PNULL != p_cfg_cell_param)
                {
                    /* SPR 23218/SES-496 Fix Start */
                    p_cell_ctx->mac_curr_trans_id = p_cfg_cell_param->transaction_id;
                    /* SPR 23218/SES-496 Fix End */
                    /* coverity_fix_27662 start */            
                    if(RRC_SUCCESS != rrc_mac_il_send_rrc_mac_sfn_req(
                                p_mac_sfn_req, 
                                RRC_LLIM_MODULE_ID,
                                RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_cfg_cell_param->cell_index),
				    			p_cfg_cell_param->transaction_id,
                                p_cfg_cell_param->cell_index))
                    {
                        RRC_LLIM_TRACE(RRC_WARNING, "UNABLE TO SEND MAC_SFN_REQ TO MAC");
                        rrc_mem_free(p_mac_sfn_req);
                        RRC_LLIM_UT_TRACE_EXIT();
                        return;
                    }
                    /* coverity_fix_27662 stop */            
                }

                if(PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index])
                {
                    p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index]->
                        explicit_start_required = RRC_LLIM_AUTOMATIC_CELL_START;
                    CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_W_FOR_MAC_SFN_CNF);
                }
            }
            break; 

        case MAC_PARTIAL_SUCCESS:
            /* process partial success from MAC */
            /* if no more tries left move to default case, otherwise - break */
            if (0 != p_cell_ctx->mac_retries_left)
            {
                if(PNULL != p_cell_ctx->p_cfg_cell_param)
                {
                    cemh_build_and_send_rrc_mac_config_cell_req(
                        p_cell_ctx->p_cfg_cell_param);
                    /* eMTC changes stop */
                }
                p_cell_ctx->mac_retries_left--;
                break;
            }

        default:
            /* 1. Stop guard timer */
            cemh_stop_procedure_timer(p_cell_ctx);


            /* 2. send RRC_PHY_DELETE_CELL_REQ */
            /* Delete on PHY level: send RRC_PHY_DELETE_CELL_REQ */
            phy_delete_cell_req.cell_index = p_cell_ctx->cell_index;

            /* fix for MAC failure start */
            p_cell_ctx->current_proc = CEMH_CELL_SETUP_MAC_FAILED;

            /* A new Guard timer is to be started here for Cell Deletion */
            cemh_start_cell_delete_procedure_timer(
                    p_cell_ctx,
                    p_rrc_llim_gl_ctx->cell_delete_duration);
         
           /* fix for MAC failure end */
           /*coverity fix 55207 */ 
            if ((PNULL != p_cfg_cell_param) &&
           /*coverity fix 55207 */ 
                    (RRC_SUCCESS == rrc_phy_il_send_rrc_phy_delete_cell_req(
                        &phy_delete_cell_req,
                        RRC_LLIM_MODULE_ID,
                       	RRC_GET_LL_MODULE_ID(RRC_PHY_1_MODULE_ID,p_cell_ctx->cell_index),
					    p_cell_ctx->transaction_id,
                        p_cfg_cell_param->cell_index)))
            {
                if(PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->
                       cell_index])
                {
                    /* As Cell is not configured at MAC, Cell Delete request
                    * should not be sent to MAC*/
                    p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index]->
                        is_mac_cell_delete_reqd = RRC_FALSE;
                }

                /* 3. Changes state*/ 
                CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF);
            }
            else
            {
                RRC_LLIM_TRACE(RRC_WARNING, "Unable to send Cell Delete to PHY");
                /* 3. send CSC_LLIM_CONFIG_CELL_RESP */
                if(PNULL != p_cfg_cell_param)
                {
                    rrc_llim_send_csc_llim_config_cell_resp_failure(
                            RRC_LLIM_CONFIG_CELL_FAIL_MAC,
                            p_cfg_cell_param->transaction_id,
                            p_cell_ctx->cell_index);
                }

                if (p_cell_ctx->p_cfg_cell_param != PNULL)
                {
                    rrc_mem_free(p_cell_ctx->p_cfg_cell_param);
                    p_cell_ctx->p_cfg_cell_param = PNULL;
                }

                if(PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index])
                {
                    /* 5. delete from Cell DB and free */
                    p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index] = PNULL;
                }

                rrc_mem_free(p_cell_ctx);
            }
           break;
    }
    rrc_mem_free(p_mac_sfn_req);

    RRC_LLIM_UT_TRACE_EXIT();
}
/*
 *   CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_mac_reconf_cell_state_mac_cnf
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function continue cell configuration if MAC is OK.
 *                  state CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF;
 *                  event CEMH_EVENT_MAC_RECONFIG_CELL_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_mac_reconf_cell_state_mac_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_mac_reconfig_cell_cnf_t       *p_mac_reconfig_cell_cnf =
        (rrc_mac_reconfig_cell_cnf_t *)p_msg;

    rrc_csc_llim_reconfig_cell_resp_t reconfig_cell_resp;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_mac_reconfig_cell_cnf);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - should be the same transaction */
    if (rrc_get_transaction_id(p_api) != p_cell_ctx->transaction_id)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    if (CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ ==
            p_cell_ctx->msg_expected_against_event)
    {
        /* Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);

        /*Write replace warning going on*/
        build_and_send_wrw_resp_to_csc(p_cell_ctx,p_mac_reconfig_cell_cnf);

        /* STATE TRANSITION*/
        CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);

        p_cell_ctx->msg_expected_against_event = N_CEMH_EVENTS;
    }
    else if (CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ ==
            p_cell_ctx->msg_expected_against_event)
    {
        /* Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);

        /*SI without ETWS going on*/
        build_and_send_si_without_etws_resp_to_csc(p_cell_ctx,p_mac_reconfig_cell_cnf);

        /* STATE TRANSITION*/
        CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);
        p_cell_ctx->msg_expected_against_event = N_CEMH_EVENTS;
    }
    else
    {
        /*Cell Reconfiguration going on*/
        switch (p_mac_reconfig_cell_cnf->response)
        {
            case MAC_SUCCESS:
#ifdef LTE_EMBMS_SUPPORTED
            p_cell_ctx->mac_retries_left = RRC_LLIM_CELL_NUM_MAX_MAC_TRIES;
            if(RECONFIG_MAC_RLC == p_cell_ctx->recfg_mode) 
            {
                /* Check if any area is to be deleted, if yes, first send
                 * rlc delete area entity req */
                cemh_build_send_rrc_rlc_delete_area_req(
                        p_cell_ctx->p_recfg_req_param,
                        p_cell_ctx);
                if(CEMH_STATE_W_FOR_RLC_DELETE_AREA_ENTITY_CNF != CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx))
                {
                    /* no areas to delete so sending rlc create area request
                     * */

                    /* send rlc area create request */
                    cemh_build_send_rrc_rlc_create_area_req(
                            p_cell_ctx->p_recfg_req_param,
                            p_cell_ctx);
                }
                if((CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx)!= CEMH_STATE_W_FOR_RLC_CREATE_AREA_ENTITY_CNF) &&
                        (CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx)!= CEMH_STATE_W_FOR_RLC_DELETE_AREA_ENTITY_CNF))
                {
                    /* 1. Stop guard timer */
                    cemh_stop_procedure_timer(p_cell_ctx);

                    /* send CSC_LLIM_RECONFIG_CELL_RESP (SUCCESS) */
                    memset_wrapper(&reconfig_cell_resp, 0, sizeof (reconfig_cell_resp));
                    reconfig_cell_resp.response = 
                        RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS;
                    p_cell_ctx->mac_retries_left = RRC_LLIM_CELL_NUM_MAX_MAC_TRIES;

                    reconfig_cell_resp.transaction_id = p_cell_ctx->transaction_id;
                    reconfig_cell_resp.cell_index     = p_cell_ctx->cell_index;

                    if (RRC_SUCCESS == rrc_llim_send_internal_msg(
                                RRC_CSC_MODULE_ID, 
                                CSC_LLIM_RECONFIG_CELL_RESP,
                                sizeof (reconfig_cell_resp),
                                &reconfig_cell_resp))
                    {
                        /* STATE TRANSITION*/
                        CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);
                    }
                    if (p_cell_ctx->p_recfg_req_param != PNULL)
                    {
                        rrc_mem_free(p_cell_ctx->p_recfg_req_param);
                        p_cell_ctx->p_recfg_req_param = PNULL;
                    }
                }
            }
            else
            {
#endif

                /* 1. Stop guard timer */
                cemh_stop_procedure_timer(p_cell_ctx);

                /* send CSC_LLIM_RECONFIG_CELL_RESP (SUCCESS) */
                memset_wrapper(&reconfig_cell_resp, 0, sizeof (reconfig_cell_resp));
                if (RECONFIG_MAC_ONLY == p_cell_ctx->recfg_mode) 
                {
                    reconfig_cell_resp.response = 
                        RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS;
                }
                if (RECONFIG_PHY_MAC == p_cell_ctx->recfg_mode)
                {
                    reconfig_cell_resp.response =
                        RRC_LLIM_RECONFIG_PHY_MAC_MODE_SUCCESS;
                }
                p_cell_ctx->mac_retries_left = RRC_LLIM_CELL_NUM_MAX_MAC_TRIES;

                reconfig_cell_resp.transaction_id = p_cell_ctx->transaction_id;
                reconfig_cell_resp.cell_index     = p_cell_ctx->cell_index;

                if (RRC_SUCCESS == rrc_llim_send_internal_msg(
                            RRC_CSC_MODULE_ID, 
                            CSC_LLIM_RECONFIG_CELL_RESP,
                            sizeof (reconfig_cell_resp),
                            &reconfig_cell_resp))
                {
                  /* STATE TRANSITION*/
                  CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);
                }
                if (p_cell_ctx->p_recfg_req_param != PNULL)
                {
                    rrc_mem_free(p_cell_ctx->p_recfg_req_param);
                    p_cell_ctx->p_recfg_req_param = PNULL;
                }
#ifdef LTE_EMBMS_SUPPORTED
            }
#endif
            break;

            default:
            /* SPR 18715 Changes start */
#ifdef LTE_EMBMS_SUPPORTED
            if((RECONFIG_MAC_RLC == p_cell_ctx->recfg_mode) &&
                    (p_cell_ctx->p_recfg_req_param->cell_reconfig.broadcast_info.presence_bitmask & 
                     CELL_RECONFIG_REQ_MCCH_UPDATED_AREA_INFO_LIST) && (!(
                             p_cell_ctx->p_recfg_req_param->cell_reconfig.broadcast_info.presence_bitmask & 
                             CELL_RECONFIG_REQ_SIB_13_INFO_PRESENCE_FLAG)))
            {
                RRC_LLIM_TRACE(RRC_BRIEF,"Request is for stop sib 13 broadcast,"
                        "so ignore failure response from MAC and send"
                        "delete area request to RLC");
                cemh_build_send_rrc_rlc_delete_area_req(
                        p_cell_ctx->p_recfg_req_param,
                        p_cell_ctx);
                return;
            }
#endif
            /* SPR 18715 Changes End */

	    /* 1. Stop guard timer */
	    cemh_stop_procedure_timer(p_cell_ctx);

	    /* 2. Check Reconfig Mode */
	    if (RECONFIG_PHY_MAC == p_cell_ctx->recfg_mode)
	    {
	        /* send CSC_LLIM_RECONFIG_CELL_RESP */
		if (RRC_FAILURE == rrc_llim_send_csc_llim_reconfig_cell_resp_failure(
					    RRC_LLIM_RECONFIG_PHY_MAC_MODE_MAC_FAIL,
					    p_cell_ctx->transaction_id,
					    p_cell_ctx->cell_index))
		{
		   RRC_LLIM_TRACE(RRC_WARNING, 
				"Message with other transaction_id - "
				"ignored");
		   if (p_cell_ctx->p_recfg_req_param != PNULL)
		   {
		      rrc_mem_free(p_cell_ctx->p_recfg_req_param);
		      p_cell_ctx->p_recfg_req_param = PNULL;
		   }
		   RRC_LLIM_UT_TRACE_EXIT();
		   return;
		}
	    }
	    else    /* MAC ONLY mode */
	    {
	         /* send CSC_LLIM_RECONFIG_CELL_RESP */
	        if (RRC_FAILURE == rrc_llim_send_csc_llim_reconfig_cell_resp_failure(
					    RRC_LLIM_RECONFIG_MAC_ONLY_MODE_FAIL,
					    p_cell_ctx->transaction_id,
					    p_cell_ctx->cell_index))
	        {
	    	     RRC_LLIM_TRACE(RRC_WARNING, 
				    "Message with other transaction_id - "
				    "ignored");
		    if (p_cell_ctx->p_recfg_req_param != PNULL)
		    {
		       rrc_mem_free(p_cell_ctx->p_recfg_req_param);
			    p_cell_ctx->p_recfg_req_param = PNULL;
		    }
		    RRC_LLIM_UT_TRACE_EXIT();
		    return;
		}
	    }
	    /* STATE TRANSITION*/
	    CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);
	    if (p_cell_ctx->p_recfg_req_param != PNULL)
	    {
		rrc_mem_free(p_cell_ctx->p_recfg_req_param);
		p_cell_ctx->p_recfg_req_param = PNULL;
	    }
	    break;
        }
#ifdef LTE_EMBMS_SUPPORTED
        if((CEMH_STATE_W_FOR_RLC_CREATE_AREA_ENTITY_CNF !=
                    CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx)) &&
                (CEMH_STATE_W_FOR_RLC_DELETE_AREA_ENTITY_CNF !=
                 CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx))
          )
#endif        

        p_cell_ctx->recfg_mode = NO_RECONFIG_REQ;
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/*
 *   CEMH_STATE_W_FOR_MAC_SFN_CNF state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_mac_sfn_state_sfn_cnf
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function request sf and sfn from MAC if MAC Ok.
 *                  state CEMH_STATE_W_FOR_MAC_SFN_CNF; event CEMH_EVENT_MAC_SFN_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_mac_sfn_state_sfn_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_mac_sfn_cnf_t               *p_mac_sfn_cnf = (rrc_mac_sfn_cnf_t *)p_msg;
    /* SPR 23218/SES-496 Fix Start */
    /* SPR 23218/SES-496 Fix End */
    rrc_phy_delete_cell_req_t       phy_delete_cell_req;
    /* Coverity 210248 Fix Start */
    rrc_transaction_id_t            transaction_id;
    /* Coverity 210248 Fix End */
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_mac_sfn_cnf);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    /* SPR 23218/SES-496 Fix Start */
    /* SPR 23218/SES-496 Fix Start */
    /* Code Removed */
    transaction_id = p_cell_ctx->mac_curr_trans_id;
    /* SPR 23218/SES-496 Fix End */
    if (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "p_rrc_llim_gl_ctx->p_p_cell_ctx  is NULL - "
                "ignored");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Precondition - should be the same transaction */
    if (rrc_get_transaction_id(p_api) != transaction_id)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Stop guard timer */
    cemh_stop_procedure_timer(p_cell_ctx);

    if ((MAC_SUCCESS == p_mac_sfn_cnf->response) &&
            (p_mac_sfn_cnf->optional_elems_present &
             RRC_MAC_SFN_INFO_PRESENT) )
    {
        if (p_cell_ctx->explicit_start_required == RRC_LLIM_AUTOMATIC_CELL_START)
        {
            rrc_csc_llim_config_cell_resp_t config_cell_resp;

            /* send CSC_LLIM_CONFIG_CELL_RESP (SUCCESS) */
            memset_wrapper(&config_cell_resp, 0, sizeof (config_cell_resp));

            config_cell_resp.transaction_id = transaction_id;
            config_cell_resp.bitmask        = 0;
            config_cell_resp.cell_index     = p_cell_ctx->cell_index;
            config_cell_resp.response       = RRC_SUCCESS;

            /* sf has been removed and sfn stored properly */

            config_cell_resp.bitmask |= RRC_CSC_LLIM_SFN_INFO_PRESENT;
            config_cell_resp.sfn    = p_mac_sfn_cnf->sfn.sfn;

            /* store SF */
            if(p_mac_sfn_cnf->optional_elems_present & RRC_MAC_SF_INFO_PRESENT)
            {
                config_cell_resp.bitmask |= RRC_CSC_LLIM_SF_INFO_PRESENT;
                config_cell_resp.sf = p_mac_sfn_cnf->sf.sf;
            }
            /* store timestamp */
            if(p_mac_sfn_cnf->optional_elems_present & RRC_MAC_TIMESTAMP_INFO_PRESENT)
            {
                config_cell_resp.bitmask |= RRC_CSC_LLIM_TIMESTAMP_INFO_PRESENT;
                config_cell_resp.timestamp.ts_sec = p_mac_sfn_cnf->timestamp.ts_sec;
                config_cell_resp.timestamp.ts_nsec = p_mac_sfn_cnf->timestamp.ts_nsec;
                RRC_LLIM_TRACE(RRC_INFO, "mac_sfn_cnf->timestamp.ts_sec = %llu", p_mac_sfn_cnf->timestamp.ts_sec);
                RRC_LLIM_TRACE(RRC_INFO, "mac_sfn_cnf->timestamp.ts_nsec = %u", p_mac_sfn_cnf->timestamp.ts_nsec);
            }

            rrc_llim_send_internal_msg(
                    RRC_CSC_MODULE_ID, 
                    CSC_LLIM_CONFIG_CELL_RESP,
                    sizeof (config_cell_resp),
                    &config_cell_resp);
        }

        CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_CELL_IN_SERVICE);
    }
    else
    {

        memset_wrapper(&phy_delete_cell_req,0,sizeof(rrc_phy_delete_cell_req_t));

        /* Delete on PHY level: send
         * RRC_PHY_DELETE_CELL_REQ */
        phy_delete_cell_req.cell_index = p_cell_ctx->cell_index;

        /* A new Guard timer is to be started here for Cell Deletion */
        cemh_start_cell_delete_procedure_timer(
                p_cell_ctx,
                p_rrc_llim_gl_ctx->cell_delete_duration);

        if (RRC_SUCCESS != rrc_phy_il_send_rrc_phy_delete_cell_req(
                    &phy_delete_cell_req,
                    RRC_LLIM_MODULE_ID,
               		RRC_GET_LL_MODULE_ID(RRC_PHY_1_MODULE_ID,phy_delete_cell_req.cell_index),
					p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index))
        {
            RRC_LLIM_TRACE(RRC_WARNING, "Unable to send Cell Delete to PHY");
        }

        if(PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx[phy_delete_cell_req.
            cell_index])
        {
            p_rrc_llim_gl_ctx->p_p_cell_ctx[phy_delete_cell_req.cell_index]->
                is_mac_cell_delete_reqd = RRC_TRUE;
        }

        p_cell_ctx->current_proc = CEMH_CELL_SETUP_MAC_SFN_FAILED;

        /* Change state */
        CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF);
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_in_service_state_cleanup_req
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function cleanups CEMH FSM.
 *                  state CEMH_STATE_CELL_IN_SERVICE; event CEMH_EVENT_OAMH_CLEANUP_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_in_service_state_cleanup_req(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    RRC_LLIM_UT_TRACE_ENTER();

    /*coverity fix start*/
    /*coverity fix stop*/
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    RRC_LLIM_TRACE(RRC_DETAILED, "Unused variable p_api =%p p_msg=%p",p_api,p_msg);  /*SPR 17777 +-*/ 

    /* delete from Cell DB and free */
    if ((PNULL != p_cell_ctx) && (p_rrc_llim_gl_ctx->p_p_cell_ctx != PNULL))
    {
        p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index] = PNULL;

        rrc_mem_free(p_cell_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/*
 *   CEMH_STATE_CELL_IN_SERVICE state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_in_service_state_csc_llim_mac_sfn_sync_req
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function send MAC SFN request message to the MAC
 *                  state CEMH_STATE_CELL_IN_SERVICE;
 *                  event CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_in_service_state_csc_llim_mac_sfn_sync_req(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_mac_sfn_req_t *p_mac_sfn_req = PNULL;
    rrc_csc_llim_config_cell_req_t  *p_cfg_cell_param = PNULL;
        p_cfg_cell_param = p_cell_ctx->p_cfg_cell_param;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* SPR 20636 Changes Start*/
    RRC_LLIM_TRACE(RRC_DETAILED, "Unused variable in fsm p_api=%p, p_msg=%p"
            " and llim_gl_ctx=%x", p_api, p_msg,(U32_64_Bit) p_rrc_llim_gl_ctx);   
    /* SPR 20636 Changes End*/

    p_mac_sfn_req = (rrc_mac_sfn_req_t*)rrc_mem_get(sizeof(rrc_mac_sfn_req_t));
    if(PNULL == p_mac_sfn_req)
    {
        RRC_LLIM_TRACE(RRC_FATAL,"\nMemeory allocation failed ");
        return;
    }
    memset_wrapper (p_mac_sfn_req, 0, sizeof(rrc_mac_sfn_req_t));

    p_mac_sfn_req->optional_elems_present |= RRC_MAC_TIMESTAMP_REQD_INFO_PRESENT;
    p_mac_sfn_req->timestamp_reqd.timestamp_reqd = RRC_TRUE;
    {
        /* SPR 23218/SES-496 Fix Start */
        p_cell_ctx->mac_curr_trans_id = p_cfg_cell_param->transaction_id;
        /* SPR 23218/SES-496 Fix End */
        /* 1. send RRC_MAC_SFN_REQ */
        if (RRC_SUCCESS != rrc_mac_il_send_rrc_mac_sfn_req(
                    p_mac_sfn_req, 
                    RRC_LLIM_MODULE_ID,
                    RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_cfg_cell_param->cell_index),
                    p_cfg_cell_param->transaction_id,
                    p_cfg_cell_param->cell_index))
        {
            RRC_LLIM_TRACE(RRC_WARNING, "[RRC_MAC_SFN_REQ] Delivery failed");
        }
    }
    rrc_mem_free(p_mac_sfn_req);

    RRC_LLIM_UT_TRACE_EXIT();
}

/*
 *   CEMH_STATE_CELL_IN_SERVICE state handlers
 */
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_in_service_state_mac_sfn_cnf
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *   
 *   DESCRIPTION  : This function gets sf and sfn from MAC if MAC Ok and
 *                  sends CSC_LLIM_SFN_IND message to CSC.
 *                  state CEMH_STATE_CELL_IN_SERVICE; event CEMH_EVENT_MAC_SFN_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_in_service_state_mac_sfn_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_mac_sfn_cnf_t *p_mac_sfn_cnf = (rrc_mac_sfn_cnf_t *)p_msg;
    /* SPR 23218/SES-496 Coverity 210247 Fix Start */
    rrc_transaction_id_t            transaction_id;
    /* SPR 23218/SES-496 Coverity 210247 Fix End */

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_mac_sfn_cnf);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* SPR 23218/SES-496 Fix Start */
    transaction_id = p_cell_ctx->mac_curr_trans_id;
    /* Precondition - should be the same transaction */
    if (rrc_get_transaction_id(p_api) != transaction_id)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    /* SPR 23218/SES-496 Fix End */
    RRC_LLIM_TRACE(RRC_DETAILED,
        "Unused variable in fsm p_api=%p and llim_gl_ctx=%p", p_api, p_rrc_llim_gl_ctx);

    if ((MAC_SUCCESS == p_mac_sfn_cnf->response) &&
            (p_mac_sfn_cnf->optional_elems_present &
             RRC_MAC_SFN_INFO_PRESENT))
    {
        rrc_csc_llim_sfn_resp_t sfn_resp;

        /* 2. send CSC_LLIM_SFN_RESP */
        memset_wrapper(&sfn_resp, 0, sizeof (rrc_csc_llim_sfn_resp_t));

        sfn_resp.cell_index = p_cell_ctx->cell_index;
        /* sf has been removed and sfn stored properly */
        sfn_resp.sfn = p_mac_sfn_cnf->sfn.sfn;
        sfn_resp.sf = p_mac_sfn_cnf->sf.sf;
        sfn_resp.timestamp.ts_sec = p_mac_sfn_cnf->timestamp.ts_sec;
        sfn_resp.timestamp.ts_nsec = p_mac_sfn_cnf->timestamp.ts_nsec;



        rrc_llim_send_internal_msg( 
                RRC_CSC_MODULE_ID, 
                CSC_LLIM_SFN_RESP,
                sizeof (sfn_resp), 
                &sfn_resp);
    }
    else
    {
        RRC_LLIM_TRACE(RRC_WARNING, "[RRC_MAC_SFN_CNF] failed");
    }
    RRC_LLIM_UT_TRACE_EXIT();
}


/******************************************************************************
*   FUNCTION NAME: cemh_fill_si_msg_req
*   INPUT        : rrc_mac_si_msg_req_t  *p_mac_si_msg_req
*                  rrc_si_msg_req_t      *p_si_msg_req
*   OUTPUT       : none
*   DESCRIPTION  : This function fills the SI Msg Req
*
*   RETURNS      : None
*
******************************************************************************/
static void cemh_fill_si_msg_req(
   rrc_mac_si_msg_req_t  *p_mac_si_msg_req,  /* Destination */
   rrc_si_msg_req_t      *p_si_msg_req)      /* Source */
{
    U8   si_msg_count = 0;
    U8   si_segment_count = 0;
    U16  si_segment_index = 0;
    U16  si_segment_length = 0;
    U8   si_msg_param_count = 0;
    U8   si_msg_offset_count = 0;
 
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT (PNULL != p_mac_si_msg_req);
    RRC_ASSERT (PNULL != p_si_msg_req);

    /* Fill si_window_size*/
    p_mac_si_msg_req->si_window_size = p_si_msg_req->si_window_size;

    /* Fill number of si messages */
    p_mac_si_msg_req->num_si_message = p_si_msg_req->num_si_message;
    p_mac_si_msg_req->si_msg_info_counter = p_si_msg_req->num_si_message;

    /* Fill si_msg_info */
    for (si_msg_count = 0; si_msg_count < p_mac_si_msg_req->si_msg_info_counter;
          ++si_msg_count)
    {
         /* Resetting the bitmask */
         p_mac_si_msg_req->si_msg_info[si_msg_count].optional_elems_present = 0;

         /* Fill SI Index */
         p_mac_si_msg_req->si_msg_info[si_msg_count].si_index =
                p_si_msg_req->si_msg_info[si_msg_count].si_index;

         /* Fill Periodicity */
         p_mac_si_msg_req->si_msg_info[si_msg_count].periodicity =
                p_si_msg_req->si_msg_info[si_msg_count].periodicity;

         /* Fill Starting SFN */
         p_mac_si_msg_req->si_msg_info[si_msg_count].starting_sfn =
                p_si_msg_req->si_msg_info[si_msg_count].starting_sfn;

         /* Fill Starting SF */
         p_mac_si_msg_req->si_msg_info[si_msg_count].starting_sf =
                p_si_msg_req->si_msg_info[si_msg_count].starting_sf;

         /* Fill number of repetitions  */
         if (RRC_NULL != p_si_msg_req->si_msg_info[si_msg_count].num_of_repetitions)
         {
             p_mac_si_msg_req->si_msg_info[si_msg_count].optional_elems_present |=
                RRC_NUM_SI_REPETITIONS_PRESENT;
             p_mac_si_msg_req->si_msg_info[si_msg_count].num_si_repetitions.\
                num_of_repetitions = p_si_msg_req->si_msg_info[si_msg_count].
                              num_of_repetitions;
         }

         /* Fill number of transmission  */
         if (RRC_NULL != p_si_msg_req->si_msg_info[si_msg_count].num_of_transmission)
         {
             p_mac_si_msg_req->si_msg_info[si_msg_count].optional_elems_present |=
                RRC_NUM_SI_TRANSMISSIONS_PRESENT;
             p_mac_si_msg_req->si_msg_info[si_msg_count].num_si_transmissions.\
                num_of_transmissions = p_si_msg_req->si_msg_info[si_msg_count].
                              num_of_transmission;
         }
         /* Fill si start offset */
         /* Fill number of offset */
         if (p_si_msg_req->si_msg_info[si_msg_count].bitmask &
		        RRC_CSC_LLIM_SI_START_OFFSET_INFO_PRESENT)
	     {
         	p_mac_si_msg_req->si_msg_info[si_msg_count].optional_elems_present |=
                RRC_NUM_SI_START_OFFSET_PRESENT;

         	for ( si_msg_offset_count = 0; si_msg_offset_count < MAX_SI_START_OFFSET; 
			si_msg_offset_count++)
         	{
                p_mac_si_msg_req->si_msg_info[si_msg_count].si_start_offset_info.
			        si_start_offset[si_msg_offset_count] =
             	p_si_msg_req->si_msg_info[si_msg_count].
			        si_start_offset.si_start_offset[si_msg_offset_count];
         	}
	     }
         
         /* Fill number of segments  */
         p_mac_si_msg_req->si_msg_info[si_msg_count].num_of_segment =
                 p_si_msg_req->si_msg_info[si_msg_count].num_of_segment;
         p_mac_si_msg_req->si_msg_info[si_msg_count].segment_counter =
                 p_si_msg_req->si_msg_info[si_msg_count].num_of_segment;

         /* Fill SI segment data and length */
         for (si_segment_count = 0; si_segment_count < p_mac_si_msg_req->
                si_msg_info[si_msg_count].num_of_segment && 
                si_segment_count < MAX_SI_SEGMENTS; ++si_segment_count)
         {
              if( si_segment_count < MAX_SI_SEGMENTS )
              {
                  si_segment_index = p_si_msg_req->si_msg_info[si_msg_count].
                      si_segment_data[si_segment_count].starting_offset;
                  if (si_segment_count == (p_mac_si_msg_req->
                              si_msg_info[si_msg_count].num_of_segment - 1))
                  {
                      si_segment_length = (U16)(p_si_msg_req->si_msg_info[si_msg_count].
                              si_length - si_segment_index);
                  }
                  else
                  {
                      if ( si_segment_count < (MAX_SI_SEGMENTS - 1))
                      {
                          si_segment_length = (U16)(p_si_msg_req->si_msg_info[si_msg_count].
                              si_segment_data[si_segment_count+1].starting_offset - si_segment_index);
                      }
                  }
                  p_mac_si_msg_req->si_msg_info[si_msg_count].si_segment_data[si_segment_count].
                      si_msg_buf_length = si_segment_length;
                  l3_memcpy_wrapper (p_mac_si_msg_req->si_msg_info[si_msg_count].
                          si_segment_data[si_segment_count].si_msg_buf,
                          p_si_msg_req->si_msg_buf + si_segment_index,
                          si_segment_length);
              }
         }

         /*Reset the si_msg_param_counter*/
         p_mac_si_msg_req->si_msg_info[si_msg_count].si_msg_param_counter = 
              p_si_msg_req->si_msg_info[si_msg_count].si_msg_param_counter;
         for ( si_msg_param_count = 0; si_msg_param_count < p_mac_si_msg_req->
                      si_msg_info[si_msg_count].si_msg_param_counter; ++si_msg_param_count)
         {
            p_mac_si_msg_req->si_msg_info[si_msg_count].si_msg_param[si_msg_param_count] =
                     p_si_msg_req->si_msg_info[si_msg_count].si_msg_param[si_msg_param_count];
         }
         
         /*Assign the rrc_mac_sched_reqd_for_sib8*/
         if (p_si_msg_req->si_msg_info[si_msg_count].bitmask &
		        RRC_CSC_LLIM_TIMESTAMP_REQUIRED_PRESENT)
         {
           	 p_mac_si_msg_req->si_msg_info[si_msg_count].optional_elems_present |=
                RRC_SCHED_REQD_FOR_SIB8_PRESENT;
             p_mac_si_msg_req->si_msg_info[si_msg_count].rrc_mac_sched_reqd_for_sib8.sched_reqd_for_sib8
                                 = p_si_msg_req->si_msg_info[si_msg_count].rrc_mac_sched_reqd_for_sib8;
         }
    }
}


/*
 *   CEMH FSM implementation
 */

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_ignored_event
 *   INPUT        : void                *param1
 *                  void                *param2
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Handler of ignored events.
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_ignored_event(
        void                *param1,            /* p_api or timer_id */
        void                *param2,            /* p_msg or p_cemh_timer_buf */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);

    RRC_LLIM_TRACE(RRC_DETAILED, "Unused variable in fsm param1=%p, param2=%p"
            " and llim_gl_ctx=%p", param1, param2, p_rrc_llim_gl_ctx);


    RRC_LLIM_TRACE(RRC_WARNING, "[%s] invalid event received in state [%s]",
            RRC_CEMH_FSM_NAME,
            CEMH_FSM_STATES_NAMES[CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx)]);

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_phy_cell_start_req 
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function processes phy cell start request.
 *                  state CEMH_STATE_CELL_OUT_OF_SERVICE; 
 *                  event CEMH_EVENT_PHY_CELL_START_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_phy_cell_start_req(    
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM 
                                                    global context data */
{
    rrc_csc_llim_cell_start_req_t *p_llim_cell_start_req = 
        (rrc_csc_llim_cell_start_req_t *)p_msg;

    rrc_phy_cell_start_req_t  *p_rrc_phy_cell_start_req;

    RRC_LLIM_TRACE(RRC_DETAILEDALL,
                        "Unused variable in fsm p_api=%p", p_api);/*SPR 17777 +-*/
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_llim_cell_start_req);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(p_llim_cell_start_req->cell_index == p_cell_ctx->cell_index);

    p_cell_ctx->transaction_id = p_llim_cell_start_req->transaction_id;

    /* A new Guard timer is to be started here for Cell START Procedure */
    cemh_start_cell_start_procedure_timer(
            p_cell_ctx,
            p_rrc_llim_gl_ctx->cell_start_duration);

    /* Send PHY Start request */

    p_rrc_phy_cell_start_req =(rrc_phy_cell_start_req_t *)rrc_mem_get(sizeof(rrc_phy_cell_start_req_t));

    if (PNULL != p_rrc_phy_cell_start_req)
    {
        memset_wrapper(p_rrc_phy_cell_start_req,0,sizeof(rrc_phy_cell_start_req_t));

        p_rrc_phy_cell_start_req->cell_index = p_cell_ctx->cell_index;

        rrc_phy_il_send_rrc_phy_cell_start_req(
                p_rrc_phy_cell_start_req,
                RRC_LLIM_MODULE_ID,
                	RRC_GET_LL_MODULE_ID(RRC_PHY_1_MODULE_ID,p_rrc_phy_cell_start_req->cell_index),
				p_cell_ctx->transaction_id,
                p_llim_cell_start_req->cell_index);


        CEMH_FSM_SET_STATE(p_cell_ctx, 
                CEMH_STATE_W_FOR_PHY_CELL_START_CNF);

	/* Coverity Fix 63298 Start */
	if ( PNULL != p_rrc_phy_cell_start_req )
	{   
	    rrc_mem_free(p_rrc_phy_cell_start_req);
	    p_rrc_phy_cell_start_req = PNULL;
	}
	/* Coverity Fix 63298 Stop */
    }

    RRC_LLIM_UT_TRACE_EXIT();

}


/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_phy_cell_start_cnf 
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function prrocesses phy cell start response.
 *                  state CEMH_STATE_W_FOR_PHY_CELL_START_CNF; event CEMH_EVENT_PHY_CELL_START_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_phy_cell_start_cnf(    
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM 
                                                    global context data */
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    RRC_LLIM_TRACE(RRC_DETAILED, "Unused variable in fsm p_api=%p ,p_msg=%p"
            "and llim_gl_ctx=%p", p_api, p_msg, p_rrc_llim_gl_ctx);     
    /* Build and send the mac_cell_start_req */
    cemh_build_and_send_mac_cell_start_req(
            p_cell_ctx->transaction_id,
            p_cell_ctx->cell_index);

    /* Change the state */
    CEMH_FSM_SET_STATE(p_cell_ctx, 
            CEMH_STATE_W_FOR_MAC_CELL_START_CNF);

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_mac_cell_start_cnf 
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function prrocesses mac cell start response.
 *                  state CEMH_STATE_W_FOR_MAC_CELL_START_CNF; event CEMH_EVENT_MAC_CELL_START_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_mac_cell_start_cnf(    
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM 
                                                    global context data */
{
    rrc_mac_cell_start_cnf_t *p_mac_cell_start =
        (rrc_mac_cell_start_cnf_t *)p_msg;

    /*cdma_fix bug 10186 start*/
    rrc_mac_sfn_req_t   *p_mac_sfn_req = PNULL;
    rrc_timestamp_t     ts = {0};
    /*cdma_fix bug 10186 stop*/

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_mac_cell_start);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx);
/* SPR:5546 start */    
    /* Stop guard timer */
    /*cdma_fix bug 10186 start*/
    /*stop procedure timer later after doing SFN,SF & TS synching with L2*/
    /*cemh_stop_procedure_timer(p_cell_ctx); */
    /*cdma_fix bug 10186 stop*/
/* SPR:5546 stop */    
    if ((MAC_SUCCESS == p_mac_cell_start->response) &&
            (p_mac_cell_start->optional_elems_present &
             RRC_MAC_CELL_START_CNF_SFN_INFO_PRESENT) )
    {
        /*cdma_fix bug 10186 start*/
        /*Taking SFN, SF, Timestamp from L2*/
        RRC_LLIM_TRACE(RRC_DETAILED,"\nSending MAC_SFN_REQ\n");
        p_mac_sfn_req = (rrc_mac_sfn_req_t*)rrc_mem_get(sizeof(rrc_mac_sfn_req_t));
        if(PNULL == p_mac_sfn_req)
        {
            RRC_LLIM_TRACE(RRC_FATAL,"\nMemeory allocation failed; unused variable p_api = %p p_rrc_llim_gl_ctx=%p",p_api,p_rrc_llim_gl_ctx);/*SPR 17777 +-*/
            return;
        }
        memset_wrapper(p_mac_sfn_req, 0, sizeof(rrc_mac_sfn_req_t));

        p_mac_sfn_req->optional_elems_present |= RRC_MAC_TIMESTAMP_REQD_INFO_PRESENT;
        p_mac_sfn_req->timestamp_reqd.timestamp_reqd = RRC_TRUE;

        /* SPR 23218/SES-496 Fix Start */
        p_cell_ctx->mac_curr_trans_id = p_cell_ctx->transaction_id;
        /* SPR 23218/SES-496 Fix End */
        if(RRC_SUCCESS != rrc_mac_il_send_rrc_mac_sfn_req(
                    p_mac_sfn_req,
                    RRC_LLIM_MODULE_ID,
                    RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_cell_ctx->cell_index),
					p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index))
        {
            RRC_LLIM_TRACE(RRC_WARNING, "UNABLE TO SEND MAC_SFN_REQ TO MAC");
            rrc_mem_free(p_mac_sfn_req);
            RRC_LLIM_UT_TRACE_EXIT();
            return;
        }

        rrc_mem_free(p_mac_sfn_req);

        /*cdma_fix  bug 10186 stop*/
    }
    else
    {
/*SPR 13817 Fix Start*/	
        cemh_stop_procedure_timer(p_cell_ctx);
/*SPR 13817 Fix Stop*/	
        rrc_llim_send_csc_llim_cell_start_resp(
                p_cell_ctx->transaction_id,
                p_cell_ctx->cell_index,
                RRC_LLIM_CELL_START_MAC_FAIL,
                RRC_NULL,
                /*cdma_fix  bug 10186 start*/
                RRC_NULL,
                ts
                /*cdma_fix  bug 10186 stop*/
                );

        CEMH_FSM_SET_STATE(p_cell_ctx, 
                CEMH_STATE_CELL_OUT_OF_SERVICE);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}          

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_phy_cell_stop_req 
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function processes phy cell stop request.
 *                  state CEMH_STATE_CELL_IN_SERVICE; 
 *                  event CEMH_EVENT_PHY_CELL_STOP_REQ
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_phy_cell_stop_req(    
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM 
                                                    global context data */
{
    rrc_csc_llim_cell_stop_req_t *p_llim_cell_stop_req =
  (rrc_csc_llim_cell_stop_req_t *)p_msg;
    RRC_LLIM_TRACE(RRC_DETAILEDALL,
                        "Unused variable in fsm p_api=%p", p_api);/*SPR 17777 +-*/


    rrc_phy_cell_stop_req_t *p_phy_cell_stop_req;

    RRC_LLIM_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_llim_cell_stop_req);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(p_llim_cell_stop_req->cell_index == p_cell_ctx->cell_index);


    p_cell_ctx->transaction_id = p_llim_cell_stop_req->transaction_id;

    /* A new Guard timer is to be started here for Cell STOP Procedure */
    cemh_start_cell_stop_procedure_timer(
            p_cell_ctx,
            p_rrc_llim_gl_ctx->cell_stop_duration);

    p_phy_cell_stop_req =(rrc_phy_cell_stop_req_t *)
                            rrc_mem_get(sizeof(rrc_phy_cell_stop_req_t));

    if (PNULL != p_phy_cell_stop_req)
    {
        memset_wrapper(p_phy_cell_stop_req,0,sizeof(rrc_phy_cell_stop_req_t));

        p_phy_cell_stop_req->cell_index = p_cell_ctx->cell_index;

        /* Send PHY Stop request */
        rrc_phy_il_send_rrc_phy_cell_stop_req(
                p_phy_cell_stop_req,
                RRC_LLIM_MODULE_ID,
                RRC_GET_LL_MODULE_ID(RRC_PHY_1_MODULE_ID,p_phy_cell_stop_req->cell_index),
		p_cell_ctx->transaction_id,
                p_llim_cell_stop_req->cell_index);

        CEMH_FSM_SET_STATE(p_cell_ctx, 
                CEMH_STATE_W_FOR_PHY_CELL_STOP_CNF);
    
        /* Coverity Fix 63299 Start */
        if ( PNULL != p_phy_cell_stop_req )
	    {
	        rrc_mem_free(p_phy_cell_stop_req);
    	    p_phy_cell_stop_req = PNULL;
	    }
        /* Coverity Fix 63299 Stop */
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_phy_cell_stop_cnf 
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *   
 *   DESCRIPTION  : This function prrocesses phy cell stop response.
 *                  state CEMH_STATE_W_FOR_PHY_CELL_STOP_CNF; event CEMH_EVENT_PHY_CELL_STOP_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_phy_cell_stop_cnf(    
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM 
                                                    global context data */
{
    rrc_return_et   result = RRC_SUCCESS;
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
 
    RRC_LLIM_TRACE(RRC_DETAILEDALL, 
            "Unused variable in fsm p_api=%p and p_msg=%p p_rrc_llim_gl_ctx=%p",
            p_api, p_msg,p_rrc_llim_gl_ctx);/*SPR 17777 +-*/ 
    /* Build and send the mac_cell_stop_req */
    result = cemh_build_and_send_mac_cell_stop_req(
            p_cell_ctx->transaction_id,
            p_cell_ctx->cell_index);

    /* Change the state */
    CEMH_FSM_SET_STATE(p_cell_ctx,
            CEMH_STATE_W_FOR_MAC_CELL_STOP_CNF);

    if (result == RRC_FAILURE)
    {
        /* Guard Timer stops as the cell fails to stop*/

        /* Stop guard timer */
        cemh_stop_procedure_timer(p_cell_ctx);

       /* Send the failure response to CSC */ 
        rrc_llim_send_csc_llim_cell_stop_resp(
                p_cell_ctx->transaction_id,
                p_cell_ctx->cell_index,
                RRC_LLIM_INTERNAL_ERROR);

        /* Change the state */
        CEMH_FSM_SET_STATE(p_cell_ctx,
                CEMH_STATE_CELL_OUT_OF_SERVICE);

        RRC_LLIM_TRACE(RRC_WARNING, "[RRC_PHY_CELL_STOP_CNF]: failed ");
    }
    
    RRC_LLIM_UT_TRACE_EXIT();

}


/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_phy_cell_stop_timer_expiry
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function prrocesses phy cell stop timer expiry response.
 *          state : CEMH_STATE_W_FOR_PHY_CELL_STOP_CNF
 *          event : CEMH_EVENT_CELL_STOP_TIMER_EXPIRED 
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_phy_cell_stop_timer_expiry(    
    void                *p_api,             /* p_api */
    void                *p_msg,             /* p_msg */
    rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM 
                                                global context data */
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    RRC_LLIM_TRACE(RRC_DETAILEDALL, 
            "Unused variable in fsm p_msg=%p p_api=%p p_rrc_llim_gl_ctx=%p", p_msg,p_api,p_rrc_llim_gl_ctx);/*SPR 17777 +-*/ 
    /* Send the failure response to CSC */ 
    rrc_llim_send_csc_llim_cell_stop_resp(
            p_cell_ctx->transaction_id,
            p_cell_ctx->cell_index,
            RRC_LLIM_CELL_STOP_PHY_FAIL);

    /* Change the state */
    CEMH_FSM_SET_STATE(p_cell_ctx,
            CEMH_STATE_CELL_OUT_OF_SERVICE);

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */

    RRC_LLIM_TRACE(RRC_WARNING, "[RRC_PHY_CELL_STOP_CNF]: Timer Expired ");

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_mac_cell_stop_cnf 
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function prrocesses mac cell stop response.
 *          state : CEMH_STATE_W_FOR_MAC_CELL_STOP_CNF
 *          event : CEMH_EVENT_MAC_CELL_STOP_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_mac_cell_stop_cnf(    
    void                *p_api,             /* p_api */
    void                *p_msg,             /* p_msg */
    rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM 
                                                global context data */
{
  RRC_LLIM_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_api);
  RRC_ASSERT(PNULL != p_cell_ctx);
    /*coverity fix start*/
    /*coverity fix stop*/

 RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

  RRC_LLIM_TRACE(RRC_DETAILEDALL,
        "Unused variable in fsm p_msg=%p p_api =%p p_rrc_llim_gl_ctx=%p", p_msg,p_api,p_rrc_llim_gl_ctx);/*SPR 17777 +-*/ 

  /* Send the success response to CSC */
  rrc_llim_send_csc_llim_cell_stop_resp(
      p_cell_ctx->transaction_id,
      p_cell_ctx->cell_index,
      RRC_LLIM_CELL_STOP_SUCCESS);

  /* Guard Timer stops */
  cemh_stop_procedure_timer(p_cell_ctx);

/* SPR 5145 Start */
  p_cell_ctx->explicit_start_required = RRC_LLIM_EXPLICIT_CELL_START;
/* SPR 5145 End */

  /* Change the state */
  CEMH_FSM_SET_STATE(p_cell_ctx,
      CEMH_STATE_CELL_OUT_OF_SERVICE);
/* SPR:5540 start */
  RRC_LLIM_TRACE(RRC_BRIEF, "[RRC_MAC_CELL_STOP_CNF]: Success ");
/* SPR:5540 stop */

  RRC_LLIM_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_mac_cell_stop_timer_expiry
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function prrocesses mac cell stop timer expiry response.
 *          state : CEMH_STATE_W_FOR_MAC_CELL_STOP_CNF
 *          event : CEMH_EVENT_CELL_STOP_TIMER_EXPIRED 
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_mac_cell_stop_timer_expiry(
    void                *p_api,             /* p_api */
    void                *p_msg,             /* p_msg */
    rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM
                                                global context data */
{

  RRC_LLIM_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_api);
  RRC_ASSERT(PNULL != p_cell_ctx);
  RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

  RRC_LLIM_TRACE(RRC_DETAILEDALL,
        "Unused variable in fsm p_msg=%p p_api=%p p_rrc_llim_gl_ctx=%p", p_msg,p_api,p_rrc_llim_gl_ctx); /*SPR 17777 +-*/ 

  /* Send the failure response to CSC */
  rrc_llim_send_csc_llim_cell_stop_resp(
      p_cell_ctx->transaction_id,
      p_cell_ctx->cell_index,
      RRC_LLIM_CELL_STOP_MAC_FAIL);

  /* Changes state to CELL_OUT_OF_SERVICE */
  CEMH_FSM_SET_STATE(p_cell_ctx,
      CEMH_STATE_CELL_OUT_OF_SERVICE);

  /* SPR 23608/SES-43 Fix Start */
  if (PNULL != p_cell_ctx->guard_timer)
  {
      RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
      /* Stop Timer */
      rrc_stop_timer(p_cell_ctx->guard_timer);
      p_cell_ctx->guard_timer = PNULL;
  }
  /* SPR 23608/SES-43 Fix End */

  RRC_LLIM_TRACE(RRC_WARNING, "[RRC_MAC_CELL_STOP_CNF]: Timer Expired ");

  RRC_LLIM_UT_TRACE_EXIT();

}


/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_phy_cell_start_timer_expiry
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function processes phy cell start timer expiry response.
 *          state : CEMH_STATE_W_FOR_PHY_CELL_START_CNF 
 *          event : CEMH_EVENT_CELL_START_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_phy_cell_start_timer_expiry(
    void                *p_api,             /* p_api */
    void                *p_msg,             /* p_msg */
    rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM
                                                global context data */
{
  rrc_return_et   result = RRC_SUCCESS;
  /*cdma_fix bug 10186 start*/
  rrc_timestamp_t ts = {0};
  /*cdma_fix  bug 10186 stop*/

  RRC_LLIM_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_api);
  RRC_ASSERT(PNULL != p_cell_ctx);
  RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

  RRC_LLIM_TRACE(RRC_DETAILEDALL, 
        "Unused variable in fsm p_msg=%p, p_api=%p p_rrc_llim_gl_ctx=%p", p_msg,p_api,p_rrc_llim_gl_ctx); /*SPR 17777 +-*/ 

  /* SPR 23608/SES-43 Fix Start */
  if (PNULL != p_cell_ctx->guard_timer)
  {
      RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
      /* Stop Timer */
      rrc_stop_timer(p_cell_ctx->guard_timer);
      p_cell_ctx->guard_timer = PNULL;
  }
  /* SPR 23608/SES-43 Fix End */


  /* Send the failure response to CSC */
  result = rrc_llim_send_csc_llim_cell_start_resp(
      p_cell_ctx->transaction_id,
      p_cell_ctx->cell_index,
      RRC_LLIM_CELL_START_PHY_FAIL,
      RRC_NULL,
      /*cdma_fix  bug 10186 start*/
      RRC_NULL,
      ts
      /*cdma_fix  bug 10186 stop */
      );

  /* Change the state */
  CEMH_FSM_SET_STATE(p_cell_ctx,
      CEMH_STATE_CELL_OUT_OF_SERVICE);

  if(RRC_FAILURE == result)
  {
    RRC_LLIM_TRACE(RRC_WARNING, "RRC_LLIM_CELL_START_PHY_FAIL unable to send to CSC");
  }
  else
  {
    RRC_LLIM_TRACE(RRC_WARNING, "RRC_LLIM_CELL_START_PHY_FAIL sent to CSC");
  }

  RRC_LLIM_TRACE(RRC_WARNING, "[RRC_PHY_CELL_START_CNF]: Timer Expired ");

  RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_mac_cell_start_timer_expiry
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function processes mac cell start timer expiry response.
 *          state : CEMH_STATE_W_FOR_MAC_CELL_START_CNF 
 *          event : CEMH_EVENT_CELL_START_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_mac_cell_start_timer_expiry(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)  /* Pointer to the LLIM
                                                    global context data */
{
    rrc_return_et   result = RRC_SUCCESS;
    /*cdma_fix  bug 10186 start*/
    rrc_timestamp_t ts = {0};
    /*cdma_fix  bug 10186 stop*/

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    RRC_LLIM_TRACE(RRC_DETAILEDALL,
        "Unused variable in fsm p_msg=%p p_api =%p p_rrc_llim_gl_ctx=%p", p_msg,p_api,p_rrc_llim_gl_ctx); /*SPR 17777 +-*/

    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */

    /* Send the failure response to CSC */
    result = rrc_llim_send_csc_llim_cell_start_resp(
            p_cell_ctx->transaction_id,
            p_cell_ctx->cell_index,
            RRC_LLIM_CELL_START_MAC_FAIL,
            RRC_NULL,
            /*cdma_fix  bug 10186 start*/
            RRC_NULL,
            ts
            /*cdma_fix  bug 10186 stop */
            );

    /* Change the state */
    CEMH_FSM_SET_STATE(p_cell_ctx,
            CEMH_STATE_CELL_OUT_OF_SERVICE);


    if(RRC_FAILURE == result)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "RRC_LLIM_CELL_START_MAC_FAIL unable to send to CSC.");
    }
    else
    {
        RRC_LLIM_TRACE(RRC_WARNING, "RRC_LLIM_CELL_START_MAC_FAIL sent to CSC.");
    }

    RRC_LLIM_TRACE(RRC_WARNING, "[RRC_MAC_CELL_START_CNF]: Timer Expired ");

    RRC_LLIM_UT_TRACE_EXIT();

}



/*cdma_fix  bug 10186 start*/
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_mac_cell_start_cnf_state_sfn_cnf
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function request sf and sfn from MAC if MAC Ok.
 *                  state  CEMH_STATE_W_FOR_MAC_CELL_START_CNF
 *                  event  CEMH_EVENT_MAC_SFN_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_mac_cell_start_cnf_state_sfn_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_mac_sfn_cnf_t  *p_mac_sfn_cnf = (rrc_mac_sfn_cnf_t *)p_msg;
    /*cdma_fix  bug 10186 start*/
    rrc_timestamp_t ts = {0};
    /*cdma_fix  bug 10186 stop */
    /* SPR 23218/SES-496 Coverity 210248 Fix Start */
    rrc_transaction_id_t            transaction_id;
    /* SPR 23218/SES-496 Coverity 210248 Fix End */


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_mac_sfn_cnf);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Stop guard timer */
    cemh_stop_procedure_timer(p_cell_ctx);
    RRC_LLIM_TRACE(RRC_DETAILEDALL,"\nStopped procedure timer; unused variable p_api=%p\n",p_api);/*SPR 17777 +-*/

    /* SPR 23218/SES-496 Fix Start */
    transaction_id = p_cell_ctx->mac_curr_trans_id;
    /* Precondition - should be the same transaction */
    if (rrc_get_transaction_id(p_api) != transaction_id)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    /* SPR 23218/SES-496 Fix End */
    if ((MAC_SUCCESS == p_mac_sfn_cnf->response) &&
            (p_mac_sfn_cnf->optional_elems_present & RRC_MAC_SFN_INFO_PRESENT) &&
            (p_mac_sfn_cnf->optional_elems_present & RRC_MAC_SF_INFO_PRESENT) &&
            (p_mac_sfn_cnf->optional_elems_present & RRC_MAC_TIMESTAMP_INFO_PRESENT))
    {
        rrc_llim_send_csc_llim_cell_start_resp(
                p_cell_ctx->transaction_id,
                p_cell_ctx->cell_index,
                RRC_LLIM_CELL_START_SUCCESS,
                p_mac_sfn_cnf->sfn.sfn,
                p_mac_sfn_cnf->sf.sf,
                p_mac_sfn_cnf->timestamp);

        RRC_LLIM_TRACE(RRC_DETAILED,"\nSent cell_start_resp from LLIM to CSC with SFN[%u],SF[%u],TS_SEC[%llu], TS_NSEC[%u]\n",
                p_mac_sfn_cnf->sfn.sfn, p_mac_sfn_cnf->sf.sf,((U64) p_mac_sfn_cnf->timestamp.ts_sec), p_mac_sfn_cnf->timestamp.ts_nsec);    

        if(PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index])
        {
            p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index]->
                explicit_start_required = RRC_LLIM_AUTOMATIC_CELL_START;
        }

        CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_CELL_IN_SERVICE);
    }
    else
    {
        rrc_llim_send_csc_llim_cell_start_resp(
                p_cell_ctx->transaction_id,
                p_cell_ctx->cell_index,
                RRC_LLIM_CELL_START_MAC_FAIL,
                RRC_NULL, RRC_NULL, ts);

        CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_CELL_OUT_OF_SERVICE);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/*cdma_fix  bug 10186 stop*/
#ifdef LTE_EMBMS_SUPPORTED
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_rlc_create_area_cell_state_rlc_cnf
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function continue cell configuration if RLC is OK.
 *                  state CEMH_STATE_W_FOR_RLC_CREATE_AREA_ENTITY_CNF;
 *                  event CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_rlc_create_area_cell_state_rlc_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_rlc_create_mbms_area_entity_cnf_t    *p_rrc_rlc_create_mbms_area_entity_cnf =
        (rrc_rlc_create_mbms_area_entity_cnf_t *)p_msg;

    rrc_csc_llim_reconfig_cell_resp_t reconfig_cell_resp;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_rlc_create_mbms_area_entity_cnf);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - should be the same transaction */
    if (rrc_get_transaction_id(p_api) != p_cell_ctx->transaction_id)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    switch (p_rrc_rlc_create_mbms_area_entity_cnf->response_code)
    {
        case RLC_SUCCESS:
        cemh_stop_procedure_timer(p_cell_ctx);
        /* send CSC_LLIM_RECONFIG_CELL_RESP (SUCCESS) */
        memset_wrapper(&reconfig_cell_resp, 0, sizeof (reconfig_cell_resp));
        reconfig_cell_resp.response =
            RRC_LLIM_RECONFIG_MAC_RLC_MODE_SUCCESS;
        reconfig_cell_resp.transaction_id = p_cell_ctx->transaction_id;
        reconfig_cell_resp.cell_index     = p_cell_ctx->cell_index;

        if (RRC_SUCCESS == rrc_llim_send_internal_msg(
                    RRC_CSC_MODULE_ID, 
                    CSC_LLIM_RECONFIG_CELL_RESP,
                    sizeof (reconfig_cell_resp),
                    &reconfig_cell_resp))
        {
            /* STATE TRANSITION*/
            CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);
        }
        if (p_cell_ctx->p_recfg_req_param != PNULL)
        {
            rrc_mem_free(p_cell_ctx->p_recfg_req_param);
            p_cell_ctx->p_recfg_req_param = PNULL;
        }
        break;

        default:      
        cemh_stop_procedure_timer(p_cell_ctx);
        if (RECONFIG_MAC_RLC == p_cell_ctx->recfg_mode)
        {
            /* send CSC_LLIM_RECONFIG_CELL_RESP */
            if (RRC_FAILURE == rrc_llim_send_csc_llim_reconfig_cell_resp_failure(
                        RRC_LLIM_RECONFIG_MAC_RLC_MODE_RLC_FAIL,
                        p_cell_ctx->transaction_id,
                        p_cell_ctx->cell_index))
            {
                RRC_LLIM_TRACE(RRC_WARNING, 
                        "Message with other transaction_id - "
                        "ignored");
                if (p_cell_ctx->p_recfg_req_param != PNULL)
                {
                    rrc_mem_free(p_cell_ctx->p_recfg_req_param);
                    p_cell_ctx->p_recfg_req_param = PNULL;
                }
                RRC_LLIM_UT_TRACE_EXIT();
                return;
            }
            /* STATE TRANSITION*/
            CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);
            if (p_cell_ctx->p_recfg_req_param != PNULL)
            {
                rrc_mem_free(p_cell_ctx->p_recfg_req_param);
                p_cell_ctx->p_recfg_req_param = PNULL;
            }
        }
        break;
    }
    p_cell_ctx->recfg_mode = NO_RECONFIG_REQ;
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_rlc_delete_area_cell_state_rlc_cnf
 *   INPUT        : void                *p_api
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function continue cell configuration if RLC is OK.
 *                  state CEMH_STATE_W_FOR_RLC_DELETE_AREA_ENTITY_CNF;
 *                  event CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_rlc_delete_area_cell_state_rlc_cnf(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_csc_llim_reconfig_cell_resp_t reconfig_cell_resp;
    rrc_return_et   result = RRC_SUCCESS;
    rrc_phy_delete_cell_req_t phy_delete_cell_req;
    rrc_csc_llim_delete_cell_resp_t delete_cell_resp;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* SPR 18354 Fix Start */
    if(CEMH_CELL_DELETE_ONGOING == p_cell_ctx->current_proc)
    {
        /* Delete on PHY level: send RRC_PHY_DELETE_CELL_REQ */
        phy_delete_cell_req.cell_index = 
            p_cell_ctx->cell_index;



        if (RRC_SUCCESS != rrc_phy_il_send_rrc_phy_delete_cell_req(
                    &phy_delete_cell_req,
                    RRC_LLIM_MODULE_ID,
                    RRC_GET_LL_MODULE_ID(RRC_PHY_1_MODULE_ID,
                        phy_delete_cell_req.cell_index),
                    p_cell_ctx->transaction_id,
                    p_cell_ctx->cell_index))
        {
            RRC_LLIM_TRACE(RRC_WARNING, "Unable to send Cell Delete to PHY");
            result = RRC_FAILURE;
        }
        if(RRC_SUCCESS == result)
        {
            CEMH_FSM_SET_STATE(p_cell_ctx, CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF);
        }
        else
        {
            cemh_stop_procedure_timer(p_cell_ctx);
            delete_cell_resp.response   = RRC_FAILURE;
            /* Send CSC_LLIM_DELETE_CELL_RESP */ 
            delete_cell_resp.transaction_id = p_cell_ctx->transaction_id;
            delete_cell_resp.cell_index     = p_cell_ctx->cell_index;
            rrc_llim_send_internal_msg( 
                    RRC_CSC_MODULE_ID, 
                    CSC_LLIM_DELETE_CELL_RESP,
                    sizeof (delete_cell_resp), 
                    &delete_cell_resp);

            /* Delete from Cell DB and free */
            p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index] = PNULL;
            rrc_mem_free(p_cell_ctx);
        }
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    /* SPR 18354 Fix End */
    /* Precondition - should be the same transaction */
    if (rrc_get_transaction_id(p_api) != p_cell_ctx->transaction_id)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Message with other transaction_id - "
                "ignored");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    /* check if any area is to be create at RLC */
    cemh_build_send_rrc_rlc_create_area_req(
            p_cell_ctx->p_recfg_req_param,
            p_cell_ctx);
    if(CEMH_STATE_W_FOR_RLC_CREATE_AREA_ENTITY_CNF != CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx))
    {
        cemh_stop_procedure_timer(p_cell_ctx);
        /* send CSC_LLIM_RECONFIG_CELL_RESP (SUCCESS) */
        memset_wrapper(&reconfig_cell_resp, 0, sizeof (reconfig_cell_resp));
        reconfig_cell_resp.response =
            RRC_LLIM_RECONFIG_MAC_RLC_MODE_SUCCESS;
        reconfig_cell_resp.transaction_id = p_cell_ctx->transaction_id;
        reconfig_cell_resp.cell_index     = p_cell_ctx->cell_index;

        if (RRC_SUCCESS == rrc_llim_send_internal_msg(
                    RRC_CSC_MODULE_ID, 
                    CSC_LLIM_RECONFIG_CELL_RESP,
                    sizeof (reconfig_cell_resp),
                    &reconfig_cell_resp))
        {
            /* STATE TRANSITION*/
            CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);
        }
        if (p_cell_ctx->p_recfg_req_param != PNULL)
        {
            rrc_mem_free(p_cell_ctx->p_recfg_req_param);
            p_cell_ctx->p_recfg_req_param = PNULL;
        }
        p_cell_ctx->recfg_mode = NO_RECONFIG_REQ;

    }
    RRC_LLIM_UT_TRACE_EXIT();
}
#endif



/* CEMH FSM handlers mapping */
static cemh_fsm_act_func_ptr cemh_fsm_tbl[N_CEMH_STATES][N_CEMH_EVENTS] =
{
    /* CEMH_STATE_IDLE */
    {
        cemh_fsm_idle_state_config_cell_req,

        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },


    /* CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_w_phy_conf_cell_state_phy_cnf,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_w_xxx_conf_cell_state_timer_expired,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },

    /* CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_w_mac_conf_cell_state_mac_cnf,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_w_xxx_conf_cell_state_timer_expired,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },

    /* CEMH_STATE_W_FOR_MAC_SFN_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_w_mac_sfn_state_sfn_cnf,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_w_xxx_conf_cell_state_timer_expired,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,  
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },
    /* CEMH_STATE_CELL_IN_SERVICE */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
	 /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_in_service_state_mac_sfn_cnf,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_in_service_state_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_in_service_state_csc_llim_mac_sfn_sync_req,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_cell_reconfig_cell_req,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_in_service_state_wrw_req,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_in_service_csc_si_without_etws_req,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_phy_cell_stop_req,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },
    /* CEMH_STATE_CELL_OUT_OF_SERVICE */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
 	/* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_in_service_state_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_cell_out_of_service_cell_reconfig_cell_req,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_phy_cell_start_req,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_phy_cell_stop_req,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },
    /* CEMH_STATE_W_FOR_PHY_RECONFIG_CELL_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_cell_reconfig_cell_req,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_w_phy_reconf_cell_state_phy_cnf,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_w_phy_reconf_cell_state_timer_expired,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },
    /* CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_in_service_state_mac_sfn_cnf,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_in_service_state_csc_llim_mac_sfn_sync_req,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_cell_reconfig_cell_req,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_w_mac_reconf_cell_state_mac_cnf,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_w_mac_reconf_cell_state_timer_expired,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_in_service_state_wrw_req,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_phy_cell_stop_req, /*Bug#9099 Fix*/
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },
    /* CEMH_STATE_W_FOR_PHY_CELL_START_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_phy_cell_start_cnf,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_phy_cell_start_timer_expiry,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },
    /* CEMH_STATE_W_FOR_MAC_CELL_START_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        /*cdma_fix  bug 10186 start*/
        /*cemh_fsm_ignored_event,*/
        cemh_fsm_w_mac_cell_start_cnf_state_sfn_cnf,
        /*cdma_fix  bug 10186 stop*/
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_mac_cell_start_cnf,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_mac_cell_start_timer_expiry,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },
    /* CEMH_STATE_W_FOR_PHY_CELL_STOP_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_phy_cell_stop_cnf,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_phy_cell_stop_timer_expiry,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },
    /* CEMH_STATE_W_FOR_MAC_CELL_STOP_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_mac_cell_stop_cnf,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_mac_cell_stop_timer_expiry,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },
    /* CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_phy_delete_cell_cnf,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_w_xxx_delete_cell_cnf_timer_expiry,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },
    /* CEMH_STATE_W_FOR_MAC_CELL_DELETE_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_mac_delete_cell_cnf,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_w_xxx_delete_cell_cnf_timer_expiry,
#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
#endif
    },
#ifdef LTE_EMBMS_SUPPORTED
   /* CEMH_STATE_W_FOR_RLC_CREATE_AREA_ENTITY_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_in_service_state_mac_sfn_cnf,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_in_service_state_csc_llim_mac_sfn_sync_req,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_cell_reconfig_cell_req,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
//#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_w_rlc_create_delete_area_cell_state_timer_expired,
//#endif
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,/*TBD*/
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_phy_cell_stop_req, /*Bug#9099 Fix*/
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
//#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_w_rlc_create_area_cell_state_rlc_cnf,
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
//#endif
    },
    /* CEMH_STATE_W_FOR_RLC_DELETE_AREA_ENTITY_CNF */
    {
        /* CEMH_EVENT_CSC_CONFIG_CELL_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_SFN_CNF */
        cemh_fsm_in_service_state_mac_sfn_cnf,
        /* CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CSC_DELETE_CELL_REQ */
        cemh_fsm_delete_cell_req,
        /* CEMH_EVENT_OAMH_CLEANUP_REQ */
        cemh_fsm_cleanup_req,
        /* CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ */
        cemh_fsm_in_service_state_csc_llim_mac_sfn_sync_req,
        /* CEMH_EVENT_CSC_RECONFIG_CELL_REQ */
        cemh_fsm_cell_reconfig_cell_req,
        /* CEMH_EVENT_PHY_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_RECONFIG_CELL_CNF */
        cemh_fsm_ignored_event,
//#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED */
        cemh_fsm_w_rlc_create_delete_area_cell_state_timer_expired,
//#endif
        /* CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ */
        cemh_fsm_ignored_event,/*TBD*/
        /* CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_REQ */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_START_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_CELL_STOP_REQ*/
        cemh_fsm_phy_cell_stop_req, /*Bug#9099 Fix*/
        /* CEMH_EVENT_PHY_CELL_STOP_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_CELL_STOP_CNF */
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_PHY_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_MAC_DELETE_CELL_CNF*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_START_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_STOP_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED*/
        cemh_fsm_ignored_event,
        /* CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF */
        cemh_fsm_ignored_event,
//#ifdef LTE_EMBMS_SUPPORTED
        /* CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF */
        cemh_fsm_w_rlc_delete_area_cell_state_rlc_cnf,
//#endif
    },
#endif

     
};

/******************************************************************************
*   FUNCTION NAME: cemh_fsm_process_event
*   INPUT        : cemh_fsm_event_et   event
*                  void                *param1
*                  void                *param2
*                  rrc_llim_cell_ctx_t *p_cell_ctx
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function call appropriate handler for incoming event in
*        current state.
*
*
******************************************************************************/
void cemh_fsm_process_event(
        cemh_fsm_event_et   event,              /* Incoming event */
        void                *param1,            /* p_api or timer_id */
        void                *param2,            /* p_msg or p_cemh_timer_buf */
        /* Cell context of current operation. Here we store CEMH FSM state too */
        rrc_llim_cell_ctx_t *p_cell_ctx,
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    cemh_fsm_state_et new_state ;
    /* Coverity Fix 96718 Start */
    rrc_cell_index_t cell_index;
    /* Coverity Fix 96718 End */
    /* Coverity Fix 96892 Start */
    if(PNULL != p_cell_ctx)
    {
        /* Coverity Fix 96892 end */
        cemh_fsm_state_et current_state = CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx);
        /* Coverity Fix 96718 Start */
        cell_index = p_cell_ctx->cell_index;
        /* Coverity Fix 96718 End */
       
        /* SPR 19273 Fix Start */
        SET_CELL_INDEX(cell_index);
        /* SPR 19273 Fix End */

        RRC_LLIM_UT_TRACE_ENTER();

        if ((current_state < N_CEMH_STATES) && (event < N_CEMH_EVENTS) )
        {

          RRC_LLIM_TRACE(RRC_INFO, "### %s Processing state %s Event: %s",
              RRC_CEMH_FSM_NAME,
              CEMH_FSM_STATES_NAMES[current_state],
              CEMH_FSM_EVENTS_NAMES[event]);

          cemh_fsm_tbl[current_state][event](
              param1, 
              param2, 
              p_cell_ctx,
              p_rrc_llim_gl_ctx);

          /*coverity 96718 Fix Start*/
          if(PNULL != p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index])
          {
            new_state = CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx);
              RRC_LLIM_TRACE(RRC_INFO, "### %s Change state %s -> %s Event: %s",
                  RRC_CEMH_FSM_NAME,
                  CEMH_FSM_STATES_NAMES[current_state],
                  CEMH_FSM_STATES_NAMES[new_state],
                  CEMH_FSM_EVENTS_NAMES[event]);
          }
          /*coverity 96718 Fix End*/

        }
        else
        {
            RRC_LLIM_TRACE(RRC_ERROR, "cemh_fsm_process_event: unexpected state %u"
                    " or event %u", current_state, event);
            RRC_ASSERT(0);
        }
        /* Coverity Fix 96892 Start */
    }
    else
    {
        RRC_LLIM_TRACE(RRC_ERROR, "cemh_fsm_process_event: p_cell_ctx is NULL");
    }
    /* Coverity Fix 96892 end */

    RRC_LLIM_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_xxx_delete_cell_cnf_timer_expiry
 *   INPUT        : cemh_fsm_event_et   event
 *                  void                *param1
 *                  void                *param2
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function waits for the MAC CELL DELETE CNF.
 *                  state any of:
 *                  CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF,
 *                  CEMH_STATE_W_FOR_MAC_CELL_DELETE_CNF,
 *                  event CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED.
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_xxx_delete_cell_cnf_timer_expiry(
        void                *p_api,             /* p_api */
        void                *p_msg,             /* p_msg */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{

    rrc_csc_llim_delete_cell_resp_t delete_cell_resp;
    /* SPR 13746 Fix Start */
    rrc_csc_llim_config_cell_resp_t config_cell_resp ;
    /* SPR 13746 Fix Stop */
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    RRC_LLIM_TRACE(RRC_DETAILED, 
            "Unused variable in fsm p_api=%p and p_msg=%p", p_api, p_msg);
    /* SPR 23608/SES-43 Fix Start */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_LLIM_TRACE(RRC_BRIEF, "Stopping Cell Context Guard Timer");
        /* Stop Timer */
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }
    /* SPR 23608/SES-43 Fix End */      
    
    /* SPR 13746 Fix Start */
    switch (p_cell_ctx->current_proc)
    {
        case  CEMH_CELL_SETUP_PHY_FAILED:
        case  CEMH_CELL_SETUP_MAC_FAILED:    
        case  CEMH_CELL_SETUP_MAC_SFN_FAILED: 

              memset_wrapper(&config_cell_resp, 0, 
                        sizeof (config_cell_resp));
              config_cell_resp.response =  
                  get_fail_response_config_te(p_cell_ctx->current_proc);
                
              /* Send CSC_LLIM_CELL_SETUP_RESP*/
              config_cell_resp.transaction_id = p_cell_ctx->transaction_id;
              config_cell_resp.cell_index     = p_cell_ctx->cell_index;
              rrc_llim_send_internal_msg(
                      RRC_CSC_MODULE_ID,
                      CSC_LLIM_CONFIG_CELL_RESP,
                      sizeof (config_cell_resp),
                      &config_cell_resp);

              break;

            case  CEMH_CELL_DELETE_ONGOING :  

	          /* 1. send CSC_LLIM_DELETE_CELL_RESP */ 
    		  delete_cell_resp.transaction_id = p_cell_ctx->transaction_id;
    		  delete_cell_resp.cell_index     = p_cell_ctx->cell_index;
    		  delete_cell_resp.response       = RRC_FAILURE;

    		  rrc_llim_send_internal_msg( 
	            RRC_CSC_MODULE_ID, 
        	    CSC_LLIM_DELETE_CELL_RESP,
	            sizeof (delete_cell_resp), 
        	    &delete_cell_resp);
		break;
            
	    default:
                RRC_LLIM_TRACE(RRC_WARNING, "cemh_fsm_w_xxx_delete_cell_cnf_timer_expiry"
			": unexpected procedure!");
                break;
    }
    /* SPR 13746 Fix Stop */

    /* 2. delete from Cell DB and free */
    if (p_rrc_llim_gl_ctx->p_p_cell_ctx != PNULL)
    {
        /* delete from Cell DB and free */
        p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index] = PNULL;

        rrc_mem_free(p_cell_ctx);

    }

    RRC_LLIM_UT_TRACE_EXIT();
}
#ifdef LTE_EMBMS_SUPPORTED                 
/******************************************************************************
 *   FUNCTION NAME: cemh_build_send_rrc_rlc_create_area_req
 *   INPUT        : rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param
 *                  rrc_llim_cell_ctx_t* p_cell_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function builds 'rlc_create_area_req_t'
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_build_send_rrc_rlc_create_area_req(
        rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param,
        rrc_llim_cell_ctx_t* p_cell_ctx)
{
    rrc_rlc_create_mbms_area_entity_req_t *rrc_rlc_create_mbms_area_entity_req_p = PNULL;
    mbms_updated_area_info_t        *mbms_updated_area_info_p   = PNULL;
    U8   index;
    U8   num_of_area;
    /* SA_FIX Start */
    m2ap_mcch_related_bcch_info_t   *mcch_related_bcch_info_p  = PNULL;
    /* SA_FIX End */
    RRC_LLIM_UT_TRACE_ENTER();
    rrc_rlc_create_mbms_area_entity_req_p = (rrc_rlc_create_mbms_area_entity_req_t*)rrc_mem_get(
            sizeof (rrc_rlc_create_mbms_area_entity_req_t));

    if(PNULL != rrc_rlc_create_mbms_area_entity_req_p)
    {

        memset_wrapper(rrc_rlc_create_mbms_area_entity_req_p, 0, sizeof (rrc_rlc_create_mbms_area_entity_req_t));
        if((p_recfg_cell_param->cell_reconfig.presence_bitmask &
                    CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG) &&
                (p_recfg_cell_param->cell_reconfig.broadcast_info.presence_bitmask
                 & CELL_RECONFIG_REQ_SIB_13_INFO_PRESENCE_FLAG))
        {
            for(index = RRC_ZERO; index <
                    p_recfg_cell_param->cell_reconfig.broadcast_info.
                    mbms_updated_area_info_list.num_valid_mbsfn_area_info;
                    index++)
            {
                mbms_updated_area_info_p =
                    &(p_recfg_cell_param->cell_reconfig.broadcast_info.
                            mbms_updated_area_info_list.mbms_updated_area_info[index]);
                if((U8)ADDED == mbms_updated_area_info_p->flag) 
                {
                    /* SA_FIX Start */
                    /* Check if area index present in sib13 list, then only send
                     * rlc create */
                    mcch_related_bcch_info_p = get_m2ap_mcch_related_bcch_info(
                            &(p_recfg_cell_param->cell_reconfig.broadcast_info.sib_type_13_info),
                            mbms_updated_area_info_p->area_id);
                    if(PNULL == mcch_related_bcch_info_p)
                    {
                        continue;
                    }
                    /* SA_FIX End */
                    num_of_area = rrc_rlc_create_mbms_area_entity_req_p->num_of_area;
                    rrc_rlc_create_mbms_area_entity_req_p->area_id[num_of_area] = 
                        mbms_updated_area_info_p->area_index;
                    rrc_rlc_create_mbms_area_entity_req_p->num_of_area++;
                }
            }
        }
        if(RRC_ZERO != rrc_rlc_create_mbms_area_entity_req_p->num_of_area)
        {
            rrc_rlc_il_send_rrc_rlc_create_mbms_area_entity_req(rrc_rlc_create_mbms_area_entity_req_p,
                    RRC_LLIM_MODULE_ID,
                    RRC_GET_LL_MODULE_ID(RRC_RLC_1_MODULE_ID,p_recfg_cell_param->cell_index),
                    p_recfg_cell_param->transaction_id,
                    p_recfg_cell_param->cell_index);
            CEMH_FSM_SET_STATE(p_cell_ctx,CEMH_STATE_W_FOR_RLC_CREATE_AREA_ENTITY_CNF);
        }
        /* Free Memory allocated to rrc_rlc_create_mbms_area_entity_req_p */
        if (rrc_rlc_create_mbms_area_entity_req_p != PNULL)
        {
            rrc_mem_free(rrc_rlc_create_mbms_area_entity_req_p);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cemh_build_send_rrc_rlc_delete_area_req
 *   INPUT        : rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param
 *                  rrc_llim_cell_ctx_t* p_cell_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function builds 'rlc_delete_area_req_t'
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_build_send_rrc_rlc_delete_area_req(
        rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param,
        rrc_llim_cell_ctx_t* p_cell_ctx)
{
    rrc_rlc_delete_mbms_area_entity_req_t *rrc_rlc_delete_mbms_area_entity_req_p = PNULL;
    mbms_updated_area_info_t        *mbms_updated_area_info_p   = PNULL;
    U8   index;
    U8   num_of_area;

    RRC_LLIM_UT_TRACE_ENTER();
    rrc_rlc_delete_mbms_area_entity_req_p = 
        (rrc_rlc_delete_mbms_area_entity_req_t*)rrc_mem_get(
                sizeof(rrc_rlc_delete_mbms_area_entity_req_t));

    if(PNULL != rrc_rlc_delete_mbms_area_entity_req_p)
    {

        memset_wrapper(rrc_rlc_delete_mbms_area_entity_req_p, 0, sizeof (rrc_rlc_delete_mbms_area_entity_req_t));
        for(index = RRC_ZERO; index <
                p_recfg_cell_param->cell_reconfig.broadcast_info.
                mbms_updated_area_info_list.num_valid_mbsfn_area_info;
                index++)
        {
            mbms_updated_area_info_p =
                &(p_recfg_cell_param->cell_reconfig.broadcast_info.
                        mbms_updated_area_info_list.mbms_updated_area_info[index]);
            if((U8)DELETED == mbms_updated_area_info_p->flag) 
            {
                /* SA_FIX Start */
                if(RRC_TRUE == check_if_area_index_reserved(p_recfg_cell_param,
                            mbms_updated_area_info_p->area_index))
                {
                    continue;
                }
                /* SA_FIX End */
                num_of_area = rrc_rlc_delete_mbms_area_entity_req_p->num_of_area;
                rrc_rlc_delete_mbms_area_entity_req_p->area_id[num_of_area] = 
                    mbms_updated_area_info_p->area_index;
                RRC_LLIM_TRACE(RRC_INFO,"API:RRC->RLC:RRC_RLC_DELETE_AREA_REQ:AreaIndex:%u",
                        mbms_updated_area_info_p->area_index);

                rrc_rlc_delete_mbms_area_entity_req_p->num_of_area++;
            }
        }
        if(RRC_ZERO != rrc_rlc_delete_mbms_area_entity_req_p->num_of_area)
        {
            rrc_rlc_il_send_rrc_rlc_delete_mbms_area_entity_req(rrc_rlc_delete_mbms_area_entity_req_p,
                    RRC_LLIM_MODULE_ID,
                    RRC_GET_LL_MODULE_ID(RRC_RLC_1_MODULE_ID,p_recfg_cell_param->cell_index),
                    p_recfg_cell_param->transaction_id,
                    p_recfg_cell_param->cell_index);
            CEMH_FSM_SET_STATE(p_cell_ctx,CEMH_STATE_W_FOR_RLC_DELETE_AREA_ENTITY_CNF);
        }
        /* Free Memory allocated to rrc_rlc_delete_mbms_area_entity_req_p */
        if (rrc_rlc_delete_mbms_area_entity_req_p != PNULL)
        {
            rrc_mem_free(rrc_rlc_delete_mbms_area_entity_req_p);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cemh_fsm_w_rlc_create_delete_area_cell_state_timer_expired
 *   INPUT        : rrc_timer_t         timer_id
 *                  void                *p_msg
 *                  rrc_llim_cell_ctx_t *p_cell_ctx
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function stop cell reconfiguration .
 *                  state any of:
 *                  CEMH_STATE_W_FOR_RLC_DELETE_AREA_ENTITY_CNF,
 *                  event CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static void cemh_fsm_w_rlc_create_delete_area_cell_state_timer_expired(
        rrc_timer_t         timer_id,           /* timer_id - unused */
        void                *p_timer_buf,       /* p_cemh_timer_buf - unused */
        rrc_llim_cell_ctx_t *p_cell_ctx,        /* cell context */
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx)
{
    rrc_llim_csc_response_et    resp_code = RRC_LLIM_RECONFIG_MAC_RLC_MODE_RLC_FAIL;
    p_cell_ctx->guard_timer = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();
    RRC_LLIM_TRACE(RRC_DETAILED, "Unused variable in fsm timer_id=%p"
            " and p_timer_buf=%p", timer_id, p_timer_buf);  
    if (RRC_SUCCESS == rrc_llim_send_csc_llim_reconfig_cell_resp_failure(
                resp_code,
                p_cell_ctx->transaction_id,
                p_cell_ctx->cell_index))
    {
        /*Change the CEMH(LLIM) state back to CEMH_STATE_CELL_IN_SERVICE */
        CEMH_SET_FSM_NEW_STATE(p_rrc_llim_gl_ctx, p_cell_ctx->cell_index);  
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    else
    {
        RRC_LLIM_TRACE(RRC_WARNING, "UNABLE TO SEND RESPONSE TO CSC");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/* SA_FIX Start */
/******************************************************************************
 *   FUNCTION NAME: check_if_area_index_reserved
 *   INPUT        : rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function stop cell reconfiguration .
 *                  state any of:
 *                  CEMH_STATE_W_FOR_RLC_DELETE_AREA_ENTITY_CNF,
 *                  event CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
static rrc_bool_et check_if_area_index_reserved(
        rrc_csc_llim_reconfig_cell_req_t* p_recfg_cell_param,
        U8   area_index)
{
    U8   count = 0;
    rrc_bool_et ret_val = RRC_FALSE;

    RRC_LLIM_UT_TRACE_ENTER();
    for(count = 0; count < MAX_MBMS_UPDATED_AREA_LIST; count++)
    {

        if(area_index == p_recfg_cell_param->cell_reconfig.broadcast_info.reserved_area_index[count])
        {
            ret_val =  RRC_TRUE;
            break;
        }
    }
    return ret_val;

}
#endif

