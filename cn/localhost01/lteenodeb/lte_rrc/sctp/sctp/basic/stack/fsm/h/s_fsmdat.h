/***************************************************************************
 *  FILE NAME  : s_fsmdat.h
 *
 *  DESCRIPTION: This file is part of Finite State Machine of SCTP. This will
 *               have functions related to data chunks processing .
 *
 *  DATE       NAME               REF#         REASON
 *  ------------------------------------------------------------
 *  02Jun00    Sandeep Mahajan     -           .Original Creation
 *  16Aug01    gsheoran            -           REL 3.0 Update
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef    __FILE_sctp_fsm_data_SEEN__
#define    __FILE_sctp_fsm_data_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    sctp_return_t
    sctp_read_message_list(
        sctp_tcb_st    *p_ass,
        sctp_U16bit    stream,
        sctp_U32bit    buffer_len,
        sctp_U8bit     *buffer,
        sctp_U32bit    *msg_len,
        sctp_U32bit    *ppi,
        sctp_Boolean_t peek_flag,
        sctp_error_t   *p_ecode );

    sctp_return_t
    sctp_read_message_list_ext(
        sctp_tcb_st        *p_ass,
        sctp_U16bit        stream,
        sctp_U32bit        buffer_len,
        sctp_U8bit         *buffer,
        sctp_U32bit        *msg_len,
        sctp_U32bit        *p_ppi,
        sctp_U32bit        peek_flag,
        sctp_sockaddr_st   *p_daddr,
        sctp_U32bit        *p_ssn,
        sctp_U32bit        *p_tsn,   /* tsn for socket draft*/
        sctp_U32bit        *p_context,
        sctp_Boolean_t     *p_unordered,
        sctp_Boolean_t     *p_partial_data_read,
        sctp_error_t       *p_ecode );


    void
    sctp_process_data_message(
        sctp_tcb_st    *p_ass,
        sctp_Boolean_t overrule_delayack );


    sctp_return_t
    sctp_senddata_on_alternate_addr(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    prev_addr_index,
        sctp_U32bit    new_addr_index );

    sctp_return_t
    sctp_calc_rcv_cong_reg(
        sctp_tcb_st         *p_assoc,
        sctp_U32bit         *p_new_cong_reg );

    sctp_return_t
    sctp_senddata_on_addr(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );

    /* Changes For CSR#1-5037386 */
    sctp_return_t
    sctp_senddata_on_addr_if_possible(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );
    /* Changes For CSR#1-5037386 Ends */

    sctp_return_t
    sctp_compare_ssn_for_smaller(
        sctp_U16bit    ssn1,
        sctp_U16bit    ssn2 );

    sctp_return_t
    sctp_compare_ssn_for_larger(
        sctp_U16bit    ssn1,
        sctp_U16bit    ssn2 );

    sctp_return_t
    sctp_compare_tsn_for_smaller(
        sctp_U32bit    tsn1,
        sctp_U32bit    tsn2 );

    sctp_return_t
    sctp_compare_tsn_for_larger(
        sctp_U32bit    tsn1,
        sctp_U32bit    tsn2 );

    sctp_return_t
    sctp_negative_ack_sacklist(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    tsn );

    sctp_return_t
    sctp_is_ssn_present(
        sctp_tcb_st    *p_ass,
        sctp_U16bit    stream,
        sctp_U16bit    ssn );

    sctp_return_t
    sctp_remove_any_unorder_chunk(
        sctp_tcb_st    *p_ass,
        sctp_U16bit    stream,
        sctp_U32bit    tsn,
        sctp_U32bit    length );

    void
    sctp_send_partial_ind_clear_buffers(
        sctp_tcb_st   *p_ass,
        sctp_U16bit   stream );

    sctp_return_t
    sctp_chk_send_par_notf(
        sctp_tcb_st    *p_ass,
        sctp_U16bit    stream,
        sctp_U32bit    tsn );

    sctp_return_t
    sctp_is_notification_required(
        sctp_tcb_st    *p_ass,
        sctp_U16bit    stream,
        sctp_U32bit    tsn,
        sctp_U32bit    *notify );

    sctpbuffer_st *
    sctp_check_data_arrival(
        sctp_tcb_st    *p_ass,
        sctp_U16bit    stream,
        sctp_U32bit    *size );

    sctp_return_t
    sctp_dynamic_chk_tsn(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    tsn );

    sctp_return_t
    sctp_update_sacklist(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    tsn );

    void
    sctp_update_message_list(
        sctp_tcb_st    *p_ass,
        sctp_U16bit    stream,
        sctpbuffer_st   *p_buf );

    sctp_return_t
    sctp_update_tsn_and_ssn(
        sctp_tcb_st   *p_ass,
        sctpbuffer_st  *p_buf );


    sctp_U32bit
    sctp_send_one_mtu(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    addr_index );


    sctp_U8bit *
    sctp_add_data_chunks(
        sctp_tcb_st    *p_ass,
        sctp_U8bit     *p_buffer,
        sctp_U32bit    remain_len,
        sctp_U16bit    *df_bit );

    sctp_return_t
    sctp_read_data_mesg(
        sctp_tcb_st        *p_ass,
        sctp_U16bit        stream,
        sctp_U32bit        buffer_len,
        sctp_U8bit         *buffer,
        sctp_U32bit        *msg_len,
        sctp_U32bit        *p_ppi,
        sctp_U32bit        flags,
        sctp_sockaddr_st   *p_daddr,
        sctp_Boolean_t     *p_unordered,
        sctp_Boolean_t     *p_partial_data_read,
        sctp_error_t       *p_ecode );

    void
    sctp_process_data_message_in_shutS(
        sctp_tcb_st    *p_ass,
        sctp_Boolean_t overrule_delayack );

    /* Changes For CSR#1-5037386 */
    sctp_U32bit
    sctp_send_one_mtu_on_alternate_addr(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    prev_addr_index,
        sctp_U32bit    new_addr_index );

    sctp_U32bit sctp_get_sack_dest_address_and_reset_flags(
        sctp_tcb_st *p_ass,
        sctp_U32bit dest_index );
    /* Changes For CSR#1-5037386 Ends */


#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_fsm_data_SEEN__ */


