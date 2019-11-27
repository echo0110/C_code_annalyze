/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cprm_rslv_p0.c,v 1.1.1.1.16.1 2010/11/03 07:04:47 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definition for determining
 *                     the packet type for uncompressed profile.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cprm_rslv_p0.c,v $
 * Revision 1.1.1.1.16.1  2010/11/03 07:04:47  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.3.32.1  2010/10/13 03:57:28  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3  2009/05/28 04:26:50  gur19836
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
#include "rohc_cprm_proto.h"

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
const 
rohc_U8bit_t rohc_p0_state_pkt_map[ROHC_CUN_ST_NORMAL - ROHC_CUN_ST_IR +1]
= {ROHC_IRP0_PKTYPE, ROHC_NORMALP0_PKTYPE};

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_resolve_pkt_typ_uncomp
******************************************************************************
**
**      DESCRIPTION:
**              It determines the type of packet for uncompressed profile
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
rohc_U8bit_t rohc_cprm_resolve_pkt_typ_uncomp ROHC_PARAMS(
            (p_entity, p_csc),
            rohc_entity_t *p_entity _AND_
                                           rohc_csc_uncomp_t *p_csc)
{   
    ROHC_ENTER_FN("rohc_cprm_resolve_pkt_typ_uncomp");
  /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    ROHC_EXIT_FN("rohc_cprm_resolve_pkt_typ_uncomp");
    return rohc_p0_state_pkt_map[ROHC_CSC_GET_STATE(p_csc) - ROHC_CUN_ST_IR];
}
