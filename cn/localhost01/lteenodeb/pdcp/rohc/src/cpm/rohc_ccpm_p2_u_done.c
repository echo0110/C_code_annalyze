/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_ccpm_p2_u_done.c,v 1.1.2.1 2010/11/03 09:12:28 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definitions for U-mode 
 *                     compressor CPM, when compressor transition state is DONE.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_ccpm_p2_u_done.c,v $
 * Revision 1.1.2.1  2010/11/03 09:12:28  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.1.2.1  2010/10/13 03:39:42  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.1.2.1  2010/09/23 05:04:15  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.4  2009/05/27 10:15:59  gur19836
 * File Header and Function Headers updated
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_ccpm_proto.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* +- SPR 17777 */
extern tickType_t pdcpCurrentTime_g;
/* +- SPR 17777 */

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*****************************************************************************
 * Function Name  : rohc_c_p2_u_ir_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when compressor mode is U, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_ir_ir_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_ir_ir_dn");
    p_csc->static_info_update_counter++;
    
    if (p_csc->static_info_update_counter >= 
        p_entity->max_static_info_update_counter)
    {
        /** Fix Start for SPR#2189, 2190, 2193 */
        /** Stop Timer Statement deleted */
        /** Fix End for SPR#2189, 2190, 2193 */
 
            
        /* start ir_return timer  */
        ROHC_START_TIMER(&(p_csc->ir_ret_timer.tdata), ROHC_MAX_IR_RETURN_TIME,
                    &(p_csc->ir_ret_timer.tid));
        p_csc->c_state = ROHC_C_ST_FO;
        p_csc->dynamic_info_update_counter = 0;
    }
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */

    ROHC_EXIT_FN("rohc_c_p2_u_ir_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_irdyn_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when compressor mode is U, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_irdyn_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_irdyn_fo_dn");
    p_csc->dynamic_info_update_counter++;
    if (p_csc->dynamic_info_update_counter >= 
    p_entity->max_dynamic_info_update_counter)
    {
        /* start fo_return timer  */
        ROHC_START_TIMER(&(p_csc->fo_ret_timer.tdata), ROHC_MAX_FO_RETURN_TIME,
                    &(p_csc->fo_ret_timer.tid));
        
        p_csc->c_state = ROHC_C_ST_SO;
    }
    
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_irdyn_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_uo_0_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_0_ET  , when compressor mode is U, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_uo_0_so_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_uo_0_so_dn");
    /* nothing to be done here*/
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_uo_0_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_ir_rtn_tmr_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_RTN_TMR_ET  , when compressor mode is U, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_ir_rtn_tmr_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_ir_rtn_tmr_ir_dn");
    /* in ir state IR-return timer should not be running */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_ir_rtn_tmr_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_ir_rtn_tmr_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_RTN_TMR_ET  , when compressor mode is U, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_ir_rtn_tmr_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_ir_rtn_tmr_fo_dn");
    p_csc->c_state  = ROHC_C_ST_IR;
    p_csc->static_info_update_counter = 0;
    /*ROHC_STOP_TIMER(p_csc->ir_ret_timer.tdata, p_csc->ir_ret_timer.tid);    
    not needed as the timer was started with repeatation=1
    and this func is invoked on timer expiry. 
    */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_ir_rtn_tmr_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_ir_rtn_tmr_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_RTN_TMR_ET  , when compressor mode is U, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_ir_rtn_tmr_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_ir_rtn_tmr_so_dn");

    /** Fix Start for SPR#2189, 2190, 2193 */
    ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);
    /** Fix End for SPR#2189, 2190, 2193 */

    p_csc->c_state  = ROHC_C_ST_IR;
    p_csc->static_info_update_counter = 0;


    /*ROHC_STOP_TIMER(p_csc->ir_ret_timer.tdata, p_csc->ir_ret_timer.tid);    */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_ir_rtn_tmr_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_fo_rtn_tmr_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  FO_RTN_TMR_ET  , when compressor mode is U, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_fo_rtn_tmr_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_fo_rtn_tmr_ir_dn");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_fo_rtn_tmr_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_fo_rtn_tmr_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  FO_RTN_TMR_ET  , when compressor mode is U, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_fo_rtn_tmr_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_fo_rtn_tmr_fo_dn");
    p_csc->send_irdyn_flag = ROHC_TRUE;
    p_csc->dynamic_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_fo_rtn_tmr_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_fo_rtn_tmr_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  FO_RTN_TMR_ET  , when compressor mode is U, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_fo_rtn_tmr_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_fo_rtn_tmr_so_dn");
    p_csc->c_state  = ROHC_C_ST_FO;
    p_csc->send_irdyn_flag = ROHC_TRUE;
    p_csc->dynamic_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_fo_rtn_tmr_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_ack_u_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_U_ET  , when compressor mode is U, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_ack_u_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_ack_u_ir_dn");
    /* Increase the IR_RETURN time */
    p_csc->ir_ret_time +=ROHC_INCREASE_IR_RET_TIMER_DELTA;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    
    ROHC_EXIT_FN("rohc_c_p2_u_ack_u_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_mt_u_to_o_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_U_TO_O_ET  , when compressor mode is U, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_mt_u_to_o_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_mt_u_to_o_ir_dn");
    /* SPR 20271 Changes Start */
    /*
    ** Mode Transition is not allowed as currently ROHC supports only
    ** U mode.
    ** p_csc->c_mode = ROHC_MODE_O; 
    ** ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata,p_csc->fo_ret_timer.tid);    
    ** ROHC_STOP_TIMER(p_csc->ir_ret_timer.tdata,p_csc->ir_ret_timer.tid) ;  
    */
    /* SPR 20271 Changes End */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_mt_u_to_o_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_mt_u_to_o_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_U_TO_O_ET  , when compressor mode is U, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_mt_u_to_o_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_mt_u_to_o_fo_dn");
    /* SPR 20271 Changes Start */
    /*
    ** Mode Transition is not allowed as currently ROHC supports only
    ** U mode.
    ** p_csc->c_mode = ROHC_MODE_O; 
    ** ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);    
    ** ROHC_STOP_TIMER(p_csc->ir_ret_timer.tdata, p_csc->ir_ret_timer.tid);   
    */
    /* SPR 20271 Changes End */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_mt_u_to_o_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_mt_u_to_o_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_U_TO_O_ET  , when compressor mode is U, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_mt_u_to_o_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_mt_u_to_o_so_dn");
    /* SPR 20271 Changes Start */
    /*
    ** Mode Transition is not allowed as currently ROHC supports only
    ** U mode.
    ** p_csc->c_mode = ROHC_MODE_O; 
    ** p_csc->c_state = ROHC_C_ST_FO;
    ** ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);   
    ** ROHC_STOP_TIMER(p_csc->ir_ret_timer.tdata ,p_csc->ir_ret_timer.tid);    
    */
    /* stop umode timers */
    /* SPR 20271 Changes End */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_mt_u_to_o_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_mt_u_to_r_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_U_TO_R_ET  , when compressor mode is U, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_mt_u_to_r_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_mt_u_to_r_ir_dn");
    /* SPR 20271 Changes Start */
    /*
    ** Mode Transition is not allowed as currently ROHC supports only
    ** U mode.
    ** p_csc->c_mode = ROHC_MODE_R; 
    ** p_csc->c_trans_state = ROHC_C_TST_PENDING ;
    ** ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);    
    ** ROHC_STOP_TIMER(p_csc->ir_ret_timer.tdata, p_csc->ir_ret_timer.tid);    
    */
    /* SPR 20271 Changes End */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_mt_u_to_r_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_mt_u_to_r_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_U_TO_R_ET  , when compressor mode is U, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_mt_u_to_r_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_mt_u_to_r_fo_dn");
    /* SPR 20271 Changes Start */
    /*
    ** Mode Transition is not allowed as currently ROHC supports only
    ** U mode.
    ** p_csc->c_trans_state = ROHC_C_TST_PENDING ;
    ** p_csc->c_mode = ROHC_MODE_R; 
    ** ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);    
    ** ROHC_STOP_TIMER(p_csc->ir_ret_timer.tdata, p_csc->ir_ret_timer.tid);    
    */
    /* SPR 20271 Changes End */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_mt_u_to_r_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_u_mt_u_to_r_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_U_TO_R_ET  , when compressor mode is U, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_u_mt_u_to_r_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_u_mt_u_to_r_so_dn");
    /* SPR 20271 Changes Start */
    /*
    ** Mode Transition is not allowed as currently ROHC supports only
    ** U mode.
    ** p_csc->c_trans_state = ROHC_C_TST_PENDING ;
    ** p_csc->c_mode = ROHC_MODE_R; 
    ** p_csc->c_state = ROHC_C_ST_FO;
    ** ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);    
    ** ROHC_STOP_TIMER(p_csc->ir_ret_timer.tdata, p_csc->ir_ret_timer.tid);    
    */
    /* stop umode timers */
    /* SPR 20271 Changes End */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_u_mt_u_to_r_so_dn");
}

