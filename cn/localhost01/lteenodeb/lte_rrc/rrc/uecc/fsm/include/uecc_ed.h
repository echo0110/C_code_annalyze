/******************************************************************************
**  Filename :  uecc_ed.h
**
**  DESCRIPTION   :  This file contains the function prototypes for UECC UE FSM
**
**
**  DATE        AUTHOR      REF             REASON
**  --------    -------     ---             ----------
**  18-May-09   Pradeep S    DDv0.2          Initial
**
**     Copyright (C) 2009 Aricent Inc . All Rights Reservfsm
******************************************************************************/

#ifndef INCLUDED_UECC_UE_ED_H
#define INCLUDED_UECC_UE_ED_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/
#include "rrc_defines.h"
#include "uecc_global_ctx.h"

/****************************************************************************
 * Macro definitions
 ****************************************************************************/

/****************************************************************************
 * Enumeration Definitions
 ****************************************************************************/

/****************************************************************************
 * Type Variables
 ****************************************************************************/

/****************************************************************************
 * Global functions definition
 ****************************************************************************/

void rrc_uecc_clean_up
(
    uecc_gb_context_t *p_uecc_gb_context  /* UECC global context */
);

void rrc_uecc_process_health_monitor_chk
(
    uecc_gb_context_t   *p_uecc_gb_context
);

#endif /* INCLUDED_UECC_UE_ED_H */

