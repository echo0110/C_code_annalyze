/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dcpm_u_pending.c,v 1.1.1.1.16.1 2010/11/03 06:51:03 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definition for U-mode
 *                     decompressor CPM, when decompressor transition state is
 *                     PENDING.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dcpm_u_pending.c,v $
 * Revision 1.1.1.1.16.1  2010/11/03 06:51:03  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.3.28.2  2010/07/07 06:49:26  gur21145
 * ALIGNMENT CORRECTED
 *
 * Revision 1.3.28.1  2010/07/07 06:42:59  gur21145
 * SPR_FIX_547
 *
 * Revision 1.3  2009/05/27 13:17:19  gur19836
 * File Header updated
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





/***************************************************************************
 *  Function Name    : rohc_d_u_ir_nc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                IR_ET  , when decompressor mode is U, state is NC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_ir_nc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_ir_nc_pnd");
    rohc_d_u_ir_nc_dn(p_entity, p_dsc);
    rohc_d_u_ack_u_nc_pnd(p_entity, p_dsc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_ir_nc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_ir_sc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                IR_ET  , when decompressor mode is U, state is SC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/


/* START_SPR_FIX_547*/
rohc_void_t rohc_d_u_ir_sc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_ir_sc_pnd");
    /* The function name was incorrect, so commented */
    //rohc_d_u_ir_sc_dn(p_entity, p_dsc);
    rohc_d_u_ir_irdyn_uor2_sc_dn(p_entity, p_dsc);
    rohc_d_u_ack_u_nc_pnd(p_entity, p_dsc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_ir_sc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_ir_fc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                IR_ET  , when decompressor mode is U, state is FC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_ir_fc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_ir_fc_pnd");
    /* The function name was incorrect, so commented */
    //rohc_d_u_ir_fc_dn(p_entity, p_dsc);
    rohc_d_u_all_fc_dn(p_entity, p_dsc);
    rohc_d_u_ack_u_nc_pnd(p_entity, p_dsc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_ir_fc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_irdyn_nc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                IRDYN_ET  , when decompressor mode is U, state is NC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_irdyn_nc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_irdyn_nc_pnd");
    /* The function name was incorrect, so commented */
    //rohc_d_u_irdyn_nc_dn(p_entity, p_dsc);
    rohc_d_u_ack_u_nc_pnd(p_entity, p_dsc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_irdyn_nc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_irdyn_sc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                IRDYN_ET  , when decompressor mode is U, state is SC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_irdyn_sc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_irdyn_sc_pnd");
    /* The function name was incorrect, so commented */
    //rohc_d_u_irdyn_sc_dn(p_entity, p_dsc);
    rohc_d_u_ir_irdyn_uor2_sc_dn(p_entity, p_dsc);
    rohc_d_u_ack_u_nc_pnd(p_entity, p_dsc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_irdyn_sc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_irdyn_fc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                IRDYN_ET  , when decompressor mode is U, state is FC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_irdyn_fc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_irdyn_fc_pnd");
    /* The function name was incorrect, so commented */
    //rohc_d_u_irdyn_fc_dn(p_entity, p_dsc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    rohc_d_u_all_fc_dn(p_entity, p_dsc);
    rohc_d_u_ack_u_nc_pnd(p_entity, p_dsc);
    ROHC_EXIT_FN("rohc_d_u_irdyn_fc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_uo_0_nc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                UO_0_ET  , when decompressor mode is U, state is NC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_uo_0_nc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_uo_0_nc_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
            "Invalid operation",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_u_uo_0_nc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_uo_0_sc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                UO_0_ET  , when decompressor mode is U, state is SC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_uo_0_sc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_uo_0_sc_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
            "Invalid operation",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_u_uo_0_sc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_uo_0_fc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                UO_0_ET  , when decompressor mode is U, state is FC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_uo_0_fc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_uo_0_fc_pnd");
    p_dsc->d_mode = ROHC_MODE_U;
    p_dsc->d_trans_state =  ROHC_D_TST_DONE;

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_uo_0_fc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_uo_1_nc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                UO_1_ET  , when decompressor mode is U, state is NC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_uo_1_nc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_uo_1_nc_pnd");

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
            "Invalid operation",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_u_uo_1_nc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_uo_1_sc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                UO_1_ET  , when decompressor mode is U, state is SC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_uo_1_sc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_uo_1_sc_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */

    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
            "Invalid operation",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_u_uo_1_sc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_uo_1_fc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                UO_1_ET  , when decompressor mode is U, state is FC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_uo_1_fc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_uo_1_fc_pnd");
    p_dsc->d_mode = ROHC_MODE_U;
    p_dsc->d_trans_state  = ROHC_D_TST_DONE;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_uo_1_fc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_uo_2_nc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                UO_2_ET  , when decompressor mode is U, state is NC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_uo_2_nc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_uo_2_nc_pnd");
    /* The function name was incorrect, so commented */
    //rohc_d_u_uo_2_nc_dn(p_entity, p_dsc);
    rohc_d_u_ack_u_nc_pnd(p_entity, p_dsc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_uo_2_nc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_uo_2_sc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                UO_2_ET  , when decompressor mode is U, state is SC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_uo_2_sc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_uo_2_sc_pnd");
    /* The function name was incorrect, so commented */
    //rohc_d_u_uo_2_sc_dn(p_entity, p_dsc);
    rohc_d_u_ir_irdyn_uor2_sc_dn(p_entity, p_dsc);
    rohc_d_u_ack_u_nc_pnd(p_entity, p_dsc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_uo_2_sc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_uo_2_fc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                UO_2_ET  , when decompressor mode is U, state is FC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_uo_2_fc_pnd ROHC_PARAMS((p_entity, p_dsc), rohc_entity_t
        *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_uo_2_fc_pnd");
    /* The function name was incorrect, so commented */
    //rohc_d_u_uo_2_fc_dn(p_entity, p_dsc);
    rohc_d_u_all_fc_dn(p_entity, p_dsc);
    rohc_d_u_ack_u_nc_pnd(p_entity, p_dsc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_uo_2_fc_pnd");
}

/*END_SPR_FIX_547*/

/***************************************************************************
 *  Function Name    : rohc_d_u_ack_u_nc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                ACK_U_ET  , when decompressor mode is U, state is NC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_ack_u_nc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_ack_u_nc_pnd");


    /* call sent Ack(U) with crc option */
    rohc_dcpm_send_ack(p_entity, p_dsc, ROHC_TRUE);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_ack_u_nc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_ack_u_sc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                ACK_U_ET  , when decompressor mode is U, state is SC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_ack_u_sc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_ack_u_sc_pnd");
    /* call sent Ack(U) with crc option */
    rohc_d_u_ack_u_nc_pnd(p_entity, p_dsc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_ack_u_sc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_ack_u_fc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                ACK_U_ET  , when decompressor mode is U, state is FC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_ack_u_fc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_ack_u_fc_pnd");
    /* call sent Ack(U) with crc option */
    rohc_d_u_ack_u_nc_pnd(p_entity, p_dsc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_d_u_ack_u_fc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_mt_u_to_o_nc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                MT_U_TO_O_ET  , when decompressor mode is U, state is NC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_mt_u_to_o_nc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_mt_u_to_o_nc_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
            "Invalid mode transition, as another "
            "mode transition is going on",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_u_mt_u_to_o_nc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_mt_u_to_o_sc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                MT_U_TO_O_ET  , when decompressor mode is U, state is SC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_mt_u_to_o_sc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_mt_u_to_o_sc_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
            "Invalid mode transition, as another "
            "mode transition is going on",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc));

    ROHC_EXIT_FN("rohc_d_u_mt_u_to_o_sc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_mt_u_to_o_fc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                MT_U_TO_O_ET  , when decompressor mode is U, state is FC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_mt_u_to_o_fc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_mt_u_to_o_fc_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
            "Invalid mode transition, as another "
            "mode transition is going on",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc));

    ROHC_EXIT_FN("rohc_d_u_mt_u_to_o_fc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_mt_u_to_r_nc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                MT_U_TO_R_ET  , when decompressor mode is U, state is NC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_mt_u_to_r_nc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_mt_u_to_r_nc_pnd");

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
            "Invalid mode transition, as another "
            "mode transition is going on",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_u_mt_u_to_r_nc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_mt_u_to_r_sc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                MT_U_TO_R_ET  , when decompressor mode is U, state is SC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_mt_u_to_r_sc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_mt_u_to_r_sc_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
            "Invalid mode transition, as another "
            "mode transition is going on",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc));

    ROHC_EXIT_FN("rohc_d_u_mt_u_to_r_sc_pnd");
}



/***************************************************************************
 *  Function Name    : rohc_d_u_mt_u_to_r_fc_pnd
 *
 *  Description      :
 *                This function provides the handling for a received event 
 *                MT_U_TO_R_ET  , when decompressor mode is U, state is FC and 
 *                transition state is PENDING for compressed profiles.
 *
 *  Parameters       :
 *         p_entity : Pointer to rohc entity(global context block)
 *         p_dsc    : Pointer to rohc decompressor stream context block
 *
 *  Return Value     :rohc_void_t
 *
 * CAUTION/NOTE:
 ****************************************************************************/



rohc_void_t rohc_d_u_mt_u_to_r_fc_pnd ROHC_PARAMS((p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_d_u_mt_u_to_r_fc_pnd");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
            "Invalid mode transition, as another "
            "mode transition is going on",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc));
    ROHC_EXIT_FN("rohc_d_u_mt_u_to_r_fc_pnd");
}

