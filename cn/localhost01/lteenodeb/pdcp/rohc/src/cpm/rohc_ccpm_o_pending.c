/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_ccpm_o_pending.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definitions for O-mode
 *                     compressor CPM, when compressor transition state is
 *                     PENDING.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_ccpm_o_pending.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/27 08:41:05  gur19836
 * File Header and function headers updated
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
 * Function Name  : rohc_c_o_irdyn_fo_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when compressor mode is O, state is FO and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_o_irdyn_fo_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_o_irdyn_fo_pnd");
	/*State transition not needed */

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_o_irdyn_fo_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_o_ack_o_ir_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_O_ET  , when compressor mode is O, state is IR and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_o_ack_o_ir_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_o_ack_o_ir_pnd");
	p_csc->c_trans_state  = ROHC_C_TST_DONE;
	p_csc->c_state  = ROHC_C_ST_FO;
	p_csc->dynamic_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_o_ack_o_ir_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_o_ack_o_fo_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_O_ET  , when compressor mode is O, state is FO and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_o_ack_o_fo_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_o_ack_o_fo_pnd");
	p_csc->c_trans_state  = ROHC_C_TST_DONE;
	p_csc->c_state  = ROHC_C_ST_SO;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_o_ack_o_fo_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_o_static_nack_o_ir_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_O_ET  , when compressor mode is O, state is IR
 *                  and transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_o_static_nack_o_ir_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_o_static_nack_o_ir_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[ STATIC_NACK_O handling ], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_o_static_nack_o_ir_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_o_static_nack_o_fo_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_O_ET  , when compressor mode is O, state is FO
 *                  and transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_o_static_nack_o_fo_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_o_static_nack_o_fo_pnd");
	p_csc->c_state  = ROHC_C_ST_IR;
	p_csc->static_info_update_counter = 0;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_o_static_nack_o_fo_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_o_nack_o_ir_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_O_ET  , when compressor mode is O, state is IR and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_o_nack_o_ir_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_o_nack_o_ir_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[ NACK_O handling ], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_o_nack_o_ir_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_o_nack_o_fo_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_O_ET  , when compressor mode is O, state is FO and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_o_nack_o_fo_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_o_nack_o_fo_pnd");
	p_csc->send_irdyn_flag = ROHC_TRUE; 
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_o_nack_o_fo_pnd");
}
