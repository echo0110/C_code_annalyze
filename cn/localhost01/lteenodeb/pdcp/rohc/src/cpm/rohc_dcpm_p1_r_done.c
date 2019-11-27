/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dcpm_p1_r_done.c,v 1.1.2.1 2010/11/09 05:06:48 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definition for R-mode
 *                     decompressor CPM, when decompressor transition state is
 *                     DONE.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dcpm_p1_r_done.c,v $
 * Revision 1.1.2.1  2010/11/09 05:06:48  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.1.2.1  2010/10/01 14:14:40  gur22059
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
 * Function Name  : rohc_d_p1_r_ir_nc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is R, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_ir_nc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                              rohc_entity_t *p_entity _AND_ 
                                              rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_ir_nc_dn");
    if(p_dsc->decom_res)
    {
        p_dsc->d_state = ROHC_D_ST_FC;
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Upwards state transition NC->FC"
                                  " and sending an ACK \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
        rohc_dcpm_p1_send_ack(p_entity, p_dsc, ROHC_TRUE);     
    }
    /*
    This is not required as compressor is in R- mode 
    and will send the IR unit it receives the ACK
    else
    {
        ROHC_LOG(LOG_INFO,("DCPM:EID[%u:%u]:CID[%u]:"
                                  "Decompression is unsuccessful "
                                  "therefore sending STATIC-NACK\n",
                                  p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc)));
        rohc_dcpm_p1_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
    } */  

    ROHC_EXIT_FN("rohc_d_p1_r_ir_nc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_ir_sc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is R, state is SC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_ir_sc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                              rohc_entity_t *p_entity _AND_ 
                                              rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_ir_sc_dn");
    if(p_dsc->decom_res)
    {
        p_dsc->d_state = ROHC_D_ST_FC;
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Upwards state transition SC->FC"
                                  " and sending an ACK \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
        rohc_dcpm_p1_send_ack(p_entity, p_dsc, ROHC_TRUE);     
    }
    else 
    {
       	p_dsc->k_2++; 
        if((p_dsc->k_2>=p_entity->k_2) && (p_dsc->n_2>=p_entity->n_2)) 
        {
            /* as per definition of context damage refer to 5.5.2.2.*/
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
                                  "therefore sending a STATIC-NACK\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));    
            rohc_dcpm_p1_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
       }    
    }
    ROHC_EXIT_FN("rohc_d_p1_r_ir_sc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_ir_fc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IR_ET  , when decompressor mode is R, state is FC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_ir_fc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                              rohc_entity_t *p_entity _AND_ 
                                              rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_ir_fc_dn");
    if(p_dsc->decom_res)
    {
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Decompression is successful "
                                  "therefore sending an ACK\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
        rohc_dcpm_p1_send_ack(p_entity, p_dsc, ROHC_TRUE);
        
    }
    else 
    {
       	if((p_dsc->k_1>=p_entity->k_1) && (p_dsc->n_1>=p_entity->n_1)) 
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
                                  "therefore sending a NACK\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
                rohc_dcpm_p1_send_nack(p_entity, p_dsc, ROHC_TRUE);
        }       
    }
        
    
    ROHC_EXIT_FN("rohc_d_p1_r_ir_fc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_irdyn_nc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is R, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_irdyn_nc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                 rohc_entity_t *p_entity _AND_ 
                                                 rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_irdyn_nc_dn");
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "IR-DYN packet is received in NC state, "
                                  "therefore sending a STATIC-NACK\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));    
    rohc_dcpm_p1_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
    ROHC_EXIT_FN("rohc_d_p1_r_irdyn_nc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_irdyn_sc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is R, state is SC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_irdyn_sc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                 rohc_entity_t *p_entity _AND_ 
                                                 rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_irdyn_sc_dn");
    rohc_d_p1_r_ir_sc_dn(p_entity, p_dsc);
    ROHC_EXIT_FN("rohc_d_p1_r_irdyn_sc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_irdyn_fc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  IRDYN_ET  , when decompressor mode is R, state is FC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_irdyn_fc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                                 rohc_entity_t *p_entity _AND_ 
                                                 rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_irdyn_fc_dn");
    rohc_d_p1_r_ir_fc_dn(p_entity, p_dsc);
    ROHC_EXIT_FN("rohc_d_p1_r_irdyn_fc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_r_0_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_ET  , when decompressor mode is R, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_r_0_dn ROHC_PARAMS((p_entity, p_dsc), 
                                            rohc_entity_t *p_entity _AND_ 
                                            rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_r_0_dn");
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "R-0 is not updating packet,"
                                  " therefore nothing needs to be done"
                                  " in DCPM\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));    
    /*rohc_dcpm_p1_send_static_nack(p_entity, p_dsc, ROHC_TRUE);*/
    ROHC_EXIT_FN("rohc_d_p1_r_r_0_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_r_0_crc_nc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_CRC_ET  , when decompressor mode is R, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_r_0_crc_nc_dn ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_r_0_crc_nc_dn");
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "IR-0-CRC packet is received in NC state, "
                                  "therefore sending a STATIC-NACK\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
    rohc_dcpm_p1_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
    
    ROHC_EXIT_FN("rohc_d_p1_r_r_0_crc_nc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_r_0_crc_sc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_CRC_ET  , when decompressor mode is R, state is SC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_r_0_crc_sc_dn ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_r_0_crc_sc_dn");
   /* rohc_dcpm_p1_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
   */
   /* as ROHC-3095 not saying on this */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "IR-0-CRC packet is received in FC state, "
                                  "therefore sending a NACK\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
    rohc_dcpm_p1_send_nack(p_entity, p_dsc, ROHC_TRUE);
   ROHC_EXIT_FN("rohc_d_p1_r_r_0_crc_sc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_r_0_crc_fc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_0_CRC_ET  , when decompressor mode is R, state is FC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_r_0_crc_fc_dn ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_r_0_crc_fc_dn");
    /* udpate the context PRM */
    /* send ack for first r_O_crc of a seq. this pkt*/
    rohc_d_p1_r_ir_fc_dn(p_entity, p_dsc);
    ROHC_EXIT_FN("rohc_d_p1_r_r_0_crc_fc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_r_1_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_1_ET  , when decompressor mode is R, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_r_1_dn ROHC_PARAMS((p_entity, p_dsc), 
                                            rohc_entity_t *p_entity _AND_ 
                                            rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_r_1_dn");
    /*rohc_dcpm_p1_send_static_nack(p_entity, p_dsc, ROHC_TRUE); */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "R-1* is not updating packet,"
                                  " therefore nothing needs to be done"
                                  " in DCPM\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_p1_r_r_1_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_r_2_nc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_2_ET  , when decompressor mode is R, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_r_2_nc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                               rohc_entity_t *p_entity _AND_ 
                                               rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_r_2_nc_dn");
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "UOR-2* packet is received in NC state, "
                                  "therefore sending a STATIC-NACK\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
    rohc_dcpm_p1_send_static_nack(p_entity, p_dsc, ROHC_TRUE);
    ROHC_EXIT_FN("rohc_d_p1_r_r_2_nc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_r_2_sc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_2_ET  , when decompressor mode is R, state is SC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_r_2_sc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                               rohc_entity_t *p_entity _AND_ 
                                               rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_r_2_sc_dn");

    rohc_d_p1_r_ir_sc_dn(p_entity, p_dsc);
    ROHC_EXIT_FN("rohc_d_p1_r_r_2_sc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_r_2_fc_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  R_2_ET  , when decompressor mode is R, state is FC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_r_2_fc_dn ROHC_PARAMS((p_entity, p_dsc), 
                                               rohc_entity_t *p_entity _AND_ 
                                               rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_r_2_fc_dn");

    rohc_d_p1_r_ir_fc_dn(p_entity, p_dsc);
    ROHC_EXIT_FN("rohc_d_p1_r_r_2_fc_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_ack_r_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ACK_R_ET  , when decompressor mode is R, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_ack_r_dn ROHC_PARAMS((p_entity, p_dsc), 
                                              rohc_entity_t *p_entity _AND_ 
                                              rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_ack_r_dn");
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "ACK_R handling, but "
                                  " nothing needs to be done in DCPM\n",
                                  p_entity->rohc_entity_id.ueIndex,\
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));

    ROHC_EXIT_FN("rohc_d_p1_r_ack_r_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_static_nack_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  STATIC_NACK_ET  , when decompressor mode is R, state is NC
 *                  and transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_static_nack_dn ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_static_nack_dn");
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "STATIC-NACK_R handling, but "
                                  " nothing needs to be done in DCPM\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));

    ROHC_EXIT_FN("rohc_d_p1_r_static_nack_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_nack_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  NACK_ET  , when decompressor mode is R, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_nack_dn ROHC_PARAMS((p_entity, p_dsc), 
                                             rohc_entity_t *p_entity _AND_ 
                                             rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_nack_dn");
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "NACK_R handling, but "
                                  " nothing needs to be done in DCPM\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));

    ROHC_EXIT_FN("rohc_d_p1_r_nack_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_mt_r_to_u_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_U_ET  , when decompressor mode is R, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_mt_r_to_u_dn ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_mt_r_to_u_dn");
    p_dsc->d_trans_state = ROHC_D_TST_INITIATED;
    p_dsc->temp_mode = ROHC_MODE_U;
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Performing mode transition(R->U) and"
                                  " temp mode in DSC is set to U and transition"
                                  " state is set to INITIATED.\n",                               
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_p1_r_mt_r_to_u_dn");
}

/*****************************************************************************
 * Function Name  : rohc_d_p1_r_mt_r_to_o_dn
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  MT_R_TO_O_ET  , when decompressor mode is R, state is NC and
 *                  transition state is DONE for compressed profiles.
 *****************************************************************************/
rohc_void_t rohc_d_p1_r_mt_r_to_o_dn ROHC_PARAMS((p_entity, p_dsc),
                               rohc_entity_t *p_entity _AND_ 
                               rohc_dsc_rtp_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_p1_r_mt_r_to_o_dn");
    p_dsc->d_trans_state = ROHC_D_TST_INITIATED;
    p_dsc->temp_mode = ROHC_MODE_O;
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Performing mode transition(R->O) and"
                                  " temp mode in DSC is set to O and transition"
                                  " state is set to INITIATED.\n",                               
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_p1_r_mt_r_to_o_dn");
}


#endif
