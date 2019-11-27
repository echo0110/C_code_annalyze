/***************************************************************************
 *  FILE NAME  : s_fsmass.h
 *
 *  DESCRIPTION: Contains the Functions for the association state machine.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef __FILE_sctp_cong_SEEN__
#define __FILE_sctp_cong_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    void
    sctp_slowstart_at_setup(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_decay_condestion_window(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_slowstart_at_retrx_timeout(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    void
    sctp_slowstart_at_sack_chunk(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    *total_bytes_acked_list );

    void
    sctp_cong_avoid_at_sack_chunk(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    num_chunks_cumtsn,
        sctp_U32bit    *total_bytes_acked_list,
        sctp_U32bit    *outstanding_list );

    void
    sctp_fast_retrns_on_sack(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_cong_SEEN__ */

