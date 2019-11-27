/***************************************************************************
 *  FILE NAME  : s_in.h
 *
 *  DESCRIPTION: Contains the Stack Initialization Module.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  18AUg15    Gurpreet Singh            REL 6.2.0 CSR 105109 Bug ID 68
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef    __FILE_sctp_init_SEEN__
#define    __FILE_sctp_init_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

#include <s_cotyps.h>
#include <s_uismty.h>

    /* This Data structures has stack init parameters */
    extern sctp_capability_st  sctp_capability;

    /*
     * State of the SCTP stack entity. This can have following possible values
     * UNINITIALIZED OR ACTIVE.
     */
    extern sctp_stack_state_et   sctp_stack_state;

    /* Global buffer queue to be maintained for storing TL packets*/
    extern LIST        TL_buffers;
    extern sctp_lock_t TL_buflist_lock;

#define SCTP_TL_ADDBUF(p_buf) \
    SCTP_LOCK(TL_buflist_lock); \
    lstAdd(&TL_buffers, (NODE *)p_buf); \
    SCTP_UNLOCK(TL_buflist_lock);

#define SCTP_TL_GETBUF(packet) \
    SCTP_LOCK(TL_buflist_lock); \
    packet = (sctp_TL_packet_st *) lstGet(&TL_buffers); \
    SCTP_UNLOCK(TL_buflist_lock);

    /* Defines for accessing stack capability:Prob should be moved to DB */
#define SCTP_IS_STACK_UDP   (sctp_capability.stack_type == SCTP_OVER_UDP)
#define SCTP_IS_STACK_INET  (sctp_capability.stack_type == SCTP_OVER_INET)
#define SCTP_IS_STACK_INET6 (sctp_capability.stack_type == SCTP_OVER_INET6)

#define SCTP_IS_ECN_CAPABLE  (sctp_capability.ecn_capable  == SCTP_TRUE)
#define SCTP_IS_PMTU_CAPABLE (sctp_capability.pmtu_capable == SCTP_TRUE)

#define SCTP_ACK_POLICY      (sctp_capability.ack_policy)
#define SCTP_SCOOKIE_POLICY  (sctp_capability.stale_cookie_policy)

    sctp_return_t
    sctp_api_config_stack_params(
        sctp_prot_params_st    *prot_params,
        sctp_error_t           *errorno );


    sctp_return_t
    sctp_api_init_stack(
        sctp_init_options_st   *init_options,
        sctp_error_t           *p_ecode );


    sctp_return_t
    sctp_api_get_stack_entity_version(
        sctp_ver_info_st       *ver_info );


    sctp_return_t
    sctp_api_parse_stack_params(
        sctp_prot_params_st    *prot_params,
        sctp_error_t           *p_ecode );

    void
    sctp_process_api_config_stack(
        sctp_prot_params_st    *prot_params );

    sctp_return_t
    sctp_api_config_stack_params(
        sctp_prot_params_st    *prot_params,
        sctp_error_t           *errorno );

    sctp_return_t
    sctp_init_stack_parameters(
        sctp_init_options_st   *init_options,
        sctp_sockaddr_list_st  local_addr_list,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_api_get_stack_entity_version(
        sctp_ver_info_st        *ver_info );

    sctp_return_t
    sctp_api_config_stack_addresses(
        sctp_bindx_flag_et  operation,
        sctp_U32bit         naddr,
        sctp_sockaddr_st    *p_addrlist,
        sctp_error_t        *errorno );

    sctp_return_t
    sctp_api_deinit_stack(
        sctp_Boolean_t       forced,
        sctp_error_t         *p_ecode );

#if defined(SCTP_STATS_ENABLED) && defined(SCTP_ASSOC_STATS_RECOVERY_SUPPORT)
    sctp_void_t
    sctp_stk_api_stats_recovery_ind(
        sctp_U32bit association_id,
        sctp_assoc_stats_st  *assoc_stats );

    sctp_void_t
    sctp_api_stats_recovery_ind(
        sctp_U32bit association_id,
        sctp_assoc_stats_st  *assoc_stats );
#endif
    /*Bug ID 68 changes s*/
    sctp_void_t
    sctp_api_set_reset_abort_flag(
        sctp_Boolean_t       stop );
    /*Bug ID 68 changes e*/
#ifdef __cplusplus
}
#endif
#endif /* __FILE_sctp_init_SEEN__ */

