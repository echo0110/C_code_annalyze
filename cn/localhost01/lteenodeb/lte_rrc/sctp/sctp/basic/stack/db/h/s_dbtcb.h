/******************************************************************************
 *  FILE NAME:
 *      s_dbtcb.h
 *
 *  DESCRIPTION:
 *      This file is part of database module of SCTP stack.
 *  This file lists datatypes and function prototypes needed by database
 *  module.
 *
 *  DATE    NAME       REFERENCE       REASON
 *  -----------------------------------------------------
 *  02June 2000     Sandeep Mahajan  -       .Original Creation
 *  19Oct04         Amit Kumar Ray           SCTP_6_0_SCTP/FUNC-CONGCON-4
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef __FILE_sctp_db_tcb_SEEN__
#define __FILE_sctp_db_tcb_SEEN__

#include <s_db.h>
#include <s_uismty.h>

#ifdef __cplusplus
extern "C" {
#endif

#define sctp_db_get_lhash_table(port)  \
    &sctp_db_main.lhash[port%SCTP_MAX_LHTABLE_SIZE]

#define sctp_db_get_bhash_table(port)  \
    &sctp_db_main.bhash[port%SCTP_MAX_BHTABLE_SIZE]

#define sctp_db_get_ass_hash_table(vtag)  \
    &sctp_db_main.ass_hash[vtag%SCTP_MAX_VERIFICATION_NODE]

    sctp_return_t
    sctp_db_get_su_id(
        sctp_U16bit           application_id,
        sctp_U16bit           *p_su_id );


    sctp_U32bit
    sctp_db_assoc_rx_buffer_num(
        sctp_tcb_st   *p_ass,
        sctp_U32bit   length );

    sctp_U32bit
    sctp_db_assoc_tx_buffer_num(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    length );

    sctp_U32bit
    sctp_db_assoc_tx_buffer_num_recompute(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    length,
        sctp_U32bit    data_size,
        sctp_U8bit     *max_buff_req,
        sctp_U8bit    *med_buff_req,
        sctp_U8bit    *min_buff_req );

    sctp_U32bit
    sctp_db_assoc_rx_window(
        sctp_tcb_st    *p_ass );

    sctp_U32bit
    sctp_db_assoc_tx_window(
        sctp_tcb_st    *p_ass );

    sctp_U32bit
    sctp_db_assoc_max_rx_window(
        sctp_tcb_st    *p_ass );

    sctp_U32bit
    sctp_db_assoc_max_tx_window(
        sctp_tcb_st     *p_ass );

    sctp_return_t
    sctp_db_get_callback_info(
        sctp_U16bit          appl_id,
        sctp_callback_func_t *pp_callback,
        sctp_Pvoid_t         *pp_appl_data );

    sctp_U32bit sctp_db_get_dafault_rwnd(
        void );

    sctp_U32bit
    sctp_db_get_number_txbuffers(
        void );

    sctp_return_t
    sctp_init_db_module(
        sctp_init_options_st   *init_options,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_deinit_db_module( void );

    sctp_return_t
    sctp_db_locate_association(
        sctp_U16bit        that_port,
        sctp_sockaddr_st   *that_address,
        sctp_U16bit        this_port,
        sctp_sockaddr_st   *this_address,
        sctp_tcb_st        **pp_ass );

    sctp_return_t
    sctp_db_validate_connect_port(
        sctp_U16bit  sctp_port,
        sctp_tcb_st    *p_assoc );

    sctp_return_t
    sctp_db_locate_server(
        sctp_U16bit             sctp_port,
        sctp_sockaddr_st        *p_addr,
        sctp_ep_st              **pp_ep );

    sctp_return_t
    sctp_db_is_port_active(
        sctp_U16bit    sctp_port );

    sctp_return_t
    sctp_db_get_free_sctpport(
        sctp_U16bit    *sctp_port );

    sctp_tcb_st *
    sctp_db_get_association_ptr(
        sctp_U32bit     ep_id,
        sctp_U32bit     assoc_id );

    sctp_return_t
    sctp_db_validate_bind_address(
        sctp_U32bit             naddr,
        sctp_sockaddr_st        addrlist[] );

    /* SPR 6274 starts*/
    sctp_return_t
    sctp_db_validate_bind_address_ext(
        sctp_U32bit             naddr,
        sctp_sockaddr_st        addrlist[] );
    /* SPR 6274 Ends*/

    sctp_return_t
    sctp_db_register_new_appl(
        sctp_U16bit           *appl_id,
        void                  *callback_func,
        void                  *p_appl_data,
        sctp_error_t          *p_ecode );

    sctp_return_t
    sctp_db_deregister_appl(
        sctp_U16bit    appl_id );

    sctp_return_t
    sctp_db_validate_appl(
        sctp_U16bit    application_id );

    sctp_tcb_st *
    sctp_db_get_free_association(
        sctp_error_t  *p_ecode );

    sctp_return_t
    sctp_db_validate_remote_addr(
        sctp_tcb_st        *p_ass,
        sctp_sockaddr_st   *rem_addr,
        sctp_U32bit        *addr_index );

    sctp_return_t
    sctp_db_validate_local_addr(
        sctp_ep_st        *p_ep,
        sctp_sockaddr_st  *local_addr,
        sctp_U32bit       *addr_index );

    sctp_return_t
    sctp_db_del_stack_address(
        sctp_U32bit               naddr,
        sctp_sockaddr_st          *p_addrlist );

    sctp_return_t
    sctp_db_add_stack_address(
        sctp_U32bit               naddr,
        sctp_sockaddr_st          *p_addrlist );

    void
    sctp_db_clean_association(
        sctp_tcb_st    *p_ass,
        sctp_Boolean_t restart_flag );

    void
    sctp_db_cleanup_queues(
        sctp_tcb_st        *p_ass,
        sctp_Boolean_t     clean_tx_queue );

    void
    sctp_db_update_assoc_on_cookie(
        sctp_tcb_st          *p_ass,
        sctp_chunk_cookie_st *cookie );

    sctp_return_t
    sctp_db_get_assoc_freelists(
        sctp_tcb_st    *p_ass,
        sctp_error_t   *p_ecode );

    sctp_return_t
    sctp_db_add_hash(
        sctp_U32bit    flag,
        sctp_ep_st     *p_ep );

    sctp_return_t
    sctp_db_del_hash(
        sctp_U32bit    flag,
        sctp_ep_st     *p_ep );

    sctp_return_t
    sctp_db_add_ass_hash(
        sctp_tcb_verf_st    *p_ass );

    sctp_return_t
    sctp_db_del_ass_hash(
        sctp_tcb_verf_st    *p_ass );

    sctp_tcb_verf_st *
    sctp_db_get_free_verf_ass_node(
        sctp_U32bit    local_tag );

    sctp_tcb_verf_st *
    sctp_db_get_verf_ass_node(
        sctp_U32bit    local_tag );

    sctp_return_t
    sctp_db_delete_assoc_verf(
        sctp_tcb_st          *p_tcb );

    sctp_return_t
    sctp_db_add_assoc_verf(
        sctp_tcb_st          *p_tcb );

    sctp_return_t
    sctp_db_locate_association_verf(
        sctp_U16bit        that_port,
        sctp_sockaddr_st   *that_address,
        sctp_U16bit        this_port,
        sctp_sockaddr_st   *this_address,
        sctp_U32bit        verf_tag,
        sctp_tcb_st        **pp_ass );

    sctp_ep_st *
    sctp_db_get_ep_ptr(
        sctp_U32bit     ep_id );

    sctp_return_t
    sctp_db_add_association(
        sctp_U32bit           ep_id,
        sctp_U16bit           that_port,
        sctp_chunk_cookie_st  *cookie,
        sctp_U32bit           *p_assoc,
        sctp_error_t          *p_ecode );

    sctp_return_t
    sctp_db_add_assoc_to_ep(
        sctp_U32bit         ep_id,
        sctp_tcb_st         *p_ass,
        sctp_error_t        *p_ecode );

    sctp_return_t
    sctp_db_delete_association(
        sctp_tcb_st    *p_ass );

    void
    sctp_db_clean_ep(
        sctp_ep_st      *p_ep,
        sctp_Boolean_t  restart_flag );

    sctp_ep_st *
    sctp_db_get_free_ep(
        sctp_error_t  *p_ecode );

    sctp_return_t
    sctp_db_bind_ep(
        sctp_U32bit              ep_id,
        sctp_U16bit              port,
        sctp_U32bit              no_addr,
        sctp_sockaddr_st         list_of_addrs[],
        sctp_error_t             *p_ecode );

    sctp_return_t
    sctp_db_bind_ep_with_af_check(
        sctp_U32bit              ep_id,
        sctp_U16bit              port,
        sctp_error_t             *p_ecode );

    sctp_return_t
    sctp_db_create_ep(
        sctp_U32bit          server_id,
        sctp_U32bit          *p_ep_id,
        sctp_error_t         *p_ecode );

    sctp_tcb_st *
    sctp_db_create_tcb(
        sctp_ep_st             *p_ep,
        sctp_U16bit            dport,
        sctp_U32bit            naddr,
        sctp_sockaddr_st       addr_list[],
        sctp_error_t           *p_err );

    sctp_return_t
    sctp_db_delete_ep(
        sctp_U32bit     ep_id,
        sctp_tcb_st     *p_ass,
        sctp_Boolean_t  ep_flag );


