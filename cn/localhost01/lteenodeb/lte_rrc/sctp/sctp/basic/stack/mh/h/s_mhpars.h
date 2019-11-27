/*******************************************************************************
 *    FILE NAME:
 *          s_mhpars.h
 *
 *    DESCRIPTION:
 *          message parsing functions
 *
 *    DATE      NAME            REFERENCE       REASON
 *    -----------------------------------------------------
 *    02Jun00   Sandeep Mahajan  -           .Original Creation
 *    29Aug01   gheoran          -           Rel 3.0
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************************/
#ifndef       __FILE_sctp_mesg_parse_SEEN__
#define       __FILE_sctp_mesg_parse_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    sctp_U16bit
    sctp_get_parameter_pad_bytes(
        sctp_U16bit     parameter_len );

    sctp_return_t
    sctp_static_chk_init_paramlen(
        sctp_U16bit     parameter_id,
        sctp_U16bit     parameter_len );

    sctp_return_t
    sctp_static_chk_initack_paramlen(
        sctp_U16bit    parameter_id,
        sctp_U16bit    parameter_len );

    sctp_return_t
    sctp_chk_init_opt_params(
        sctp_U8bit          *buffer,
        sctp_U16bit         chunk_offset,
        sctp_U16bit         chunk_len,
        sctp_chunk_init_st  *init,
        sctp_Boolean_t      *unrecognise_param_present,
        sctp_U16bit         *unrecognise_param_len,
        sctp_U8bit          *p_unrecognise_param,
        sctp_U8bit          *p_unresolved_addr,
        sctp_U16bit         *non_init_param_len,
        sctp_U8bit          *p_non_init_params,
        sctp_error_t        *p_ecode );

    sctp_return_t
    sctp_chk_initack_opt_params(
        sctp_U8bit             *buffer,
        sctp_U16bit            chunk_offset,
        sctp_U16bit            chunk_len,
        sctp_chunk_initack_st  *initack,
        sctp_Boolean_t         *unrecognise_param_present,
        sctp_U16bit            *unrecognise_param_len,
        sctp_U8bit             *p_unrecognise_param,
        sctp_U8bit             *p_unresolved_addr,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_chk_init_fixed_params(
        sctp_U8bit             *buffer,
        sctp_U32bit            chunk_offset,
        sctp_chunk_init_st     *init_chunk,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_parse_chk_error_code(
        sctp_U8bit             *buffer,
        sctp_U32bit            chunk_offset,
        sctp_U16bit            chunk_len,
        sctp_U32bit            *num_errors,
        sctp_error_cause_st    *error_codes,
        sctp_error_t           *p_ecode );

    sctp_U32bit
    sctp_get_chunk_padd_bytes(
        sctp_U8bit             chunk_id,
        sctp_U16bit            chunk_len );

    sctp_return_t
    sctp_static_chk_chunk_len(
        sctp_U8bit             chunk_id,
        sctp_U16bit            chunk_len,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_static_chk_chunk_id(
        sctp_U8bit             chunk_id,
        sctp_error_t           *p_ecode );

    void
    sctp_init_valid_chunk_list(
        sctp_U8bit             *valid_chunks_list,
        sctp_Boolean_t         init_value );

    sctp_return_t
    sctp_chk_TLdata_chunks_and_len(
        sctp_U32bit            size,
        sctp_U8bit             *buffer,
        sctp_U32bit            *num_chunks,
        sctp_chunk_info_st     *chunk_id_list,
        sctp_Boolean_t         *unrecognise_chunk_present,
        sctp_U16bit            *unrecognise_chunk_len,
        sctp_U8bit             *p_unrecognise_chunk,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_chk_chunk_combination_valid(
        sctp_U32bit            num_of_chunks,
        sctp_chunk_info_st     *chunk_id_list,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_static_chk_initack_paramid(
        sctp_U16bit            parameter_id,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_static_chk_init_paramid(
        sctp_U16bit            parameter_id,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_parse_chk_chunk_init(
        sctp_U8bit             *buffer,
        sctp_U32bit            chunk_offset,
        sctp_chunk_init_st     *init_chunk,
        sctp_Boolean_t         *unrecognise_param_present,
        sctp_U16bit            *unrecognise_param_len,
        sctp_U8bit             *p_unrecognise_param,
        sctp_U8bit             *p_unresolved_addr,
        sctp_U16bit            *non_init_params_len,
        sctp_U8bit             *p_non_init_params,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_parse_chk_chunk_initack(
        sctp_U8bit             *buffer,
        sctp_U32bit            chunk_offset,
        sctp_chunk_initack_st  *initack_chunk,
        sctp_Boolean_t         *unrecognise_param_present,
        sctp_U16bit            *unrecognise_param_len,
        sctp_U8bit             *p_unrecognise_param,
        sctp_U8bit             *p_unresolved_addr,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_parse_chk_chunk_abort(
        sctp_U8bit             *buffer,
        sctp_U32bit            chunk_offset,
        sctp_U32bit            *num_errors,
        sctp_error_cause_st    *error_codes,
        sctp_error_t           *p_ecode );

    extern sctp_return_t
    sctp_parse_chk_chunk_error(
        sctp_U8bit             *buffer,
        sctp_U32bit            chunk_offset,
        sctp_U32bit            *num_errors,
        sctp_error_cause_st    *error_codes,
        sctp_error_t           *p_ecode );

    extern sctp_return_t
    sctp_parse_chk_chunk_heartbeatack(
        sctp_U8bit             *buffer,
        sctp_U32bit            chunk_offset,
        sctp_tcb_st            *p_assoc,
        sctp_chunk_hb_st       *heartbeatack_chunk,
        sctp_error_t           *p_ecode );

    extern sctp_return_t
    sctp_parse_chk_chunk_sack(
        sctp_U8bit             *buffer,
        sctp_U32bit            chunk_offset,
        sctp_chunk_sack_st     *sack_chunk,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_parse_packet_header(
        sctp_U8bit          *buffer,
        sctp_U32bit         size,
        sctp_packet_st      *p_packet,
        sctp_error_t        *ecode );

    sctp_return_t
    sctp_parse_chk_TL_datagram(
        sctp_U8bit          *buffer,
        sctp_U32bit         size,
        sctp_tcb_st         *p_ass,
        sctp_packet_st      *p_packet,
        sctp_U32bit         *num_of_chunks,
        sctp_chunk_info_st  *chunk_id_list,
        sctp_Boolean_t      *unrecognise_chunk_present,
        sctp_U16bit         *unrecognise_chunk_len,
        sctp_U8bit          *p_unrecognise_chunk,
        sctp_Boolean_t      resolver_flag,
        sctp_error_t        *ecode );

#ifdef SCTP_DYNAMIC_IP_SUPPORT

    extern sctp_U8bit *
    sctp_parse_chk_addr_param(
        sctp_U8bit            *p_buf,
        sctp_U16bit           chunk_len,
        sctp_sockaddr_st      *p_addr,
        sctp_error_t          *p_ecode );

    sctp_return_t
    sctp_parse_chk_chunk_asconf(
        sctp_U8bit            *buffer,
        sctp_U32bit           chunk_offset,
        sctp_addr_conf_st     *p_msg,
        sctp_error_t          *p_ecode );

    sctp_return_t
    sctp_parse_chk_chunk_asconfack(
        sctp_U8bit            *buffer,
        sctp_U32bit           chunk_offset,
        sctp_addr_conf_ack_st *p_msg,
        sctp_error_t          *p_ecode );

#endif

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_mesg_parse_SEEN__ */


