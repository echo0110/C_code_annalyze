/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dcpm_p6_r_done.c,v 1.0.0.0 2013/06/21 04:29:05 gur30693 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definition for R-mode
 *                     decompressor CPM, when decompressor transition state is
 *                     DONE.
 *
 ******************************************************************************
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

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/*****************************************************************************
 * Function Name  : rohcDP6IrIrCRNoContext
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ROHC_D_IR_IRCR_ET for IR and IR-CR packet types and state is 
 *                  NC for compressed profiles.
 *****************************************************************************/
     /* + SPR 17439 */
rohc_void_t rohcDP6IrIrCRNoContext( 
         rohc_entity_t *p_entity , 
         rohc_dsc_tcp_t *p_dsc)
{
    ROHC_ENTER_FN("rohcDP6IrIrCRNoContext");
    if(p_dsc->decom_res)
    {
        p_dsc->d_state = ROHC_D_ST_FC; 
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Upwards state transition NC->FC"
                                  " and sending an ACK \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  /* SPR 9591 fix start */
                                  ROHC_GET_DSC_CID(p_dsc));
                                  /* SPR 9591 fix end */
        rohcDcpmP6SendAck(p_entity,p_dsc);     
    }
    else
    {
        /* SPR 9591 fix start */
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Decompression is unsuccessful "
                                  "therefore sending STATIC-NACK\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
        /* SPR 9591 fix end */
        rohcDcpmP6SendStaticNack(p_entity,p_dsc);
    }  
     /* + SPR 17439 */
    ROHC_EXIT_FN("rohcDP6IrIrCRNoContext");
}

/*****************************************************************************
 * Function Name  : rohcDP6IrIrCRStaticContext
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ROHC_D_IR_IRCR_ET for IR and IR-CR packet types, state is SC
 *                  for compressed profiles.
 *****************************************************************************/
     /* + SPR 17439 */
rohc_void_t rohcDP6IrIrCRStaticContext( 
         rohc_entity_t *p_entity , 
         rohc_dsc_tcp_t *p_dsc)
{
    ROHC_ENTER_FN("rohcDP6IrIrCRStaticContext");
    if(p_dsc->decom_res)
    {
        p_dsc->d_state = ROHC_D_ST_FC;
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Upwards state transition SC->FC"
                                  " and sending an ACK \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  /* SPR 9591 fix start */
                                  ROHC_GET_DSC_CID(p_dsc));
                                  /* SPR 9591 fix end */
        rohcDcpmP6SendAck(p_entity, p_dsc);     
    }
    else 
    {
        ((rohc_dsc_tcp_t*)p_dsc)->d_state = ROHC_D_ST_NC;
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
        rohcDcpmP6SendStaticNack(p_entity,p_dsc);
     /* - SPR 17439 */
    }
    ROHC_EXIT_FN("rohcDP6IrIrCRStaticContext");
}

/*****************************************************************************
 * Function Name  : rohcDP6AllFullContext
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ROHC_D_OTHERS_ET, when decompressor state is FC 
 *                  for compressed profiles.
 *****************************************************************************/
     /* + SPR 17439 */
rohc_void_t rohcDP6AllFullContext( 
         rohc_entity_t *p_entity , 
         rohc_dsc_tcp_t *p_dsc)
{
    ROHC_ENTER_FN("rohcDP6AllFullContext");
    if(p_dsc->decom_res)
    {
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Decompression is successful\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
    }
    else 
    {
        p_dsc->d_state = ROHC_D_ST_SC;
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                "Downwards state transition FC->SC\n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc));

        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                "Decompression is unsuccessful "
                "therefore sending a NACK\n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc));
        rohcDcpmP6SendNack(p_entity, p_dsc);
     /* - SPR 17439 */
    }

    ROHC_EXIT_FN("rohcDP6AllFullContext");
}

/*****************************************************************************
 * Function Name  : rohcDP6IrdynCOStaticContext
 * Inputs         : p_entity - Pointer to rohc entity(global context block)
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function provides the handling for a received event
 *                  ROHC_D_IR_DYN_CO_ET, when decompressor state is SC
 *                  for compressed profiles.
 *****************************************************************************/
     /* + SPR 17439 */
rohc_void_t rohcDP6IrdynCOStaticContext ( 
        rohc_entity_t *p_entity , 
        rohc_dsc_tcp_t *p_dsc)
{
    ROHC_ENTER_FN("rohcDP6IrdynCOStaticContext");

    if(p_dsc->decom_res)
    {
        p_dsc->d_state = ROHC_D_ST_FC;
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Decompression is successful "
                                  "therefore sending an ACK\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc));
        rohcDcpmP6SendAck(p_entity, p_dsc);
        
    }
    else 
    {

        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                "IR-DYN or Common Compressed packet decoding fails SC state, "
                "therefore sending a NACK\n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc));    
        /* FOR CRC FAILURE */
        rohcDcpmP6SendNack(p_entity,p_dsc);      
     /* - SPR 17439 */
    }
    ROHC_EXIT_FN("rohcDP6IrdynCOStaticContext");
}

