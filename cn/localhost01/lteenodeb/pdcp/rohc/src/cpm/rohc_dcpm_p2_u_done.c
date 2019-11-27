/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dcpm_p2_u_done.c,v 1.1.2.1 2010/11/03 09:10:29 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definition for U-mode
 *                     decompressor CPM, when decompressor transition state is
 *                     DONE.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dcpm_p2_u_done.c,v $
 * Revision 1.1.2.1  2010/11/03 09:10:29  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.1.2.1  2010/10/01 14:16:10  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
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
 * Function Name  : rohc_d_p2_u_ir_nc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is U, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_u_ir_nc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                              rohc_entity_t *p_entity _AND_ 
                                              rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_u_ir_nc_dn");
	
	if(p_dsc->decom_res)
	{
		p_dsc->d_state = ROHC_D_ST_FC;
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
        /* +- SPR 17777 */
		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Decompressor State is updated to FC\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));		
	}
	ROHC_EXIT_FN("rohc_d_p2_u_ir_nc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_u_ir_irdyn_uor2_sc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET/IR_DYN_ET/UOR2_ET , when decompressor mode is U, 
 *                  state is SC and transition state is DONE for compressed
 *                  profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_u_ir_irdyn_uor2_sc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                         rohc_entity_t *p_entity _AND_ 
                                                         rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_u_ir_irdyn_uor2_sc_dn");
	
	if(p_dsc->decom_res)
	{
		p_dsc->d_state = ROHC_D_ST_FC;
		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Decompressor State is updated to FC\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	}
	else 
	{
		p_dsc->k_2++; 
                if((p_dsc->k_2>=p_entity->k_2) && (p_dsc->n_2>=p_entity->n_2))	
		{
			p_dsc->d_state = ROHC_D_ST_NC;
			p_dsc->k_2 = 0;			
			p_dsc->n_2 = 0;
            /* +- SPR 17777 */
            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
            /* +- SPR 17777 */
			ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
	                          "Decompressor State is updated to NC\n",
	                          p_entity->rohc_entity_id.ueIndex,
                              p_entity->rohc_entity_id.lcId,
	                          ROHC_GET_DSC_CID(p_dsc));
		}
	}
		
	ROHC_EXIT_FN("rohc_d_p2_u_ir_irdyn_uor2_sc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_u_all_fc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRT/UO_1/UO_2/UO_0/IR_DYN , when decompressor mode is U,
 *                  state is FC and transition state is DONE for compressed
 *                  profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_u_all_fc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                               rohc_entity_t *p_entity _AND_ 
                                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_u_all_fc_dn");
	if( !(p_dsc->decom_res))
	{
		p_dsc->k_1++;	
		if((p_dsc->k_1>=p_entity->k_1) && (p_dsc->n_1>=p_entity->n_1)) 
		{
			p_dsc->d_state = ROHC_D_ST_SC;
            /* +- SPR 17777 */
            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
            /* +- SPR 17777 */
			ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
	                          "Decompressor State is updated to SC\n",
	                          p_entity->rohc_entity_id.ueIndex,
                              p_entity->rohc_entity_id.lcId,
	                          ROHC_GET_DSC_CID(p_dsc));
			p_dsc->k_1 = 0;			
			p_dsc->n_1 = 0;
		}
	}
	ROHC_EXIT_FN("rohc_d_p2_u_all_fc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_u_ack_u_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_U_ET  , when decompressor mode is U, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_u_ack_u_dn ROHC_PARAMS((p_entity, p_dsc), 
                                              rohc_entity_t *p_entity _AND_ 
                                              rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_u_ack_u_dn");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "ACK_U handling at decompressor side, "
		                          " but nothing needs to be done.",		                          
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));	
	ROHC_EXIT_FN("rohc_d_p2_u_ack_u_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_u_mt_u_to_o_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_U_TO_O_ET  , when decompressor mode is U, state is 
 *                  NC/SC/FC and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_u_mt_u_to_o_dn ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_u_mt_u_to_o_dn");
	p_dsc->d_mode = ROHC_MODE_O;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Performing mode transition(U->O) and"
		                          "mode in DSC is set to O, but transition"
		                          " state is not modified.\n",		                         
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	ROHC_EXIT_FN("rohc_d_p2_u_mt_u_to_o_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_u_mt_u_to_r_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_U_TO_R_ET  , when decompressor mode is U, state is NC
 *                  and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_u_mt_u_to_r_dn ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_u_mt_u_to_r_dn");
	p_dsc->temp_mode = ROHC_MODE_R;
	p_dsc->d_trans_state = ROHC_D_TST_INITIATED;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Performing mode transition(U->R) and"
		                          "temp mode in DSC is set to R, but transition"
		                          " state is not modified.\n",		                         
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	ROHC_EXIT_FN("rohc_d_p2_u_mt_u_to_r_dn");
}

