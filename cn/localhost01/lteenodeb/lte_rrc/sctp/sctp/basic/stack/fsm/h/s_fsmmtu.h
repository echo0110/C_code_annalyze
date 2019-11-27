/***************************************************************************
 *  FILE NAME  : s_fsmmtu.h
 *
 *  DESCRIPTION: Contains the Path MTU related functions.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef    __FILE_sctp_pathmtu_SEEN__
#define    __FILE_sctp_pathmtu_SEEN__


#ifdef __cplusplus
extern "C" {
#endif

    extern LIST mtu_freelist;

    extern LIST mtu_activelist;

    extern sctp_timer_t  pmtu_timer_id;

    typedef    struct
    {
        struct node         __header__;
        sctp_time_t         time_stamp;
        sctp_sockaddr_st    addr;
        sctp_U32bit         mtu_size;
        sctp_U16bit         spare;
        sctp_U32bit         proposed_mtu_size;
    } sctp_pmtu_node_st;


    sctp_return_t
    sctp_init_pmtu_database(
        sctp_error_t    *p_ecode );

    sctp_return_t
    sctp_deinit_pmtu_database( void );


    sctp_return_t
    sctp_get_pathmtu(
        sctp_tcb_st         *p_ass,
        sctp_sockaddr_st    *dest_addr,
        sctp_U16bit         *mtu_size );

    sctp_return_t
    sctp_estimate_next_pmtu_V4(
        sctp_U32bit         *mtu_size );

    sctp_return_t
    sctp_estimate_next_pmtu_V6(
        sctp_U16bit         *mtu_size );

    sctp_return_t
    sctp_set_dest_parameters(
        sctp_sockaddr_st      *dest_addr,
        sctp_U16bit           mtu_size );

    void
    sctp_process_pmtu_dbupdate_timeout(
        void );

    void
    sctp_discover_pmtu(
        sctp_tcb_st         *p_ass,
        sctp_sockaddr_st    *p_dest_addr );

    sctp_return_t
    sctp_get_dest_parameters(
        sctp_sockaddr_st   *dest_addr,
        sctp_U16bit        *mtu_size,
        sctp_time_t        *time_stamp );

    sctp_return_t
    sctp_set_pmtu_capable(
        sctp_tcb_st         *p_ass,
        sctp_U32bit         parameter_value,
        sctp_error_t        *p_ecode );

    sctp_return_t
    sctp_set_mtu_size(
        sctp_tcb_st         *p_ass,
        sctp_U32bit         parameter_value,
        sctp_error_t        *p_ecode );


#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_pathmtu_SEEN__ */


