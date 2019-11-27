/*******************************************************************************
 *    FILE NAME:  s_uismif.h
 *
 *    DESCRIPTION:
 *    This file list all the interface functions with system management.
 *
 *
 *    DATE        NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    02June 2000     Sandeep Mahajan  -            Original Creation
 *    08Oct2002       Manish Rajpal     SPR 4927    Updating Prototypes
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************************/
#ifndef     __FILE_sctp_sm_if_SEEN__
#define     __FILE_sctp_sm_if_SEEN__

#ifdef  __cplusplus
extern "C" {
#endif


    sctp_return_t
    sctp_api_enable_assoc_trc(
        sctp_U32bit        ep_id,
        sctp_trc_et        trace_type,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_api_disable_assoc_trc(
        sctp_U32bit        ep_id,
        sctp_trc_et        ttype,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_api_get_trace_info(
        sctp_U32bit        ep_id,
        sctp_U8bit         *trace_info,
        sctp_error_t       *p_ecode );


    sctp_return_t
    sctp_api_init_stack(
        sctp_init_options_st  *init_options,
        sctp_error_t         *p_ecode );

    sctp_return_t
    sctp_api_get_stack_entity_version(
        sctp_ver_info_st     *ver_info );

    sctp_return_t
    sctp_api_config_stack_params(
        sctp_prot_params_st    *prot_params,
        sctp_error_t           *errorno );

    sctp_return_t
    sctp_api_init_stats(
        sctp_stats_et    stats_type,
        sctp_error_t     *p_ecode );

    sctp_return_t
    sctp_api_get_stats(
        sctp_stats_et    stats_type,
        sctp_assoc_t     association_id,
        sctp_U8bit       *stats_data,
        sctp_Boolean_t       reset_flag,
        sctp_error_t     *p_ecode );

    sctp_return_t
    sctp_api_disable_stats(
        sctp_stats_et    stats_type,
        sctp_error_t     *p_ecode );

    sctp_return_t
    sctp_api_enable_stats(
        sctp_stats_et    stats_type,
        sctp_error_t     *p_ecode );


    sctp_return_t
    sctp_api_get_error_level(
        sctp_error_flag_et    error_flag,
        sctp_error_level_et   *p_error_level,
        sctp_error_t          *p_ecode );

    sctp_return_t
    sctp_api_set_error_level(
        sctp_error_flag_et     error_flag ,
        sctp_error_level_et    error_level,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_api_enable_exception_handling(
        user_handler_fn_pt          user_handler,
        void                       *arg,
        sctp_error_t                *p_ecode );

    sctp_return_t
    sctp_api_disable_exception_handling(
        sctp_error_t                *p_ecode );


#ifdef  __cplusplus
}
#endif


#endif

