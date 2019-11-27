#ifdef ENDC_ENABLED
/*******************************************************************************  
*  FILE :  uecc_rrm_dc_bearer_reconfig_fsm.h
*
*  DESCRIPTION   :  This file contains the function prototypes for UECC RRM 
*                   initiated DC BEARER MODIFY FSM.
*
*
*  DATE        AUTHOR      REF             REASON
*  --------    -------     ---             ----------
*  16-Aug-16   gur20882                    Initial
*
*     Copyright (C) 2009 Aricent Inc . All Rights Reserved
******************************************************************************/
#ifndef INCLUDED_UECC_RRM_DC_BEARER_RECONFIG_FSM_H
#define INCLUDED_UECC_RRM_DC_BEARER_RECONFIG_FSM_H 

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
extern uecc_fsm_procedure_t uecc_fsm_rrm_dc_bearer_change_procedure;

#endif /* INCLUDED_UECC_RRM_DC_BEARER_RECONFIG_FSM_H */
#endif