#ifdef SCTP_UI_SOCK_API

    sctp_return_t
    sctp_db_add_data_ind(
        sctp_tcb_st     *p_tcb,
        sctp_U32bit     n_ind );

    sctp_return_t
    sctp_db_clear_data_ind(
        sctp_tcb_st      *p_tcb,
        sctp_U32bit      n_ind );

    sctp_return_t
    sctp_db_check_data_ind(
        sctp_tcb_st      *p_tcb );

    void
    sctp_db_lock_ulpqueue(
        sctp_ulpqueue_st   *p_ulpq );

    void
    sctp_db_unlock_ulpqueue(
        sctp_ulpqueue_st   *p_ulpq );

    sctp_ulpqueue_node_st *
    sctp_db_get_ntfy_node( void );

    void
    sctp_db_free_ntfy_node(
        sctp_ulpqueue_node_st   *p_node );

    sctp_return_t
    sctp_db_ulpqueue_add_ntfy(
        sctp_ulpqueue_st       *p_ulpq,
        sctp_ulpqueue_node_st  *p_ntfy );

    sctp_ulpqueue_node_st *
    sctp_db_ulpqueue_rem_ntfy(
        sctp_ulpqueue_st   *p_ulpq );

    void
    sctp_db_ulpqueue_empty(
        sctp_ulpqueue_st   *p_ulpq );

    void
    sctp_db_ulpqueue_copy(
        sctp_U32bit        assoc_id,
        sctp_ulpqueue_st   *p_srcq,
        sctp_ulpqueue_st   *p_dstq );

