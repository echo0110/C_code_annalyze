/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_ccpm_p2_o_done.c,v 1.1.2.1 2010/11/03 09:12:05 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definitions for O-mode 
 *                     compressor CPM, when compressor transition state is DONE.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_ccpm_p2_o_done.c,v $
 * Revision 1.1.2.1  2010/11/03 09:12:05  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.1.2.1  2010/10/13 03:36:39  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.1.2.1  2010/09/23 04:59:18  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.3  2009/05/27 08:10:08  gur19836
 * File Headers and Function Headers updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
/* SPR 14084 fix start */
/* flag ROHC_MODE_TRIGGER removed */
/* SPR 14084 fix end */
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
 * Function Name  : rohc_c_p2_o_ir_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when compressor mode is O, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_ir_ir_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_ir_ir_dn");
    p_csc->static_info_update_counter++;
    if (p_csc->static_info_update_counter >= 
            p_entity->max_static_info_update_counter)
    {
        p_csc->c_state = ROHC_C_ST_FO;
    }
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_o_ir_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_irdyn_fo_dn 
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when compressor mode is O, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_irdyn_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_irdyn_fo_dn");
    p_csc->dynamic_info_update_counter++;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Incrementing dynamic info counter[%u]\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc),
                                  p_csc->dynamic_info_update_counter);
    if (p_csc->dynamic_info_update_counter >= 
            p_entity->max_dynamic_info_update_counter)
    {
        p_csc->c_state = ROHC_C_ST_SO;
        ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "State is updated to SO\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
    }   

    ROHC_EXIT_FN("rohc_c_p2_o_irdyn_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_uo_0_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_0_ET  , when compressor mode is O, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_uo_0_so_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_uo_0_so_dn");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation, but nothing needs to "
                                  "be done in CCPM",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
    /* do nothing */
    /* stay in the same state */
    ROHC_EXIT_FN("rohc_c_p2_o_uo_0_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_ack_o_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_O_ET  , when compressor mode is O, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_ack_o_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_ack_o_ir_dn");
    p_csc->c_state  = ROHC_C_ST_FO;
    p_csc->static_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_o_ack_o_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_ack_o_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_O_ET  , when compressor mode is O, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_ack_o_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_ack_o_fo_dn");
    p_csc->c_state  = ROHC_C_ST_SO;
    p_csc->dynamic_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_o_ack_o_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_ack_o_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_O_ET  , when compressor mode is O, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_ack_o_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_ack_o_so_dn");
    /* do nothing */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[ ACK_O handling ], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
    /* stay in the same */
    ROHC_EXIT_FN("rohc_c_p2_o_ack_o_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_static_nack_o_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_O_ET  , when compressor mode is O, state is IR
 *                  and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_static_nack_o_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_static_nack_o_ir_dn");
    /* do nothing */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[ STATIC_ACK_O handling ], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
    /* It is valid*/
    ROHC_EXIT_FN("rohc_c_p2_o_static_nack_o_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_static_nack_o_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_O_ET  , when compressor mode is O, state is
 *                  FO and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_static_nack_o_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_static_nack_o_fo_dn");
    p_csc->c_state = ROHC_C_ST_IR;
    p_csc->static_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_o_static_nack_o_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_nack_o_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_O_ET  , when compressor mode is O, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_nack_o_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_nack_o_ir_dn");
    /* do nothing */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[ NACK_O handling ], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
    ROHC_EXIT_FN("rohc_c_p2_o_nack_o_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_nack_o_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_O_ET  , when compressor mode is O, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_nack_o_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_nack_o_fo_dn");
    /* mark to send ir-dyn */
    p_csc->send_irdyn_flag = ROHC_TRUE;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_o_nack_o_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_nack_o_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_O_ET  , when compressor mode is O, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_nack_o_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_nack_o_so_dn");
    p_csc->send_irdyn_flag = ROHC_TRUE;
    p_csc->c_state = ROHC_C_ST_FO;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_o_nack_o_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_mt_o_to_u_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_O_TO_U_ET  , when compressor mode is O, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_mt_o_to_u_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_mt_o_to_u_ir_dn");
    p_csc->c_trans_state = ROHC_C_TST_PENDING;
    p_csc->c_mode = ROHC_MODE_U;
    p_csc->static_info_update_counter  = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_o_mt_o_to_u_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_mt_o_to_u_fo_dn 
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_O_TO_U_ET  , when compressor mode is O, state is FO
 *                  and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_mt_o_to_u_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_mt_o_to_u_fo_dn");
    p_csc->c_trans_state = ROHC_C_TST_PENDING;
    p_csc->c_mode = ROHC_MODE_U;
    p_csc->dynamic_info_update_counter  = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_o_mt_o_to_u_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_mt_o_to_u_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_O_TO_U_ET  , when compressor mode is O, state is SO
 *                  and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_mt_o_to_u_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_mt_o_to_u_so_dn");
    p_csc->c_trans_state = ROHC_C_TST_PENDING;
    p_csc->c_mode = ROHC_MODE_U;
    p_csc->c_state = ROHC_C_ST_FO;
    p_csc->dynamic_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_o_mt_o_to_u_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_mt_o_to_r_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_o_to_r_ET  , when compressor mode is O, state is IR 
 *                  and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_mt_o_to_r_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_mt_o_to_r_ir_dn");
    p_csc->c_trans_state = ROHC_C_TST_PENDING;
    p_csc->c_mode = ROHC_MODE_R;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
   
    ROHC_EXIT_FN("rohc_c_p2_o_mt_o_to_r_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_mt_o_to_r_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_o_to_r_ET  , when compressor mode is O, state is FO
 *                  and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_mt_o_to_r_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_mt_o_to_r_fo_dn");
    p_csc->c_trans_state = ROHC_C_TST_PENDING;
    p_csc->c_mode = ROHC_MODE_R;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_o_mt_o_to_r_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_o_mt_o_to_r_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_o_to_r_ET  , when compressor mode is O, state is SO
 *                  and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_o_mt_o_to_r_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
    ROHC_ENTER_FN("rohc_c_p2_o_mt_o_to_r_so_dn");
    p_csc->c_trans_state = ROHC_C_TST_PENDING;
    p_csc->c_mode = ROHC_MODE_R;
    p_csc->c_state = ROHC_C_ST_FO;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_c_p2_o_mt_o_to_r_so_dn");
}


