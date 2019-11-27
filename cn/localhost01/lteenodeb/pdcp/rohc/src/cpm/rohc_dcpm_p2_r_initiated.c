/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dcpm_p2_r_initiated.c,v 1.1.2.1 2010/11/03 09:07:51 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definition for R-mode
 *                     decompressor-CPM, when the transition state is
 *                     INITIATED.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dcpm_p2_r_initiated.c,v $
 * Revision 1.1.2.1  2010/11/03 09:07:51  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.1.2.1  2010/10/01 14:15:06  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.3  2009/05/27 12:58:54  gur19836
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
#ifdef ROHC_MODE_TRIGGER
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

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*****************************************************************************
 * Function Name  : rohc_d_p2_r_ir_nc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is R, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_ir_nc_in ROHC_PARAMS((p_entity, p_dsc), 
                                              rohc_entity_t *p_entity _AND_ 
                                              rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_ir_nc_in");
	if(p_dsc->decom_res)
	{
		p_dsc->d_state = ROHC_D_ST_FC;
		p_dsc->d_trans_state = ROHC_D_TST_PENDING;
		p_dsc->d_mode = p_dsc->temp_mode; 
		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Upwards state transition NC->FC,"
		                          " Transition state is updated to PENDING,"
		                          " mode is set to %u" 
		                          " and sending ACK \n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc),p_dsc->d_mode);
		rohc_dcpm_p2_send_ack(p_entity, p_dsc, ROHC_TRUE);		
	}
	/*else
	{
		rohc_dcpm_p2_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
	}*/

	ROHC_EXIT_FN("rohc_d_p2_r_ir_nc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_ir_sc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is R, state is SC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_ir_sc_in ROHC_PARAMS((p_entity, p_dsc), 
                                              rohc_entity_t *p_entity _AND_ 
                                              rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_ir_sc_in");
	if(p_dsc->decom_res)
	{
		p_dsc->d_state = ROHC_D_ST_FC;
		p_dsc->d_trans_state = ROHC_D_TST_PENDING;
		p_dsc->d_mode = p_dsc->temp_mode; 
		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Upwards state transition NC->FC,"
		                          " Transition state is updated to PENDING,"
		                          " mode is set to %u" 
		                          " and sending ACK \n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc),p_dsc->d_mode);
		rohc_dcpm_p2_send_ack(p_entity, p_dsc, ROHC_TRUE);		
	}
	else 
	{
		p_dsc->k_2++; 
                if((p_dsc->k_2>=p_entity->k_2) && (p_dsc->n_2>=p_entity->n_2))	
		{
			p_dsc->d_state = ROHC_D_ST_NC;
			p_dsc->k_2 = 0;			
			p_dsc->n_2 = 0;
			ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Downwards state transition SC->NC\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
                
        	
        	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Decompression is unsuccessful "
		                          "therefore sending STATIC-NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
		
			rohc_dcpm_p2_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
		}	
	}
	ROHC_EXIT_FN("rohc_d_p2_r_ir_sc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_ir_fc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is R, state is FC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_ir_fc_in ROHC_PARAMS((p_entity, p_dsc), 
                                              rohc_entity_t *p_entity _AND_ 
                                              rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_ir_fc_in");
	if(p_dsc->decom_res)
	{
		p_dsc->d_trans_state = ROHC_D_TST_PENDING;
		p_dsc->d_mode = p_dsc->temp_mode; 
		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          " Transition state is updated to PENDING,"
		                          " mode is set to %u" 
		                          " and sending ACK \n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc),p_dsc->d_mode);
		rohc_dcpm_p2_send_ack(p_entity, p_dsc, ROHC_TRUE);		
	}
	else 
	{
		p_dsc->k_1++;
		if((p_dsc->k_1>=p_entity->k_1) && (p_dsc->n_1>=p_entity->n_1))	
		{
			p_dsc->d_state = ROHC_D_ST_NC;
			p_dsc->k_1 = 0;			
			p_dsc->n_1 = 0;
		
			              ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Downwards state transition SC->NC\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
        
        		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Decompression is unsuccessful "
		                          "therefore sending NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
        		rohc_dcpm_p2_send_nack(p_entity, p_dsc, ROHC_TRUE);
			
		}
		
	}
	ROHC_EXIT_FN("rohc_d_p2_r_ir_fc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_irdyn_nc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is R, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_irdyn_nc_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_irdyn_nc_in");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "IR-DYN packet is received in NC state, "
		                          "therefore sending STATIC-NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));	

	rohc_dcpm_p2_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
	ROHC_EXIT_FN("rohc_d_p2_r_irdyn_nc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_irdyn_sc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is R, state is SC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_irdyn_sc_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_irdyn_sc_in");
	rohc_d_p2_r_ir_sc_in(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_p2_r_irdyn_sc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_irdyn_fc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is R, state is FC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_irdyn_fc_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_irdyn_fc_in");
	rohc_d_p2_r_ir_fc_in(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_p2_r_irdyn_fc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_r_0_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_ET  , when decompressor mode is R, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_r_0_in ROHC_PARAMS((p_entity, p_dsc), 
                                            rohc_entity_t *p_entity _AND_ 
                                            rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_r_0_in");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "R-0 is not updating packet,"
		                          " therefore nothing needs to be done"
		                          " in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));	
	ROHC_EXIT_FN("rohc_d_p2_r_r_0_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_r_0_crc_nc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_CRC_ET  , when decompressor mode is R, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_r_0_crc_nc_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_r_0_crc_nc_in");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "IR-0-CRC packet is received in NC state, "
		                          "therefore sending STATIC-NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	rohc_dcpm_p2_send_static_nack(p_entity, p_dsc, ROHC_TRUE);

	ROHC_EXIT_FN("rohc_d_p2_r_r_0_crc_nc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_r_0_crc_sc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_CRC_ET  , when decompressor mode is R, state is SC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_r_0_crc_sc_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_r_0_crc_sc_in");
	/*rohc_dcpm_send_static_nack(p_entity, p_dsc, ROHC_TRUE);*/
	rohc_d_p2_r_ir_sc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_p2_r_r_0_crc_sc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_r_0_crc_fc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_CRC_ET  , when decompressor mode is R, state is FC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_r_0_crc_fc_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_r_0_crc_fc_in");
    rohc_d_p2_r_ir_fc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_p2_r_r_0_crc_fc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_r_1_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_1_ET  , when decompressor mode is R, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_r_1_in ROHC_PARAMS((p_entity, p_dsc), 
                                            rohc_entity_t *p_entity _AND_ 
                                            rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_r_1_in");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "R-1* is not updating packet,"
		                          " therefore nothing needs to be done"
		                          " in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	ROHC_EXIT_FN("rohc_d_p2_r_r_1_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_r_2_nc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_2_ET  , when decompressor mode is R, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_r_2_nc_in ROHC_PARAMS((p_entity, p_dsc), 
                                               rohc_entity_t *p_entity _AND_ 
                                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_r_2_nc_in");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "UOR-2* packet is received in NC state, "
		                          "therefore sending STATIC-NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	rohc_dcpm_p2_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
	ROHC_EXIT_FN("rohc_d_p2_r_r_2_nc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_r_2_sc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_2_ET  , when decompressor mode is R, state is SC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_r_2_sc_in ROHC_PARAMS((p_entity, p_dsc), 
                                               rohc_entity_t *p_entity _AND_ 
                                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_r_2_sc_in");
	rohc_d_p2_r_ir_sc_in(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_p2_r_r_2_sc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_r_2_fc_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_2_ET  , when decompressor mode is R, state is FC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_r_2_fc_in ROHC_PARAMS((p_entity, p_dsc), 
                                               rohc_entity_t  *p_entity _AND_ 
                                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_r_2_fc_in");
	rohc_d_p2_r_ir_fc_in(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_p2_r_r_2_fc_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_ack_r_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_R_ET  , when decompressor mode is R, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_ack_r_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_ack_r_in");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "ACK_R handling, but "
		                          " nothing needs to be done in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	ROHC_EXIT_FN("rohc_d_p2_r_ack_r_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_static_nack_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_ET  , when decompressor mode is R, state is NC
 *                  and transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_static_nack_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_static_nack_in");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "STAIC-NACK_R handling, but "
		                          " nothing needs to be done in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_p2_r_static_nack_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_nack_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_ET  , when decompressor mode is R, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_nack_in ROHC_PARAMS((p_entity, p_dsc), 
                                             rohc_entity_t *p_entity _AND_ 
                                             rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_nack_in");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "NACK_R handling, but "
		                          " nothing needs to be done in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_p2_r_nack_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_mt_r_to_u_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_U_ET  , when decompressor mode is R, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_mt_r_to_u_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_mt_r_to_u_in");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Invalid mode transition (R->U), as another "
								  "mode transition is going on",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_p2_r_mt_r_to_u_in");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_r_mt_r_to_o_in
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_O_ET  , when decompressor mode is R, state is NC and
 *                  transition state is INITIATED for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_r_mt_r_to_o_in ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_r_mt_r_to_o_in");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Invalid mode transition(R->O), as another "
								  "mode transition is going on",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_p2_r_mt_r_to_o_in");
}


#endif
