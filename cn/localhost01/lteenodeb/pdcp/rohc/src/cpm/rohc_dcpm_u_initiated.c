/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dcpm_u_initiated.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definition for U-mode
 *                     decompressor CPM, when decompressor transition state is
 *                     INITIATED.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dcpm_u_initiated.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/27 13:11:53  gur19836
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
#include "rohc_dcpm_proto.h"

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
 * Function Name  : rohc_d_u_ir_nc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is U, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_u_ir_nc_in ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_ir_nc_in");
    if(p_dsc->decom_res)
    {
        p_dsc->d_state = ROHC_D_ST_FC;      
        p_dsc->d_trans_state = ROHC_D_TST_PENDING;
        p_dsc->d_mode = p_dsc->temp_mode;
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Decompressor State is updated to FC, "
                                  "Transition state is set to PENDING and "
                                  "mode is set to %u\n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->d_mode);      
    }
    ROHC_EXIT_FN("rohc_d_u_ir_nc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_u_ir_sc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is U, state is SC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_u_ir_irdyn_uor2_sc_in ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_ir_sc_in");
    if(p_dsc->decom_res)
    {
        p_dsc->d_state = ROHC_D_ST_FC;      
        p_dsc->d_trans_state = ROHC_D_TST_PENDING;
        p_dsc->d_mode = p_dsc->temp_mode;
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Decompressor State is updated to FC, "
                                  "Transition state is set to PENDING and "
                                  "mode is set to %u\n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->d_mode);              
    }
    ROHC_EXIT_FN("rohc_d_u_ir_sc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_u_ir_fc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is U, state is FC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_u_ir_irdyn_uor2_fc_in ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_ir_fc_in");
    if(p_dsc->decom_res)
    {
        p_dsc->d_trans_state = ROHC_D_TST_PENDING;
        p_dsc->d_mode = p_dsc->temp_mode;
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Decompressor " 
                                  "Transition State is set to PENDING and "
                                  "mode is set to %u and sending an ACK\n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->d_mode);             
        rohc_dcpm_send_ack(p_entity, p_dsc, ROHC_TRUE);     
    }
    ROHC_EXIT_FN("rohc_d_u_ir_fc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_u_ack_u_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_U_ET  , when decompressor mode is U, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_u_ack_u_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_ack_u_in");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "ACK_U handling at decompressor side, "
                                  " but nothing needs to be done.",                               
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));    
    ROHC_EXIT_FN("rohc_d_u_ack_u_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_u_mt_u_to_o_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_U_TO_O_ET  , when decompressor mode is U, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_u_mt_u_to_o_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_mt_u_to_o_in");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Invalid mode transition(U->O), as another "
                                  "mode transition is going on",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_u_mt_u_to_o_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_u_mt_u_to_r_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_U_TO_R_ET  , when decompressor mode is U, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_u_mt_u_to_r_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_mt_u_to_r_in");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Invalid mode transition(U->R), as another "
                                  "mode transition is going on",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_u_mt_u_to_r_in");
}

