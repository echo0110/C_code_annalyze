/******************************************************************************
**  FILE NAME     :  oamh_fsm.h
**
**  DESCRIPTION   :  This file contains the function prototypes for OAMH Module
**
**
**  DATE        AUTHOR      REF             REASON
**  --------    -------     ---             ----------
**  12-May-09   Shashank    DDv0.2          Initial
**  21-May-09   Shashank    DDv0.2          Review comments incorporated.
**  27-May-09   Shashank    DDv0.2      Formatl review comments incorporated
**  14-Oct-09   OleksandrM  DDv1.41     Global redesign
**
**     Copyright (C) 2009 Aricent Inc . All Rights Reservfsm
******************************************************************************/
#ifndef _OAMH_FSM_H_
#define _OAMH_FSM_H_

/****************************************************************************
 * Header File Includes
 ****************************************************************************/
#include "oamh_ed.h"
#include "oamh_logger.h"

/****************************************************************************
 * Extern definitions
 ****************************************************************************/
extern const S8 * OAMH_FSM_STATES_NAMES[];
extern const S8 * OAMH_FSM_EVENTS_NAMES[];

/****************************************************************************
 * Macro definitions
 ****************************************************************************/
#define RRC_OAMH_FSM_NAME "OAMH_FSM"

#define OAMH_FSM_INIT(p_ctx) \
    do{\
        (p_ctx)->current_state = OAMH_INIT; \
        RRC_OAMH_FSM_INIT_TRACE(RRC_OAMH_FSM_NAME,\
                            OAMH_FSM_STATES_NAMES[OAMH_INIT]);\
    } \
    while (0)

#define OAMH_GET_FSM_STATE(gb_ctx) ((gb_ctx)->current_state)

#define OAMH_SET_FSM_STATE(gb_ctx,new_state) do{\
    (gb_ctx)->current_state=(new_state);\
    RRC_OAMH_FSM_SET_STATE_TRACE(RRC_OAMH_FSM_NAME, \
            OAMH_FSM_STATES_NAMES[new_state]);      \
    }while(0)

/****************************************************************************
 * Enumeration Definitions
 ****************************************************************************/
typedef enum
{
    OAMH_INIT,                      /* Initial state */
    OAMH_STATE_W_FOR_COMM_PROV_REQ, /* Waiting for comm/provision request */
    OAMH_STATE_W_FOR_CLEANUP_RESP,  /* Waiting for cleanup responses */
    OAMH_STATE_ACTIVE,              /* Active state */
    OAMH_NUM_OF_STATES              /* Number of states */
}oamh_state_et;

/****************************************************************************
 * Types Definitions
 ****************************************************************************/
typedef void (*oamh_fsm_ret)(rrc_oamh_gb_context_t *, void *);

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/
void oamh_fsm_process_event
(
    rrc_oamh_gb_context_t *,
    oamh_event_et,
    U16,
    void *
);

void oamh_reset_context(rrc_oamh_gb_context_t *);

rrc_return_t rrc_oamh_send_internal_msg
(
    rrc_module_id_t,
    U16,
    U16,
    void *
);

#endif /* _OAMH_FSM_H_ */

