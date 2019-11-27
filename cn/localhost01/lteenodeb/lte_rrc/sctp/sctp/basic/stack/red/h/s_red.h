/******************************************************************************
 *    FILE NAME:    s_red.h
 *
 *    DESCRIPTION:
 *    This file    is part    of SCTP    Stack redundancy module.
 *
 *
 *    DATE        NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    3 April 2001     Vakul Garg  -       .Original Creation
 *
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef    __FILE_sctp_red_SEEN__
#define    __FILE_sctp_red_SEEN__

#ifdef __cplusplus
extern "C" {
#endif


#include <s_cogbl.h>
#include <s_cotyps.h>

#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_CLI_RECV_COM_THREAD

    /*This global variable determines whether stack is ACTIVE or STANDBY*/
    extern sctp_stack_red_state_et  sctp_stack_red_state;

    /*This global variable determines whether stack is START or STOP*/
    extern sctp_stack_proc_state_et sctp_stack_proc_state;

    /* Functions */

    extern sctp_return_t
    sctp_init_red_module(
        sctp_error_t   *errorno );

    extern sctp_return_t
    sctp_deinit_red_module( void );

    extern sctp_return_t
    sctp_api_set_proc_state(
        sctp_stack_proc_state_et  stack_state,
        sctp_U32bit               seed,
        sctp_error_t              *errorno );

    extern sctp_return_t
    sctp_api_set_red_state(
        sctp_stack_red_state_et   stack_state,
        sctp_error_t              *errorno );

    extern sctp_return_t
    sctp_api_get_stack_red_db(
        sctp_red_stack_db_st    *db,
        sctp_error_t            *errorno );


    extern sctp_return_t
    sctp_api_set_stack_red_db(
        sctp_red_stack_db_st   *db,
        sctp_error_t           *errorno );

    extern sctp_U8bit *
    sctp_api_get_assoc_red_db(
        sctp_U32bit   assoc_id,
        sctp_U32bit   *len,
        sctp_error_t  *errorno );

    extern sctp_return_t
    sctp_api_set_assoc_red_db(
        sctp_U32bit    assoc_id,
        sctp_U8bit     *assoc_data,
        sctp_error_t   *errorno );


    extern sctp_return_t
    sctp_api_get_stack_health(
        sctp_stack_red_state_et    *red_state,
        sctp_stack_proc_state_et   *proc_state,
        sctp_error_t               *errorno );

#endif /*SCTP_CLI_RECV_COM_THREAD*/
#endif /*SCTP_UI_MESSAGEBASED*/
#endif /*SCTP_RED_SUPPORT*/

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_red_SEEN__*/
