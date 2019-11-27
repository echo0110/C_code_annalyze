/******************************************************************************
**  Filename :  uecc_common.h
**
**  DESCRIPTION   :  This file contains the states event and defenitions
**                    common to various fsm.
**
**
**  DATE        AUTHOR      REF             REASON
**  --------    -------     ---             ----------
**  18-May-09   Pradeep S    DDv0.2          Initial
**
**     Copyright (C) 2009 Aricent Inc . All Rights Reservfsm
******************************************************************************/

#ifndef INCLUDED_UECC_COMMON_H
#define INCLUDED_UECC_COMMON_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/


/****************************************************************************
 * Macro definitions
 ****************************************************************************/
#define UECC_RELEASE_VERSION    10
#define UECC_LAST_ID            0xFFFF


/****************************
 *  LTE related definitions
 ****************************/

/* 3GPP TS 36331-850: 6.2.2 RRCConnectionReject data -
    waitTime field default value */
#define UECC_LTE_RRC_CONNECTION_REJECT_WAIT_TIME_FIELD_DEFAULT_VALUE   2 /*sec*/

/* 3GPP TS 36331-850: 6.3.6-RRC-TransactionIdentifier should be in range 0..3 */
#define UECC_LTE_RRC_TRANSACTION_ID_MASK        0x3

/* Measurment Re-config START */
#define UECC_LTE_RRC_RESET_TRANSACTION_ID_MASK        0x0
/* Measurment Re-config END */

/* Default LC id for SRB1 */
#define UECC_LTE_RRC_SRB1_LC_ID                 0x1

/* Default LC id for SRB2 */
#define UECC_LTE_RRC_SRB2_LC_ID                 0x2


/* Max number of RAT for which ue capability is needed */
#define UECC_MAX_NUM_OF_UE_RAT_ENQUIRY                   512


/****************************
 *  FSMs related definitions
 ****************************/

#define UECC_TIMER_OAMH_INIT_IND_DEFAULT_DURATION       1000 /* in ms */
#define UECC_TIMER_RCS_DEFAULT_DURATION                 3000 /* in ms */
#define UECC_TIMER_RM_DEFAULT_DURATION                  3000 /* in ms */
#define UECC_TIMER_RRM_UE_CAPABILITY_ENQ_DURATION       3000 /* in ms */
#define UECC_TIMER_RCR_DEFAULT_DURATION                 3000 /* in ms */
#define UECC_TIMER_MME_GUARD_DEFAULT_DURATION           50 /* in ms */

#define UECC_TIMER_CONTXT_MOD_DEFAULT_DURATION          3000 /* in ms */
#define UECC_TIMER_ERAB_REL_CMD_DEFAULT_DURATION        15000 /* in ms */
#define UECC_TIMER_ERAB_REL_IND_DEFAULT_DURATION        5000 /* in ms */    
/*Start E-RAB MODIFY Timer: Value to be Updated */
#define UECC_TIMER_ERAB_MODIFY_DEFAULT_DURATION         5000 /* in ms */
/*End E-RAB MODIFY Timer: Value to be Updated */
#define UECC_TIMER_RRM_UE_RECONFIG_DEFAULT_DURATION     5000 /* in ms */
#define UECC_TIMER_ERAB_SETUP_DEFAULT_DURATION          5000 /* in ms */

#define UECC_TIMER_RRM_COUNTER_CHECK_DEFAULT_DURATION   5000 /* in ms */

#define UECC_TIMER_RLF_W_FOR_REEST_TIMER_DEFAULT_DURATION   3000
#define UECC_TIMER_RLF_UE_SUSPEND_TIMER_DEFAULT_DURATION    1000
#define UECC_TIMER_STORE_UE_CONTEXT_DEFAULT_DURATION    5000 /* in ms */
#define UECC_TIMER_HEALTH_MONITOR_DEFAULT_DURATION    5000 /* in ms */

/*REL 1.2:HANDOVER PROCEDURE START*/
#define UECC_TIMER_HO_PREP_DEFAULT_DURATION             1000 /* in ms */
#define UECC_TIMER_HO_OVERALL_DEFAULT_DURATION          3000 /* in ms */
#define UECC_TIMER_LL_GUARD_DEFAULT_DURATION            1000 /* in ms */
#define UECC_TIMER_OFFSET_DEFAULT_DURATION              0000 /* in ms */
#define UECC_TIMER_MC_GUARD_DURATION                    1000 /* in ms */
#define UECC_TIMER_TRG_HO_GUARD_DURATION                1000 /* in ms */
#define UECC_TIMER_CRE_GUARD_DURATION                   5000 /* in ms */

/* SPR 4118 Start */
#define UECC_TIMER_INTEGRITY_FAILURE_DEFAULT_DURATION   5000 /* in ms */
/* SPR 4118 Stop */

/* Bug 817 Fix Start */
#define UECC_TIMER_INTRA_CELL_HO_GUARD_DURATION         1000 /* in ms */
/* Bug 817 Fix Stop */
#define UECC_TIMER_UE_INFO_DURATION                     3000 /* in ms */
/*REL 1.2:HANDOVER PROCEDURE END*/

#define UECC_TIMER_CCO_GUARD_DEFAULT_DURATION           15000
/* Timeout value for waiting LLIM delete UE response */
#define UECC_RCS_DELETE_UE_TIMEOUT_DEFAULT_VALUE        1000 /*ms*/
#define UECC_TIMER_RLF_W_REEST_TIMER_DEFAULT_DURATION   300000
#define UECC_TIMER_RLF_P_TIMER_DEFAULT_DURATION         1000
#define LLIM_SUSPEND_UE_ENTITY_TIMER_DEFAULT_DURATION   1000

/*NR_DC Code Change Start*/
#define UECC_TIMER_DC_PREP_DEFAULT_DURATION                 5000 /* in ms */
#define UECC_TIMER_DC_BEARER_CHANGE_GUARD_DEFAULT_DURATION  5000 /* in ms */
#define UECC_TIMER_ENDC_SGNB_REL_TIMER_DEFAULT_DURATION     50000 /* in ms */
#define UECC_TIMER_ERAB_RELEASE_ENDC_GUARD_TIMER_DEFAULT_DURATION     5000 /* in ms */
/*NR_DC Code Change Stop*/

/* UF: Add event for main fsm */
#define UECC_RCS_FAILURE_RELEASE_REQ            0xFFFF
#define UECC_UE_CONTEXT_LIST_SIZE_DEFAULT       5

/* Delivery status value for SRB PDUs */
#define UECC_LLIM_SRB_SERVICEREQUESTED_DELIVERY_STATUS_TRUE  1
#define UECC_LLIM_SRB_SERVICEREQUESTED_DELIVERY_STATUS_FALSE 0

/* Security capabilities - Integrity protection algorithm bits*/
#define UECC_INTEGRITY_PROTECTION_EIA0_BIT 0x0001
#define UECC_INTEGRITY_PROTECTION_EIA1_BIT 0x0002
#define UECC_INTEGRITY_PROTECTION_EIA2_BIT 0x0004

/* Security capabilities - Ciphering protection algorithm bits*/
#define UECC_CIPHERING_EEA0_BIT 0x0001
#define UECC_CIPHERING_EEA1_BIT 0x0002
#define UECC_CIPHERING_EEA2_BIT 0x0004

#define UECC_NAS_TRANSACTION_ID 0x8000

#endif /* INCLUDED_UECC_COMMON_H */
