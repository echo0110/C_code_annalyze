/*******************************************************************************  
*  FILE :  uecc_erab_rrm_ue_reconfig_fsm.h
*
*  DESCRIPTION   :  This file contains the function prototypes for UECC RRM 
*                   INITIATED UE RECONFIGURATION FSM
*
*
*  DATE        AUTHOR      REF             REASON
*  --------    -------     ---             ----------
*  01-Oct-12   Aricent                     Initial
*
*     Copyright (C) 2012 Aricent Inc . All Rights Reserved
******************************************************************************/

#ifndef INCLUDED_UECC_RRM_UE_RECONFIG_FSM_H
#define INCLUDED_UECC_RRM_UE_RECONFIG_FSM_H

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
 /* CA start */
#define CLEAR_FLAG 4
 /* CA end */
/****************************************************************************
 * Enumeration Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/
extern uecc_fsm_procedure_t uecc_fsm_ue_rrm_ue_reconfig_procedure;

#endif /* INCLUDED_UECC_RRM_UE_RECONFIG_FSM_H */



