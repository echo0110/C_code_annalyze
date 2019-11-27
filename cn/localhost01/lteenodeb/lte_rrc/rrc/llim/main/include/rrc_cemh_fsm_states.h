/******************************************************************************
*
*   FILE NAME:
*       rrc_cemh_fsm_states.h
*
*   DESCRIPTION:
*       This header file contains states of CEMH FSM
*
*   DATE            AUTHOR      REFERENCE       REASON
*   15 Apr 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_CEMH_FSM_STATES_H_
#define _RRC_CEMH_FSM_STATES_H_


/* keep in sync with cemh_fsm_tbl! */
typedef enum
{
    CEMH_STATE_IDLE,
    CEMH_STATE_W_FOR_PHY_CONFIG_CELL_CNF,
    CEMH_STATE_W_FOR_MAC_CONFIG_CELL_CNF,
    CEMH_STATE_W_FOR_MAC_SFN_CNF,
    CEMH_STATE_CELL_IN_SERVICE,
    CEMH_STATE_CELL_OUT_OF_SERVICE,
    CEMH_STATE_W_FOR_PHY_RECONFIG_CELL_CNF,
    CEMH_STATE_W_FOR_MAC_RECONFIG_CELL_CNF,
    CEMH_STATE_W_FOR_PHY_CELL_START_CNF,
    CEMH_STATE_W_FOR_MAC_CELL_START_CNF,
    CEMH_STATE_W_FOR_PHY_CELL_STOP_CNF,
    CEMH_STATE_W_FOR_MAC_CELL_STOP_CNF,
    CEMH_STATE_W_FOR_PHY_CELL_DELETE_CNF,
    CEMH_STATE_W_FOR_MAC_CELL_DELETE_CNF,
#ifdef LTE_EMBMS_SUPPORTED
    CEMH_STATE_W_FOR_RLC_CREATE_AREA_ENTITY_CNF,
    CEMH_STATE_W_FOR_RLC_DELETE_AREA_ENTITY_CNF,
#endif
    N_CEMH_STATES,
} cemh_fsm_state_et;

#endif /* _RRC_CEMH_FSM_STATES_H_ */

