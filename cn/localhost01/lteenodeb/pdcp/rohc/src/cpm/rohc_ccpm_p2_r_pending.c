/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_ccpm_p2_r_pending.c,v 1.1.2.1 2010/11/03 09:12:49 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definitions for R-mode
 *                     compressor CPM, when compressor transition state is 
 *                     PENDING.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_ccpm_p2_r_pending.c,v $
 * Revision 1.1.2.1  2010/11/03 09:12:49  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.1.2.1  2010/10/13 03:39:12  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.1.2.1  2010/09/23 05:03:33  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.3  2009/05/27 09:57:23  gur19836
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
 * Function Name  : rohc_c_p2_r_ir_ir_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when compressor mode is R, state is IR and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_r_ir_ir_pnd ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_p2_r_ir_ir_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[IR-Packet handling], "
                                  "but nothing needs to be " 
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_p2_r_ir_ir_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_r_irdyn_fo_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when compressor mode is R, state is FO and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_r_irdyn_fo_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_p2_r_irdyn_fo_pnd");
   /* don't move to SO */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[IR-DYN Packet handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));      
	ROHC_EXIT_FN("rohc_c_p2_r_irdyn_fo_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_r_r_2_fo_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_2_ET  , when compressor mode is R, state is FO and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_r_r_2_fo_pnd ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_p2_r_r_2_fo_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[R2-* packet handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));

	ROHC_EXIT_FN("rohc_c_p2_r_r_2_fo_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_r_ack_r_ir_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_R_ET  , when compressor mode is R, state is IR and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_r_ack_r_ir_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_p2_r_ack_r_ir_pnd");
	p_csc->c_trans_state = ROHC_C_TST_DONE;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
	                          "ACK_R handling,the transition state is updated "
	                          "to DONE\n",
	                          p_entity->rohc_entity_id.ueIndex,
                              p_entity->rohc_entity_id.lcId,
	                          ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_p2_r_ack_r_ir_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_r_static_nack_ir_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_ET  , when compressor mode is R, state is IR and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_r_static_nack_ir_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_p2_r_static_nack_ir_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[STATIC_NACK_R handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_p2_r_static_nack_ir_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_r_static_nack_fo_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_ET  , when compressor mode is R, state is FO and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_r_static_nack_fo_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_p2_r_static_nack_fo_pnd");
	p_csc->c_state = ROHC_C_ST_IR;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_p2_r_static_nack_fo_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_r_nack_ir_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_ET  , when compressor mode is R, state is IR and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_r_nack_ir_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_p2_r_nack_ir_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[NACK_R handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_p2_r_nack_ir_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_c_p2_r_nack_fo_pnd
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_ET  , when compressor mode is R, state is FO and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_p2_r_nack_fo_pnd ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_udp_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_p2_r_nack_fo_pnd");
	p_csc->send_irdyn_flag = ROHC_TRUE;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_p2_r_nack_fo_pnd");
}

