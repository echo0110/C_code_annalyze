/*******************************************************************************
 *    FILE NAME:
 *    s_db.h
 *
 *    DESCRIPTION:
 *        This file is part of database module of SCTP stack.
 *    This file lists datatypes needed by database module.
 *
 *    DATE      NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *    02June 2000    Sandeep Mahajan  -    .Original Creation
 *    27Aug02         mrajpal               SPR 4800
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************************/
#ifndef    __FILE_sctp_db_SEEN__
#define    __FILE_sctp_db_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

#include <s_cotyps.h>
#include <s_osmutx.h>
#include <s_cotyps.h>
#include <s_select.h>
    typedef struct
    {
        sctp_U8bit               *min_pool;
        sctp_U8bit               *med_pool;
        sctp_U8bit               *max_pool;
    } sctp_mem_pool_st;

    typedef struct
    {
        sctpbuffer_st             *min_buffer;
        sctpbuffer_st             *med_buffer;
        sctpbuffer_st             *max_buffer;
    } sctp_buffer_pool_st;

    typedef struct
    {
        struct node                 __header__;
        sctp_U32bit                 local_tag;
        sctp_Boolean_t              flags;
        sctp_U8bit                  n_assoc;
        LIST                        assoc_list;
    } sctp_tcb_verf_st;


    /* Database main structure */
    typedef struct
    {

        sctp_lock_t              lock;

        sctp_U32bit              max_appl;
        sctp_U32bit              num_active_appl;
        sctp_appl_info_st        *appl_list;

        sctp_U32bit              max_ep;
        sctp_U32bit              num_active_ep;
        sctp_U32bit              last_allocated_ep;
        sctp_ep_st               *ep_table;

        LIST                     ep_list;

        sctp_U32bit              max_assoc;

        sctp_tcb_st              *assoc_table;
        LIST                     assoc_freelist;

#ifdef SCTP_UI_SOCK_API
        sctp_ulpqueue_node_st    *p_ulpq;
        LIST                     ulpq_freelist;

        sctp_data_indication_st  *p_data_ind;
        LIST                     data_ind_freelist;

        sctp_select_st           *p_select;
#endif /* SCTP_UI_SOCK_API */

        sctp_hash_st             bhash[SCTP_MAX_BHTABLE_SIZE];
        sctp_hash_st             lhash[SCTP_MAX_LHTABLE_SIZE];
        sctp_hash_st             ass_hash[SCTP_MAX_VERIFICATION_NODE];

        sctp_tcb_verf_st         *verf_ass_table;

        sctp_U32bit              min_tx_buffers;
        sctp_U32bit              med_tx_buffers;
        sctp_U32bit              max_tx_buffers;
        sctp_U32bit              min_rx_buffers;
        sctp_U32bit              med_rx_buffers;
        sctp_U32bit              max_rx_buffers;
        sctp_U32bit              min_reserve_buffers;
        sctp_U32bit              med_reserve_buffers;
        sctp_U32bit              max_reserve_buffers;

        sctp_U32bit              max_streams_per_assoc;
        sctp_U16bit              init_in_streams;
        sctp_U16bit              init_out_streams;

        sctp_mem_pool_st         rx_mem_pool;
        sctp_mem_pool_st         tx_mem_pool;
        sctp_mem_pool_st         reserve_mem_pool;

        sctp_buffer_pool_st      rx_buffer;
        sctp_buffer_pool_st      tx_buffer;
        sctp_buffer_pool_st      reserve_buffer;

        sctp_buffer_list_st      rx_list;
        sctp_buffer_list_st      tx_list;
        sctp_buffer_list_st      reserve_list;

        sctp_prot_params_st      sctp_def_prot_params; /* default protocol params */
        sctp_cong_reg_def_st     trx_cong_reg_def;
        sctp_cong_reg_def_st     rcv_cong_reg_def;

        sctp_U32bit              num_local_addrs;
        sctp_sockaddr_st         *addr_list;
        /*SPR 4800*/
        /*
         *this should be a part of e_shell_info_st and
         *API needs to be provisioned in e_shell itself
         */
        sctp_U32bit              eshell_select_time;
        /*SPR 4800*/
        sctp_U8bit               hostname[SCTP_MAX_HOSTNAME_SIZE];
        sctp_U8bit               num_server_created_port;
        server_created_addr_st   server_created_addr_info[SCTP_MAX_SERVER_CREATED_PORT];
    } sctp_db_main_st;

    extern sctp_db_main_st        sctp_db_main;

    /*Buffer used by sctp receive thread*/
    extern sctp_U8bit            *sctp_recv_thr_buff;

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_db_SEEN__ */

