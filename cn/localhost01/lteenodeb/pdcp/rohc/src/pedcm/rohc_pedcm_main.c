/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_pedcm_main.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the entry points of the CPM module.
 *                     It is for the compressor and contains the event handlers.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_pedcm_main.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 03:39:33  gur19836
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
#include "rohc_pedcm_proto.h"
     /* + SPR 17439 */
#include "rohc_pedcm_ext_if.h"
     /* - SPR 17439 */
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
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*****************************************************************************
**      FUNCTION:
**              rohc_pedcm_hndlr
******************************************************************************
**
**      DESCRIPTION:
**              This function invokes the SN repairing and correction 
**              algorithms in case of CRC failure.  
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**      RETURN VALUE:
**              rohc_return_t (ROHC_SUCCESS/ROHC_FAILURE)
**      NOTES:
**               
*************************************************************************** */
rohc_return_t rohc_pedcm_hndlr ROHC_PARAMS( (p_entity, p_dsc),
        rohc_entity_t *p_entity _AND_ rohc_dsc_t *p_dsc)
{
    ROHC_ENTER_FN("rohc_pedcm_hndlr");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,
            "");
	return ROHC_FAILURE;
}
