/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dcpm_o_pending.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definition for O-mode
 *                     decompressor CPM, when decompressor transition state is
 *                     PENDING.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dcpm_o_pending.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
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
 * Function Name  : rohc_d_o_ir_nc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is O, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_ir_nc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_ir_nc_pnd");
	rohc_d_o_ir_nc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_o_ir_nc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_ir_sc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is O, state is SC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_ir_sc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_ir_sc_pnd");
	rohc_d_o_ir_sc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_o_ir_sc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_ir_fc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is O, state is FC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_ir_fc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_ir_fc_pnd");
	rohc_d_o_ir_fc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_o_ir_fc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_irdyn_nc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is O, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_irdyn_nc_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_irdyn_nc_pnd");
	rohc_d_o_irdyn_nc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_o_irdyn_nc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_irdyn_sc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is O, state is SC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_irdyn_sc_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_irdyn_sc_pnd");
	rohc_d_o_irdyn_sc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_o_irdyn_sc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_irdyn_fc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is O, state is FC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_irdyn_fc_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_irdyn_fc_pnd");
	rohc_d_o_irdyn_fc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_o_irdyn_fc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_uo_0_nc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_0_ET  , when decompressor mode is O, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_uo_0_nc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_uo_0_nc_pnd");
	rohc_d_o_uo_0_nc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_o_uo_0_nc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_uo_0_sc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_0_ET  , when decompressor mode is O, state is SC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_uo_0_sc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_uo_0_sc_pnd");
	rohc_d_o_uo_0_sc_dn(p_entity, p_dsc);
	ROHC_EXIT_FN("rohc_d_o_uo_0_sc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_uo_0_fc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_0_ET  , when decompressor mode is O, state is FC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_uo_0_fc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_uo_0_fc_pnd");
	if(p_dsc->decom_res)
	{
		p_dsc->d_trans_state  = ROHC_D_TST_DONE;
	}
	else
	{
		rohc_d_o_uo_0_fc_dn(p_entity, p_dsc);
	}

	ROHC_EXIT_FN("rohc_d_o_uo_0_fc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_uo_1_nc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_1_ET  , when decompressor mode is O, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_uo_1_nc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_uo_1_nc_pnd");
	rohc_d_o_uo_1_nc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_o_uo_1_nc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_uo_1_sc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_1_ET  , when decompressor mode is O, state is SC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_uo_1_sc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_uo_1_sc_pnd");
	rohc_d_o_uo_1_sc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_o_uo_1_sc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_uo_1_fc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_1_ET  , when decompressor mode is O, state is FC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_uo_1_fc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_uo_1_fc_pnd");
	if(p_dsc->decom_res)
	{
		p_dsc->d_trans_state  = ROHC_D_TST_DONE;
	}
	else
	{
		rohc_d_o_uo_1_fc_dn(p_entity, p_dsc );
	}

	ROHC_EXIT_FN("rohc_d_o_uo_1_fc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_uo_2_nc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_2_ET  , when decompressor mode is O, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_uo_2_nc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_uo_2_nc_pnd");
	rohc_d_o_uo_2_nc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_o_uo_2_nc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_uo_2_sc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_2_ET  , when decompressor mode is O, state is SC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_uo_2_sc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_uo_2_sc_pnd");
	rohc_d_o_uo_2_sc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_o_uo_2_sc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_uo_2_fc_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  UO_2_ET  , when decompressor mode is O, state is FC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_uo_2_fc_pnd ROHC_PARAMS(
					(p_entity, p_dsc),
				 	rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_uo_2_fc_pnd");
	rohc_d_o_uo_2_fc_dn(p_entity, p_dsc);

	ROHC_EXIT_FN("rohc_d_o_uo_2_fc_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_ack_o_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_O_ET  , when decompressor mode is O, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_ack_o_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_ack_o_pnd");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "ACK_O handling, but "
		                          " nothing needs to be done in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));
	ROHC_EXIT_FN("rohc_d_o_ack_o_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_static_nack_o_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_O_ET  , when decompressor mode is O, state is NC
 *                  and transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_static_nack_o_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_static_nack_o_pnd");
		ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "ACK_O handling, but "
		                          " nothing needs to be done in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_o_static_nack_o_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_nack_o_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_O_ET  , when decompressor mode is O, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_nack_o_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_nack_o_pnd");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "ACK_O handling, but "
		                          " nothing needs to be done in DCPM\n",
		                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_o_nack_o_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_mt_o_to_u_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_O_TO_U_ET  , when decompressor mode is O, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_mt_o_to_u_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_mt_o_to_u_pnd");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Invalid mode transition(O->U), as another "
								  "mode transition is going on",
		                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_o_mt_o_to_u_pnd");
}

/*****************************************************************************
 * Function Name  : rohc_d_o_mt_o_to_r_pnd
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_O_TO_R_ET  , when decompressor mode is O, state is NC and
 *                  transition state is PENDING for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_o_mt_o_to_r_pnd ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
	ROHC_ENTER_FN("rohc_d_o_mt_o_to_r_pnd");
	ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
		                          "Invalid mode transition(O->R), as another "
								  "mode transition is going on",
		                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
		                          ROHC_GET_DSC_CID(p_dsc));

	ROHC_EXIT_FN("rohc_d_o_mt_o_to_r_pnd");
}


#endif
