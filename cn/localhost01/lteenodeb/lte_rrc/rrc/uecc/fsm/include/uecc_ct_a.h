/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_ct_a.h,v 1.5 2010/10/13 07:03:14 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : UECC Capability transfer activity definitions
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_ct_a.h,v $
 * Revision 1.5  2010/10/13 07:03:14  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.4.4.1  2010/10/04 13:53:58  gur04518
 * Removed TABS
 *
 * Revision 1.4  2010/04/07 15:32:59  gur21006
 * warnings removed
 *
 * Revision 1.3  2010/02/18 04:19:02  gur10248
 * LTE_RRC_Rel1.1_18feb
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
 * Revision 1.1.2.3  2009/11/16 18:32:52  ukr16032
 * UECC ISA activity implementation started
 *
 * Revision 1.1.2.2  2009/11/15 10:02:59  ukr15916
 * beautification
 *
 * Revision 1.1.2.1  2009/11/14 16:29:55  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 * Revision 1.1.2.1  2009/11/12 14:36:33  ukr16032
 * UECC - Initial UE context setup procedure moved to uecc_icsp.*
 *
 *
 ****************************************************************************/

#ifndef UECC_CT_A_H
#define UECC_CT_A_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/

#include "uecc_fsm_common.h"
#include "uecc_ue_ctx.h"

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

extern uecc_fsm_activity_t uecc_fsm_ue_capability_transfer_activity;

extern uecc_fsm_procedure_t uecc_fsm_ue_capability_rrm_enq_procedure;

void uecc_fsm_ue_capability_rrm_enq_procedure_finish_success(
        uecc_ue_context_t      *p_ue_context,
                void*               p_api
);
void uecc_fsm_ue_capability_rrm_enq_procedure_finish_failure(
        uecc_ue_context_t*  p_ue_context,
        void*                   p_api           
);
void uecc_fsm_rrm_ue_capability_enq_stop_timer(
    uecc_ue_context_t   *p_ue_context
);
/*SPR_17893_START*/
void uecc_rrm_ue_capability_enq_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);

void uecc_ue_capability_information_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);

void uecc_ue_capability_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);
/*SPR_17893_END*/

#endif /* UECC_CT_A_H */

