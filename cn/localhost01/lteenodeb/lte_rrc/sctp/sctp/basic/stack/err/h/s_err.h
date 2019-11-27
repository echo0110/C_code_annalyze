/******************************************************************************
*   FILE NAME:
*    s_err.h
*
*   DESCRIPTION:
*    This file is part of Error Handling Module of SCTP. This will list all
*    the ERROR definitions.This will also include Macros for Reporting Errors.
*
*      DATE           NAME              REFERENCE        REASON
*     -----------------------------------------------------
*     02June 2000     Sandeep Mahajan  -       .Original Creation
*
*    Copyright (C) 2006 Aricent Inc . All Rights Reserved
******************************************************************************/
#ifndef  __FILE_sctp_err_SEEN__
#define  __FILE_sctp_err_SEEN__


#ifdef __cplusplus
extern "C" {
#endif

#ifdef SCTP_ERROR_ENABLED


    /******************************************************************
     For exception handlig support
    *******************************************************************/
    typedef struct
    {
        sctp_Boolean_t      enabled;
        user_handler_fn_pt  user_handler_fn;
        void               *arg;
    } sctp_sig_info_st;


    extern sctp_return_t
    sctp_err_default_exception_handler(
        sctp_signal_et              signo,
        sctp_error_t                *p_ecode );


    /*******************************************************************/
    extern sctp_return_t
    sctp_init_error_module(
        sctp_error_t    *p_ecode );

    extern void
    sctp_err_reporter(
        sctp_U32bit        error_type,
        sctp_U32bit        error_level,
        sctp_U32bit        ecode ,
        sctp_U8bit         *info_str );



    /* Macro for Error reporting */

    extern sctp_U8bit sctp_error_state;

    extern sctp_error_level_et    sctp_error_table[SCTP_MAX_ERROR_FLAG + 1];


#define  SCTP_ERROR(error_flag, error_level, ecode, info_str) \
    sctp_err_reporter(error_flag, error_level, ecode, (sctp_U8bit *)info_str)


#else
#define    SCTP_ERROR(error_flag, error_level, ecode, info_str)
#endif


    /*** Forward Declarations   ***/
    extern sctp_return_t
    sctp_api_get_error_level(
        sctp_error_flag_et    error_flag,
        sctp_error_level_et   *p_error_level,
        sctp_error_t          *p_ecode );

    extern sctp_return_t
    sctp_api_set_error_level(
        sctp_error_flag_et     error_flag ,
        sctp_error_level_et    error_level,
        sctp_error_t           *p_ecode );

    extern sctp_return_t
    sctp_api_enable_exception_handling(
        user_handler_fn_pt          user_handler,
        void                       *arg,
        sctp_error_t                *p_ecode );

    extern sctp_return_t
    sctp_api_disable_exception_handling(
        sctp_error_t                *p_ecode );

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_err_SEEN__ */


