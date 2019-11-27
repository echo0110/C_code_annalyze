/******************************************************************************
 *    FILE NAME:
 *       s_dbbuf.h
 *
 *    DESCRIPTION:
 *       This file is part of database module of SCTP stack.
 *       Buffer get/free functions.
 *
 *    DATE        NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *    02June 2000     Sandeep Mahajan  -       .Original Creation
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************************/
#ifndef    __FILE_sctp_db_buffer_SEEN__
#define    __FILE_sctp_db_buffer_SEEN__


#ifdef __cplusplus
extern "C" {
#endif

    sctpbuffer_st *
    sctp_get_receive_buffer(
        sctp_tcb_st        *p_ass,
        sctp_U32bit        size );

    void
    sctp_free_receive_buffer(
        sctp_tcb_st        *p_ass,
        sctpbuffer_st      *p_buf );

    sctpbuffer_st *
    sctp_get_transmit_buffer(
        sctp_tcb_st        *p_ass,
        sctp_U32bit        size );

    void
    sctp_user_free_receive_buffer(
        sctp_tcb_st          *p_ass,
        sctpbuffer_st        *p_buf );

    void
    sctp_free_transmit_buffer(
        sctp_tcb_st        *p_ass,
        sctpbuffer_st      *p_buf );

    void
    sctp_user_free_transmit_buffer(
        sctp_tcb_st          *p_ass,
        sctpbuffer_st        *p_buf );

    sctp_return_t
    sctp_db_add_buf_to_list(
        sctp_buffer_list_st *p_list,
        sctp_U32bit         num_max_buf,
        sctp_U32bit         num_med_buf,
        sctp_U32bit         num_min_buf );

    sctp_return_t
    sctp_db_del_buf_from_list(
        sctp_buffer_list_st *p_list,
        sctp_U32bit         num_max_buf,
        sctp_U32bit         num_med_buf,
        sctp_U32bit         num_min_buf );

    sctp_return_t
    sctp_db_verify_cong_reg_def(
        sctp_cong_reg_def_st cong_reg_def,
        sctp_error_t        *p_ecode );

    void
    sctp_get_cong_reg_def(
        sctp_Boolean_t        is_transmit,
        sctp_Boolean_t        def_flag,
        sctp_U32bit           server_id,
        sctp_cong_reg_def_st  *p_cong_reg_def );

    sctp_return_t
    sctp_db_set_rx_buffers(
        sctp_tcb_st         *p_assoc,
        sctp_U32bit         n_bytes,
        sctp_error_t        *p_ecode );

    sctp_return_t
    sctp_db_set_tx_buffers(
        sctp_tcb_st         *p_assoc,
        sctp_U32bit         n_bytes,
        sctp_error_t        *p_ecode );

    sctp_return_t
    sctp_db_set_tx_frags(
        sctp_tcb_st         *p_assoc,
        sctp_count_frags_st  count_frags,
        sctp_Boolean_t       is_increment,
        sctp_error_t        *p_ecode );

    sctp_return_t
    sctp_db_set_rx_frags(
        sctp_tcb_st         *p_assoc,
        sctp_count_frags_st  count_frags,
        sctp_Boolean_t       is_increment,
        sctp_error_t        *p_ecode );

    sctp_return_t
    sctp_db_get_txrx_frags(
        sctp_tcb_st         *p_assoc,
        sctp_count_frags_st  *tx_num_frags,
        sctp_count_frags_st  *rx_num_frags );

    sctp_return_t
    sctp_db_get_res_txrx_frags(
        sctp_count_frags_st  *tx_num_frags,
        sctp_count_frags_st  *rx_num_frags );

    void
    sctp_db_init_mtu_buffers(
        sctp_U32bit max_count );

    void
    sctp_db_free_mtu_buffer(
        sctpmtubuf_st  *p_buf );

    sctpmtubuf_st *
    sctp_db_get_mtu_buffer( void );


#ifdef __cplusplus
}
#endif


#endif /* __FILE_sctp_db_buffer_SEEN__ */


