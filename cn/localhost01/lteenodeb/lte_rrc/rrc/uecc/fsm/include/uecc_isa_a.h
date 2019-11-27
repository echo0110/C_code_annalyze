/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_isa_a.h,v 1.4 2010/10/13 07:03:19 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : UECC Initial security activation activity definitions
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_isa_a.h,v $
 * Revision 1.4  2010/10/13 07:03:19  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.2.10.1  2010/07/21 08:32:16  gur15697
 * HO ERAB code Merging changes
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
 * Revision 1.1.2.1  2009/11/16 18:32:52  ukr16032
 * UECC ISA activity implementation started
 *
 *
 ****************************************************************************/

#ifndef UECC_ISA_A_H
#define UECC_ISA_A_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/

#include "uecc_fsm_common.h"

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Macro definitions
 ****************************************************************************/

/****************************************************************************
 * Enumeration Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

extern uecc_fsm_activity_t uecc_fsm_initial_security_activation_activity;

/****************************************************************************
 * Public functions definitions
 ****************************************************************************/
/* State UECC_UE_RADIO_LINK_FAILURE_HANDLING_ONGOING */
void uecc_fsm_ue_radio_link_failure_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);
#endif /* UECC_ISA_A_H */