#endif /* SCTP_UI_SOCK_API */

    sctp_tcb_st *
    sctp_db_get_assoc_from_ep(
        sctp_ep_st         *p_ep );

    sctp_return_t
    sctp_db_is_tcb_in_ep(
        sctp_ep_st         *p_ep,
        sctp_U16bit        dport,
        sctp_U32bit        naddr,
        sctp_sockaddr_st   daddr_list[],
        sctp_tcb_st        **pp_tcb,
        sctp_error_t       *p_err );

    sctp_return_t
    sctp_db_is_listenable(
        sctp_ep_st         *p_ep,
        sctp_error_t       *p_err );


    sctp_return_t
    sctp_db_addr_bindable(
        sctp_U16bit        port,
        sctp_U32bit        naddr,
        sctp_sockaddr_st   addr_list[] );

    sctp_return_t
    sctp_db_chk_addr_for_delete(
        sctp_ep_st               *p_ep,
        sctp_U16bit              port,
        sctp_U32bit              naddr,
        sctp_sockaddr_st         addr_list[],
        sctp_error_t             *p_err );

    sctp_return_t
    sctp_db_rem_addr_after_chks(
        sctp_ep_st         *p_ep,
        sctp_U16bit        port,
        sctp_U32bit        naddr,
        sctp_sockaddr_st  addr_list[],
        sctp_error_t       *p_err );

    sctp_return_t
    sctp_db_addr_connectable(
        sctp_ep_st         *p_ep,
        sctp_U16bit        port,
        sctp_U32bit        naddr,
        sctp_sockaddr_st   addr_list[],
        sctp_error_t       *p_err );

    sctp_return_t
    sctp_db_create_duplicate_ep(
        sctp_ep_st         *p_ep,
        sctp_tcb_st        *p_tcb,
        sctp_ep_st         **pp_new_ep,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_db_lct_n_updt_assoc_wo_dprt(
        sctp_U16bit        that_port,
        sctp_U32bit        naddr,
        sctp_sockaddr_st   that_address[],
        sctp_U16bit        this_port,
        sctp_sockaddr_st   this_address[],
        sctp_tcb_st        **pp_ass );

    sctp_return_t
    sctp_db_locate_ep_created_from_server(
        sctp_U16bit             sctp_port,
        sctp_sockaddr_st        *p_addr,
        sctp_ep_st              **pp_ep );

    sctp_return_t
    sctp_db_create_duplicate_ep_with_port(
        sctp_ep_st           *p_ep,
        sctp_ep_st           **pp_new_ep,
        sctp_U16bit          port,
        sctp_error_t         *p_ecode );

    void sctp_db_add_server_created_port( sctp_U16bit  new_port,
                                          sctp_U32bit              naddr,
                                          sctp_sockaddr_st         addr_list[] );

    void sctp_db_delete_server_created_port( sctp_U16bit  new_port,
                                             sctp_U32bit              naddr,
                                             sctp_sockaddr_st         addr_list[] );

    sctp_return_t
    sctp_db_find_server_created_port( sctp_U16bit  new_port,
                                      sctp_U32bit              naddr,
                                      sctp_sockaddr_st         addr_list[] );

    sctp_return_t
    sctp_db_validate_config_stack_address(
        sctp_bindx_flag_et        operation,
        sctp_U32bit               naddr,
        sctp_sockaddr_st          *p_addrlist,
        sctp_error_t              *p_ecode );

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_db_tcb_SEEN__ */





