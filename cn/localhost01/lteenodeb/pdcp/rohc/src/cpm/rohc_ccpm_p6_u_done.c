/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definitions for U-mode 
 *                     compressor CPM, when compressor transition state is DONE.
 *
 ******************************************************************************
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
 * Function Name  : rohc_c_p6_ir_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when compressor mode is U, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p6_ir_ir_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_ir_ir_dn");
    p_entity->max_static_info_update_counter = ROHC_MAX_P6_IR_STATIC_INFO_COUNTER;
    p_csc->static_info_update_counter++;
    if (p_csc->static_info_update_counter >= 
        p_entity->max_static_info_update_counter)
    {
        ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);
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

    ROHC_EXIT_FN("rohc_c_p6_ir_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_dyn_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when compressor mode is U, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p6_dyn_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_dyn_fo_dn");
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
    
    ROHC_EXIT_FN("rohc_c_p6_dyn_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_rnd_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ROHC_C_U_CO_P6_RND_ET , when compressor mode is U, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p6_rnd_so_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_rnd_so_dn");
    /* nothing to be done here*/
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p6_rnd_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_common_comp_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ROHC_C_U_CO_P6_COMMON_ET , when compressor mode is U, state 
 *                  is SO and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p6_common_comp_so_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_common_comp_so_dn");
    /* nothing to be done here*/
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p6_common_comp_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_ir_rtn_tmr_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_RTN_TMR_ET  , when compressor mode is U, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p6_ir_rtn_tmr_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_ir_rtn_tmr_dn");
    p_csc->c_state  = ROHC_C_ST_IR;
    p_csc->static_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p6_ir_rtn_tmr_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_fo_rtn_tmr_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  FO_RTN_TMR_ET  , when compressor mode is U, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p6_fo_rtn_tmr_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_fo_rtn_tmr_so_dn");
    p_csc->c_state  = ROHC_C_ST_FO;
    p_csc->send_irdyn_flag = ROHC_TRUE;
    p_csc->dynamic_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p6_fo_rtn_tmr_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_ack_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_ET  , when state is IR.
 *****************************************************************************/
rohc_void_t rohc_c_p6_ack_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_ack_ir_dn");
    p_csc->c_state  = ROHC_C_ST_FO;
    p_csc->static_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p6_ack_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_ack_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_ET  , when state is FO.
 *****************************************************************************/
rohc_void_t rohc_c_p6_ack_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_ack_fo_dn");
    p_csc->c_state  = ROHC_C_ST_SO;
    p_csc->dynamic_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p6_ack_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_ack_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_ET  , when state is SO 
 *****************************************************************************/
rohc_void_t rohc_c_p6_ack_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_ack_so_dn");
    /* do nothing */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[ ACK handling ], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
    /* stay in the same */
    ROHC_EXIT_FN("rohc_c_p6_ack_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_nack_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_ET  state is IR
 *****************************************************************************/
rohc_void_t rohc_c_p6_nack_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_nack_ir_dn");
    /* do nothing */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[ NACK handling ], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
    ROHC_EXIT_FN("rohc_c_p6_nack_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_nack_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_ET  state is FO
 *****************************************************************************/
rohc_void_t rohc_c_p6_nack_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_nack_fo_dn");
    /* mark to send ir-dyn */
    p_csc->send_irdyn_flag = ROHC_TRUE;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p6_nack_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_nack_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_ET  state is SO
 *****************************************************************************/
rohc_void_t rohc_c_p6_nack_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_nack_so_dn");
    ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);
    p_csc->send_irdyn_flag = ROHC_TRUE;
    p_csc->c_state = ROHC_C_ST_FO;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p6_nack_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_static_nack_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_ET  state is IR
 *****************************************************************************/
rohc_void_t rohc_c_p6_static_nack_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_static_nack_ir_dn");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    /* do nothing */
    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[ NACK handling ], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
    ROHC_EXIT_FN("rohc_c_p6_static_nack_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p6_static_nack_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_ET  state is FO or SO
 *****************************************************************************/
rohc_void_t rohc_c_p6_static_nack_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_tcp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p6_static_nack_fo_dn");
    ROHC_STOP_TIMER(p_csc->ir_ret_timer.tdata, p_csc->ir_ret_timer.tid);
    ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);
    p_csc->c_state = ROHC_C_ST_IR;
    p_csc->static_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p6_static_nack_fo_dn");
}

