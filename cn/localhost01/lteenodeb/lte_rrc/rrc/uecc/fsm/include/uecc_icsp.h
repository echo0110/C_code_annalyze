/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_icsp.h,v 1.6 2010/10/13 07:03:18 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : UECC Initial UE context procedure definitions
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_icsp.h,v $
 * Revision 1.6  2010/10/13 07:03:18  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.4  2010/08/05 08:05:54  gur15697
 * merging round 2
 *
 * Revision 1.3.4.3  2010/07/21 08:32:16  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.3.4.2  2010/07/15 05:49:58  gur16768
 * code review comments incorporate
 *
 * Revision 1.3.4.1  2010/06/23 10:37:49  gur18569
 * Added Target Handover related changes
 *
 * Revision 1.3  2010/03/12 05:49:20  gur22133
 * ut_ue_capability_changes
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.4.2  2009/12/08 11:22:51  ukr16032
 * UECC - inspection log items LG_20, LG_23, LG_24, LG_26, LG_27, LG_28, LG_29, LG_34, LG_40, LG_41 fixed
 *
 * Revision 1.1.4.1  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.2  2009/11/15 10:02:59  ukr15916
 * beautification
 *
 * Revision 1.1.2.1  2009/11/12 14:36:33  ukr16032
 * UECC - Initial UE context setup procedure moved to uecc_icsp.*
 *
 *
 ****************************************************************************/

#ifndef UECC_ICSP_H
#define UECC_ICSP_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/

#include "uecc_ue_ctx.h"
#include "uecc_fsm_common.h"
#include "rrc_rrm_il_composer.h"


/****************************************************************************
 * Private Variables
 ****************************************************************************/
/****************************************************************************
 * External data types
 ****************************************************************************/
struct _uecc_ue_context_t;
/****************************************************************************
 * Public functions definitions
 ****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: uecc_fsm_icsp_count_erab_list
*
*   DESCRIPTION:
*       This function counts the no. of erabs in the erab list
*
*   RETURNS:
*       Number of erabs
*
******************************************************************************/
rrc_return_t  uecc_fsm_icsp_count_erab_list(
    struct _uecc_ue_context_t* p_ue_context);

/****************************************************************************
 * Exported Macro definitions
 ****************************************************************************/

/****************************************************************************
 * Enumeration Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/


extern uecc_fsm_procedure_t uecc_fsm_ue_initial_context_setup_procedure;
/****************************************************************************
 * Public functions definitions
 ****************************************************************************/
void uecc_fsm_ue_initial_context_setup_procedure_finish_fail(
       struct _uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

/* This function is used by ICS and HO procedures */
rrc_return_t uecc_fsm_icsp_check_and_process_security_capability(
        uecc_ue_context_t*  p_ue_context,
        void  *p_ue_security_capabilities
);

#endif /* UECC_ICSP_H */

