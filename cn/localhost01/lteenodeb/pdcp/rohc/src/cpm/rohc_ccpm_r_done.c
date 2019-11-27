/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_ccpm_r_done.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definitions for R-mode 
 *                     compressor CPM, when compressor transition state is DONE.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_ccpm_r_done.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/27 09:30:02  gur19836
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
 * Function Name  : rohc_c_r_ir_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when compressor mode is R, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_ir_ir_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_ir_ir_dn");
	/*do nothing */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[IR-Packet handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_r_ir_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_irdyn_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when compressor mode is R, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_irdyn_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_irdyn_fo_dn");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[IR-DYN Packet handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_r_irdyn_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_r_0_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_ET  , when compressor mode is R, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_r_0_so_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_r_0_so_dn");
	/*do nothing , taken care in CPRM */
	/*p_csc->r0_crc_ack_flag = ROHC_FALSE;*/
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[R0 packet handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_r_r_0_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_r_0_crc_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_CRC_ET  , when compressor mode is R, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_r_0_crc_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_r_0_crc_so_dn");
	/*do nothing , taken care in CPRM */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[R0-CRC packet handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_r_r_0_crc_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_r_1_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_1_ET  , when compressor mode is R, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_r_1_fo_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_r_1_fo_dn");
	/*do nothing , taken care in CPRM */
	/*p_csc->uor2_ack_flag = ROHC_FALSE;*/
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[R1-* packet handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_r_r_1_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_r_2_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_2_ET  , when compressor mode is R, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_r_2_fo_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_r_2_fo_dn");
/*do nothing , taken care in CPRM */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[R2-* packet handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_r_r_2_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_ack_r_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_R_ET  , when compressor mode is R, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_ack_r_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_ack_r_ir_dn");
	p_csc->c_state  = ROHC_C_ST_FO;  
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[ACK_R handling], "
                                  "the state is udadated to FO \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
                                
	ROHC_EXIT_FN("rohc_c_r_ack_r_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_ack_r_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_R_ET  , when compressor mode is R, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_ack_r_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_ack_r_fo_dn");
	p_csc->c_state  = ROHC_C_ST_SO;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[ACK_R handling], "
                                  "the state is udadated to SO \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_r_ack_r_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_ack_r_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_R_ET  , when compressor mode is R, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_ack_r_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_ack_r_so_dn");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[ACK_R handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_r_ack_r_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_static_nack_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_ET  , when compressor mode is R, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_static_nack_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_static_nack_ir_dn");
	/* do nothing */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[STATIC_NACK_R handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_r_static_nack_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_static_nack_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_ET  , when compressor mode is R, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_static_nack_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_static_nack_fo_dn");
	p_csc->c_state = ROHC_C_ST_IR;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_r_static_nack_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_static_nack_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_ET  , when compressor mode is R, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_static_nack_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_static_nack_so_dn");
	p_csc->c_state = ROHC_C_ST_IR;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_r_static_nack_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_nack_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_ET  , when compressor mode is R, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_nack_ir_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_nack_ir_dn");
	/*do nothing*/
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                  "Valid operation[NACK_R handling], "
                                  "but nothing needs to be "
                                  "done in CCPM \n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_CSC_CID(p_csc));
	ROHC_EXIT_FN("rohc_c_r_nack_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_nack_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_ET  , when compressor mode is R, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_nack_fo_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_nack_fo_dn");
	p_csc->send_irdyn_flag = ROHC_TRUE;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_r_nack_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_nack_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_ET  , when compressor mode is R, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_nack_so_dn ROHC_PARAMS((p_entity, p_csc), rohc_entity_t
                                       *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_nack_so_dn");
	p_csc->c_state = ROHC_C_ST_FO;
	p_csc->send_irdyn_flag = ROHC_TRUE;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_r_nack_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_mt_r_to_u_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_U_ET  , when compressor mode is R, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_mt_r_to_u_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_mt_r_to_u_ir_dn");
	p_csc->c_trans_state  = ROHC_C_TST_PENDING;
	p_csc->c_mode = ROHC_MODE_U;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_r_mt_r_to_u_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_mt_r_to_u_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_U_ET  , when compressor mode is R, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_mt_r_to_u_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_mt_r_to_u_fo_dn");
	p_csc->c_trans_state  = ROHC_C_TST_PENDING;
	p_csc->c_mode = ROHC_MODE_U;
	
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_r_mt_r_to_u_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_mt_r_to_u_so_dn
 * Inputs         :  p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_U_ET  , when compressor mode is R, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_mt_r_to_u_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_mt_r_to_u_so_dn");
	p_csc->c_trans_state  = ROHC_C_TST_PENDING;
	p_csc->c_mode = ROHC_MODE_U;
	p_csc->c_state = ROHC_C_ST_FO;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_r_mt_r_to_u_so_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_mt_r_to_o_ir_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_O_ET  , when compressor mode is R, state is IR and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_mt_r_to_o_ir_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_mt_r_to_o_ir_dn");
	p_csc->c_trans_state  = ROHC_C_TST_PENDING;
	p_csc->c_mode = ROHC_MODE_O;
	
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */

	ROHC_EXIT_FN("rohc_c_r_mt_r_to_o_ir_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_mt_r_to_o_fo_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_O_ET  , when compressor mode is R, state is FO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_mt_r_to_o_fo_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_mt_r_to_o_fo_dn");
	p_csc->c_trans_state  = ROHC_C_TST_PENDING;
	p_csc->c_mode = ROHC_MODE_O;
	
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_r_mt_r_to_o_fo_dn");
}

/*****************************************************************************
 * Function Name  : rohc_c_r_mt_r_to_o_so_dn
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_O_ET  , when compressor mode is R, state is SO and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_c_r_mt_r_to_o_so_dn ROHC_PARAMS((p_entity, p_csc),
                        rohc_entity_t *p_entity _AND_ rohc_csc_t *p_csc)
{
	ROHC_ENTER_FN("rohc_c_r_mt_r_to_o_so_dn");
	p_csc->c_trans_state  = ROHC_C_TST_PENDING;
	p_csc->c_mode = ROHC_MODE_O;
	p_csc->c_state = ROHC_C_ST_FO;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_c_r_mt_r_to_o_so_dn");
}



