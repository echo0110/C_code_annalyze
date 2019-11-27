/***************************************************************************
 *  FILE NAME  : s_fsmrx.h
 *
 *  DESCRIPTION: Contains the Functions for the Receive State Machine.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef    __FILE_sctp_rx_fsm_SEEN__
#define    __FILE_sctp_rx_fsm_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    void
    sctp_rx_state_machine(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    event,
        sctp_U32bit    addr_index );

    void
    sctp_rxfsm_Null_MsgData(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_rxfsm_SackP_MsgData(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_rxfsm_SackP_TimerDeack(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_rxfsm_Null_UserData(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_rxfsm_SackP_UserData(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_rx_fsm_SEEN__ */

