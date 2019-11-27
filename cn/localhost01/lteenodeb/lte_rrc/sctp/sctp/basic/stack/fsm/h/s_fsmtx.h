/***************************************************************************
 *  FILE NAME  : s_fsmtx.h
 *
 *  DESCRIPTION: Contains the Functions for the Transmit State Machine.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef    __FILE_sctp_tx_fsm_SEEN__
#define    __FILE_sctp_tx_fsm_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    void
    sctp_tx_state_machine(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    event,
        sctp_U32bit    addr_index );

    void
    sctp_txfsm_Gener_Sack(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_txfsm_Null_Userdata(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_txfsm_Awack_Userdata(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_txfsm_Null_TimerData(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_txfsm_Awack_TimerData(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_generic_handle_abort_message(
        sctp_tcb_st    *p_ass );

    void
    sctp_update_rto(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index,
        sctp_U32bit    rtt );

    sctp_U32bit
    sctp_get_next_active_addr_index(
        sctp_tcb_st    *p_ass,
        sctp_U32bit   last_addr_index );

    /* Changes for CSR#1-4777901 */
    sctp_U32bit
    sctp_get_possible_next_active_addr_index(
        sctp_tcb_st    *p_ass,
        sctp_U32bit   last_addr_index );
    /* Changes for CSR#1-4777901 Ends */

    sctp_return_t
    sctp_get_next_saddr_index_with_af(
        sctp_tcb_st           *p_tcb,
        sctp_U32bit           *p_src_pri,
        sctp_sa_family_t      af,
        sctp_error_t          *p_ecode );

    sctp_return_t
    sctp_is_tsn_present(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    tsn );

    sctp_return_t
    sctp_get_lowest_tsn_on_addr(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index,
        sctp_U32bit    *lowest_tsn );

    sctp_return_t
    sctp_process_param_cumtsn(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    *num_cumacks,
        sctp_U32bit    *total_bytes_acked_list,
        sctp_error_t   *p_ecode );

    void
    sctp_update_rwnd_retransflag_ontimeout(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_get_outstanding_list(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    *p_outstanding_list );

    void
    sctp_move_data_to_primary_addr(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    pr_addr_index );

    /* Changes For CSR#1-5037386 */
    void
    sctp_move_data_to_primary_addr_from_prev(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    prev_index,
        sctp_U32bit    pr_addr_index );
    /* Changes For CSR#1-5037386 Ends */

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_tx_fsm_SEEN__ */


