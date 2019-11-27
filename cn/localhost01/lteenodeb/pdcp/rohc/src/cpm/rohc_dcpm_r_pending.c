/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dcpm_r_pending.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definition for R-mode
 *                     decompressor CPM, when decompressor transition state is
 *                     PENDING.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dcpm_r_pending.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
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
 * Function Name  : rohc_d_r_ir_nc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is R, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_ir_nc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_ir_nc_pnd");
	rohc_d_r_ir_nc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_r_ir_nc_pnd"); 
}

/*****************************************************************************
 * Function Name  : rohc_d_r_ir_sc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is R, state is SC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_ir_sc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_ir_sc_pnd");
	rohc_d_r_ir_sc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_r_ir_sc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_ir_fc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is R, state is FC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_ir_fc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_ir_fc_pnd");
	rohc_d_r_ir_fc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_r_ir_fc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_irdyn_nc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is R, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_irdyn_nc_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_irdyn_nc_pnd");
	rohc_d_r_irdyn_nc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_r_irdyn_nc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_irdyn_sc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is R, state is SC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_irdyn_sc_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_irdyn_sc_pnd");
	rohc_d_r_irdyn_sc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_r_irdyn_sc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_irdyn_fc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is R, state is FC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_irdyn_fc_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_irdyn_fc_pnd");
	rohc_d_r_irdyn_fc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_r_irdyn_fc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_0_nc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_ET  , when decompressor mode is R, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_0_nc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_0_nc_pnd");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "R-0 is not updating packet and received "
		                          "in the NC state "
		                          " therefore nothing needs to be done"
		                          " in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));	
	ROHC_EXIT_FN("rohc_d_r_r_0_nc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_0_sc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_ET  , when decompressor mode is R, state is SC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_0_sc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_0_sc_pnd");
	/*rohc_d_r_r_0_sc_dn(p_entity, p_dsc);*/
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "R-0 is not updating packet and received "
		                          "in the SC state "
		                          " therefore nothing needs to be done"
		                          " in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));	
	ROHC_EXIT_FN("rohc_d_r_r_0_sc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_0_fc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_ET  , when decompressor mode is R, state is FC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_0_fc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_0_fc_pnd");
	/*rohc_d_r_r_0_fc_dn(p_entity, p_dsc);*/
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
	                          "R-0 (not updating) packet received "
	                          "in the FC state and PENDING transiton state"
	                          " therefore updating the transition state to"
	                          " DONE\n",
	                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
	                          ROHC_GET_DSC_CID(p_dsc));	
	p_dsc->d_trans_state  = ROHC_D_TST_DONE;
	ROHC_EXIT_FN("rohc_d_r_r_0_fc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_0_crc_nc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_CRC_ET  , when decompressor mode is R, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_0_crc_nc_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_0_crc_nc_pnd");
	rohc_d_r_r_0_crc_nc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_r_r_0_crc_nc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_0_crc_sc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_CRC_ET  , when decompressor mode is R, state is SC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_0_crc_sc_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_0_crc_sc_pnd");
	rohc_d_r_r_0_crc_sc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_r_r_0_crc_sc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_0_crc_fc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_CRC_ET  , when decompressor mode is R, state is FC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_0_crc_fc_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_0_crc_fc_pnd");
	if(p_dsc->decom_res)
	{
		p_dsc->d_trans_state  = ROHC_D_TST_DONE;
		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
	                          "R-0-CRC packet received "
	                          "in the FC state and PENDING transiton state"
	                          " therefore updating the transition state to"
	                          " DONE\n",
	                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
	                          ROHC_GET_DSC_CID(p_dsc));
	}
	else
	{
		rohc_d_r_r_0_crc_fc_dn(p_entity, p_dsc);
	}
	ROHC_EXIT_FN("rohc_d_r_r_0_crc_fc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_1_nc_pnd 
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_1_ET  , when decompressor mode is R, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_1_nc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_1_nc_pnd");
	rohc_d_r_r_1_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_r_r_1_nc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_1_sc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_1_ET  , when decompressor mode is R, state is SC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_1_sc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_1_sc_pnd");
	rohc_d_r_r_1_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_r_r_1_sc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_1_fc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_1_ET  , when decompressor mode is R, state is FC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_1_fc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_1_fc_pnd");
	
	p_dsc->d_trans_state  = ROHC_D_TST_DONE;
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
	                          "R-1-* (not updating) packet received "
	                          "in the FC state and PENDING transiton state"
	                          " therefore updating the transition state to"
	                          " DONE\n",
	                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
	                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_r_r_1_fc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_2_nc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_2_ET  , when decompressor mode is R, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_2_nc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_2_nc_pnd");
	rohc_d_r_r_2_nc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_r_r_2_nc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_2_sc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_2_ET  , when decompressor mode is R, state is SC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_2_sc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_2_sc_pnd");
	rohc_d_r_r_2_sc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_r_r_2_sc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_r_2_fc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_2_ET  , when decompressor mode is R, state is FC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_r_2_fc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_r_2_fc_pnd");
	rohc_d_r_r_2_fc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_r_r_2_fc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_ack_r_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_R_ET  , when decompressor mode is R, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_ack_r_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_ack_r_pnd");
	rohc_d_r_ack_r_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_r_ack_r_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_static_nack_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_ET  , when decompressor mode is R, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_static_nack_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_static_nack_pnd");
	rohc_d_r_static_nack_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_r_static_nack_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_nack_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_ET  , when decompressor mode is R, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_nack_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
                                             *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_nack_pnd");
	rohc_d_r_nack_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_r_nack_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_mt_r_to_u_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_U_ET  , when decompressor mode is R, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_mt_r_to_u_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_mt_r_to_u_pnd");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Invalid mode transition(R->U), as another "
								  "mode transition is going on",
		                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_r_mt_r_to_u_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_r_mt_r_to_o_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_O_ET  , when decompressor mode is R, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_r_mt_r_to_o_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_r_mt_r_to_o_pnd");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Invalid mode transition(R->O), as another "
								  "mode transition is going on",
		                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_r_mt_r_to_o_pnd");
}


#endif
