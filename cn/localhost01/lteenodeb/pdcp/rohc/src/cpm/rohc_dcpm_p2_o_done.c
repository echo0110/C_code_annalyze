/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dcpm_p2_o_done.c,v 1.1.2.1 2010/11/03 09:10:05 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function declaration for O-mode
 *                     decompressor CPM, when decompressor transition state is
 *                     DONE.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dcpm_p2_o_done.c,v $
 * Revision 1.1.2.1  2010/11/03 09:10:05  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.1.2.1  2010/10/01 14:12:54  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.3  2009/05/27 12:27:18  gur19836
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
 * Function Name  : rohc_d_p2_o_ir_nc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is O, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_ir_nc_dn ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_ir_nc_dn");
	if(p_dsc->decom_res)
	{
		p_dsc->d_state = ROHC_D_ST_FC;
		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                  "Upwards state transition NC->FC"
		                  " and sending ACK \n",
		                  p_entity->rohc_entity_id.ueIndex,
                          p_entity->rohc_entity_id.lcId,
		                  ROHC_GET_DSC_CID(p_dsc));
				                          
		rohc_dcpm_p2_send_ack(p_entity, p_dsc, ROHC_TRUE);		
	}
	else
	{
		/* decom failed */
		/* send static-NACK*/
		/* RFC-3095 5.4.2.2 */
		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                  "Decompression is unsuccessful "
		                  "therefore sending STATIC-NACK\n",
		                  p_entity->rohc_entity_id.ueIndex,
                          p_entity->rohc_entity_id.lcId,
                          ROHC_GET_DSC_CID(p_dsc));
		rohc_dcpm_p2_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
	}	
	ROHC_EXIT_FN("rohc_d_p2_o_ir_nc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_ir_sc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is O, state is SC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_ir_sc_dn ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_ir_sc_dn");
	if(p_dsc->decom_res)
	{
		p_dsc->d_state = ROHC_D_ST_FC;
		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Upwards state transition SC->FC"
		                          " and sending ACK \n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
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
	ROHC_EXIT_FN("rohc_d_p2_o_ir_sc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_ir_fc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is O, state is FC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_ir_fc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                              rohc_entity_t  *p_entity _AND_ 
                                              rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_ir_fc_dn");
	if(p_dsc->decom_res)
	{
		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Decompression is successful "
		                          "therefore sending ACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
		rohc_dcpm_p2_send_ack(p_entity, p_dsc, ROHC_TRUE);
		
	}
	else 
	{
		p_dsc->k_1++;
		if((p_dsc->k_1 >= p_entity->k_1) && (p_dsc->n_1 >= p_entity->n_1))
		{
				p_dsc->d_state = ROHC_D_ST_SC;
				p_dsc->k_1 = 0;			
				p_dsc->n_1 = 0;
				ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Downwards state transition SC->NC\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
		
			ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
			                          "Decompression is unsuccessful "
			                          "therefore sending STATIC_NACK\n",
			                          p_entity->rohc_entity_id.ueIndex,
                                      p_entity->rohc_entity_id.lcId,
			                          ROHC_GET_DSC_CID(p_dsc));
			rohc_dcpm_p2_send_static_nack(p_entity, p_dsc, ROHC_TRUE);	
		}
		
	}
		
	
	ROHC_EXIT_FN("rohc_d_p2_o_ir_fc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_irdyn_nc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is O, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_irdyn_nc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                 rohc_entity_t *p_entity _AND_
                                                 rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_irdyn_nc_dn");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "IR-DYN packet received in NC state, "
		                          "therefore sending STATIC_NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));	
	rohc_dcpm_p2_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
	ROHC_EXIT_FN("rohc_d_p2_o_irdyn_nc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_irdyn_sc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is O, state is SC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_irdyn_sc_dn ROHC_PARAMS((p_entity, p_dsc),
                                                 rohc_entity_t *p_entity _AND_
                                                 rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_irdyn_sc_dn");
	rohc_d_p2_o_ir_sc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_p2_o_irdyn_sc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_irdyn_fc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is O, state is FC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_irdyn_fc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                 rohc_entity_t *p_entity _AND_
                                                 rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_irdyn_fc_dn");
	rohc_d_p2_o_ir_fc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_p2_o_irdyn_fc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_uo_0_nc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_0_ET  , when decompressor mode is O, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_uo_0_nc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                rohc_entity_t *p_entity _AND_
                                                rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_uo_0_nc_dn");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "UO-0 packet received in NC state, "
		                          "therefore sending STATIC_NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));		
	rohc_dcpm_p2_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
	ROHC_EXIT_FN("rohc_d_p2_o_uo_0_nc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_uo_0_sc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_0_ET  , when decompressor mode is O, state is SC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_uo_0_sc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                rohc_entity_t *p_entity _AND_
                                                rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_uo_0_sc_dn");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "UO-0 packet received in SC state, "
		                          "therefore sending NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	rohc_dcpm_p2_send_nack(p_entity, p_dsc, ROHC_TRUE);
	ROHC_EXIT_FN("rohc_d_p2_o_uo_0_sc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_uo_0_fc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_0_ET  , when decompressor mode is O, state is FC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_uo_0_fc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                rohc_entity_t *p_entity _AND_ 
                                                rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_uo_0_fc_dn");
	if(!p_dsc->decom_res )
	{
		p_entity->k_1++;	
		if((p_dsc->k_1>=p_entity->k_1) && (p_dsc->n_1>=p_entity->n_1))	
		{
				p_dsc->d_state = ROHC_D_ST_SC;
				p_dsc->k_1 = 0;			
				p_dsc->n_1 = 0;
				ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Decompressor state is updated to FC->SC\n",
		                           p_entity->rohc_entity_id.ueIndex,
                                   p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
		
				ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Decompression failed for received UO-0 packet"
		                          "therefore sending NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
				rohc_dcpm_p2_send_nack(p_entity, p_dsc, ROHC_TRUE);	
		}		
	}
	
	
	ROHC_EXIT_FN("rohc_d_p2_o_uo_0_fc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_uo_1_nc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_1_ET  , when decompressor mode is O, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_uo_1_nc_dn ROHC_PARAMS((p_entity, p_dsc),
                                                rohc_entity_t *p_entity _AND_ 
                                                rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_uo_1_nc_dn");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "UO-1 packet receiveed in NC state and "
		                          "therefore sending STATIC_NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	rohc_dcpm_p2_send_static_nack(p_entity, p_dsc, ROHC_TRUE);

	ROHC_EXIT_FN("rohc_d_p2_o_uo_1_nc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_uo_1_sc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_1_ET  , when decompressor mode is O, state is SC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_uo_1_sc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                rohc_entity_t *p_entity _AND_ 
                                                rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_uo_1_sc_dn");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "UO-1 packet received in SC state and "
		                          "therefore sending NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	rohc_dcpm_p2_send_nack(p_entity, p_dsc, ROHC_TRUE);

	ROHC_EXIT_FN("rohc_d_p2_o_uo_1_sc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_uo_1_fc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_1_ET  , when decompressor mode is O, state is FC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_uo_1_fc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                rohc_entity_t *p_entity _AND_ 
                                                rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_uo_1_fc_dn");
	if(!p_dsc->decom_res )
	{
	
		if((p_dsc->k_1>=p_entity->k_1) && (p_dsc->n_1>=p_entity->n_1))	
		{
				p_dsc->d_state = ROHC_D_ST_SC;
				p_dsc->k_1 = 0;			
				p_dsc->n_1 = 0;
				ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Downwards state transition FC->SC\n",
		                           p_entity->rohc_entity_id.ueIndex,
                                   p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
		
				ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Decompression failed for received UO-1 "
		                          "packet and therefore sending NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
				rohc_dcpm_p2_send_nack(p_entity, p_dsc, ROHC_TRUE);	
		}		
	}
	ROHC_EXIT_FN("rohc_d_p2_o_uo_1_fc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_uo_2_nc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_2_ET  , when decompressor mode is O, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_uo_2_nc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                rohc_entity_t *p_entity _AND_ 
                                                rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_uo_2_nc_dn");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "UO-1 packet received in SC state and "
		                          "therefore sending NACK\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	rohc_dcpm_p2_send_static_nack(p_entity, p_dsc, ROHC_TRUE);

	ROHC_EXIT_FN("rohc_d_p2_o_uo_2_nc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_uo_2_sc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_2_ET  , when decompressor mode is O, state is SC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_uo_2_sc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                rohc_entity_t *p_entity _AND_ 
                                                rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_uo_2_sc_dn");
	rohc_d_p2_o_ir_sc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_p2_o_uo_2_sc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_uo_2_fc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_2_ET  , when decompressor mode is O, state is FC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_uo_2_fc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                rohc_entity_t *p_entity _AND_ 
                                                rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_uo_2_fc_dn");
	rohc_d_p2_o_ir_sc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_p2_o_uo_2_fc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_ack_o_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_O_ET  , when decompressor mode is O, state is NC/SC/FC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_ack_o_dn ROHC_PARAMS((p_entity, p_dsc), 
                                              rohc_entity_t *p_entity _AND_ 
                                              rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_ack_o_dn");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "ACK_O handling, but "
		                          " nothing needs to be done in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_p2_o_ack_o_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_static_nack_o_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_O_ET  , when decompressor mode is O, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_static_nack_o_dn ROHC_PARAMS((p_entity, p_dsc),
                                                   rohc_entity_t *p_entity _AND_ 
                                                   rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_static_nack_o_dn");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "STATIC_NACK_O handling, but "
		                          " nothing needs to be done in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_p2_o_static_nack_o_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_nack_o_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_O_ET  , when decompressor mode is O, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_nack_o_dn ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_nack_o_dn");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "NACK_O handling, but "
		                          " nothing needs to be done in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));


	ROHC_EXIT_FN("rohc_d_p2_o_nack_o_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_mt_o_to_u_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_O_TO_U_ET  , when decompressor mode is O, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_mt_o_to_u_dn ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_mt_o_to_u_dn");
	p_dsc->d_trans_state  = ROHC_D_TST_INITIATED;
	p_dsc->temp_mode = ROHC_MODE_U;
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Performing mode transition(O->U) and"
		                          " temp mode in DSC is set to U and transition"
		                          " state is set to INITIATED.\n",		                         
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	ROHC_EXIT_FN("rohc_d_p2_o_mt_o_to_u_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p2_o_mt_o_to_r_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_O_TO_R_ET  , when decompressor mode is O, state is NC/FS/SC
 *                  and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p2_o_mt_o_to_r_dn ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_udp_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_p2_o_mt_o_to_r_dn");
	p_dsc->d_trans_state  = ROHC_D_TST_INITIATED;
	p_dsc->temp_mode = ROHC_MODE_R;
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Performing mode transition(O->R) and"
		                          " temp mode in DSC is set to R and transition"
		                          " state is set to INITIATED.\n",		                         
		                          p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	ROHC_EXIT_FN("rohc_d_p2_o_mt_o_to_r_dn");
}


#endif
