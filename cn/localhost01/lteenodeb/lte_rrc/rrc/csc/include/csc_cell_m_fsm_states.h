/******************************************************************************
*
*   FILE NAME:
*       csc_cell_m_fsm_states.h
*
*   DESCRIPTION:
*       This header file contains states of CSC Cell Manager FSM
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_CELL_M_FSM_STATES_H_
#define _CSC_CELL_M_FSM_STATES_H_

/* keep in sync with cell_m_fsm_tbl! */
typedef enum
{
    CELL_M_STATE_IDLE,
    CELL_M_STATE_W_FOR_LLIM_CONFIG_CELL_RESP,
    CELL_M_STATE_CELL_IN_SERVICE,
    CELL_M_STATE_CELL_OUT_OF_SERVICE,
    CELL_M_STATE_W_FOR_LLIM_DELETE_CELL_RESP,
    CELL_M_STATE_W_FOR_UECC_DELETE_ALL_UE_RESP,
    CELL_M_STATE_W_FOR_LLIM_RECONFIG_CELL_RESP,
    CELL_M_STATE_W_FOR_CELL_DELETE_REQ, 
    CELL_M_STATE_W_FOR_PWS_RESP,
    CELL_M_STATE_W_FOR_KILL_RESP,
    CELL_M_STATE_W_FOR_PWS_FAILURE_RESP,
    CELL_M_STATE_W_FOR_LLIM_CELL_START_RESP,
    CELL_M_STATE_W_FOR_LLIM_CELL_STOP_RESP,
    N_CELL_M_STATES
} cell_m_fsm_state_et;

#endif /* _CSC_CELL_M_FSM_STATES_H_ */

